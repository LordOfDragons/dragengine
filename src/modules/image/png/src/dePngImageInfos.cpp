/* 
 * Drag[en]gine PNG Image Module
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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dePngImageInfos.h"

#include <dragengine/common/exceptions.h>


// class dePngImageInfo
//////////////////////////

// constructor, destructor
dePngImageInfo::dePngImageInfo(const char *aFilename){
	filename = NULL;
	readStruct = NULL;
	infoStruct = NULL;
	filename = new char[strlen(aFilename)+1];
	if(!filename) DETHROW(deeOutOfMemory);
	strcpy(filename, aFilename);
}
dePngImageInfo::~dePngImageInfo(){
	if(readStruct){
		png_destroy_read_struct(&readStruct, &infoStruct, NULL);
	}
	if(filename) delete [] filename;
}

// management
int dePngImageInfo::GetWidth(){
	return width;
}
int dePngImageInfo::GetHeight(){
	return height;
}
int dePngImageInfo::GetDepth(){
	return 1;
}
int dePngImageInfo::GetComponentCount(){
	return componentCount;
}
int dePngImageInfo::GetBitCount(){
	return bitCount;
}
