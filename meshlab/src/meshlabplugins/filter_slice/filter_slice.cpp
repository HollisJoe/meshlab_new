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

#include "filter_slice.h"
//#include <stdlib.h>
#include <vcg/complex/intersection.h>
#include <vcg/complex/edgemesh/update/bounding.h>
#include <vcg/complex/trimesh/update/bounding.h>
#include <vcg/complex/trimesh/update/flag.h>
#include <vcg/complex/trimesh/refine.h>
#include <vcg/complex/trimesh/clean.h>
#include <vcg/complex/trimesh/append.h>
#include <vcg/complex/trimesh/update/selection.h>
#include <algorithm>


#include "filter_slice_functors.h"
#include <wrap/gl/glu_tesselator.h>
#include <vcg/complex/trimesh/allocate.h>

#include "kdtree.h"

using namespace std;
using namespace vcg;

// Constructor usually performs only two simple tasks of filling the two lists
//  - typeList: with all the possible id of the filtering actions
//  - actionList with the corresponding actions. If you want to add icons to your filtering actions you can do here by construction the QActions accordingly

ExtraFilter_SlicePlugin::ExtraFilter_SlicePlugin ()
{
	typeList << FP_PARALLEL_PLANES
					 <<FP_RECURSIVE_SLICE;

  foreach(FilterIDType tt , types())
	  actionList << new QAction(filterName(tt), this);
}

// ST() must return the very short string describing each filtering action
// (this string is used also to define the menu entry)
const QString ExtraFilter_SlicePlugin::filterName(FilterIDType filterId)
{
  switch(filterId) {
		case FP_PARALLEL_PLANES :  return QString("Cross section parallel planes");
		case FP_RECURSIVE_SLICE :  return QString("Cross section recursive");
		default : assert(0);
	}
  return QString("error!");
}

// Info() must return the longer string describing each filtering action
// (this string is used in the About plugin dialog)
const QString ExtraFilter_SlicePlugin::filterInfo(FilterIDType filterId)
{
 switch(filterId) {
		case FP_PARALLEL_PLANES :  return QString("Export one or more cross sections of the current mesh relative to one of the XY, YZ or ZX axes in svg format. By default, the cross-section goes through the middle of the object (Cross plane offset == 0).");
    case FP_RECURSIVE_SLICE :  return QString("Create a Sliceform model to cut out and assemble");
		default : assert(0);
	}
  return QString("error!");
}

