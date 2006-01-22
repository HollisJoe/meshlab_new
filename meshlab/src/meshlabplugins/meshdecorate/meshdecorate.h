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
Revision 1.7  2006/01/22 14:47:16  glvertex
Puts ticks on X axis... Still working on...

Revision 1.6  2006/01/19 23:56:44  glvertex
Starting quoted box (simply draws xyz axes)

Revision 1.5  2005/12/12 22:47:35  cignoni
Added plugin info methods

Revision 1.4  2005/12/12 11:19:41  cignoni
Added bbox corners and axis,
cleaned up the identification between by string of decorations

****************************************************************************/
#ifndef EXTRADECORATEPLUGIN_H
#define EXTRADECORATEPLUGIN_H

#include <QObject>
#include <QAction>
#include <QList>

#include <meshlab/meshmodel.h>
#include <meshlab/interfaces.h>

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

	float niceRound(float value);

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
  void DrawAxis(MeshModel &m);
	void DrawQuotedBox(MeshModel &m);

  virtual void Decorate(QAction *a, MeshModel &m, RenderMode &rm, GLArea *gla);

};

#endif
