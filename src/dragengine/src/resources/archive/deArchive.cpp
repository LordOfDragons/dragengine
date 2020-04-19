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

#include "deArchive.h"
#include "deArchiveManager.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/archive/deBaseArchiveContainer.h"



// Class deArchive
////////////////////

// Constructor, destructor
////////////////////////////


deArchive::deArchive( deArchiveManager *manager, deVirtualFileSystem *vfs,
	const char *filename, TIME_SYSTEM modificationTime ) :
deFileResource( manager, vfs, filename, modificationTime ),
pPeerContainer( NULL ){
}

deArchive::~deArchive(){
	SetPeerContainer( NULL );
}



// System Peers
/////////////////



// Peers
//////////

void deArchive::SetPeerContainer( deBaseArchiveContainer *peer ){
	if( peer == pPeerContainer ){
		return;
	}
	
	if( pPeerContainer ){
		delete pPeerContainer;
	}
	pPeerContainer = peer;
}
