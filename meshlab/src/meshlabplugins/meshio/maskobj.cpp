/****************************************************************************
* MeshLab                                                           o o     *
* An extendible mesh processor                                    o     o   *
*                                                                _   O  _   *
* Copyright(C) 2005, 2006                                          \/)\/    *
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
 Revision 1.6  2006/01/19 11:21:14  fmazzant
 deleted old savemaskobj & old MaskObj

 Revision 1.5  2006/01/10 16:52:19  fmazzant
 update ply::PlyMask -> io::Mask

 Revision 1.4  2006/01/04 16:48:37  fmazzant
 changed PM_VERTTEXCOORD in PM_WEDGTEXCOORD

 Revision 1.3  2005/12/13 14:02:50  fmazzant
 added the rescue of the materials of the obj

 Revision 1.2  2005/12/11 00:34:22  fmazzant
 bug-fix: added the type of return of the MaskObj method::MaskObjToInt()

 Revision 1.1  2005/12/09 16:37:20  fmazzant
 maskobj for select element to save

 
*****************************************************************************/


//#include "maskobj.h"
//#include <wrap/io_trimesh/io_mask.h>
//
//MaskObj::MaskObj()
//{
//	this->isfirst=true;
//
//	this->faces = true;
//	this->vertexs = true;
//
//	this->binary = false;
//	this->colorV = false;
//	this->colorF = false;
//	this->normal = false;
//	this->texture = false;
//}
//
//int MaskObj::MaskObjToInt()
//{
//	int mask=0;
//	if(faces)	{mask |= vcg::tri::io::Mask::IOM_FACEQUALITY;}
//	if(vertexs)	{mask |= vcg::tri::io::Mask::IOM_VERTQUALITY;}
//	if(colorV)	{mask |= vcg::tri::io::Mask::IOM_VERTCOLOR;}
//	if(colorF)	{mask |= vcg::tri::io::Mask::IOM_FACECOLOR;}
//	if(normal)	{mask |= vcg::tri::io::Mask::IOM_VERTNORMAL;}
//	if(texture)	{mask |= vcg::tri::io::Mask::IOM_WEDGTEXCOORD;}
//	return mask;
//}