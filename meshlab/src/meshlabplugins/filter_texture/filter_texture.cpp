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
#include <pair.h>

#include <meshlab/meshmodel.h>
#include <meshlab/interfaces.h>

#include <vcg/complex/trimesh/clean.h>
#include <vcg/complex/trimesh/attribute_seam.h>
#include <vcg/space/triangle2.h>

#include "filter_texture.h"

FilterTexturePlugin::FilterTexturePlugin() 
{ 
	typeList << FP_UVTOCOLOR
			<< FP_UV_WEDGE_TO_VERTEX
			<< FP_BASIC_TRIANGLE_MAPPING;
  
	foreach(FilterIDType tt , types())
		actionList << new QAction(filterName(tt), this);
}

const QString FilterTexturePlugin::filterName(FilterIDType filterId) const 
{
	switch(filterId) {
		case FP_UVTOCOLOR : return QString("UV to Color"); 
		case FP_UV_WEDGE_TO_VERTEX : return QString("Convert PerWedge UV into PerVertex UV");
		case FP_BASIC_TRIANGLE_MAPPING : return QString("Basic Triangle Mapping");
		default : assert(0); 
	}
}

// Info() must return the longer string describing each filtering action 
// (this string is used in the About plugin dialog)
const QString FilterTexturePlugin::filterInfo(FilterIDType filterId) const
{
	switch(filterId)
	{
		case FP_UVTOCOLOR :  return QString("Maps the UV Space into a color space, thus colorizing mesh vertices according to UV coords.");
		case FP_UV_WEDGE_TO_VERTEX : return QString("Converts per Wedge Texture Coordinates to per Vertex Texture Coordinates splitting vertices with not coherent Wedge coordinates.");
		case FP_BASIC_TRIANGLE_MAPPING : return QString("Builds a basic parametrization");
		default : assert(0); 
	}
	return QString("Unknown Filter");
}

int FilterTexturePlugin::getPreConditions(QAction *a) const
{
	switch (ID(a))
	{
		case FP_UVTOCOLOR : return MeshFilterInterface::FP_VertexTexCoord;
		case FP_UV_WEDGE_TO_VERTEX : return MeshFilterInterface::FP_WedgeTexCoord;
		case FP_BASIC_TRIANGLE_MAPPING : return MeshFilterInterface::FP_Face;
		default: assert(0);
	}
	return MeshFilterInterface::FP_Generic;
}

const int FilterTexturePlugin::getRequirements(QAction *a)
{
	switch (ID(a))
	{
		case FP_UVTOCOLOR :
		case FP_UV_WEDGE_TO_VERTEX :
		case FP_BASIC_TRIANGLE_MAPPING :
			return MeshModel::MM_NONE;
		default: assert(0);	
	}
	return MeshModel::MM_NONE;
}

int FilterTexturePlugin::postCondition( QAction *a) const
{
	switch (ID(a))
	{
		case FP_UVTOCOLOR : return MeshModel::MM_VERTCOLOR;
		case FP_UV_WEDGE_TO_VERTEX : return MeshModel::MM_UNKNOWN;
		case FP_BASIC_TRIANGLE_MAPPING : return MeshModel::MM_WEDGTEXCOORD;
		default: assert(0);	
	}
	return MeshModel::MM_NONE;
}

