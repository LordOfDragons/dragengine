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

#include "gdeSky.h"
#include "gdeSkyList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeSky
/////////////////

// Constructor, destructor
////////////////////////////

gdeSkyList::gdeSkyList(){
}

gdeSkyList::gdeSkyList( const gdeSkyList &list ) :
pSkys( list.pSkys ){
}

gdeSkyList::~gdeSkyList(){
}



// Management
///////////////

int gdeSkyList::GetCount() const{
	return pSkys.GetCount();
}

gdeSky *gdeSkyList::GetAt( int index ) const{
	return ( gdeSky* )pSkys.GetAt( index );
}

gdeSky *gdeSkyList::GetWithPath( const char *path ) const{
	const int count = pSkys.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		gdeSky * const sky = ( gdeSky* )pSkys.GetAt( i );
		
		if( sky->GetPath() == path ){
			return sky;
		}
	}
	
	return NULL;
}

int gdeSkyList::IndexOf( gdeSky *sky ) const{
	return pSkys.IndexOf( sky );
}

int gdeSkyList::IndexOfWithPath( const char *path ) const{
	const int count = pSkys.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( gdeSky* )pSkys.GetAt( i ) )->GetPath() == path ){
			return i;
		}
	}
	
	return -1;
}

bool gdeSkyList::Has( gdeSky *sky ) const{
	return pSkys.Has( sky );
}

bool gdeSkyList::HasWithPath( const char *path ) const{
	const int count = pSkys.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( gdeSky* )pSkys.GetAt( i ) )->GetPath() == path ){
			return true;
		}
	}
	
	return false;
}

void gdeSkyList::Add( gdeSky *sky ){
	if( ! sky || HasWithPath( sky->GetPath() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pSkys.Add( sky );
}

void gdeSkyList::Remove( gdeSky *sky ){
	pSkys.Remove( sky );
}

void gdeSkyList::RemoveAll(){
	pSkys.RemoveAll();
}



gdeSkyList &gdeSkyList::operator=( const gdeSkyList &list ){
	pSkys = list.pSkys;
	return *this;
}
