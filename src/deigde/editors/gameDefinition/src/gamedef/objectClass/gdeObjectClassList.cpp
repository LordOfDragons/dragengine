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

#include "gdeObjectClass.h"
#include "gdeObjectClassList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeObjectClass
/////////////////////////

// Constructor, destructor
////////////////////////////

gdeObjectClassList::gdeObjectClassList(){
}

gdeObjectClassList::gdeObjectClassList( const gdeObjectClassList &list ) :
pObjectClasses( list.pObjectClasses ){
}

gdeObjectClassList::~gdeObjectClassList(){
}



// Management
///////////////

int gdeObjectClassList::GetCount() const{
	return pObjectClasses.GetCount();
}

gdeObjectClass *gdeObjectClassList::GetAt( int index ) const{
	return ( gdeObjectClass* )pObjectClasses.GetAt( index );
}

gdeObjectClass *gdeObjectClassList::GetNamed( const char *name ) const{
	const int count = pObjectClasses.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		gdeObjectClass * const objectClass = ( gdeObjectClass* )pObjectClasses.GetAt( i );
		
		if( objectClass->GetName() == name ){
			return objectClass;
		}
	}
	
	return NULL;
}

int gdeObjectClassList::IndexOf( gdeObjectClass *objectClass ) const{
	return pObjectClasses.IndexOf( objectClass );
}

int gdeObjectClassList::IndexOfNamed( const char *name ) const{
	const int count = pObjectClasses.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( gdeObjectClass* )pObjectClasses.GetAt( i ) )->GetName() == name ){
			return i;
		}
	}
	
	return -1;
}

bool gdeObjectClassList::Has( gdeObjectClass *objectClass ) const{
	return pObjectClasses.Has( objectClass );
}

bool gdeObjectClassList::HasNamed( const char *name ) const{
	const int count = pObjectClasses.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( gdeObjectClass* )pObjectClasses.GetAt( i ) )->GetName() == name ){
			return true;
		}
	}
	
	return false;
}

void gdeObjectClassList::Add( gdeObjectClass *objectClass ){
	if( ! objectClass || HasNamed( objectClass->GetName() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pObjectClasses.Add( objectClass );
}

void gdeObjectClassList::Remove( gdeObjectClass *objectClass ){
	pObjectClasses.Remove( objectClass );
}

void gdeObjectClassList::RemoveAll(){
	pObjectClasses.RemoveAll();
}



gdeObjectClassList &gdeObjectClassList::operator=( const gdeObjectClassList &list ){
	pObjectClasses = list.pObjectClasses;
	return *this;
}
