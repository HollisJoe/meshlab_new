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

#include "filter_plymc.h"
#include <vcg/complex/trimesh/append.h>
#include <wrap/io_trimesh/export_vmi.h>

#include "plymc.h"
#include "simplemeshprovider.h"

using namespace vcg;

// Constructor usually performs only two simple tasks of filling the two lists 
//  - typeList: with all the possible id of the filtering actions
//  - actionList with the corresponding actions. If you want to add icons to your filtering actions you can do here by construction the QActions accordingly

PlyMCPlugin::PlyMCPlugin() 
{ 
    typeList << FP_PLYMC;
  
  foreach(FilterIDType tt , types())
	  actionList << new QAction(filterName(tt), this);
}

// ST() must return the very short string describing each filtering action 
// (this string is used also to define the menu entry)
 QString PlyMCPlugin::filterName(FilterIDType filterId) const 
{
  switch(filterId) {
        case FP_PLYMC :  return QString("VCG Reconstruction");
		default : assert(0); 
	}
}

// Info() must return the longer string describing each filtering action 
// (this string is used in the About plugin dialog)
 QString PlyMCPlugin::filterInfo(FilterIDType filterId) const
{
  switch(filterId) {
        case FP_PLYMC :  return QString("The surface reconstrction algorithm that have been used for a long time inside the ISTI-Visual Computer Lab. It is mostly a variant of the Curless et al.  approach with some original weighting schemes, a different expansion rule, a new aprroach to hole filling through voluem filling.");
		default : assert(0); 
	}
	return QString("Unknown Filter");
}

// The FilterClass describes in which generic class of filters it fits. 
// This choice affect the submenu in which each filter will be placed 
// More than a single class can be choosen.
 PlyMCPlugin::FilterClass PlyMCPlugin::getClass(QAction *a)
{
  switch(ID(a))
	{
        case FP_PLYMC :  return MeshFilterInterface::Remeshing;
		default : assert(0); 
	}
	return MeshFilterInterface::Generic;
}

// This function define the needed parameters for each filter. Return true if the filter has some parameters
// it is called every time, so you can set the default value of parameters according to the mesh
// For each parmeter you need to define, 
// - the name of the parameter, 
// - the string shown in the dialog 
// - the default value
// - a possibly long string describing the meaning of that parameter (shown as a popup help in the dialog)
void PlyMCPlugin::initParameterSet(QAction *action,MeshModel &m, RichParameterSet & parlst) 
{
     switch(ID(action))
     {
        case FP_PLYMC :
          parlst.addParam(new RichAbsPerc("voxSize",m.cm.bbox.Diag()/100.0,0,m.cm.bbox.Diag(),"Voxel Side", "VoxelSide"));
          parlst.addParam(new RichInt("subdiv",1,"SubVol Splitting","The level of recursive splitting of the subvolume reconstruction process. '2'' means that a 2x2x2 partition is created and the reconstruction proess generate 8 matching meshes. It is useful for reconsruction at very high resolution."));
          parlst.addParam(new RichFloat("geodesic",3.0,"Geodesic Weighting"," "));
          parlst.addParam(new RichBool("openResult",true,"Show Result","if not checked the result is only saved into the current directory"));
          parlst.addParam(new RichInt("smoothNum",1,"Volume Laplacian iter","the level of recursive splitting of the volume"));
          parlst.addParam(new RichInt("wideNum",3,"Widening","the level of recursive splitting of the volume"));
          parlst.addParam(new RichBool("mergeColor",true,"Merge Color","the level of recursive splitting of the volume"));

        break;
		default : assert(0); 
	}
}

// The Real Core Function doing the actual mesh processing.
bool PlyMCPlugin::applyFilter(QAction */*filter*/, MeshDocument &md, RichParameterSet & par, vcg::CallBackPos * /*cb*/)
{
    srand(time(NULL));

    tri::PlyMC<SMesh,SimpleMeshProvider<SMesh> > pmc;
    tri::PlyMC<SMesh,SimpleMeshProvider<SMesh> >::Parameter &p = pmc.p;

    int subdiv=par.getInt("subdiv");

    p.IDiv=Point3i(subdiv,subdiv,subdiv);
    p.IPosS=Point3i(0,0,0);
    p.IPosE[0]=p.IDiv[0]-1; p.IPosE[1]=p.IDiv[1]-1; p.IPosE[2]=p.IDiv[2]-1;
    printf("AutoComputing all subVolumes on a %ix%ix%i\n",p.IDiv[0],p.IDiv[1],p.IDiv[2]);

    p.VoxSize=par.getAbsPerc("voxSize");
    p.QualitySmoothVox = par.getFloat("geodesic");
    p.SmoothNum = par.getInt("smoothNum");
    p.WideNum = par.getInt("wideNum");
    p.NCell=0;
    p.MergeColor=p.VertSplatFlag=par.getBool("mergeColor");
    foreach(MeshModel*mm, md.meshList)
    {
        if(mm->visible)
        {
            SMesh sm;
            mm->updateDataMask(MeshModel::MM_FACEQUALITY);
            tri::Append<SMesh,CMeshO>::Mesh(sm, mm->cm);
            tri::UpdatePosition<SMesh>::Matrix(sm, mm->cm.Tr,true);
            tri::UpdateBounding<SMesh>::Box(sm);
            //QString mshTmpPath=QDir::tempPath()+QString("/")+QString(mm->shortName())+QString(".vmi");
            QString mshTmpPath=QString("__TMP")+QString(mm->shortName())+QString(".vmi");
            qDebug("Saving tmp file %s",qPrintable(mshTmpPath));
            int retVal = tri::io::ExporterVMI<SMesh>::Save(sm,qPrintable(mshTmpPath) );
            if(retVal!=0)
            {
                qDebug("Failed to write vmi temp file %s",qPrintable(mshTmpPath));
                return false;
            }
            pmc.MP.AddSingleMesh(qPrintable(mshTmpPath));
        }
    }

    pmc.Process();

    if(par.getBool("openResult"))
    {
    for(size_t i=0;i<p.OutNameVec.size();++i)
        {
            MeshModel *mp=md.addNewMesh(p.OutNameVec[i].c_str());
            tri::io::ImporterPLY<CMeshO>::Open(mp->cm,p.OutNameVec[i].c_str());
            tri::UpdateBounding<CMeshO>::Box(mp->cm);
            tri::UpdateNormals<CMeshO>::PerVertexPerFace(mp->cm);
        }
    }
//
//    for(int i=0;i<pmc.MP.size();++i)
//            QFile::remove(pmc.MP.MeshName(i).c_str());

   return true;
}

Q_EXPORT_PLUGIN(PlyMCPlugin)
