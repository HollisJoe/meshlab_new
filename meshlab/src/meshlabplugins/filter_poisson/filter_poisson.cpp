/****************************************************************************
* MeshLab                                                           o o     *
* A versatile mesh processing toolbox                             o     o   *
*                                                                _   O  _   *
* Copyright(C) 2005                                                \/)\/    *
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
/****************************************************************************
  History
$Log: samplefilter.cpp,v $
Revision 1.3  2006/11/29 00:59:20  cignoni
Cleaned plugins interface; changed useless help class into a plain string

Revision 1.2  2006/11/27 06:57:21  cignoni
Wrong way of using the __DATE__ preprocessor symbol

Revision 1.1  2006/09/25 09:24:39  e_cerisoli
add samplefilter

****************************************************************************/

#include <QtGui>

#include <math.h>
#include <stdlib.h>
#include <time.h>

#include <meshlab/meshmodel.h>
#include <meshlab/interfaces.h>

#include <vcg/complex/trimesh/clean.h>
#include <vcg/complex/trimesh/update/normal.h>
#include <vcg/complex/trimesh/update/bounding.h>
#include <vcg/complex/trimesh/create/platonic.h>

#include "filter_poisson.h"
#include "src/Geometry.h"
#include "src/PoissonParam.h"

using namespace std;
using namespace vcg;

int Execute2(PoissonParam &Par, vector<Point3D<float> > Pts, vector<Point3D<float> > Nor, 	CoredVectorMeshData &mesh, Point3D<float> &newCenter, float &newScale, vcg::CallBackPos *cb );


// Constructor usually performs only two simple tasks of filling the two lists 
//  - typeList: with all the possible id of the filtering actions
//  - actionList with the corresponding actions. If you want to add icons to your filtering actions you can do here by construction the QActions accordingly

PoissonPlugin::PoissonPlugin() 
{ 
	typeList << FP_POISSON_RECON;
  
  foreach(FilterIDType tt , types())
	  actionList << new QAction(filterName(tt), this);
}

// ST() must return the very short string describing each filtering action 
// (this string is used also to define the menu entry)
const QString PoissonPlugin::filterName(FilterIDType filterId) 
{
  switch(filterId) {
		case FP_POISSON_RECON :  return QString("Poisson Reconstruction"); 
		default : assert(0); 
	}
	return QString("Error: Unknown Filter"); 
}

// Info() must return the longer string describing each filtering action 
// (this string is used in the About plugin dialog)
const QString PoissonPlugin::filterInfo(FilterIDType filterId)
{
  switch(filterId) {
		case FP_POISSON_RECON :  return QString("Use the points and normal to build a surface using the Poisson Surface reconstruction approach."); 
		default : assert(0); 
	}
	return QString("Error: Unknown Filter"); 
}

const PluginInfo &PoissonPlugin::pluginInfo()
{
   static PluginInfo ai;
   ai.Date=tr(__DATE__);
	 ai.Version = tr("1.0");
	 ai.Author = ("Paolo Cignoni (simply ported the original code of Michael Kazhdan and Matthew Bolitho)");
   return ai;
 }

// This function define the needed parameters for each filter. Return true if the filter has some parameters
// it is called every time, so you can set the default value of parameters according to the mesh
// For each parmeter you need to define, 
// - the name of the parameter, 
// - the string shown in the dialog 
// - the default value
// - a possibly long string describing the meaning of that parameter (shown as a popup help in the dialog)
void PoissonPlugin::initParameterSet(QAction *action,MeshModel &m, FilterParameterSet & parlst) 
//void PoissonPlugin::initParList(QAction *action, MeshModel &m, FilterParameterSet &parlst)
{
	 switch(ID(action))	 {
		case FP_POISSON_RECON :  
 		  //parlst.addBool ("RecomputeNormals",
			//								false,
			//								"Recompute normals",
			//								"Do not use the current normals, but recompute them from scratch considering the vertices of the mesh as an unstructured point cloud.");
 		  //parlst.addBool ("UseConfidence",
			//								true,
			//								"Use Quality",
			//								"Use the per vertex quality as a confidence value\n");
			parlst.addInt ("OctDepth",
											6,
											"Octree Depth",
											"Set the depth of the Octree used for extracting the final surface. Suggested range 5..10. Higher numbers mean higher precision in the reconstruction but also higher processing times. Be patient.\n");
			parlst.addInt ("SolverDivide",
											6,
											"Solver Divide",
											"This integer argument specifies the depth at which a block Gauss-Seidel solver is used to solve the Laplacian equation.\n"
											"Using this parameter helps reduce the memory overhead at the cost of a small increase in reconstruction time. \n"
											"In practice, the authors have found that for reconstructions of depth 9 or higher a subdivide depth of 7 or 8 can reduce the memory usage.\n"
											"The default value is 8.\n");
			parlst.addFloat ("SamplesPerNode",
											1.0,
											"Samples per Node",
											"This floating point value specifies the minimum number of sample points that should fall within an octree node as the octree\n"
											"construction is adapted to sampling density. For noise-free samples, small values in the range [1.0 - 5.0] can be used.\n"
											"For more noisy samples, larger values in the range [15.0 - 20.0] may be needed to provide a smoother, noise-reduced, reconstruction.\n"
											"The default value is 1.0.");
	
			break;
											
		default : assert(0); 
	}
}

