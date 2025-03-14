/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
