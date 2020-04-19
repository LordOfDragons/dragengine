/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeSkin.h"
#include "gdeSkinList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeSkin
//////////////////

// Constructor, destructor
////////////////////////////

gdeSkinList::gdeSkinList(){
}

gdeSkinList::gdeSkinList( const gdeSkinList &list ) :
pSkins( list.pSkins ){
}

gdeSkinList::~gdeSkinList(){
}



// Management
///////////////

int gdeSkinList::GetCount() const{
	return pSkins.GetCount();
}

gdeSkin *gdeSkinList::GetAt( int index ) const{
	return ( gdeSkin* )pSkins.GetAt( index );
}

gdeSkin *gdeSkinList::GetWithPath( const char *path ) const{
	const int count = pSkins.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		gdeSkin * const skin = ( gdeSkin* )pSkins.GetAt( i );
		
		if( skin->GetPath() == path ){
			return skin;
		}
	}
	
	return NULL;
}

int gdeSkinList::IndexOf( gdeSkin *skin ) const{
	return pSkins.IndexOf( skin );
}

int gdeSkinList::IndexOfWithPath( const char *path ) const{
	const int count = pSkins.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( gdeSkin* )pSkins.GetAt( i ) )->GetPath() == path ){
			return i;
		}
	}
	
	return -1;
}

bool gdeSkinList::Has( gdeSkin *skin ) const{
	return pSkins.Has( skin );
}

bool gdeSkinList::HasWithPath( const char *path ) const{
	const int count = pSkins.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( gdeSkin* )pSkins.GetAt( i ) )->GetPath() == path ){
			return true;
		}
	}
	
	return false;
}

void gdeSkinList::Add( gdeSkin *skin ){
	if( ! skin || HasWithPath( skin->GetPath() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pSkins.Add( skin );
}

void gdeSkinList::Remove( gdeSkin *skin ){
	pSkins.Remove( skin );
}

void gdeSkinList::RemoveAll(){
	pSkins.RemoveAll();
}



gdeSkinList &gdeSkinList::operator=( const gdeSkinList &list ){
	pSkins = list.pSkins;
	return *this;
}
