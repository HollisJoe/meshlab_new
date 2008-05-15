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
#include <QtGui>

#include <math.h>
#include <stdlib.h>
#include "meshselect.h"
#include <vcg/complex/trimesh/update/selection.h>
#include <vcg/complex/trimesh/stat.h>

using namespace vcg;

const QString SelectionFilterPlugin::filterName(FilterIDType filter) 
{
 switch(filter)
  {
	  case FP_SELECT_ALL :		             return QString("Select All");
	  case FP_SELECT_NONE :		             return QString("Select None");
	  case FP_SELECT_INVERT :		           return QString("Invert Selection");
	  case FP_SELECT_DELETE_FACE :		     return QString("Delete Selected Faces");
	  case FP_SELECT_DELETE_FACEVERT :		 return QString("Delete Selected Faces and Vertices");
	  case FP_SELECT_ERODE :		           return QString("Erode Selection");
	  case FP_SELECT_DILATE :		           return QString("Dilate Selection");
	  case FP_SELECT_BORDER_FACES:		     return QString("Select Border Faces");
	  case FP_SELECT_BY_QUALITY :		       return QString("Select by Quality");
	  case FP_SELECT_BY_RANGE:						 return QString("Select by Coord range");
  }
  return QString("Unknown filter");
}

SelectionFilterPlugin::SelectionFilterPlugin() 
{
  typeList << 
    FP_SELECT_ALL <<
    FP_SELECT_NONE <<
    FP_SELECT_DELETE_FACE <<
    FP_SELECT_DELETE_FACEVERT <<
    FP_SELECT_ERODE <<
    FP_SELECT_DILATE <<
    FP_SELECT_BORDER_FACES <<
		FP_SELECT_INVERT <<
		FP_SELECT_BY_QUALITY
;
  
  FilterIDType tt;
  
  foreach(tt , types())
    {
      actionList << new QAction(filterName(tt), this);
      if(tt==FP_SELECT_DELETE_FACE){
            actionList.last()->setShortcut(QKeySequence (Qt::Key_Delete));
            actionList.last()->setIcon(QIcon(":/images/delete_face.png"));
      }
      if(tt==FP_SELECT_DELETE_FACEVERT){
            actionList.last()->setShortcut(QKeySequence ("Shift+Del"));
            actionList.last()->setIcon(QIcon(":/images/delete_facevert.png"));
      }
    }	  
}
SelectionFilterPlugin::~SelectionFilterPlugin() 
{
	for (int i = 0; i < actionList.count() ; i++ ) {
		delete actionList.at(i);
	}
}
void SelectionFilterPlugin::initParameterSet(QAction *action, MeshModel &m, FilterParameterSet &parlst)
{
		switch(ID(action))
		{
			case FP_SELECT_BY_QUALITY: 
					{
						std::pair<float,float> minmax =  tri::Stat<CMeshO>::ComputePerVertexQualityMinMax(m.cm); 
						float minq=minmax.first;
						float maxq=minmax.second;
						parlst.addAbsPerc("minQ",minq*.75+maxq*.25,minq,maxq,"Min Quality", "Minimum acceptable quality value");
						parlst.addAbsPerc("maxQ",minq*.25+maxq*.75,minq,maxq,"Max Quality", "Maximum acceptable quality value");
					}
					break;
		}
}
// Return true if the specified action has an automatic dialog.
// return false if the action has no parameters or has an self generated dialog.
bool SelectionFilterPlugin::autoDialog(QAction *action)
{
	 switch(ID(action))
	 {
		 case FP_SELECT_BY_QUALITY:
			 return true;
	 }
  return false;
}


