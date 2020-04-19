/* 
 * Drag[en]gine Game Engine
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

#include "deFileResource.h"
#include "deFileResourceList.h"
#include "../common/exceptions.h"



// Class deFileResourceList
/////////////////////////////

// Constructor, destructor
////////////////////////////

deFileResourceList::deFileResourceList(){
}

deFileResourceList::~deFileResourceList(){
}



// Management
///////////////

deResource *deFileResourceList::GetWithFilename( deVirtualFileSystem *vfs, const char *filename ) const{
	if( ! vfs || ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	deFileResource *resource = ( deFileResource* )GetRoot();
	while( resource ){
		if( ! resource->GetOutdated()
		&& resource->GetVirtualFileSystem() == vfs
		&& resource->GetFilename() == filename ){
			return resource;
		}
		resource = ( deFileResource* )resource->GetLLManagerNext();
	}
	
	return NULL;
}