// The Real Core Function doing the actual mesh processing.
// Move Vertex of a random quantity
bool PoissonPlugin::applyFilter(QAction *filter, MeshModel &m, FilterParameterSet & par, vcg::CallBackPos *cb)
{
//	MeshModel &m=*md->mm();

  vector<Point3D<float> > Pts(m.cm.vn);
	vector<Point3D<float> > Nor(m.cm.vn); 	
	CoredVectorMeshData mesh;

	int cnt=0;
	CMeshO::VertexIterator vi;
	for(vi=m.cm.vert.begin(); vi!=m.cm.vert.end(); ++vi)
	if(!(*vi).IsD()){
			(*vi).N().Normalize();
			for(int ii=0;ii<3;++ii){
					Pts[cnt].coords[ii]=(*vi).P()[ii];
					Nor[cnt].coords[ii]=(*vi).N()[ii];
			}
			++cnt;
		}
	assert(cnt==m.cm.vn);
	// Log function dump textual info in the lower part of the MeshLab screen. 
	PoissonParam pp;
	pp.Depth=par.getInt("OctDepth");
	pp.SamplesPerNode = par.getFloat("SamplesPerNode");
	pp.SolverDivide=par.getInt("SolverDivide");
	Point3D<float> center;
	float scale;
	
	int ret= Execute2(pp, Pts, Nor, mesh,center,scale,cb);
	mesh.resetIterator();
	Log(0,"Successfully created a mesh of %i vert and %i faces",mesh.outOfCorePointCount()+mesh.inCorePoints.size(),mesh.triangleCount());
	
	m.cm.Clear();
	
	tri::Allocator<CMeshO>::AddVertices(m.cm,mesh.outOfCorePointCount()+mesh.inCorePoints.size());
	tri::Allocator<CMeshO>::AddFaces(m.cm,mesh.triangleCount());

  Point3D<float> p;
	int i;
	for (i=0; i < int(mesh.inCorePoints.size()); i++){
		p=mesh.inCorePoints[i];
		m.cm.vert[i].P()[0] = p.coords[0]*scale+center.coords[0];
		m.cm.vert[i].P()[1] = p.coords[1]*scale+center.coords[1];
		m.cm.vert[i].P()[2] = p.coords[2]*scale+center.coords[2];
		}
	for (int ii=0; ii < mesh.outOfCorePointCount(); ii++){
		mesh.nextOutOfCorePoint(p);
		m.cm.vert[ii+i].P()[0] = p.coords[0]*scale+center.coords[0];
		m.cm.vert[ii+i].P()[1] = p.coords[1]*scale+center.coords[1];
		m.cm.vert[ii+i].P()[2] = p.coords[2]*scale+center.coords[2];
	}

TriangleIndex tIndex;
int inCoreFlag;
int nr_faces=mesh.triangleCount();	
for (i=0; i < nr_faces; i++){
		//
		// create and fill a struct that the ply code can handle
		//
		mesh.nextTriangle(tIndex,inCoreFlag);
		if(!(inCoreFlag & CoredMeshData::IN_CORE_FLAG[0])){tIndex.idx[0]+=int(mesh.inCorePoints.size());}
		if(!(inCoreFlag & CoredMeshData::IN_CORE_FLAG[1])){tIndex.idx[1]+=int(mesh.inCorePoints.size());}
		if(!(inCoreFlag & CoredMeshData::IN_CORE_FLAG[2])){tIndex.idx[2]+=int(mesh.inCorePoints.size());}
		for(int j=0; j < 3; j++)
		{
			m.cm.face[i].V(j) = &m.cm.vert[tIndex.idx[j]];
		}
		//ply_put_element(ply, (void *) &ply_face);
		//delete[] ply_face.vertices;
	}  // for, write faces


//	for(int i=0;i<mesh.inCorePoints.size();++i){
//		mesh.triangles[i].idx[0]+=mesh.inCorePoints.size();
//		mesh.triangles[i].idx[1]+=mesh.inCorePoints.size();
//		mesh.triangles[i].idx[2]+=mesh.inCorePoints.size();
//		}
//	Build(m.cm,mesh.inCorePoints,mesh.triangles);
	Log(0,"Successfully created a mesh of %i faces",m.cm.vn);
	
	vcg::tri::UpdateBounding<CMeshO>::Box(m.cm);
  vcg::tri::UpdateNormals<CMeshO>::PerVertexNormalizedPerFace(m.cm);
  
	return true;
}


Q_EXPORT_PLUGIN(PoissonPlugin)