// The FilterClass describes in which generic class of filters it fits. 
// This choice affect the submenu in which each filter will be placed 
// More than a single class can be choosen.
const FilterTexturePlugin::FilterClass FilterTexturePlugin::getClass(QAction *a)
{
  switch(ID(a))
	{
		case FP_UVTOCOLOR : return FilterClass(MeshFilterInterface::VertexColoring + MeshFilterInterface::Texture);
		case FP_UV_WEDGE_TO_VERTEX : 
		case FP_BASIC_TRIANGLE_MAPPING : return MeshFilterInterface::Texture;
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
void FilterTexturePlugin::initParameterSet(QAction *action, MeshModel &m, RichParameterSet & parlst) 
{
	switch(ID(action))	{
		case FP_UVTOCOLOR :
			parlst.addParam(new RichEnum("colorspace", 0, QStringList("Red-Green") << "Hue-Saturation", "Color Space", "The color space used to mapping UV to"));
			break;
		case FP_UV_WEDGE_TO_VERTEX : 
			break;
		case FP_BASIC_TRIANGLE_MAPPING :
			parlst.addParam(new RichInt("sidedim", 0, "Quads per line", "Indicates how many triangles have to be put on each line (every quad contains two triangles)\nLeave 0 for automatic calculation"));
			parlst.addParam(new RichInt("textdim", 1024, "Texture Dimension (px)", "Gives an indication on how big the texture is"));
			parlst.addParam(new RichInt("border", 1, "Inter-Triangle border (px)", "Specifies how many pixels to be left between triangles in parametrization domain"));
			parlst.addParam(new RichEnum("method", 0, QStringList("Basic") << "Space-optimizing", "Method", "Choose space optimizing to map smaller faces into smaller triangles in parametrizazion domain"));
			break;
		default : assert(0); 
	}
}


/////// FUNCTIONS NEEDED BY "UV WEDGE TO VERTEX" FILTER
inline void ExtractVertex(const CMeshO & srcMesh, const CMeshO::FaceType & f, int whichWedge, const CMeshO & dstMesh, CMeshO::VertexType & v)
{
	(void)srcMesh;
	(void)dstMesh;
	// This is done to preserve every single perVertex property
	// perVextex Texture Coordinate is instead obtained from perWedge one.
	v.ImportLocal(*f.cV(whichWedge));
	v.T() = f.cWT(whichWedge);
}

inline bool CompareVertex(const CMeshO & m, const CMeshO::VertexType & vA, const CMeshO::VertexType & vB)
{
	(void)m;
	return (vA.cT() == vB.cT());
}

/////// FUNCTIONS NEEDED BY "BASIC PARAMETRIZATION" FILTER
inline int getLongestEdge(const CMeshO::FaceType & f)
{
	int res=0;
	const CMeshO::CoordType &p0=f.cP(0), &p1=f.cP(1), p2=f.cP(2);
	double  maxd01 = SquaredDistance(p0,p1);
    double  maxd12 = SquaredDistance(p1,p2);
    double  maxd20 = SquaredDistance(p2,p0);
    if(maxd01 > maxd12)
        if(maxd01 > maxd20)     res = 0;
        else                    res = 2;
		else
			if(maxd12 > maxd20)     res = 1;
			else                    res = 2;
	return res;
}

inline bool cmp( pair<uint,double> a, pair<uint,double> b ) {
	return a.second > b.second;
}

typedef vcg::Triangle2<CMeshO::FaceType::TexCoordType::ScalarType> Tri2;

inline void buildTrianglesCache(std::vector<Tri2> &arr, int idx, int maxLevels, float border, float quadSize)
{
	static const float sqrt2 = sqrt(2.0);
	
	assert(idx >= -1);
	Tri2 &t0 = arr[2*idx+2];
	Tri2 &t1 = arr[2*idx+3];
	if (idx == -1)
	{
		// build triangle 0
		t0.P(0).X() = quadSize - (0.5 + 1.0/sqrt2)*border;
		t0.P(1).X() = 0.5 * border;
		t0.P(0).Y() = 1.0 - t0.P(1).X();
		t0.P(1).Y() = 1.0 - t0.P(0).X();
		t0.P(2).X() = t0.P(1).X();
		t0.P(2).Y() = t0.P(0).Y();
		// build triangle 1
		t1.P(0).X() = (0.5 + 1.0/sqrt2)*border;
		t1.P(1).X() = quadSize - 0.5 * border;
		t1.P(0).Y() = 1.0 - t1.P(1).X();
		t1.P(1).Y() = 1.0 - t1.P(0).X();
		t1.P(2).X() = t1.P(1).X();
		t1.P(2).Y() = t1.P(0).Y();
	}
	else {
		// split triangle idx in t0 t1
		Tri2 &t = arr[idx];
		Tri2::CoordType midPoint = (t.P(0) + t.P(1)) / 2;
		Tri2::CoordType vec01 = (t.P(1) - t.P(0)).Normalize() * (border/2.0);
		t0.P(0) = t.P(1);
		t1.P(1) = t.P(0);
		t0.P(2) = midPoint + vec01;
		t1.P(2) = midPoint - vec01;
		t0.P(1) = t.P(2) + ( (t.P(1)-t.P(2)).Normalize() * border / sqrt2 );
		t1.P(0) = t.P(2) + ( (t.P(0)-t.P(2)).Normalize() * border / sqrt2 );
	}
	if (--maxLevels <= 0) return;
	buildTrianglesCache (arr, 2*idx+2, maxLevels, border, quadSize);
	buildTrianglesCache (arr, 2*idx+3, maxLevels, border, quadSize);
	
}

#define CheckError(x,y); if ((x)) {this->errorMessage = (y); return false;}
///////////////////////////////////////////////////////


// The Real Core Function doing the actual mesh processing.
bool FilterTexturePlugin::applyFilter(QAction *filter, MeshModel &m, RichParameterSet &par, vcg::CallBackPos *cb)
{
	switch(ID(filter))	 {
		case FP_UVTOCOLOR : {
			int vcount = m.cm.vert.size();
			int colsp = par.getEnum("colorspace");
			if (!m.hasDataMask(MeshModel::MM_VERTCOLOR))
				m.updateDataMask(MeshModel::MM_VERTCOLOR);
			for (int i=0; i<vcount; ++i)
			{
				CMeshO::VertexType &v = m.cm.vert[i];
				if (!v.IsD())
				{
					// 'Normalized' 0..1 
					float normU, normV;
					normU = v.T().U() - (int)v.T().U();
					if (normU < 0.) normU += 1.;
					normV = v.T().V() - (int)v.T().V();
					if (normV < 0.) normV += 1.;
					
					switch(colsp) {
						// Red-Green color space
						case 0 : v.C() = vcg::Color4b((int)floor((normU*255)+0.5), (int)floor((normV*255)+0.5), 0, 255); break;
						// Hue-Saturation color space
						case 1 : {
							vcg::Color4f c;
							c.SetHSVColor(normU, normV, 1.0);
							v.C().Import(c);
						}
							break;
						default : assert(0);
					};
				}
				cb(i*100/vcount, "Colorizing vertices from UV coordinates ...");
			}
		}
		break;
		
		case FP_UV_WEDGE_TO_VERTEX : {
			int vn = m.cm.vn;
			if (!m.hasDataMask(MeshModel::MM_VERTTEXCOORD))
				m.updateDataMask(MeshModel::MM_VERTTEXCOORD);
			vcg::tri::AttributeSeam::SplitVertex(m.cm, ExtractVertex, CompareVertex);
			if (m.cm.vn != vn)
			{
				if (m.hasDataMask(MeshModel::MM_FACEFACETOPO))
					m.clearDataMask(MeshModel::MM_FACEFACETOPO);
				if (m.hasDataMask(MeshModel::MM_VERTFACETOPO))
					m.clearDataMask(MeshModel::MM_VERTFACETOPO);
			}
		}
		break;
		
		case FP_BASIC_TRIANGLE_MAPPING : {
			if (!m.hasDataMask(MeshModel::MM_WEDGTEXCOORD))
				m.updateDataMask(MeshModel::MM_WEDGTEXCOORD);
			
			bool adv;
			switch(par.getEnum("method")) {
				case 0 : adv = false; break; // Basic
				case 1 : adv = true; break;  // Advanced
				default : assert(0);
			};
			
			static const float sqrt2 = sqrt(2.0);
			
			if (adv)
			{ //ADVANCED
			
			// Get Parameters
			int sideDim = par.getInt("sidedim");
			int textDim = par.getInt("textdim");
			int pxBorder = par.getInt("border");
			
			float border = ((float)pxBorder) / textDim;
			
			// Pre checks
			CheckError(textDim <= 0, "Texture Dimension has an incorrect value");
			CheckError(border < 0,   "Inter-Triangle border has an incorrect value");
			CheckError(sideDim < 0,  "Quads per line border has an incorrect value");
			
			// Creates a vector of pair <face index, double area> area ordered (not increasingly) O(n logn) UNACCEPTABLE
			std::vector<pair<uint,double> > faces;
			for (uint i=0; i<m.cm.face.size(); ++i)
				if (!m.cm.face[i].IsD()) faces.push_back( pair<uint,double>(i, DoubleArea(m.cm.face[i])) );
			sort(faces.begin(), faces.end(), cmp);
			
			int faceNo = faces.size();
			
			// Counts faces for each halfening level O(n)
			std::vector<int> k;
			double baseArea = faces.front().second / 2.0;
			int idx=0, maxHalfening=0;
			while (idx < faceNo) {
				int no=0;
				while (idx<faceNo && faces[idx].second>baseArea) { ++no; ++idx; }
				k.push_back(no);
				baseArea /= 2.0;
				++maxHalfening;
			}
			
			// Calculating optimal dimension (considering faces areas and border dimension)
			int dim = 0;
			int halfeningLevels = 0;
			
			double qn = 0., divisor = 2.0;
			int rest = faceNo, oneFact = 1, sqrt2Fact = 1;
			while (halfeningLevels < maxHalfening)
			{
				int tmp;
				if (sideDim == 0) tmp = (int)ceil(sqrt(qn + rest/divisor));
				else tmp = sideDim;

				if (1.0/tmp < (sqrt2Fact/sqrt2 + oneFact)*border) break;
				
				rest -= k[halfeningLevels];
				qn += k[halfeningLevels] / divisor;
				divisor *= 2.0;
				
				if (halfeningLevels%2)
					oneFact *= 2;
				else
					sqrt2Fact *= 2;
				
				dim = tmp;
				halfeningLevels++;
			}
			
			// Post checks
			CheckError(halfeningLevels==0 && sideDim!=0, "Quads per line aren't enough to obtain a correct parametrization");
			CheckError(halfeningLevels==0 && sideDim==0, "Inter-Triangle border is too much");
			
			//Create cache of possible triangles (need only translation in correct position)
			std::vector<Tri2> cache((1 << (halfeningLevels+1))-2);
			buildTrianglesCache(cache, -1, halfeningLevels, border, 1.0/dim);
			
			// Setting texture coordinates (finally)
			Tri2::CoordType origin(0., 0.);
			Tri2::CoordType tmp;
			idx = 0;
			int currLevel = 1, kidx = 0;
			for (int i=0; i<dim && idx<faceNo; ++i)
			{
				origin.Y() = -((float)i)/dim;
				for (int j=0; j<dim && idx<faceNo; j++)
				{
					origin.X() = ((float)j)/dim;
					for (int pos=(1<<currLevel)-2; pos<(1<<(currLevel+1))-2 && idx<faceNo; ++pos, ++idx)
					{
						while (k[kidx] == 0) {
							if (++kidx < halfeningLevels)
							{
								++currLevel;
								pos = 2*pos+2;
							}
						}
						int fidx = faces[idx].first;
						int lEdge = getLongestEdge(m.cm.face[fidx]);
						Tri2 &t = cache[pos];
						tmp = t.P0(lEdge) + origin;
						m.cm.face[fidx].WT(0) = CFaceO::TexCoordType(tmp.X(), tmp.Y());
						m.cm.face[fidx].WT(0).N() = 0;
						tmp = t.P1(lEdge) + origin;
						m.cm.face[fidx].WT(1) = CFaceO::TexCoordType(tmp.X(), tmp.Y());
						m.cm.face[fidx].WT(1).N() = 0;
						tmp = t.P2(lEdge) + origin;
						m.cm.face[fidx].WT(2) = CFaceO::TexCoordType(tmp.X(), tmp.Y());
						m.cm.face[fidx].WT(2).N() = 0;
						--k[kidx];
						cb(idx*100/faceNo, "Generating parametrization...");
					}
				}
			}
			assert(idx == faceNo);
			Log(GLLogStream::FILTER, "Biggest triangle's catheti are %.2f px long", (cache[0].P(0)-cache[0].P(2)).Norm() * textDim);
			Log(GLLogStream::FILTER, "Smallest triangle's catheti are %.2f px long", (cache[cache.size()-1].P(0)-cache[cache.size()-1].P(2)).Norm() * textDim);
			
			}
			else
			{ //BASIC
			
			// Get Parameters
			int sideDim = par.getInt("sidedim");
			int textDim = par.getInt("textdim");
			int pxBorder = par.getInt("border");
			
			//Get total faces and total undeleted face
			int faceNo = m.cm.face.size();
			int faceNotD = 0;
			for (CMeshO::FaceIterator fi=m.cm.face.begin(); fi!=m.cm.face.end(); ++fi)
				if (!fi->IsD()) ++faceNotD;
			
			// Minimum side dimension to get correct halfsquared triangles
			int optimalDim = ceilf(sqrtf(faceNotD/2.));
			if (sideDim == 0) sideDim = optimalDim;
			else {
				CheckError(optimalDim > sideDim, "Quads per line aren't enough to obtain a correct parametrization");
			}
			CheckError(textDim <= 0, "Texture Dimension has an incorrect value");
			
			
			//Calculating border size in UV space
			float border = ((float)pxBorder) / textDim;
			CheckError(border < 0, "Inter-Triangle border has an incorrect value");
			CheckError(border*(1.0+sqrt2)*sideDim > 1.0, "Inter-Triangle border is too much");
			
			float bordersq2 = border / sqrt2;
			float halfborder = border / 2;
			
			bool odd = true;
			CFaceO::TexCoordType botl, topr;
			int idx=0;
			botl.V() = 1.;
			for (int i=0; i<sideDim && idx<faceNo; ++i)
			{
				topr.V() = botl.V();
				topr.U() = 0.;
				botl.V() = 1.0 - 1.0/sideDim*(i+1);
				for (int j=0; j<2*sideDim && idx<faceNo; ++idx)
				{
					if (!m.cm.face[idx].IsD())
					{
						int lEdge = getLongestEdge(m.cm.face[idx]);
						if (odd) {
							botl.U() = topr.U();
							topr.U() = 1.0/sideDim*(j/2+1);
							CFaceO::TexCoordType bl(botl.U()+halfborder, botl.V()+halfborder+bordersq2);
							CFaceO::TexCoordType tr(topr.U()-(halfborder+bordersq2), topr.V()-halfborder);
							bl.N() = 0;
							tr.N() = 0;
							m.cm.face[idx].WT(lEdge) = bl;
							m.cm.face[idx].WT((++lEdge)%3) = tr;
							m.cm.face[idx].WT((++lEdge)%3) = CFaceO::TexCoordType(bl.U(), tr.V());
							m.cm.face[idx].WT(lEdge%3).N() = 0;
						} else {
							CFaceO::TexCoordType bl(botl.U()+(halfborder+bordersq2), botl.V()+halfborder);
							CFaceO::TexCoordType tr(topr.U()-halfborder, topr.V()-(halfborder+bordersq2));
							bl.N() = 0;
							tr.N() = 0;
							m.cm.face[idx].WT(lEdge) = tr;
							m.cm.face[idx].WT((++lEdge)%3) = bl;
							m.cm.face[idx].WT((++lEdge)%3) = CFaceO::TexCoordType(tr.U(), bl.V());
							m.cm.face[idx].WT(lEdge%3).N() = 0;
						}
						cb(idx*100/faceNo, "Generating parametrization...");
						odd=!odd; ++j;
					}
				}
			}
			Log(GLLogStream::FILTER, "Triangles catheti are %.2f px long", (1.0/sideDim-border-bordersq2)*textDim);
			}
		}
		break;
			
		default: assert(0);
	}
	
	return true;
}

Q_EXPORT_PLUGIN(FilterTexturePlugin)
