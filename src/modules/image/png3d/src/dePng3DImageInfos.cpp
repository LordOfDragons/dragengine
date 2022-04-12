/* 
 * Drag[en]gine 3D PNG Image Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dePng3DImageInfos.h"

#include <dragengine/common/exceptions.h>



// Class dePng3DImageInfo
///////////////////////////

// Constructor, destructor
////////////////////////////

dePng3DImageInfo::dePng3DImageInfo( const char *nfilename ){
	if( ! nfilename ) DETHROW( deeInvalidParam );
	
	filename = nfilename;
	
	SetIsCompressed( true );
}
dePng3DImageInfo::~dePng3DImageInfo(){
}



// Management
///////////////

int dePng3DImageInfo::GetWidth(){
	return width;
}

int dePng3DImageInfo::GetHeight(){
	return height;
}

int dePng3DImageInfo::GetDepth(){
	return depth;
}

int dePng3DImageInfo::GetComponentCount(){
	return componentCount;
}

int dePng3DImageInfo::GetBitCount(){
	return bitCount;
}
