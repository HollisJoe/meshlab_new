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
$Log$
Revision 1.16  2006/05/25 04:57:45  cignoni
Major 0.7 release. A lot of things changed. Colorize interface gone away, Editing and selection start to work.
Optional data really working. Clustering decimation totally rewrote. History start to work. Filters organized in classes.

Revision 1.15  2006/02/22 12:24:41  cignoni
Restructured Quoted Box.

Revision 1.14  2006/02/19 22:17:17  glvertex
Applied gcc patch

Revision 1.13  2006/02/17 16:09:31  glvertex
Partial restyle in drawAxis and drawQuotedBox
A lot of optimizations

Revision 1.12  2006/02/15 16:27:33  glvertex
- Added labels to the quoted box
- Credits

Revision 1.11  2006/02/03 16:36:24  glvertex
- Some renaming
- Quoted axis now also draw lines

Revision 1.10  2006/02/03 11:05:14  alemochi
Modified axis and added arrows.

Revision 1.9  2006/01/26 00:38:59  glvertex
Quoted box: draws xyz axes candidates

Revision 1.8  2006/01/22 23:37:59  glvertex
Choosing axes candidates

Revision 1.7  2006/01/22 14:47:16  glvertex
Puts ticks on X axis... Still working on...

Revision 1.6  2006/01/19 23:56:44  glvertex
Starting quoted box (simply draws xyz axes)

****************************************************************************/
#ifndef EXTRADECORATEPLUGIN_H
#define EXTRADECORATEPLUGIN_H

#include <QObject>
#include <QAction>
#include <QList>

#include <meshlab/meshmodel.h>
#include <meshlab/interfaces.h>
//#include "../../meshlab/mainwindow.h"

class ExtraMeshDecoratePlugin : public QObject, public MeshDecorateInterface
{
  Q_OBJECT
  Q_INTERFACES(MeshDecorateInterface)
  virtual const ActionInfo &Info(QAction *);
  virtual const PluginInfo &Info();
  
	QList <QAction *> actionList;
  enum {
    DP_SHOW_NORMALS = 1,
    DP_SHOW_BOX_CORNERS = 2,
    DP_SHOW_AXIS = 3,
		DP_SHOW_QUOTED_BOX = 4,
  };

  const QString ST(int id) const;

private:
	float niceRound2(float value,float base);
	float niceRound(float value);

	float calcSlope(const Point3d &a,const Point3d &b,float dim,int spacing,double *mm,double *mp,int *vp);

	void	drawTickedLine(const Point3d &p1,const Point3d &p2,float dim,float tickDist);
	void	drawQuotedLine(const Point3d &a,const Point3d &b,float aVal, float bVal,float tickDist,QGLWidget *gla, QFont qf);


	void	chooseX(Box3f &box,double *modelview,double *projection,int *viewport,Point3d &x1,Point3d &x2);
	void	chooseY(Box3f &box,double *modelview,double *projection,int *viewport,Point3d &y1,Point3d &y2);
	void	chooseZ(Box3f &box,double *modelview,double *projection,int *viewport,Point3d &z1,Point3d &z2);

public:
     
	ExtraMeshDecoratePlugin()
	{
		QAction * qa;
    qa= new QAction(ST(DP_SHOW_NORMALS),this);
		qa->setCheckable(true);
		actionList << qa;
    qa= new QAction(ST(DP_SHOW_BOX_CORNERS),this);
    qa->setCheckable(true);
		actionList << qa;
		qa= new QAction(ST(DP_SHOW_AXIS),this);
		qa->setCheckable(true);
		actionList << qa;
		qa= new QAction(ST(DP_SHOW_QUOTED_BOX),this);
		qa->setCheckable(true);
		actionList << qa;
	}

	QList<QAction *> actions () const {return actionList;}

  void DrawBBoxCorner(MeshModel &m);
  void DrawAxis(MeshModel &m,QGLWidget* gla, QFont qf);
	void DrawQuotedBox(MeshModel &m,QGLWidget *gla, QFont qf);

  virtual void Decorate(QAction *a, MeshModel &m, RenderMode &rm, QGLWidget *gla,QFont qf);

};

#endif
