/****************************************************************************
**
** Copyright (C) 2005-2005 Trolltech AS. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software.
**
** See http://www.trolltech.com/pricing.html or email sales@trolltech.com for
** information about Qt Commercial License Agreements.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include <QtGui>

#include <meshlab/meshmodel.h>
#include <meshlab/interfaces.h>
#include <meshlab/glarea.h>

#include "dummy.h"

using namespace vcg;

void DummyPlugin::Render(QAction *a, MeshModel &m, RenderMode &rm, GLArea *gla)
{
	if(a->text() == "action 1")
	{
		rm.drawColor = GLW::CMNone;
		return;
	}
	
	if(a->text() == "action 2")
	{
		rm.drawColor = GLW::CMPerVert;
		return;
	}

	if(a->text() == "action 2")
	{
		rm.drawColor = GLW::CMPerFace;
		return;
	}

}

Q_EXPORT_PLUGIN(DummyPlugin)