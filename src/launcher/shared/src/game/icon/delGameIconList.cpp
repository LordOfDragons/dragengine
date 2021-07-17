/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "delGameIcon.h"
#include "delGameIconList.h"

#include <dragengine/common/exceptions.h>


// Class delGameIconList
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

delGameIconList::delGameIconList(){
}

delGameIconList::~delGameIconList(){
	RemoveAll();
}



// Management
///////////////

int delGameIconList::GetCount() const{
	return pIcons.GetCount();
}

delGameIcon *delGameIconList::GetAt( int index ) const{
	return ( delGameIcon* )pIcons.GetAt( index );
}

delGameIcon *delGameIconList::GetWithSize( int size ) const{
	const int count = pIcons.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delGameIcon * const icon = ( delGameIcon* )pIcons.GetAt( i );
		if( icon->GetSize() == size ){
			return icon;
		}
	}
	
	return nullptr;
}

delGameIcon *delGameIconList::GetWithSizeLargest( int size ) const{
	const int count = pIcons.GetCount();
	delGameIcon *bestIcon = nullptr;
	int i;
	
	for( i=0; i<count; i++ ){
		delGameIcon * const icon = ( delGameIcon* )pIcons.GetAt( i );
		if( icon->GetSize() > size ){
			continue;
		}
		
		if( ! bestIcon || icon->GetSize() > bestIcon->GetSize() ){
			bestIcon = icon;
		}
	}
	
	return bestIcon;
}

delGameIcon *delGameIconList::GetWithSizeSmallest( int size ) const{
	const int count = pIcons.GetCount();
	delGameIcon *bestIcon = nullptr;
	int i;
	
	for( i=0; i<count; i++ ){
		delGameIcon * const icon = ( delGameIcon* )pIcons.GetAt( i );
		if( icon->GetSize() < size ){
			continue;
		}
		
		if( ! bestIcon || icon->GetSize() < bestIcon->GetSize() ){
			bestIcon = icon;
		}
	}
	
	return bestIcon;
}

bool delGameIconList::Has( delGameIcon *icon ) const{
	return pIcons.Has( icon );
}

bool delGameIconList::HasWithSize( int size ) const{
	const int count = pIcons.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const delGameIcon * const icon = ( delGameIcon* )pIcons.GetAt( i );
		if( icon->GetSize() == size ){
			return true;
		}
	}
	
	return false;
}

int delGameIconList::IndexOf( delGameIcon *icon ) const{
	return pIcons.IndexOf( icon );
}

int delGameIconList::IndexOfWithSize( int size ) const{
	const int count = pIcons.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const delGameIcon * const icon = ( delGameIcon* )pIcons.GetAt( i );
		if( icon->GetSize() == size ){
			return i;
		}
	}
	
	return -1;
}

void delGameIconList::Add( delGameIcon *icon ){
	if( ! icon || HasWithSize( icon->GetSize() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pIcons.Add( icon );
}

void delGameIconList::Remove( delGameIcon *icon ){
	const int index = IndexOf( icon );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pIcons.RemoveFrom( index );
}

void delGameIconList::RemoveAll(){
	pIcons.RemoveAll();
}
