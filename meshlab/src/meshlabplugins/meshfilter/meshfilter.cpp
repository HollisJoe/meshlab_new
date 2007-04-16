/****************************************************************************
 * MeshLab                                                           o o     *
 * A versatile mesh processing toolbox                             o     o   *
 *                                                                _   O  _   *
 * Copyright(C) 2005                                                \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
 * All rights reserved.																											 *
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
$Log$
Revision 1.94  2007/04/16 09:25:29  cignoni
** big change **
Added Layers managemnt.
Interfaces are changing again...

Revision 1.93  2007/03/27 12:20:18  cignoni
Revamped logging iterface, changed function names in automatic parameters, better selection handling

Revision 1.92  2007/03/20 16:23:10  cignoni
Big small change in accessing mesh interface. First step toward layers

Revision 1.91  2007/02/25 21:31:49  cignoni
new parameters for quadric simplification

Revision 1.90  2007/02/09 09:10:06  pirosu
Added ToolTip support for standard parameters

Revision 1.89  2007/02/08 23:46:17  pirosu
merged srcpar and par in the GetStdParameters() function

Revision 1.88  2007/02/08 16:10:15  cignoni
Added more parameters to holefilling and quadric simplification

Revision 1.87  2007/02/08 13:39:58  pirosu
Added Quadric Simplification(with textures) Filter

Revision 1.86  2007/01/19 09:12:37  cignoni
Added parameters for quality,selection and boundary preservation

Revision 1.85  2007/01/11 19:52:26  pirosu
fixed bug for QT 4.1.0/dotnet2003
removed the request of the window title to the plugin. The action description is used instead.

Revision 1.84  2007/01/11 10:38:58  cignoni
Renamed ambiguous min/max vars e added a missing return in the getParameters

Revision 1.83  2006/12/27 21:41:58  pirosu
Added improvements for the standard plugin window:
split of the apply button in two buttons:ok and apply
added support for parameters with absolute and percentage values

Revision 1.82  2006/12/13 17:37:27  pirosu
Added standard plugin window support

Revision 1.81  2006/12/11 23:50:07  cignoni
corrected border management after holefilling filter

Revision 1.80  2006/12/06 21:39:57  cignoni
Hole filling no intersection

Revision 1.79  2006/12/01 08:52:24  cignoni
Better text in the help, and corrected bug  the hole size parameter

Revision 1.78  2006/12/01 00:02:52  cignoni
Removed use of the local invertfaces function and used the function in clean.h
added progress to holefilling

Revision 1.77  2006/11/30 11:40:34  cignoni
Updated the calls to the hole filling functions to the new interface

Revision 1.76  2006/11/29 00:59:18  cignoni
Cleaned plugins interface; changed useless help class into a plain string

Revision 1.75  2006/11/27 06:57:20  cignoni
Wrong way of using the __DATE__ preprocessor symbol

Revision 1.74  2006/11/09 17:41:25  cignoni
mismatch : with ;

Revision 1.73  2006/11/09 17:25:51  cignoni
Cleaned RemoveNonManifold

Revision 1.72  2006/11/07 11:47:23  cignoni
gcc compiling issues

Revision 1.71  2006/11/07 09:22:31  cignoni
Wrote correct Help strings, and added required cleardatamask

Revision 1.70  2006/10/22 21:09:35  cignoni
Added Close Hole

Revision 1.69  2006/10/19 08:57:44  cignoni
Added working ball pivoting and normal creation

Revision 1.68  2006/10/16 08:57:29  cignoni
Added management of selection in laplacian filter

Revision 1.67  2006/10/10 21:13:08  cignoni
Added remove non manifold and quadric simplification filter.

Revision 1.66  2006/10/09 13:53:36  corsini
fix face inversion

Revision 1.65  2006/06/19 15:16:01  cignoni
Update of normals after refinement

Revision 1.64  2006/06/19 05:29:12  cignoni
changed include order for disambiguating gcc specialization template rule

Revision 1.63  2006/06/07 10:25:28  cignoni
init of bbox diag for generic lenght dialog

Revision 1.62  2006/05/25 06:24:14  cignoni
Decimator dialog no more necessary

Revision 1.61  2006/05/25 04:57:45  cignoni
Major 0.7 release. A lot of things changed. Colorize interface gone away, Editing and selection start to work.
Optional data really working. Clustering decimation totally rewrote. History start to work. Filters organized in classes.

Revision 1.60  2006/04/18 06:57:35  zifnab1974
syntax errors for gcc 3.4.5 resolved

Revision 1.59  2006/04/12 15:12:18  cignoni
Added Filter classes (cleaning, meshing etc)

Revision 1.58  2006/02/20 20:52:36  giec
replace refine and detacher dialog whit GnericELDialog

Revision 1.57  2006/02/15 22:13:01  giec
Modify the Detucher function call

Revision 1.56  2006/02/13 16:18:51  cignoni
Used the treshold param...

Revision 1.55  2006/02/06 10:18:27  mariolatronico
reverted to old behaviour, removed QEdgeLength

Revision 1.54  2006/02/05 11:22:40  mariolatronico
changed spinbox to QEdgeLength widget

Revision 1.53  2006/02/03 13:46:47  mariolatronico
spell check, remuve -> remove

Revision 1.52  2006/01/31 14:42:27  mariolatronico
fake commit, removed only some history logs

Revision 1.51  2006/01/31 14:37:40  mariolatronico
trashold -> threshold

Revision 1.50  2006/01/30 21:26:53  giec
-changed the voice of the menu from Decimator in Cluster decimator

Revision 1.49  2006/01/26 16:49:50  giec
Bugfix the new signature for decimator function call

Revision 1.48  2006/01/25 21:06:24  giec
Implemented percentile for detucher's dialog

Revision 1.47  2006/01/23 21:47:58  giec
Update detucherdialog with the diagonal percentage spinbox.

Revision 1.46  2006/01/22 16:43:32  mariolatronico
added update bbox and normal after transform dialog

Revision 1.45  2006/01/22 14:11:04  mariolatronico
added scale to unit box, move obj center. Rotate around object and origin are not working actually.

****************************************************************************/
#include <QtGui>

