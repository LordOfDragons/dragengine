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

#include "gdeProperty.h"
#include "gdePropertyList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeProperty
//////////////////////

// Constructor, destructor
////////////////////////////

gdePropertyList::gdePropertyList(){
}

gdePropertyList::gdePropertyList( const gdePropertyList &list ) :
pProperties( list.pProperties ){
}

gdePropertyList::~gdePropertyList(){
}



// Management
///////////////

int gdePropertyList::GetCount() const{
	return pProperties.GetCount();
}

gdeProperty *gdePropertyList::GetAt( int index ) const{
	return ( gdeProperty* )pProperties.GetAt( index );
}

gdeProperty *gdePropertyList::GetNamed( const char *name ) const{
	const int count = pProperties.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		gdeProperty * const property = ( gdeProperty* )pProperties.GetAt( i );
		
		if( property->GetName() == name ){
			return property;
		}
	}
	
	return NULL;
}

int gdePropertyList::IndexOf( gdeProperty *property ) const{
	return pProperties.IndexOf( property );
}

int gdePropertyList::IndexOfNamed( const char *name ) const{
	const int count = pProperties.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( gdeProperty* )pProperties.GetAt( i ) )->GetName() == name ){
			return i;
		}
	}
	
	return -1;
}

bool gdePropertyList::Has( gdeProperty *property ) const{
	return pProperties.Has( property );
}

bool gdePropertyList::HasNamed( const char *name ) const{
	const int count = pProperties.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( gdeProperty* )pProperties.GetAt( i ) )->GetName() == name ){
			return true;
		}
	}
	
	return false;
}

void gdePropertyList::Add( gdeProperty *property ){
	if( ! property || HasNamed( property->GetName() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pProperties.Add( property );
}

void gdePropertyList::Remove( gdeProperty *property ){
	pProperties.Remove( property );
}

void gdePropertyList::RemoveAll(){
	pProperties.RemoveAll();
}



gdePropertyList &gdePropertyList::operator=( const gdePropertyList &list ){
	pProperties = list.pProperties;
	return *this;
}
