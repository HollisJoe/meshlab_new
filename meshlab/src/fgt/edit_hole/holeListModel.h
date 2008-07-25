/****************************************************************************
* VCGLib                                                            o o     *
* Visual and Computer Graphics Library                            o     o   *
*                                                                _   O  _   *
* Copyright(C) 2004                                                \/)\/    *
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
#ifndef HOLELISTMODEL_H
#define HOLELISTMODEL_H

#include <QWidget>
#include <QtGui>
#include "fgtHole.h"
#include <meshlab/meshmodel.h>
#include <meshlab/interfaces.h>
#include "vcg/simplex/face/pos.h"
#include "vcg/complex/trimesh/base.h"
#include "vcg/space/color4.h"



class HoleListModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	enum FillerState
	{
		Selection, Filled
	};

	typedef vcg::tri::Hole<CMeshO> vcgHole;
	typedef vcg::tri::Hole<CMeshO>::Info HoleInfo;
	typedef FgtHole<CMeshO>  HoleType;
	typedef std::vector< HoleType > HoleVector;
	typedef vcg::face::Pos<CMeshO::FaceType> PosType;
	

	HoleListModel(MeshModel *m, QObject *parent = 0);
	virtual ~HoleListModel() { clearModel(); };

	inline int rowCount(const QModelIndex &parent = QModelIndex()) const { return holes.size(); };
	inline int columnCount(const QModelIndex &parent = QModelIndex()) const 
	{
		if(state == HoleListModel::Selection) return 3;
		else return 5; 
	};
	
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &child) const { return QModelIndex(); };

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	
	Qt::ItemFlags flags(const QModelIndex &index) const;
	bool setData( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole ); 


	inline void setState(HoleListModel::FillerState s) { state = s; emit layoutChanged(); };
	void toggleSelectionHoleFromBorderFace(CFaceO *bface);
	void clearModel();
	void updateModel();
	void drawHoles() const;
	void fill(bool antiSelfIntersection);
	void acceptFilling(bool forcedCancel=false);
	inline FillerState getState() const { return state; }
	inline MeshModel* getMesh() const { return mesh; }

private:
	MeshModel *mesh;
	FillerState state;	

public:
	HoleVector holes;
	int userBitHole;
};

#endif
