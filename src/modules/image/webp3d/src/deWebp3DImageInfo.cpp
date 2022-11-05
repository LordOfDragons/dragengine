/* 
 * Drag[en]gine WebP-3D Image Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "deWebp3DImageInfo.h"


// Class deWebp3DImageInfo
////////////////////////////

// Constructor, destructor
////////////////////////////

deWebp3DImageInfo::deWebp3DImageInfo( const decString &filename ) :
pFilename( filename ),
width( 0 ),
height( 0 ),
hasAlpha( false ){
}

deWebp3DImageInfo::~deWebp3DImageInfo(){
}



// Management
///////////////

int deWebp3DImageInfo::GetWidth(){
	return width;
}

int deWebp3DImageInfo::GetHeight(){
	return height;
}

int deWebp3DImageInfo::GetDepth(){
	return depth;
}

int deWebp3DImageInfo::GetComponentCount(){
	return hasAlpha ? 4 : 3;
}

int deWebp3DImageInfo::GetBitCount(){
	return 8;
}
