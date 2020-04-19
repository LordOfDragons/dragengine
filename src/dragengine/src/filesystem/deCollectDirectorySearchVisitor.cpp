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

#include <stdlib.h>
#include <string.h>

#include "dePathList.h"
#include "deCollectDirectorySearchVisitor.h"
#include "deVirtualFileSystem.h"
#include "../common/file/decPath.h"



// Class deCollectDirectorySearchVisitor
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deCollectDirectorySearchVisitor::deCollectDirectorySearchVisitor( bool recursive ) :
pRecursion( recursive ){
}

deCollectDirectorySearchVisitor::~deCollectDirectorySearchVisitor(){
}



// Management
///////////////

void deCollectDirectorySearchVisitor::SetRecursion( bool recursion ){
	pRecursion = recursion;
}



// Visiting
/////////////

bool deCollectDirectorySearchVisitor::VisitFile( const deVirtualFileSystem &vfs, const decPath &path ){
	return true;
}

bool deCollectDirectorySearchVisitor::VisitDirectory( const deVirtualFileSystem &vfs, const decPath &path ){
	pDirectories.Add( path );
	if( pRecursion ){
		vfs.SearchFiles( path, *this );
	}
	return true;
}

bool deCollectDirectorySearchVisitor::VisitSpecial( const deVirtualFileSystem &vfs, const decPath &path ){
	return true;
}
