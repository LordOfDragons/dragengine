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
#include "deCollectFileSearchVisitor.h"
#include "deVirtualFileSystem.h"
#include "../common/file/decPath.h"



// Class deCollectFileSearchVisitor
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deCollectFileSearchVisitor::deCollectFileSearchVisitor() :
pRecursive( false ){
}

deCollectFileSearchVisitor::deCollectFileSearchVisitor( const char *pattern, bool recursive ) :
pRecursive( recursive )
{
	AddPattern( pattern );
}

deCollectFileSearchVisitor::~deCollectFileSearchVisitor(){
}



// Management
///////////////

void deCollectFileSearchVisitor::AddPattern( const char *pattern ){
	decPath path;
	path.AddComponent( pattern );
	pPatterns.Add( path );
}

void deCollectFileSearchVisitor::SetRecursive( bool recursion ){
	pRecursive = recursion;
}



// Visiting
/////////////

bool deCollectFileSearchVisitor::VisitFile( const deVirtualFileSystem &, const decPath &path ){
	const int patternCount = pPatterns.GetCount();
	if( patternCount == 0 ){
		pFiles.Add( path );
		return true;
	}
	
	int i;
	for( i=0; i<patternCount; i++ ){
		if( path.MatchesPattern( pPatterns.GetAt( i ) ) ){
			pFiles.Add( path );
			return true;
		}
	}
	
	return true;
}

bool deCollectFileSearchVisitor::VisitDirectory( const deVirtualFileSystem &vfs, const decPath &path ){
	if( pRecursive ){
		vfs.SearchFiles( path, *this );
	}
	return true;
}

bool deCollectFileSearchVisitor::VisitSpecial( const deVirtualFileSystem &, const decPath &path ){
	return true;
}
