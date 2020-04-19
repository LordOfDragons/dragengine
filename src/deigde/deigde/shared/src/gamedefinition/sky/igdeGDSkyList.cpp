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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeGDSky.h"
#include "igdeGDSkyList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDSky
////////////////////

// Constructor, destructor
////////////////////////////

igdeGDSkyList::igdeGDSkyList(){
}

igdeGDSkyList::~igdeGDSkyList(){
}



// Management
///////////////

int igdeGDSkyList::GetCount() const{
	return pSkies.GetCount();
}

igdeGDSky *igdeGDSkyList::GetAt( int index ) const{
	return ( igdeGDSky* )pSkies.GetAt( index );
}

igdeGDSky *igdeGDSkyList::GetWithPath( const char *path ) const{
	if( ! path ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pSkies.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		igdeGDSky * const sky = ( igdeGDSky* )pSkies.GetAt( i );
		if( sky->GetPath().Equals( path ) ){
			return sky;
		}
	}
	
	return NULL;
}

igdeGDSky *igdeGDSkyList::GetWithName( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pSkies.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		igdeGDSky * const sky = ( igdeGDSky* )pSkies.GetAt( i );
		if( sky->GetName().Equals( name ) ){
			return sky;
		}
	}
	
	return NULL;
}

int igdeGDSkyList::IndexOf( igdeGDSky *sky ) const{
	return pSkies.IndexOf( sky );
}

int igdeGDSkyList::IndexOfWithPath( const char *path ) const{
	if( ! path ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pSkies.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( igdeGDSky* )pSkies.GetAt( i ) )->GetPath() == path ){
			return i;
		}
	}
	
	return -1;
}

int igdeGDSkyList::IndexOfWithName( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pSkies.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( igdeGDSky* )pSkies.GetAt( i ) )->GetName() == name ){
			return i;
		}
	}
	
	return -1;
}

int igdeGDSkyList::IndexOfWithPathOrName( const char *path, const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pSkies.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const igdeGDSky &sky = *( ( igdeGDSky* )pSkies.GetAt( i ) );
		if( sky.GetPath() == path || sky.GetName() == name ){
			return i;
		}
	}
	
	return -1;
}

bool igdeGDSkyList::Has( igdeGDSky *sky ) const{
	return pSkies.Has( sky );
}

bool igdeGDSkyList::HasWithPath( const char *path ) const{
	return IndexOfWithPath( path ) != -1;
}

bool igdeGDSkyList::HasWithName( const char *name ) const{
	return IndexOfWithName( name ) != -1;
}

bool igdeGDSkyList::HasWithPathOrName( const char *path, const char *name ) const{
	return IndexOfWithPathOrName( path, name ) != -1;
}

void igdeGDSkyList::Add( igdeGDSky *sky ){
	if( ! sky || HasWithPathOrName( sky->GetPath(), sky->GetName() ) ){
		DETHROW( deeInvalidParam );
	}
	pSkies.Add( sky );
}

void igdeGDSkyList::InsertAt( igdeGDSky *sky, int index ){
	if( ! sky ){
		DETHROW( deeInvalidParam );
	}
	pSkies.Insert( sky, index );
}

void igdeGDSkyList::MoveTo( igdeGDSky *sky, int index ){
	pSkies.Move( sky, index );
}

void igdeGDSkyList::Remove( igdeGDSky *sky ){
	pSkies.Remove( sky );
}

void igdeGDSkyList::RemoveAll(){
	pSkies.RemoveAll();
}



igdeGDSkyList &igdeGDSkyList::operator=( const igdeGDSkyList &list ){
	pSkies = list.pSkies;
	return *this;
}