#include <math.h>
#include <stdlib.h>
#include "meshfilter.h"
#include <vcg/complex/trimesh/clean.h>
#include <vcg/complex/trimesh/stat.h>
#include <vcg/complex/trimesh/smooth.h>
#include <vcg/complex/trimesh/hole.h>
#include <vcg/complex/trimesh/clustering.h>
#include <vcg/complex/trimesh/update/color.h>
#include <vcg/complex/trimesh/update/position.h>
#include <vcg/complex/trimesh/update/bounding.h>
#include <vcg/complex/trimesh/update/selection.h>
#include <vcg/space/normal_extrapolation.h>

#include "refine_loop.h"

#include "../../meshlab/GLLogStream.h"
#include "../../meshlab/LogStream.h"

using namespace vcg;

void QuadricSimplification(CMeshO &m,int  TargetFaceNum, float QualityThr, bool PreserveBoundary, bool PreserveNormal, bool OptimalPlacement, bool Selected, CallBackPos *cb);
void QuadricTexSimplification(CMeshO &m,int  TargetFaceNum, float QualityThr,float c, CallBackPos *cb);

ExtraMeshFilterPlugin::ExtraMeshFilterPlugin() 
{
  typeList << FP_LOOP_SS<< 
    FP_BUTTERFLY_SS<< 
    FP_REMOVE_UNREFERENCED_VERTEX<<
    FP_REMOVE_DUPLICATED_VERTEX<< 
    FP_REMOVE_FACES_BY_AREA<<
    FP_REMOVE_FACES_BY_EDGE<<
    FP_LAPLACIAN_SMOOTH<< 
    FP_HC_LAPLACIAN_SMOOTH<< 
    FP_TWO_STEP_SMOOTH<< 
    FP_CLUSTERING<< 
    FP_QUADRIC_SIMPLIFICATION<<
	  FP_QUADRIC_TEXCOORD_SIMPLIFICATION<<
    FP_MIDPOINT<< 
    FP_REORIENT <<
    FP_INVERT_FACES<<
    FP_REMOVE_NON_MANIFOLD<<
    FP_NORMAL_EXTRAPOLATION<<
    FP_CLOSE_HOLES<<
    FP_TRANSFORM;
  
  FilterType tt;
  
  foreach(tt , types())
	    actionList << new QAction(ST(tt), this);
	
	genericELD = new GenericELDialog();
	genericELD->hide();
	
	transformDialog = new TransformDialog();
	transformDialog->hide();

	lastq_QualityThr = 0.3f;
	lastq_PreserveBoundary = false;
	lastq_PreserveNormal = false;
	lastq_OptimalPlacement = true;
	lastq_Selected = false;

	lastqtex_QualityThr = 0.3f;
	lastqtex_extratw = 0.0;
}

