/****************************************************************************
* MeshLab                                                           o o     *
* An extendible mesh processor                                    o     o   *
*                                                                _   O  _   *
* Copyright(C) 2005, 2006                                          \/)\/    *
* Visual Computing Lab                                            /\/|      *
* ISTI - Italian National Research Council                           |      *
*                                                                    \      *
* All rights reserved.                                                      *
*                                                                           *
* This program is free software; you can redistribute it and/or modify      *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 2 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
****************************************************************************/

#include <Qt>
#include <QtGui>

#include "filter_fractal.h"
#include <common/meshmodel.h>
#include <common/interfaces.h>
#include <vcg/complex/trimesh/clean.h>
#include <vcg/complex/trimesh/update/bounding.h>
#include <vcg/complex/trimesh/allocate.h>
#include <vcg/complex/trimesh/refine.h>
#include <vcg/complex/trimesh/smooth.h>
#include <vcg/math/base.h>
#include <vcg/space/plane3.h>

using namespace std;
using namespace vcg;

// ------- MeshFilterInterface implementation ------------------------
FilterFractal::FilterFractal()
{
    typeList << CR_FRACTAL_TERRAIN << FP_FRACTAL_MESH << FP_CRATERS;
    FilterIDType tt;
    foreach(tt , types())
        actionList << new QAction(filterName(tt), this);
}

QString FilterFractal::filterName(FilterIDType filterId) const
{
    switch (filterId) {
    case CR_FRACTAL_TERRAIN:
        return QString("Fractal Terrain");
        break;
    case FP_FRACTAL_MESH:
        return QString("Fractal Displacement");
        break;
    case FP_CRATERS:
        return QString("Craters Generation");
        break;
    default:
        assert(0); return QString("error");
        break;
    }
}

QString FilterFractal::filterInfo(FilterIDType filterId) const
{
    QString filename, description;
    switch (filterId) {
    case CR_FRACTAL_TERRAIN:
    case FP_FRACTAL_MESH:
        filename = ":/ff_fractal_description.txt";
        break;
    case FP_CRATERS:
        filename = ":/ff_craters_description.txt";
        break;
    default:
        assert(0); return QString("error");
        break;
    }

    QFile f(filename);
    if(f.open(QFile::ReadOnly))
    {
        QTextStream stream(&f);
        description = stream.readAll();
        f.close();
    }

    if(filterId == FP_FRACTAL_MESH)
    {
        description += "<br /><br />Hint: search a good compromise between offset and height factor parameter.";
    }

    return description;
}

void FilterFractal::initParameterSet(QAction* filter,MeshDocument &md, RichParameterSet &par)
{
    switch(ID(filter))
    {
    case CR_FRACTAL_TERRAIN:
    case FP_FRACTAL_MESH:
        initParameterSetForFractalDisplacement(filter, md, par);
        break;
    case FP_CRATERS:
        initParameterSetForCratersGeneration(md, par);
        break;
    }
}

void FilterFractal::initParameterSetForFractalDisplacement(QAction *filter, MeshDocument &md, RichParameterSet &par)
{
    bool terrain_filter = (ID(filter) == CR_FRACTAL_TERRAIN);

    if(terrain_filter) {
        par.addParam(new RichInt("steps", 8, "Subdivision steps:", "Defines the detail of the generated terrain. Allowed values are in range [2,9]. Use values from 6 to 9 to obtain reasonable results."));
        par.addParam(new RichDynamicFloat("maxHeight", 0.2, 0, 1, "Max height:", "Defines the maximum perturbation height as a fraction of the terrain's side."));
    } else {
        float diag = md.mm()->cm.bbox.Diag();
        par.addParam(new RichAbsPerc("maxHeight", 0.02 * diag, 0, 0.5*diag, "Max height:", "Defines the maximum height for the perturbation."));
    }

    par.addParam(new RichDynamicFloat("scale", 1, 0, 10, "Scale factor:", "Scales the fractal perturbation in and out. Values larger than 1 mean zoom out; values smaller than one mean zoom in."));
    if (!terrain_filter)
    {
        par.addParam(new RichInt("smoothingSteps", 5, "Normals smoothing steps:", "Face normals will be smoothed to make the perturbation more homogeneous. This parameter represents the number of smoothing steps." ));
    }
    par.addParam(new RichFloat("seed", 1, "Seed:", "By varying this seed, the terrain morphology will change.\nDon't change the seed if you want to refine the current terrain morphology by changing the other parameters."));

    QStringList algList;
    algList << "fBM (fractal Brownian Motion)" << "Standard multifractal" << "Heterogeneous multifractal" << "Hybrid multifractal terrain" << "Ridged multifractal terrain";
    par.addParam(new RichEnum("algorithm", 4, algList, "Algorithm", "The algorithm with which the fractal terrain will be generated."));
    par.addParam(new RichDynamicFloat("octaves", 8.0, 1.0, 20.0, "Octaves:", "The number of Perlin noise frequencies that will be used to generate the terrain. Reasonable values are in range [2,9]."));
    par.addParam(new RichFloat("lacunarity", 4.0, "Lacunarity:", "The gap between noise frequencies. This parameter is used in conjunction with fractal increment to compute the spectral weights that contribute to the noise in each octave."));
    par.addParam(new RichFloat("fractalIncrement", terrain_filter? 0.5 : 0.2, "Fractal increment:", "This parameter defines how rough the generated terrain will be. The range of reasonable values changes according to the used algorithm, however you can choose it in range [0.2, 1.5]."));
    par.addParam(new RichFloat("offset", 0.9, "Offset:", "This parameter controls the multifractality of the generated terrain. If offset is low, then the terrain will be smooth."));
    par.addParam(new RichFloat("gain", 2.5, "Gain:", "Ignored in all the algorithms except the ridged one. This parameter defines how hard the terrain will be."));
    par.addParam(new RichBool("saveAsQuality", false, "Save as vertex quality", "Saves the perturbation value as vertex quality."));
}

