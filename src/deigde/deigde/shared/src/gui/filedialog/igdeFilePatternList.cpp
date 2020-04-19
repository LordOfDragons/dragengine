/* 
 * Drag[en]gine IGDE
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
#include "igdeFilePattern.h"
#include "igdeFilePatternList.h"
#include "dragengine/common/exceptions.h"



// Class igdeFilePatternList
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeFilePatternList::igdeFilePatternList(){
	pFilePatterns = NULL;
	pFilePatternCount = 0;
	pFilePatternSize = 0;
}

igdeFilePatternList::igdeFilePatternList( const igdeFilePatternList &list ){
	pFilePatterns = NULL;
	pFilePatternCount = 0;
	pFilePatternSize = 0;
	*this = list;
}

igdeFilePatternList::~igdeFilePatternList(){
	RemoveAllFilePatterns();
	if( pFilePatterns ){
		delete [] pFilePatterns;
	}
}



// Management
///////////////

igdeFilePattern *igdeFilePatternList::GetFilePatternAt( int index ) const{
	if( index < 0 || index >= pFilePatternCount ) DETHROW( deeOutOfBoundary );
	
	return pFilePatterns[ index ];
}

int igdeFilePatternList::IndexOfFilePattern( igdeFilePattern *filePattern ) const{
	if( ! filePattern ) DETHROW( deeInvalidParam );
	int f;
	
	for( f=0; f<pFilePatternCount; f++ ){
		if( filePattern == pFilePatterns[ f ] ){
			return f;
		}
	}
	
	return -1;
}

bool igdeFilePatternList::HasFilePattern( igdeFilePattern *filePattern ) const{
	if( ! filePattern ) DETHROW( deeInvalidParam );
	int f;
	
	for( f=0; f<pFilePatternCount; f++ ){
		if( filePattern == pFilePatterns[ f ] ){
			return true;
		}
	}
	
	return false;
}

void igdeFilePatternList::AddFilePattern( igdeFilePattern *filePattern ){
	if( HasFilePattern( filePattern ) ) DETHROW( deeInvalidParam );
	
	if( pFilePatternCount == pFilePatternSize ){
		int newSize = pFilePatternSize * 3 / 2 + 1;
		igdeFilePattern **newArray = new igdeFilePattern*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pFilePatterns ){
			memcpy( newArray, pFilePatterns, sizeof( igdeFilePattern* ) * pFilePatternSize );
			delete [] pFilePatterns;
		}
		pFilePatterns = newArray;
		pFilePatternSize = newSize;
	}
	
	pFilePatterns[ pFilePatternCount ] = filePattern;
	pFilePatternCount++;
}

void igdeFilePatternList::RemoveFilePattern( igdeFilePattern *filePattern ){
	int i, index = IndexOfFilePattern( filePattern );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	for( i=index+1; i<pFilePatternCount; i++ ){
		pFilePatterns[ i - 1 ] = pFilePatterns[ i ];
	}
	pFilePatternCount--;
	
	delete filePattern;
}

void igdeFilePatternList::RemoveAllFilePatterns(){
	while( pFilePatternCount > 0 ){
		pFilePatternCount--;
		delete pFilePatterns[ pFilePatternCount ];
	}
}



// Operators
//////////////

igdeFilePatternList &igdeFilePatternList::operator=( const igdeFilePatternList &list ){
	const int count = list.GetFilePatternCount();
	igdeFilePattern *filePattern = NULL;
	int i;
	
	RemoveAllFilePatterns();
	
	for( i=0; i<count; i++ ){
		try{
			filePattern = new igdeFilePattern( *list.GetFilePatternAt( i ) );
			AddFilePattern( filePattern );
			filePattern = NULL;
			
		}catch( const deException & ){
			if( filePattern ){
				delete filePattern;
			}
			throw;
		}
	}
	
	return *this;
}