const ExtraMeshFilterPlugin::FilterClass ExtraMeshFilterPlugin::getClass(QAction *a)
{
  switch(ID(a))
  {
    case FP_REMOVE_UNREFERENCED_VERTEX :
    case FP_REMOVE_DUPLICATED_VERTEX :
    case FP_REMOVE_FACES_BY_AREA:
    case FP_REMOVE_FACES_BY_EDGE :
    case FP_REMOVE_NON_MANIFOLD:
      return MeshFilterInterface::Cleaning; 
    case FP_BUTTERFLY_SS :
    case FP_LOOP_SS :
    case FP_MIDPOINT :
         return MeshFilterInterface::Remeshing; 

    default : return MeshFilterInterface::Generic;
  }
}


const QString ExtraMeshFilterPlugin::ST(FilterType filter) 
{
 switch(filter)
  {
	case FP_LOOP_SS :		                  return QString("Loop Subdivision Surfaces");
	case FP_BUTTERFLY_SS :								return QString("Butterfly Subdivision Surfaces");
	case FP_REMOVE_UNREFERENCED_VERTEX :	return QString("Remove Unreferenced Vertex");
	case FP_REMOVE_DUPLICATED_VERTEX :		return QString("Remove Duplicated Vertex");
	case FP_REMOVE_FACES_BY_AREA :     		return QString("Remove Zero Area Faces");
	case FP_REMOVE_FACES_BY_EDGE :				return QString("Remove Faces with edges longer than...");
  case FP_LAPLACIAN_SMOOTH :						return QString("Laplacian Smooth");
  case FP_HC_LAPLACIAN_SMOOTH :					return QString("HC Laplacian Smooth");
  case FP_TWO_STEP_SMOOTH :	    				return QString("TwoStep Smooth");
	case FP_QUADRIC_SIMPLIFICATION :      return QString("Quadric Edge Collapse Decimation");
	case FP_QUADRIC_TEXCOORD_SIMPLIFICATION :      return QString("Quadric Edge Collapse Decimation (with texture)");
	case FP_CLUSTERING :	                return QString("Clustering decimation");
	case FP_MIDPOINT :										return QString("Midpoint Subdivision Surfaces");
	case FP_REORIENT :	                  return QString("Re-orient");
	case FP_INVERT_FACES:									return QString("Invert Faces");
	case FP_TRANSFORM:	                	return QString("Apply Transform");
	case FP_REMOVE_NON_MANIFOLD:	        return QString("Remove Non Manifold Faces");
	case FP_NORMAL_EXTRAPOLATION:	        return QString("Compute normals for point sets");
	case FP_CLOSE_HOLES:	          return QString("Close Holes");
          
    
	default: assert(0);
  }
  return QString("error!");

}

ExtraMeshFilterPlugin::~ExtraMeshFilterPlugin() {
//	delete refineDialog;
	delete genericELD;
//	delete decimatorDialog;
	if (transformDialog)
		delete transformDialog;
	for (int i = 0; i < actionList.count() ; i++ ) {
		delete actionList.at(i);
	}
}