void FilterFractal::initParameterSetForCratersGeneration(MeshDocument &md, RichParameterSet &par)
{
    int meshCount = md.meshList.size();

    // tries to detect the target mesh
    MeshModel* target = md.mm();
    MeshModel* samples = md.mm();
    MeshModel* tmpMesh;
    if (target->cm.fn == 0){ // this is probably the samples layer
        for(int i=0; i<meshCount; i++)
        {
            tmpMesh = md.meshList.at(i);
            if (tmpMesh->cm.fn > 0)
            {
                target = tmpMesh;
                break;
            }
        }
    }

    par.addParam(new RichMesh("target_mesh", target, &md, "Target mesh:", "The mesh on which craters will be generated."));
    par.addParam(new RichMesh("samples_mesh", samples, &md, "Samples layer:", "The samples that represent the central points of craters."));
    par.addParam(new RichInt("seed", 0, "Seed:", "The seed with which the random number generator is initialized. The random generator generates radius and depth for each crater into the given range."));
    par.addParam(new RichInt("smoothingSteps", 5, "Normals smoothing steps:", "Vertex normals are smoothed this number of times before generating craters."));

    QStringList algList;
    algList << "f1 (Gaussian)" << "f2 (Multiquadric)" << "f3";
    par.addParam(new RichEnum("rbf", 1, algList, "Radial function:", "The radial function used to generate craters."));

    par.addParam(new RichDynamicFloat("min_radius", 0.3, 0, 1, "Min crater radius:", "Defines the minimum radius of craters in range [0, 1]. Values near 0 mean very small craters."));
    par.addParam(new RichDynamicFloat("max_radius", 0.6, 0, 1, "Max crater radius:", "Defines the maximum radius of craters in range [0, 1]. Values near 1 mean very large craters."));
    par.addParam(new RichDynamicFloat("min_depth", 0.3, 0, 1, "Min crater depth:", "Defines the minimum depth of craters in range [0, 1]."));
    par.addParam(new RichDynamicFloat("max_depth", 0.6, 0, 1, "Max crater depth:", "Defines the maximum depth of craters in range [0, 1]. Values near 1 mean very deep craters."));
    par.addParam(new RichDynamicFloat("elevation", 0.4, 0, 1, "Elevation:", "Defines how much the crater rise itself from the mesh surface, giving an \"impact-effect\"."));

    QStringList blendList;
    blendList << "Exponential blending" << "Linear blending" << "Gaussian blending" << "f3 blending";
    par.addParam(new RichEnum("blend", 0, blendList, "Blending algorithm:", "The algorithm that is used to blend the perturbation towards the mesh surface."));
    par.addParam(new RichDynamicFloat("blendThreshold", 0.8, 0, 1, "Blending threshold:", "The fraction of craters radius beyond which the radial function is replaced with the blending function."));
    par.addParam(new RichBool("successiveImpacts", true, "Successive impacts", "If not checked, the impact-effects of generated craters will be superimposed with each other."));
    par.addParam(new RichBool("ppNoise", false, "Postprocessing noise", "Slightly perturbates the craters with a noise function."));
    par.addParam(new RichBool("invert", false, "Invert perturbation", "If checked, inverts the sign of radial perturbation to create bumps instead of craters."));
    par.addParam(new RichBool("save_as_quality", false, "Save as vertex quality", "Saves the perturbation as vertex quality."));
    return;
}

