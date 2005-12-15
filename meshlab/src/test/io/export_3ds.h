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

/****************************************************************************
  History

 $Log$
 Revision 1.1  2005/12/15 12:27:58  fmazzant
 first commit 3ds

 

****************************************************************************/

#ifndef __VCGLIB_EXPORT_3DS
#define __VCGLIB_EXPORT_3DS

#include <wrap/callback.h>
#include <vcg/complex/trimesh/allocate.h>
#include <wrap/ply/io_mask.h>

namespace vcg {
namespace tri {
namespace io {


	template <class SaveMeshType>
	class Exporter3DS
	{
	public:	
		typedef typename SaveMeshType::FaceIterator FaceIterator;
		typedef typename SaveMeshType::VertexIterator VertexIterator;
		typedef typename SaveMeshType::VertexType VertexType;
	
		static bool SaveASCII(SaveMeshType &m, const char * filename)	
		{
			return false;
		}

		static bool SaveBinary(SaveMeshType &m, const char * filename)
		{
			return false;
		}
		
		static bool Save(SaveMeshType &m, const char * filename, bool binary)
		{
			if(binary) return SaveBinary(m,filename);
			else return SaveASCII(m,filename);
		}

	}; // end class

} // end Namespace tri
} // end Namespace io
} // end Namespace vcg

#endif