// This function define the needed parameters for each filter. Return true if the filter has some parameters
// it is called every time, so you can set the default value of parameters according to the mesh
// For each parmeter you need to define,
// - the name of the parameter,
// - the string shown in the dialog
// - the default value
// - a possibly long string describing the meaning of that parameter (shown as a popup help in the dialog)
//void ExtraSamplePlugin::initParameterSet(QAction *action,MeshModel &m, FilterParameterSet & parlst)
void ExtraFilter_SlicePlugin::initParameterSet(QAction *filter, MeshModel &m, FilterParameterSet &parlst)
{
  vcg::tri::UpdateBounding<CMeshO>::Box(m.cm);
	parlst.addEnum   ("planeAxis", 0, QStringList()<<"YZ Axis"<<"XZ Axis"<<"XY Axis", tr("Axis"), tr("The Slicing plane will be done perpendicular to the axis"));
	switch(ID(filter))
	{
    case FP_PARALLEL_PLANES :
    {
      parlst.addFloat  ("planeOffset", 0.0, "Cross plane offset", "Specify an offset of the cross-plane. The offset corresponds to the distance between the center of the object and the point where the plan crosses it. By default (Cross plane offset == 0), the plane crosses the center of the object, so the offset can be positive or negetive");
      // BBox min=0, BBox center=1, Origin=2
      parlst.addEnum   ("relativeTo",0,QStringList()<<"Bounding box min"<<"Bounding box Center"<<"Origin","plane reference","Specify the reference from which the planes are shifted");
      //parlst.addBool   ("absOffset",false,"Absolute offset", "if true the above offset is absolute is relative to the origin of the coordinate system, if false the offset is relative to the center of the bbox.");
      //parlst.addAbsPerc("planeDist", 0.0,0,m.cm.bbox.Diag(), "Distance between planes", "Step value between each plane for automatically generating cross-sections. Should be used with the bool selection above.");
			parlst.addEnum	 ("units",0,QStringList()<<"cm"<<"in","Units","units in which the objects is measured");
			parlst.addFloat	 ("length",29,"Length","Length of the object referred to the units specified above");
			parlst.addFloat  ("eps",0.3,"Medium thickness","Thickness of the medium where the pieces will be cut away");
      parlst.addInt    ("planeNum", 10, "Number of Planes", "Step value between each plane for automatically generating cross-sections. Should be used with the bool selection above.");

			QStringList nn=QString(m.fileName.c_str()).split("/");

			QString name=nn.last().left(nn.last().lastIndexOf("."));
			if (name=="")
				name="Slice";
      parlst.addString ("filename", name, "filename","Name of the svg files and of the folder contaning them, it is automatically created in the Sample folder of the Meshlab tree");
      parlst.addBool   ("singleFile", true, "Single SVG","Automatically generate a series of cross-sections along the whole length of the object and store each plane in a separate SVG file. The distance between each plane is given by the step value below");
			parlst.addBool   ("capBase",true,"Cap input mesh holes","Eventually cap the holes of the input mesh before applying the filter");
			parlst.addBool   ("hideBase",true,"Hide Original Mesh","Hide the Original Mesh");
			parlst.addBool   ("hideSlices",true,"Hide Slices","Hide the Generated Slices");
			parlst.addBool   ("hidePlanes",false,"Hide Planes","Hide the Generated Slicing Planes");
    }
      break;
    case FP_RECURSIVE_SLICE:
			parlst.addBool   ("capBase",true,"Cap input mesh holes","Eventually cap the holes of the input mesh before applying the filter");
			parlst.addEnum	 ("units",0,QStringList()<<"cm"<<"in","Units","units in which the objects is measured");
			parlst.addFloat	 ("length",29,"Length","Length of the object referred to the units specified above");
			parlst.addFloat  ("eps",0.3,"Medium thickness","Thickness of the medium where the pieces will be cut away");
			parlst.addInt		 ("iter",2,"iterations","iterations");
      break;
    default : assert(0);
  }
}