bool FilterFractal::applyFilter(QAction* filter, MeshDocument &md, RichParameterSet &par, vcg::CallBackPos* cb)
{
    switch(ID(filter))
    {
    case CR_FRACTAL_TERRAIN:
    case FP_FRACTAL_MESH:
        {
            MeshModel* mm = md.mm();
            float maxHeight = .0;
            int smoothingSteps = 0;

            if(ID(filter) == CR_FRACTAL_TERRAIN)
            {
                int steps = par.getInt("steps");
                steps = ((steps<2)? 2: steps);
                float gridSide = .0;
                generateGrid(mm->cm, steps, gridSide);
                maxHeight = par.getDynamicFloat("maxHeight") * gridSide;
            } else {
                maxHeight = par.getAbsPerc("maxHeight");
                smoothingSteps = par.getInt("smoothingSteps");
            }

            FractalArgs<float> args(mm, par.getEnum("algorithm"),par.getFloat("seed"),
                par.getFloat("octaves"), par.getFloat("lacunarity"),
                par.getFloat("fractalIncrement"), par.getFloat("offset"), par.getFloat("gain"),
                maxHeight, par.getDynamicFloat("scale"), smoothingSteps, par.getBool("saveAsQuality"));

            if(args.saveAsQuality)
                mm->updateDataMask(MeshModel::MM_VERTQUALITY);
            return FractalPerturbation<float>::computeFractalPerturbation<CMeshO>(mm->cm, args, cb);
        }
        break;
    case FP_CRATERS:
        return applyCratersGenerationFilter(md, par, cb);
        break;
    }
    return false;
}

bool FilterFractal::applyCratersGenerationFilter(MeshDocument &md, RichParameterSet & par, vcg::CallBackPos *cb)
{
    if (md.meshList.size() < 2)
    {
        errorMessage = "There must be at least two layers to apply the craters generation filter.";
        return false;
    }

    CMeshO* samples = &(par.getMesh("samples_mesh")->cm);
    if (samples->face.size() > 0)
    {
        errorMessage = "The sample layer selected is not a sample layer.";
        return false;
    }

    float minRadius = par.getDynamicFloat("min_radius");
    float maxRadius = par.getDynamicFloat("max_radius");
    if (maxRadius <= minRadius)
    {
        errorMessage =  "Min radius is greater than max radius.";
        return false;
    }

    float minDepth = par.getDynamicFloat("min_depth");
    float maxDepth = par.getDynamicFloat("max_depth");
    if (maxDepth <= minDepth)
    {
        errorMessage = "Min depth is greater than max depth.";
        return false;
    }

    // reads parameters
    CratersArgs<float> args(par.getMesh("target_mesh"), par.getMesh("samples_mesh"), par.getEnum("rbf"),
                            par.getInt("seed"), minRadius, maxRadius, minDepth, maxDepth,
                            par.getInt("smoothingSteps"), par.getBool("save_as_quality"), par.getBool("invert"),
                            par.getBool("ppNoise"), par.getBool("successiveImpacts"),
                            par.getDynamicFloat("elevation"), par.getEnum("blend"),
                            par.getDynamicFloat("blendThreshold"));

    return generateCraters(args, cb);
}

MeshFilterInterface::FilterClass FilterFractal::getClass(QAction* filter)
{
    switch(ID(filter)) {
    case CR_FRACTAL_TERRAIN:
        return MeshFilterInterface::MeshCreation;
        break;
    case FP_FRACTAL_MESH:
    case FP_CRATERS:
        return MeshFilterInterface::Smoothing;
        break;
    default: assert(0);
        return MeshFilterInterface::Generic;
    }
}

int FilterFractal::getRequirements(QAction *filter)
{
    switch(ID(filter)) {
    case CR_FRACTAL_TERRAIN:
        return MeshModel::MM_NONE;
        break;
    case FP_FRACTAL_MESH:
    case FP_CRATERS:
        return MeshModel::MM_FACEFACETOPO;
        break;
    default: assert(0);
    }
}

int FilterFractal::postCondition(QAction *filter) const
{
    switch(ID(filter))
    {
    case CR_FRACTAL_TERRAIN:
    case FP_FRACTAL_MESH:
    case FP_CRATERS:
        return MeshModel::MM_VERTCOORD | MeshModel::MM_VERTNORMAL | MeshModel::MM_FACENORMAL | MeshModel::MM_VERTQUALITY;
        break;
    default: assert(0);
    }
}
// ----------------------------------------------------------------------