bool SelectionFilterPlugin::applyFilter(QAction *action, MeshModel &m, FilterParameterSet & par, vcg::CallBackPos * cb) 
{
	CMeshO::FaceIterator fi;
	CMeshO::VertexIterator vi;
	switch(ID(action))
  {
  case FP_SELECT_DELETE_FACE : 
		for(fi=m.cm.face.begin();fi!=m.cm.face.end();++fi)
      if(!(*fi).IsD() && (*fi).IsS() ) tri::Allocator<CMeshO>::DeleteFace(m.cm,*fi);
			m.clearDataMask(MeshModel::MM_FACETOPO | MeshModel::MM_BORDERFLAG);
    break;
  case FP_SELECT_DELETE_FACEVERT : 
		tri::UpdateSelection<CMeshO>::ClearVertex(m.cm);
		tri::UpdateSelection<CMeshO>::VertexFromFaceStrict(m.cm);  
    for(fi=m.cm.face.begin();fi!=m.cm.face.end();++fi)
      if(!(*fi).IsD() && (*fi).IsS() )
					tri::Allocator<CMeshO>::DeleteFace(m.cm,*fi);
		for(vi=m.cm.vert.begin();vi!=m.cm.vert.end();++vi)
			if(!(*vi).IsD() && (*vi).IsS() )
					tri::Allocator<CMeshO>::DeleteVertex(m.cm,*vi);
			m.clearDataMask(MeshModel::MM_FACETOPO | MeshModel::MM_BORDERFLAG);
    break;
  case FP_SELECT_ALL    : tri::UpdateSelection<CMeshO>::AllFace(m.cm);     break;
  case FP_SELECT_NONE   : tri::UpdateSelection<CMeshO>::ClearFace(m.cm);   break;
  case FP_SELECT_INVERT : tri::UpdateSelection<CMeshO>::InvertFace(m.cm);  break;
  case FP_SELECT_ERODE  : tri::UpdateSelection<CMeshO>::VertexFromFaceStrict(m.cm);  
                          tri::UpdateSelection<CMeshO>::FaceFromVertexStrict(m.cm); 
  break;
  case FP_SELECT_DILATE : tri::UpdateSelection<CMeshO>::VertexFromFaceLoose(m.cm);  
                          tri::UpdateSelection<CMeshO>::FaceFromVertexLoose(m.cm); 
  break;
  case FP_SELECT_BORDER_FACES: tri::UpdateSelection<CMeshO>::FaceFromBorder(m.cm);  
  break;
  case FP_SELECT_BY_QUALITY: 
		{
			float minQ = par.getAbsPerc("minQ");	
			float maxQ = par.getAbsPerc("maxQ");	
			tri::UpdateSelection<CMeshO>::VertexFromQualityRange(m.cm, minQ, maxQ);  
			tri::UpdateSelection<CMeshO>::FaceFromVertexStrict(m.cm);
		}
	break;
  

  default:  assert(0);
  }
  return true;
}

 const QString SelectionFilterPlugin::filterInfo(FilterIDType filterId) 
 {
  switch(filterId)
  {
    case FP_SELECT_DILATE : return tr("Dilate (expand) the current set of selected faces");  
    case FP_SELECT_DELETE_FACE : return tr("Delete the current set of selected faces, vertices that remains unreferenced are not deleted.");  
    case FP_SELECT_DELETE_FACEVERT : return tr("Delete the current set of selected faces and all the vertices surrounded by that faces.");  
    case FP_SELECT_ERODE  : return tr("Erode (reduce) the current set of selected faces");  
    case FP_SELECT_INVERT : return tr("Invert the current set of selected faces");  
    case FP_SELECT_NONE   : return tr("Clear the current set of selected faces");  
    case FP_SELECT_ALL    : return tr("Select all the faces of the current mesh");  
    case FP_SELECT_BORDER_FACES    : return tr("Select all the faces on the boundary");  
    case FP_SELECT_BY_QUALITY    : return tr("Select all the faces with all the vertexes within the specified quality range");  
  }  
  assert(0);
  return QString();
 }

 const PluginInfo &SelectionFilterPlugin::pluginInfo() 
{
   static PluginInfo ai; 
   ai.Date=tr(__DATE__);
	 ai.Version = tr("0.5");
	 ai.Author = ("Paolo Cignoni");
   return ai;
 } 

const int SelectionFilterPlugin::getRequirements(QAction *action)
{
 switch(ID(action))
  {
   case FP_SELECT_BORDER_FACES:   return  MeshModel::MM_BORDERFLAG;
  }
}

Q_EXPORT_PLUGIN(SelectionFilterPlugin)
