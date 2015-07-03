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

#ifndef __ML_SCENE_RENDERER_H
#define __ML_SCENE_RENDERER_H

#include <GL/glew.h>
#include "../common/meshmodel.h"
#include <wrap/gl/gl_mesh_attributes_feeder.h>

#include <QGLWidget>
#include <QObject>
#include <QMap>





class MLThreadSafeMemoryInfo;


class MLThreadSafeGLMeshAttributesFeeder : public vcg::GLMeshAttributesFeeder<CMeshO>
{
public:
	struct MLThreadSafeTextureNamesContainer
	{
		MLThreadSafeTextureNamesContainer();
		~MLThreadSafeTextureNamesContainer();

		void push_back(GLuint textid);
		size_t size() const;
		bool empty() const;
		void clear();
		GLuint& operator[](size_t ii);
		inline std::vector<GLuint>& textId() {return _tmid;};
	private:
		std::vector<GLuint> _tmid;
		mutable QReadWriteLock _lock;
	};


	MLThreadSafeGLMeshAttributesFeeder(CMeshO& mesh,MLThreadSafeMemoryInfo& gpumeminfo,size_t perbatchtriangles);
	~MLThreadSafeGLMeshAttributesFeeder() {};

	void setPerBatchTriangles(size_t perbatchtriangles);
	
	size_t perBatchTriangles() const;
	
	bool renderedWithBO() const;
	
	GLuint bufferObjectHandle() const;

	void meshAttributesUpdated(int mask);

	bool setupRequestedAttributes(unsigned int viewid,vcg::GLFeederInfo::ReqAtts& rq);

	void deAllocateBO();
	
	void drawWire(unsigned int viewid);

	void drawFlatWire(unsigned int viewid);

	void drawPoints(unsigned int viewid);

	void drawTriangles(unsigned int viewid);

	void drawBBox(unsigned int viewid);
private:
	mutable QReadWriteLock _lock;
	MLThreadSafeTextureNamesContainer _textids;
};

class GLArea;

class MLSceneGLSharedDataContext : public QGLWidget
{
	Q_OBJECT
public:

	MLSceneGLSharedDataContext(MeshDocument& md,MLThreadSafeMemoryInfo& gpumeminfo,bool highprecision,size_t perbatchtriangles = 100000,QWidget* parent = NULL);

	~MLSceneGLSharedDataContext();

	MLThreadSafeMemoryInfo& memoryInfoManager() const
	{
		return _gpumeminfo;
	}
	
	inline bool highPrecisionRendering() const
	{
		return _highprecision;
	}

	void initializeGL();

	void deAllocateGPUSharedData();

	void renderScene(GLArea& glarea);

	MLThreadSafeGLMeshAttributesFeeder* meshAttributeFeeder(int meshid) const;
	inline const Point3m& globalSceneCenter() const {return _globalscenecenter;}

public slots:
	void meshInserted(int meshid);
	void meshRemoved(int meshid);

private:
	void computeSceneGlobalCenter();

	QMap< int, MLThreadSafeGLMeshAttributesFeeder* > _scene;
	MeshDocument& _md;	
	MLThreadSafeMemoryInfo& _gpumeminfo;
	size_t _perbatchtriangles;
	Point3m _globalscenecenter;
	bool _highprecision;

	GLuint _vaohandle;
}; 

//WARNING!!!!!!!THIS CLASS MUST BE CONSIDERED AS HIGHLY TEMPORARY. IT MUST BE REMOVED AS SOON AS POSSIBLE!
struct MLSceneRenderModeAdapter
{
	static void renderModeToReqAtts(const RenderMode& rm,vcg::GLFeederInfo::ReqAtts& rq);
private:
	static vcg::GLFeederInfo::PRIMITIVE_MODALITY renderModeToPrimitiveModality(const RenderMode& rm);
	static void renderModeColorToReqAtts(const RenderMode& rm,vcg::GLFeederInfo::ReqAtts& rq);
	static void renderModeTextureToReqAtts(const RenderMode& rm,vcg::GLFeederInfo::ReqAtts& rq);
};

#endif