const QString ExtraMeshFilterPlugin::Info(FilterType filterID)
{
  switch(filterID)
  {
    case FP_LOOP_SS :                   return tr("Apply Loop's Subdivision Surface algorithm. It is an approximate method which subdivide each triangle in four faces. It works for every triangle and has rules for extraordinary vertices");  
    case FP_BUTTERFLY_SS :  			      return tr("Apply Butterfly Subdivision Surface algorithm. It is an interpolated method, defined on arbitrary triangular meshes. The scheme is known to be C1 but not C2 on regular meshes");  
    case FP_MIDPOINT : 			            return tr("Splits every edge in two");  
    case FP_REMOVE_UNREFERENCED_VERTEX: return tr("Check for every vertex on the mesh if it is referenced by a face and removes it");  
    case FP_REMOVE_DUPLICATED_VERTEX : 	return tr("Check for every vertex on the mesh if there are two vertices with same coordinates and removes it");  
    case FP_REMOVE_FACES_BY_AREA : 			return tr("Removes null faces (the one with area equal to zero)");  
    case FP_REMOVE_FACES_BY_EDGE : 			return tr("Remove from the mesh all triangles whose have an edge with lenght greater or equal than a threshold");  
    case FP_LAPLACIAN_SMOOTH :          return tr("Laplacian smooth of the mesh: for each vertex it calculates the average position with nearest vertex");  
    case FP_HC_LAPLACIAN_SMOOTH : 			return tr("HC Laplacian Smoothing, extended version of Laplacian Smoothing, based on the paper of Vollmer, Mencl, and M�ller");  
    case FP_TWO_STEP_SMOOTH : 			    return tr("Two Step Smoothing, a feature preserving/enhancing fairing filter. It is based on a Normal Smoothing and vertex fitting smoothing, based on the paper of ...");  
    case FP_CLUSTERING : 			          return tr("Collapse vertices by creating a three dimensional grid enveloping the mesh and discretizes them based on the cells of this grid");  
    case FP_QUADRIC_SIMPLIFICATION: 		return tr("Simplify a mesh using a Quadric based Edge Collapse Strategy, better than clustering but slower");          
    case FP_QUADRIC_TEXCOORD_SIMPLIFICATION:return tr("Simplify a textured mesh using a Quadric based Edge Collapse Strategy, better than clustering but slower");          
    case FP_REORIENT : 			            return tr("Re-orient in a consistent way all the faces of the mesh");  
    case FP_INVERT_FACES : 			        return tr("Invert faces orientation, flip the normal of the mesh");  
    case FP_TRANSFORM : 	              return tr("Apply transformation, you can rotate, translate or scale the mesh");  
    case FP_NORMAL_EXTRAPOLATION :      return tr("Compute the normals of the vertices of a  mesh without exploiting the triangle connectivity, useful for dataset with no faces"); 
    case FP_CLOSE_HOLES :         return tr("Close holes smaller than a given threshold"); 
  }
  assert(0);
  return QString();
}


const PluginInfo &ExtraMeshFilterPlugin::Info()
{
   static PluginInfo ai;
   ai.Date=tr(__DATE__);
	 ai.Version = tr("0.5");
	 ai.Author = ("Paolo Cignoni, Mario Latronico, Andrea Venturi, Marco Pirosu");
   return ai;
 }

const int ExtraMeshFilterPlugin::getRequirements(QAction *action)
{
  switch(ID(action))
  {
    case FP_REMOVE_NON_MANIFOLD:
    case FP_LOOP_SS :
    case FP_BUTTERFLY_SS : 
    case FP_MIDPOINT :      
    case FP_CLOSE_HOLES :
           return MeshModel::MM_FACETOPO | MeshModel::MM_BORDERFLAG;
    case FP_HC_LAPLACIAN_SMOOTH:  
    case FP_LAPLACIAN_SMOOTH:     return MeshModel::MM_BORDERFLAG;
    case FP_TWO_STEP_SMOOTH:      return MeshModel::MM_VERTFACETOPO;
    case FP_REORIENT:             return MeshModel::MM_FACETOPO;
    case FP_REMOVE_UNREFERENCED_VERTEX:
    case FP_REMOVE_DUPLICATED_VERTEX:
    case FP_REMOVE_FACES_BY_AREA:
    case FP_REMOVE_FACES_BY_EDGE:
    case FP_CLUSTERING:
    case FP_TRANSFORM:
    case FP_NORMAL_EXTRAPOLATION:
    case FP_INVERT_FACES:         return 0;
    case FP_QUADRIC_SIMPLIFICATION:
	case FP_QUADRIC_TEXCOORD_SIMPLIFICATION:
		return MeshModel::MM_VERTFACETOPO | MeshModel::MM_BORDERFLAG;
    default: assert(0); 
  }
  return 0;
}