// The Real Core Function doing the actual mesh processing.
// Move Vertex of a random quantity
bool ExtraFilter_SlicePlugin::applyFilter(QAction *filter, MeshDocument &m, FilterParameterSet &parlst, vcg::CallBackPos *cb)
{
	if(parlst.getBool("capBase"))
	{
		MeshModel* cap= new MeshModel();
		capHole(m.mm(),cap);
		tri::Append<CMeshO,CMeshO>::Mesh(m.mm()->cm, cap->cm);
		m.mm()->updateDataMask(MeshModel::MM_FACEFACETOPO | MeshModel::MM_FACEFLAGBORDER);
		tri::UpdateTopology<CMeshO>::FaceFace(m.mm()->cm);
		delete cap;
	}
	float length=parlst.getFloat("length");
	float eps=parlst.getFloat("eps");
	eps=m.mm()->cm.bbox.Diag()*(eps/length);
	switch(ID(filter))
	{
		case FP_PARALLEL_PLANES :
		{
			Point3f planeAxis(0,0,0);
			int axisIndex = parlst.getEnum("planeAxis");
			assert(axisIndex >=0 && axisIndex <3);
			planeAxis[axisIndex] = 1.0f;
			//float planeDist = parlst.getAbsPerc("planeDist");
			float planeDist=0;
			float planeOffset = parlst.getFloat("planeOffset");
			int	planeNum = parlst.getInt("planeNum");
			int units=parlst.getEnum("units");


			//bool absOffset = parlst.getBool("absOffset");
      int reference=parlst.getEnum("relativeTo");
			Point3f planeCenter;
			Plane3f slicingPlane;
			pr.numCol=max((int)sqrt(planeNum*1.0f),2);
			pr.numRow=max(planeNum/pr.numCol,2);
			//da rivedere per le misure
			pr.sizeCm=Point2f(4,4);
			pr.projDir = planeAxis;
			pr.projCenter =  m.mm()->cm.bbox.Center();
			pr.scale = 2.0/m.mm()->cm.bbox.Diag();
			pr.lineWidthPt=200;

			bool hideBase=parlst.getBool("hideBase");
			bool hideSlices=parlst.getBool("hideSlices");
			bool hidePlanes=parlst.getBool("hidePlanes");
			vector<MyEdgeMesh*> ev;
			if (hideBase)
				m.mm()->visible=false;
			Box3f bbox=m.mm()->cm.bbox;
			MeshModel* base=m.mm();
			MeshModel* orig=m.mm();
			for(int i=0;i<planeNum;++i)
			{
				if (!tri::Clean<CMeshO>::IsTwoManifoldFace(base->cm) || (tri::Clean<CMeshO>::CountNonManifoldVertexFF(base->cm,false) != 0))
				{
					Log(0,"Mesh is not two manifold, cannot apply filter");
					return false;
				}

				QString s;
				s.sprintf("calculating slice %d",i+1);
				cb((i+1)*100.0f/planeNum,s.toStdString().c_str());
				switch(reference)
				{
				  case 2:
						planeCenter = planeAxis*planeOffset;  //origin
            break;
				  case 0:
						planeCenter = bbox.min+planeAxis*planeOffset*(bbox.Diag()/2.0);  //bbox min
						if (planeNum!=1)
							planeDist=(bbox.Dim()*planeAxis)/(planeNum-1);
            break;
				  case 1:
						planeCenter = bbox.Center()+ planeAxis*planeOffset*(bbox.Diag()/2.0);  //bbox center
						if (planeNum!=1)
							planeDist=(bbox.Dim()*planeAxis)/(2*(planeNum-1));
            break;
				}

				planeCenter+=planeAxis*planeDist*i;
				slicingPlane.Init(planeCenter,planeAxis);
				//clear the selection flags
        vcg::tri::UpdateFlags<CMeshO>::FaceBorderFromNone(base->cm);

        SlicedEdge<CMeshO> slicededge(slicingPlane);
        SlicingFunction<CMeshO> slicingfunc(slicingPlane);
        //after the RefineE call, the mesh will be half vertices selected
        vcg::RefineE<CMeshO, SlicingFunction<CMeshO>, SlicedEdge<CMeshO> >
             (base->cm, slicingfunc, slicededge, false, cb);
        vcg::tri::UpdateNormals<CMeshO>::PerVertexPerFace(base->cm);

				MeshModel *slice1= new MeshModel();
				m.meshList.push_back(slice1);
				QString layername;
				layername.sprintf("slice_%d-%d.ply",i,i+1);
				slice1->fileName = layername.toStdString().c_str();								// mesh name
				slice1->updateDataMask(MeshModel::MM_FACEFACETOPO | MeshModel::MM_FACEFLAGBORDER);
				vcg::tri::UpdateSelection<CMeshO>::VertexFromQualityRange(base->cm,VERTEX_LEFT,VERTEX_LEFT);
        vcg::tri::UpdateSelection<CMeshO>::FaceFromVertexLoose(base->cm);
				if (hideSlices)
					slice1->visible=false;

        createSlice(base,slice1);
				vcg::tri::UpdateFlags<CMeshO>::FaceBorderFromNone(slice1->cm);

				MeshModel* cap= new MeshModel();
				m.meshList.push_back(cap);
				layername.sprintf("plane_%d.ply",i+1);
				cap->fileName = layername.toStdString().c_str();								// mesh name
				cap->updateDataMask(MeshModel::MM_FACEFACETOPO | MeshModel::MM_FACEFLAGBORDER);
				capHole(slice1,cap);
				if (eps!=0)
				{
					MeshModel* dup= new MeshModel();
					m.meshList.push_back(dup);
					layername.sprintf("plane_%d_extruded.ply",i+1);
					dup->fileName = layername.toStdString().c_str();								// mesh name
					dup->updateDataMask(MeshModel::MM_FACEFACETOPO | MeshModel::MM_FACEFLAGBORDER);
					extrude(cap, dup, eps, planeAxis);
				}
				if (hidePlanes)
					cap->visible=false;
				tri::Append<CMeshO,CMeshO>::Mesh(slice1->cm, cap->cm);
				tri::Clean<CMeshO>::RemoveDuplicateVertex(slice1->cm);
				vcg::tri::UpdateTopology<CMeshO>::FaceFace(slice1->cm);
				vcg::tri::UpdateNormals<CMeshO>::PerVertexPerFace(slice1->cm);

				MeshModel* slice2= new MeshModel();
				m.meshList.push_back(slice2);
				layername.sprintf("slice_%d-%d.ply",i+1,i+2);
				slice2->fileName = layername.toStdString().c_str();								// mesh name
				slice2->updateDataMask(MeshModel::MM_FACEFACETOPO | MeshModel::MM_FACEFLAGBORDER);
				vcg::tri::UpdateSelection<CMeshO>::VertexFromQualityRange(base->cm,VERTEX_RIGHT,VERTEX_RIGHT);
				vcg::tri::UpdateSelection<CMeshO>::FaceFromVertexLoose(base->cm);
				createSlice(base,slice2);
				vcg::tri::UpdateFlags<CMeshO>::FaceBorderFromNone(slice2->cm);
				tri::Append<CMeshO,CMeshO>::Mesh(slice2->cm, cap->cm);
				tri::Clean<CMeshO>::RemoveDuplicateVertex(slice2->cm);
				vcg::tri::UpdateTopology<CMeshO>::FaceFace(slice2->cm);
				vcg::tri::UpdateNormals<CMeshO>::PerVertexPerFace(slice2->cm);
				if (hideSlices)
					slice2->visible=false;
				if (i!=0)
					m.delMesh(base);
				base=slice2;
	      //this is used to generate svd slices
				MyEdgeMesh *edgeMesh = new MyEdgeMesh();
				vcg::Intersection<CMeshO, MyEdgeMesh, float>(orig->cm, slicingPlane , *edgeMesh);
				vcg::edg::UpdateBounding<MyEdgeMesh>::Box(*edgeMesh);
				ev.push_back(edgeMesh);
			}

			QString fname=parlst.getString("filename");
			if(fname=="")
        fname="Slice.svg";
      if (!fname.endsWith(".svg"))
        fname+=".svg";
			vcg::edg::io::ExporterSVG<MyEdgeMesh>::Save(ev, fname.toStdString().c_str(), pr);
		}
		break;
		case FP_RECURSIVE_SLICE:
		{
			KDTree<CMeshO> *kdt=new KDTree<CMeshO>(&m,&m.mm()->cm,eps);
			int iter=parlst.getInt("iter");
			for(int i=0;i<iter;i++)
			{
				kdt->Slice();
			}
			vcg::tri::UpdateBounding<CMeshO>::Box(m.mm()->cm);
    }
    break;
	}
	return true;
}

