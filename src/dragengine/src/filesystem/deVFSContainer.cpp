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
#include "deVFSContainer.h"
#include "../common/exceptions.h"



// Class deVFSContainer
/////////////////////////

// Constructor, destructor
////////////////////////////

deVFSContainer::deVFSContainer() :
pRootPath( decPath::CreatePathUnix( "/" ) ),
pHidden( false ){
}

deVFSContainer::deVFSContainer( const decPath &rootPath ) :
pRootPath( rootPath ),
pHidden( false ){
}

deVFSContainer::~deVFSContainer(){
}



// Management
///////////////

void deVFSContainer::SetHidden( bool hidden ){
	pHidden = hidden;
}

int deVFSContainer::GetHiddenPathCount() const{
	return pHiddenPath.GetCount();
}

const decPath &deVFSContainer::GetHiddenPathAt( int index ) const{
	return pHiddenPath.GetAt( index );
}

bool deVFSContainer::HasHiddenPath( const decPath &path ) const{
	return pHiddenPath.Has( path );
}

void deVFSContainer::AddHiddenPath( const decPath &path ){
	pHiddenPath.AddIfAbsent( path );
}

void deVFSContainer::RemoveHiddenPath( const decPath &path ){
	pHiddenPath.RemoveIfPresent( path );
}

void deVFSContainer::RemoveAllHiddenPath(){
	pHiddenPath.RemoveAll();
}

bool deVFSContainer::IsPathHiddenBelow( const decPath &path ){
	const int count = pHiddenPath.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		if( pHiddenPath.GetAt( i ).IsEqualOrParentOf( path ) ){
			return true;
		}
	}
	return false;
}