bool ExtraMeshFilterPlugin::getStdFields(QAction *action, MeshModel &m, StdParList &parlst)
{
	float maxVal;

	 switch(ID(action))
	 {
		case FP_QUADRIC_SIMPLIFICATION:
		  parlst.addFieldInt  ("TargetFaceNum","Target number of faces", (m.cm.sfn>0) ? m.cm.sfn/2 : m.cm.fn/2);
		  parlst.addFieldFloat("QualityThr","Quality threshold",lastq_QualityThr,"Quality threshold for penalizing bad shaped faces.\nThe value is in the range [0..1]\n 0 accept any kind of face (no penalties),\n 0.5  penalize faces with quality < 0.5, proportionally to their shape\n");
		  parlst.addFieldBool ("PreserveBoundary","Preserve Boundary of the mesh",lastq_PreserveBoundary,"The simplification process tries not to destroy mesh boundaries");
		  parlst.addFieldBool ("PreserveNormal","Preserve Normal",lastq_PreserveNormal,"Try to avoid face flipping effects and try to preserve the original orientation of the surface");
		  parlst.addFieldBool ("OptimalPlacement","Optimal position of simplified vertices",lastq_OptimalPlacement,"Each collapsed vertex is placed in the position minimizing the quadric error.\n It can fail (creating bad spikes) in case of very flat areas. \nIf disabled edges are collapsed onto one of the two original vertices. ");
		  parlst.addFieldBool ("Selected","Simplify only selected faces",m.cm.sfn>0,"The simplification is applied only to the selected set of faces.\n Take care of the target number of faces!");
		  break;
		case FP_QUADRIC_TEXCOORD_SIMPLIFICATION:
		  parlst.addFieldInt  ("TargetFaceNum","Target number of faces",(int)(m.cm.fn/2));
		  parlst.addFieldFloat("QualityThr","Quality threshold",lastqtex_QualityThr,"Quality threshold for penalizing bad shaped faces");
		  parlst.addFieldFloat("Extratcoordw","Texture discontinuity extra weight",lastqtex_extratw,"Additional weight for each extra Texture Coordinates for every (selected) vertex");
		  break;
		case FP_CLOSE_HOLES:
		  parlst.addFieldInt ("MaxHoleSize","Max size to be closed ",(int)30,"The size is expressed as number of edges composing the hole boundary");
		  parlst.addFieldBool("Selected","Close holes with selected faces",m.cm.sfn>0);
		  parlst.addFieldBool("NewFaceSelected","Select the newly created faces",true);
		  parlst.addFieldBool("SelfIntersection","Prevent creation of selfIntersecting faces",true);
		  break;
		case FP_LOOP_SS:
		case FP_BUTTERFLY_SS: 
		case FP_MIDPOINT: 
		case FP_REMOVE_FACES_BY_EDGE:
		case FP_CLUSTERING:
		  maxVal = m.cm.bbox.Diag();
		  parlst.addFieldAbsPerc("Threshold","Threshold",maxVal*0.01,0,maxVal);
		  parlst.addFieldBool ("Selected","Affect only selected faces",m.cm.sfn>0);
		  break;
		case FP_TWO_STEP_SMOOTH:
			parlst.addFieldInt  ("stepSmoothNum","Smoothing steps", (int) 3, "");
			parlst.addFieldFloat("normalThr","Feature Angle Threshold (deg)", (float) 60, "Specify a threshold angle for features that you want to be preserved.\nFeatures forming angles LARGER than the specified threshold will be preserved.");
			parlst.addFieldInt  ("stepNormalNum","Normal Smoothing steps", (int) 20, "");
      parlst.addFieldBool ("Selected","Affect only selected faces",m.cm.sfn>0);
		break;
		
	  default:
		  return false;
	 }

	 return true;
}
bool ExtraMeshFilterPlugin::getStdParameters(QAction *action, QWidget *parent, MeshModel &m,FilterParameter &par)
{
	 switch(ID(action))
	 {
	    case FP_QUADRIC_SIMPLIFICATION:
		case FP_QUADRIC_TEXCOORD_SIMPLIFICATION:
	    case FP_CLOSE_HOLES:
		case FP_LOOP_SS :
		case FP_BUTTERFLY_SS : 
		case FP_MIDPOINT : 
		case FP_REMOVE_FACES_BY_EDGE:
		case FP_CLUSTERING:
		case FP_TWO_STEP_SMOOTH:
		  return true;
  	 }

	 return getParameters(action,parent,m,par);
}