// -------------------- Private functions -------------------------------
bool FilterFractal::generateCraters(CratersArgs<float> &args, vcg::CallBackPos *cb)
{
    typedef std::pair<VertexPointer, FacePointer>   SampleFace;
    typedef std::vector<SampleFace>                 SampleFaceVector;
    typedef CMeshO::PerVertexAttributeHandle<float> Handle;

    // enables per-vertex-quality if needed
    if(args.save_as_quality)
        args.target_model->updateDataMask(MeshModel::MM_VERTQUALITY);

    // smoothes vertex normals
    cb(0, "Smoothing vertex normals..");
    tri::Smooth<CMeshO>::VertexNormalLaplacian(*(args.target_mesh), args.smoothingSteps, false);

    // finds samples faces
    args.target_model->updateDataMask(MeshModel::MM_FACEFACETOPO);
    args.target_model->updateDataMask(MeshModel::MM_FACEMARK);
    SampleFaceVector sfv;
    CratersUtils<CMeshO>::FindSamplesFaces(args.target_mesh, args.samples_mesh, sfv);

    // detectes crater faces and applies the radial perturbation
    int cratersNo = args.samples_mesh->vert.size(), currentCrater = 0;
    char buffer[50];
    SampleFaceVector::iterator sfvi;
    SampleFace p;
    float radius = .0, depth = .0;
    std::vector<FacePointer> craterFaces;

    // per-vertex float attribute
    Handle h = tri::Allocator<CMeshO>::AddPerVertexAttribute<float>(*(args.target_mesh), std::string("perturbation"));
    for(VertexIterator vi=args.target_mesh->vert.begin(); vi!=args.target_mesh->vert.end(); ++vi)
    {
        h[vi] = .0;
    }

    // calculates the perturbation and stores it in the per-vertex-attribute
    for(sfvi=sfv.begin(); sfvi!=sfv.end(); ++sfvi)
    {
        p = (*sfvi);
        radius = args.generateRadius();
        depth = args.generateDepth();
        sprintf(buffer, "Generating crater %i...", currentCrater);
        cb(100*(currentCrater++)/cratersNo, buffer);
        CratersUtils<CMeshO>::GetCraterFaces(args.target_mesh, p.second, p.first, radius, craterFaces);
        CratersUtils<CMeshO>::GetRadialPerturbation(args, p.first, craterFaces, radius, depth, h);
    }

    for(VertexIterator vi=args.target_mesh->vert.begin(); vi!=args.target_mesh->vert.end(); ++vi)
    {
        if(h[vi] == .0) continue;

        if(args.save_as_quality)
        {
            (*vi).Q() = h[vi];
        } else
        {
            (*vi).P() += ((*vi).N() * h[vi]);
        }
    }

    // updates bounding box and normals
    tri::Allocator<CMeshO>::DeletePerVertexAttribute(*(args.target_mesh), std::string("perturbation"));
    vcg::tri::UpdateBounding<CMeshO>::Box(*(args.target_mesh));
    vcg::tri::UpdateNormals<CMeshO>::PerVertexNormalizedPerFaceNormalized(*(args.target_mesh));
    return true;
}

/* Generates a squared grid of triangles whose side will be passed back in the
   gridSide parameter. The subSteps parameter represents the number of subdivision
   steps during the grid generation. */
void FilterFractal::generateGrid(CMeshO& m, int subSteps, float& gridSide)
{
    m.Clear();
    int k = (int)(pow(2, subSteps)), k2 = k+1, vertexCount = k2*k2, faceCount = 2*k*k, i=0, j=0;
    double x = .0, y = .0;
    gridSide = k2;

    vcg::tri::Allocator<CMeshO>::AddVertices(m, vertexCount);
    vcg::tri::Allocator<CMeshO>::AddFaces(m, faceCount);

    VertexIterator vi;
    VertexPointer ivp[vertexCount];
    for(vi = m.vert.begin(); vi!=m.vert.end(); ++vi)
    {
        x = (i%k2);
        y = (i/k2);
        (*vi).P() = CoordType(x, y, .0);
        ivp[i++] = &*vi;
    }

    FaceIterator fi = m.face.begin();
    int evenFace[3] = {0, 1, k2}, oddFace[3] = {1, k2+1, k2};
    for(i=0; i<k; i++)
    {
        for(j=0; j<k; j++)
        {
            (*fi).V(0) = ivp[evenFace[0]++];
            (*fi).V(1) = ivp[evenFace[1]++];
            (*fi).V(2) = ivp[evenFace[2]++];
            ++fi;
            (*fi).V(0) = ivp[oddFace[0]++];
            (*fi).V(1) = ivp[oddFace[1]++];
            (*fi).V(2) = ivp[oddFace[2]++];
            if(fi != m.face.end())++fi;
        }
        evenFace[0]++; evenFace[1]++; evenFace[2]++;
        oddFace[0]++;  oddFace[1]++;  oddFace[2]++;
    }
    vcg::tri::UpdateBounding<CMeshO>::Box(m);
}
// ---------------------------------------------------------------------

Q_EXPORT_PLUGIN(FilterFractal)

