 /****************************************************************************
* MeshLab                                                           o o     *
* A versatile mesh processing toolbox                             o     o   *
*                                                                _   O  _   *
* Copyright(C) 2007                                                \/)\/    *
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

#ifndef FILTERVIRTUALRANGESCANPLUGIN_H
#define FILTERVIRTUALRANGESCANPLUGIN_H

#include <QObject>
#include <QStringList>
#include <QString>

#include <common/interfaces.h>
#include <QGLContext>
#include <GL/glew.h>

#include "vs/sampler.h"

using namespace vs;

class FilterVirtualRangeScan : public QObject, public MeshFilterInterface
{
    Q_OBJECT
    Q_INTERFACES(MeshFilterInterface)

public:
    FilterVirtualRangeScan();
    ~FilterVirtualRangeScan();

    virtual QString filterName(FilterIDType filter) const;
    virtual QString filterInfo(FilterIDType filter) const;

    virtual int getRequirements(QAction *);
    virtual void initParameterSet(QAction*, MeshModel&, RichParameterSet &){ ;}
    virtual void initParameterSet(QAction *, MeshDocument &, RichParameterSet &);

    virtual bool applyFilter (QAction*  filter, MeshDocument &md, RichParameterSet & par, vcg::CallBackPos *cb);

    virtual int postCondition(QAction *action) const;
    virtual FilterClass getClass(QAction *);

private:
    VSParameters vrsParams;

    enum {FP_VRS};

    QGLContext* innerContext;
};

#endif