bool ExtraMeshFilterPlugin::getParameters(QAction *action, QWidget *parent, MeshModel &m,FilterParameter &par)
{

 switch(ID(action))
  {
	case FP_TRANSFORM:
      {
        transformDialog->setMesh(&m.cm);
		    int continueValue = transformDialog->exec();
 		    if (continueValue == QDialog::Rejected)
 			    return false;
      	Matrix44f matrix = transformDialog->getTransformation();
        par.addMatrix44("Transform",matrix);
        break;
      } 		
    case FP_REMOVE_FACES_BY_AREA:
    case FP_REMOVE_UNREFERENCED_VERTEX:
    case FP_REMOVE_DUPLICATED_VERTEX:
    case FP_REORIENT:
    case FP_INVERT_FACES:
    case FP_HC_LAPLACIAN_SMOOTH:
    case FP_LAPLACIAN_SMOOTH:
    case FP_REMOVE_NON_MANIFOLD:
    case FP_NORMAL_EXTRAPOLATION:
       return true; // no parameters
   default :assert(0);
  }
  return true;
}
	
bool ExtraMeshFilterPlugin::applyFilter(QAction *filter, MeshModel &m, FilterParameter & par, vcg::CallBackPos *cb)
{
	if( getClass(filter)==Remeshing)
  {
    if ( ! vcg::tri::Clean<CMeshO>::IsTwoManifoldFace(m.cm) ) {
      QMessageBox::warning(0, QString("Can't continue"), QString("Mesh faces not 2 manifold")); // text
      return false; // can't continue, mesh can't be processed
    }

    bool selected=par.getBool("Selected");	
    float threshold = par.getFloat("Threshold");		

    switch(ID(filter)) {
      case FP_LOOP_SS :
        RefineOddEvenE<CMeshO, OddPointLoop<CMeshO>, EvenPointLoop<CMeshO> >
          (m.cm, OddPointLoop<CMeshO>(), EvenPointLoop<CMeshO>(), threshold, selected, cb);
        break;
      case FP_BUTTERFLY_SS :
        Refine<CMeshO,MidPointButterfly<CMeshO> >
          (m.cm, MidPointButterfly<CMeshO>(), threshold, selected, cb);
        break;
      case FP_MIDPOINT :
        Refine<CMeshO,MidPoint<CMeshO> >
          (m.cm, MidPoint<CMeshO>(), threshold, selected, cb);
    }
	 vcg::tri::UpdateNormals<CMeshO>::PerVertexNormalizedPerFace(m.cm);
  }
	if (ID(filter) == FP_REMOVE_FACES_BY_EDGE ) {
    bool selected  = par.getBool("Selected");	
    float threshold = par.getFloat("Threshold");		
	  if(selected) tri::Clean<CMeshO>::RemoveFaceOutOfRangeEdgeSel<true>(m.cm,0,threshold );
         else    tri::Clean<CMeshO>::RemoveFaceOutOfRangeEdgeSel<false>(m.cm,0,threshold );
    m.clearDataMask(MeshModel::MM_FACETOPO | MeshModel::MM_BORDERFLAG);
	}

  if(filter->text() == ST(FP_REMOVE_FACES_BY_AREA) )
	  {
	    int nullFaces=tri::Clean<CMeshO>::RemoveFaceOutOfRangeArea(m.cm,0);
	    Log(GLLogStream::Info, "Removed %d null faces", nullFaces);
      m.clearDataMask(MeshModel::MM_FACETOPO | MeshModel::MM_BORDERFLAG);
	  }

  if(filter->text() == ST(FP_REMOVE_UNREFERENCED_VERTEX) )
	  {
	    int delvert=tri::Clean<CMeshO>::RemoveUnreferencedVertex(m.cm);
	    Log(GLLogStream::Info, "Removed %d unreferenced vertices",delvert);
	  }

	if(filter->text() == ST(FP_REMOVE_DUPLICATED_VERTEX) )
	  {
	    int delvert=tri::Clean<CMeshO>::RemoveDuplicateVertex(m.cm);
	    Log(GLLogStream::Info, "Removed %d duplicated vertices", delvert);
	    if (delvert != 0)
	      vcg::tri::UpdateNormals<CMeshO>::PerVertexNormalizedPerFace(m.cm);
	  }

	if(filter->text() == ST(FP_REMOVE_NON_MANIFOLD) )
	  {
	    int nonManif=tri::Clean<CMeshO>::RemoveNonManifoldFace(m.cm);
	    
			if(nonManif) Log(GLLogStream::Info, "Removed %d Non Manifold Faces", nonManif);
							else Log(GLLogStream::Info, "Mesh is two-manifold. Nothing done.", nonManif);
	  }

	if(filter->text() == ST(FP_REORIENT) )
	  {
	    bool oriented;
	    bool orientable;
	    tri::Clean<CMeshO>::IsOrientedMesh(m.cm, oriented,orientable);
	    vcg::tri::UpdateNormals<CMeshO>::PerVertexNormalizedPerFace(m.cm);
	  }

	if(filter->text() == ST(FP_LAPLACIAN_SMOOTH))
	  {
		  tri::UpdateSelection<CMeshO>::ClearVertex(m.cm);
      size_t cnt=tri::UpdateSelection<CMeshO>::VertexFromFaceStrict(m.cm);
      if(cnt>0) LaplacianSmooth(m.cm,1,true);
      else LaplacianSmooth(m.cm,1,false);
			 Log(GLLogStream::Info, "Smoothed %d vertices", cnt>0 ? cnt : m.cm.vn);	   
	    tri::UpdateNormals<CMeshO>::PerVertexNormalizedPerFace(m.cm);	    
	  }

	if(filter->text() == ST(FP_HC_LAPLACIAN_SMOOTH))
	  {
      size_t cnt=tri::UpdateSelection<CMeshO>::VertexFromFaceStrict(m.cm);
      if(cnt>0) HCSmooth(m.cm,1,true);
      else HCSmooth(m.cm,1,false);
	    tri::UpdateNormals<CMeshO>::PerVertexNormalizedPerFace(m.cm);	    
	  }

  if(filter->text() == ST(FP_TWO_STEP_SMOOTH))
	  {
      //size_t cnt=tri::UpdateSelection<CMeshO>::VertexFromFaceStrict(m.cm);
			int stepSmoothNum = par.getInt("stepSmoothNum");
			float normalThr   = cos(math::ToRad(par.getFloat("normalThr")));
      int stepNormalNum = par.getInt("stepNormalNum");

      //size_t cnt=tri::UpdateSelection<CMeshO>::VertexFromFaceStrict(m.cm);
      tri::UpdateNormals<CMeshO>::PerFaceNormalized(m.cm);
      PasoDobleSmoothFast(m.cm, stepSmoothNum, normalThr, stepNormalNum);
      tri::UpdateNormals<CMeshO>::PerVertexNormalizedPerFace(m.cm);	    
	  }

 	if(filter->text() == ST(FP_CLUSTERING))
	  {
      bool selected  = par.getBool("Selected");	
      float threshold = par.getFloat("Threshold");		
      vcg::tri::Clustering<CMeshO, vcg::tri::AverageCell<CMeshO> > Grid;
      Grid.Init(m.cm.bbox,100000,threshold);
      Grid.Add(m.cm);
      Grid.Extract(m.cm);
	    vcg::tri::UpdateNormals<CMeshO>::PerVertexNormalizedPerFace(m.cm);
      m.clearDataMask(MeshModel::MM_FACETOPO | MeshModel::MM_BORDERFLAG);
	  }

	if (filter->text() == ST(FP_INVERT_FACES) ) 
	{
	  tri::Clean<CMeshO>::FlipMesh(m.cm);
		vcg::tri::UpdateNormals<CMeshO>::PerVertexNormalizedPerFace(m.cm);
    m.clearDataMask(MeshModel::MM_FACETOPO | MeshModel::MM_BORDERFLAG);
	}

	if (filter->text() == ST(FP_TRANSFORM) ) {
    Matrix44f matrix= par.getMatrix44("Transform");		

		Log(GLLogStream::Info,
 							 transformDialog->getLog().toAscii().data());
 		
 		vcg::tri::UpdatePosition<CMeshO>::Matrix(m.cm, matrix);
		vcg::tri::UpdateNormals<CMeshO>::PerVertexNormalizedPerFace(m.cm);
		vcg::tri::UpdateBounding<CMeshO>::Box(m.cm);
	}


	if (filter->text() == ST(FP_QUADRIC_SIMPLIFICATION) ) {
  
		int TargetFaceNum = par.getInt("TargetFaceNum");		
		lastq_QualityThr = par.getFloat("QualityThr");
		lastq_PreserveBoundary = par.getBool("PreserveBoundary");
		lastq_PreserveNormal = par.getBool("PreserveNormal");
		lastq_OptimalPlacement = par.getBool("OptimalPlacement");
		lastq_Selected = par.getBool("Selected");

		QuadricSimplification(m.cm,TargetFaceNum,lastq_QualityThr, lastq_PreserveBoundary,lastq_PreserveNormal, lastq_OptimalPlacement,lastq_Selected,  cb);
		tri::UpdateNormals<CMeshO>::PerVertexNormalizedPerFace(m.cm);
		tri::UpdateBounding<CMeshO>::Box(m.cm);
	}


	if (filter->text() == ST(FP_QUADRIC_TEXCOORD_SIMPLIFICATION) ) {
  
		int TargetFaceNum = par.getInt("TargetFaceNum");		
		lastqtex_QualityThr = par.getFloat("QualityThr");
		lastqtex_extratw = par.getFloat("Extratcoordw");

	   QuadricTexSimplification(m.cm,TargetFaceNum,lastqtex_QualityThr,lastqtex_extratw,  cb);
		tri::UpdateNormals<CMeshO>::PerVertexNormalizedPerFace(m.cm);
		 tri::UpdateBounding<CMeshO>::Box(m.cm);
	}

  if (filter->text() == ST(FP_NORMAL_EXTRAPOLATION) ) {
    NormalExtrapolation<vector<CVertexO> >::ExtrapolateNormals(m.cm.vert.begin(), m.cm.vert.end(), 10,-1,NormalExtrapolation<vector<CVertexO> >::IsCorrect,  cb);
	}

	if(filter->text() == ST(FP_CLOSE_HOLES))
	  {
      size_t OriginalSize= m.cm.face.size();
      int MaxHoleSize = par.getInt("MaxHoleSize");		
	    bool SelectedFlag = par.getBool("Selected");
      bool SelfIntersectionFlag = par.getBool("SelfIntersection");
      bool NewFaceSelectedFlag = par.getBool("NewFaceSelected");
		  
      if( SelfIntersectionFlag ) 
          tri::Hole<CMeshO>::EarCuttingIntersectionFill<tri::SelfIntersectionEar< CMeshO> >(m.cm,MaxHoleSize,SelectedFlag);		
      else 
          tri::Hole<CMeshO>::EarCuttingFill<vcg::tri::MinimumWeightEar< CMeshO> >(m.cm,MaxHoleSize,SelectedFlag,cb);

      assert(tri::Clean<CMeshO>::IsFFAdjacencyConsistent(m.cm));
      tri::UpdateNormals<CMeshO>::PerVertexNormalized(m.cm);	    

      // hole filling filter does not correctly update the border flags (but the topology is still ok!) 
      m.clearDataMask(MeshModel::MM_BORDERFLAG);
      if(NewFaceSelectedFlag)
      {
        tri::UpdateSelection<CMeshO>::ClearFace(m.cm);   
        for(int i=OriginalSize;i<m.cm.face.size();++i)
          if(!m.cm.face[i].IsD()) m.cm.face[i].SetS();
      }
      //tri::UpdateTopology<CMeshO>::FaceFace(m.cm);	    
	  }

	return true;
}
Q_EXPORT_PLUGIN(ExtraMeshFilterPlugin)
