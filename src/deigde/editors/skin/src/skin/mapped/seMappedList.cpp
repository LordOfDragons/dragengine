/* 
 * Drag[en]gine IGDE Skin Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "seMapped.h"
#include "seMappedList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class seMapped
////////////////////

// Constructor, destructor
////////////////////////////

seMappedList::seMappedList(){
}

seMappedList::~seMappedList(){
}



// Management
///////////////

int seMappedList::GetCount() const{
	return pMapped.GetCount();
}

seMapped *seMappedList::GetAt( int index ) const{
	return ( seMapped* )pMapped.GetAt( index );
}

seMapped *seMappedList::GetNamed( const char *name ) const{
	const int count = pMapped.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		seMapped * const mapped = ( seMapped* )pMapped.GetAt( i );
		if( mapped->GetName().Equals( name ) ){
			return mapped;
		}
	}
	
	return nullptr;
}

int seMappedList::IndexOf( seMapped *mapped ) const{
	return pMapped.IndexOf( mapped );
}

int seMappedList::IndexOfNamed( const char *name ) const{
	const int count = pMapped.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( seMapped* )pMapped.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

bool seMappedList::Has( seMapped *mapped ) const{
	return pMapped.Has( mapped );
}

bool seMappedList::HasNamed( const char *name ) const{
	const int count = pMapped.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( seMapped* )pMapped.GetAt( i ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void seMappedList::Add( seMapped *mapped ){
	DEASSERT_NOTNULL( mapped )
	DEASSERT_FALSE( HasNamed( mapped->GetName() ) )
	
	pMapped.Add( mapped );
}

void seMappedList::Remove( seMapped *mapped ){
	pMapped.Remove( mapped );
}

void seMappedList::RemoveAll(){
	pMapped.RemoveAll();
}



seMappedList &seMappedList::operator=( const seMappedList &list ){
	pMapped = list.pMapped;
	return *this;
}
