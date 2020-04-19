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
#include "deFileResourceManager.h"
#include "../common/exceptions.h"
#include "../filesystem/deVirtualFileSystem.h"



// Class deFileResource
/////////////////////////

// Constructor, destructor
////////////////////////////

deFileResource::deFileResource( deFileResourceManager *resourceManager,
	deVirtualFileSystem *vfs, const char *filename, TIME_SYSTEM modificationTime ) : 
deResource( resourceManager ),
pVirtualFileSystem( NULL ),
pFilename( filename ),
pModificationTime( modificationTime ),
pAsynchron( false ),
pOutdated( false )
{
	if( vfs ){
		pVirtualFileSystem = vfs;
		vfs->AddReference();
	}
}

deFileResource::~deFileResource(){
	if( pVirtualFileSystem ){
		pVirtualFileSystem->FreeReference();
	}
}



// Management
///////////////

void deFileResource::SetModificationTime( TIME_SYSTEM modificationTime ){
	pModificationTime = modificationTime;
}

void deFileResource::SetAsynchron( bool asynchron ){
	pAsynchron = asynchron;
}

void deFileResource::MarkOutdated(){
	pOutdated = true;
}