void ExtraFilter_SlicePlugin::extrude(MeshModel* orig, MeshModel* dest, float eps, Point3f planeAxis)
{

	tri::Append<CMeshO,CMeshO>::Mesh(dest->cm, orig->cm);
	tri::UpdateTopology<CMeshO>::FaceFace(dest->cm);
	//create the clone, move it eps/2 on the left and invert its normals
	for (int i=0;i<dest->cm.vert.size();i++)
		dest->cm.vert[i].P()-=planeAxis*eps/2;
	tri::Clean<CMeshO>::FlipMesh(dest->cm);
	vcg::tri::UpdateTopology<CMeshO>::FaceFace(dest->cm);
	vcg::tri::UpdateNormals<CMeshO>::PerVertexPerFace(dest->cm);
	//find the border outlines
	std::vector< std::vector<Point3f> > outlines;
  std::vector<Point3f> outline;
  vcg::tri::UpdateFlags<CMeshO>::VertexClearV(dest->cm);
  vcg::tri::UpdateFlags<CMeshO>::FaceBorderFromNone(dest->cm);
  int nv=0;
  for(int i=0;i<dest->cm.face.size();i++)
  {
    for (int j=0;j<3;j++)
    if (!dest->cm.face[i].IsV() && dest->cm.face[i].IsB(j))
    {
      CFaceO* startB=&(dest->cm.face[i]);
      vcg::face::Pos<CFaceO> p(startB,j);
	    do
      {
				p.V()->SetV();
        outline.push_back(p.V()->P());
        p.NextB();
				nv++;
      }
      while(!p.V()->IsV());
      outlines.push_back(outline);
      outline.clear();
    }
  }
	if (nv<2) return;
	//I have at least 3 vertexes
	MeshModel* tempMesh= new MeshModel();
	tri::Append<CMeshO,CMeshO>::Mesh(tempMesh->cm, orig->cm);
	for (int i=0;i<tempMesh->cm.vert.size();i++)
		tempMesh->cm.vert[i].P()+=planeAxis*eps/2;
	//create the clone and move it eps/2 on the right
	tri::Append<CMeshO,CMeshO>::Mesh(dest->cm, tempMesh->cm);
	delete tempMesh;
	//create the new mesh and the triangulation between the clones
	tempMesh= new MeshModel();
	CMeshO::VertexIterator vi=vcg::tri::Allocator<CMeshO>::AddVertices(tempMesh->cm,2*nv);

	//I have at least 6 vertexes
	for (int i=0;i<outlines.size();i++)
  {
		(&*vi)->P()=outlines[i][0];
		CVertexO* v0=(&*vi); ++vi;
		(&*vi)->P()=outlines[i][0]+planeAxis*eps;
		CVertexO* v1=(&*vi); ++vi;
		CVertexO* vs0=v0;	//we need the start point
		CVertexO* vs1=v1; //to close the loop
//		Log(0,"%d",outlines[i].size());
		for(int j=1;j<outlines[i].size();++j)
		{
			(&*vi)->P()=outlines[i][j];
			CVertexO* v2=(&*vi); ++vi;
			(&*vi)->P()=outlines[i][j]+planeAxis*eps;
			CVertexO* v3=(&*vi); ++vi;
			CMeshO::FaceIterator fi=vcg::tri::Allocator<CMeshO>::AddFaces(tempMesh->cm,2);

			(&*fi)->V(2)=v0;
			(&*fi)->V(1)=v1;
			(&*fi)->V(0)=v2;
			++fi;
			(&*fi)->V(0)=v1;
			(&*fi)->V(1)=v2;
			(&*fi)->V(2)=v3;

			v0=v2;
			v1=v3;
			if (j==outlines[i].size()-1)
			{
				CMeshO::FaceIterator fi=vcg::tri::Allocator<CMeshO>::AddFaces(tempMesh->cm,2);
				(&*fi)->V(2)=v0;
				(&*fi)->V(1)=v1;
				(&*fi)->V(0)=vs0;
				++fi;
				(&*fi)->V(0)=v1;
				(&*fi)->V(1)=vs0;
				(&*fi)->V(2)=vs1;
			}
		}
  }

	tri::Append<CMeshO,CMeshO>::Mesh(dest->cm, tempMesh->cm);
	tri::Clean<CMeshO>::RemoveDuplicateVertex(dest->cm);
  tri::UpdateTopology<CMeshO>::FaceFace(dest->cm);
	tri::UpdateNormals<CMeshO>::PerVertexPerFace(dest->cm);

	delete tempMesh;

}
const MeshFilterInterface::FilterClass ExtraFilter_SlicePlugin::getClass(QAction *filter)
{
	switch(ID(filter))
	{
		case FP_PARALLEL_PLANES :
		case FP_RECURSIVE_SLICE :
		return MeshFilterInterface::Generic;
		default: assert(0);
	}
	return MeshFilterInterface::Generic;
}

bool ExtraFilter_SlicePlugin::autoDialog(QAction *action)
{
  switch(ID(action))
  {
  case  FP_RECURSIVE_SLICE:
  case FP_PARALLEL_PLANES: return true;
	default: return false;
  }
}

void ExtraFilter_SlicePlugin::createSlice(MeshModel* currentMesh, MeshModel* destMesh)
{
  tri::Append<CMeshO,CMeshO>::Mesh(destMesh->cm, currentMesh->cm, true);
  tri::UpdateTopology<CMeshO>::FaceFace(destMesh->cm);
  tri::UpdateBounding<CMeshO>::Box(destMesh->cm);						// updates bounding box
  destMesh->cm.Tr = currentMesh->cm.Tr;								// copy transformation
}

void ExtraFilter_SlicePlugin::capHole(MeshModel* currentMesh, MeshModel* destMesh)
{

  std::vector< std::vector<Point3f> > outlines;
  std::vector<Point3f> outline;
  vcg::tri::UpdateFlags<CMeshO>::VertexClearV(currentMesh->cm);
  vcg::tri::UpdateFlags<CMeshO>::FaceBorderFromNone(currentMesh->cm);
  int nv=0;
  for(int i=0;i<currentMesh->cm.face.size();i++)
  {
    for (int j=0;j<3;j++)
    if (!currentMesh->cm.face[i].IsV() && currentMesh->cm.face[i].IsB(j))
    {
      CFaceO* startB=&(currentMesh->cm.face[i]);
      vcg::face::Pos<CFaceO> p(startB,j);
	    do
      {
				p.V()->SetV();
        outline.push_back(p.V()->P());
        p.NextB();
				nv++;
      }
      while(!p.V()->IsV());
      outlines.push_back(outline);
      outline.clear();
    }
  }
	if (nv<2) return;
  CMeshO::VertexIterator vi=vcg::tri::Allocator<CMeshO>::AddVertices(destMesh->cm,nv);
  for (int i=0;i<outlines.size();i++)
  {
    for(int j=0;j<outlines[i].size();++j,++vi)
			(&*vi)->P()=outlines[i][j];
  }

  std::vector<int> indices;
  glu_tesselator::tesselate(outlines, indices);
//  Log(0,"%d new faces",indices.size());
  std::vector<Point3f> points;
  glu_tesselator::unroll(outlines, points);
  CMeshO::FaceIterator fi=tri::Allocator<CMeshO>::AddFaces(destMesh->cm,nv-2);
  for (size_t i=0; i<indices.size(); i+=3,++fi)
  {
    (*&fi)->V(0)=&destMesh->cm.vert[ indices[i+0] ];
    (*&fi)->V(1)=&destMesh->cm.vert[ indices[i+1] ];
    (*&fi)->V(2)=&destMesh->cm.vert[ indices[i+2] ];
  }
	tri::Clean<CMeshO>::RemoveDuplicateVertex(destMesh->cm);
  vcg::tri::UpdateTopology<CMeshO>::FaceFace(destMesh->cm);
  vcg::tri::UpdateBounding<CMeshO>::Box(destMesh->cm);
}

Q_EXPORT_PLUGIN(ExtraFilter_SlicePlugin)
