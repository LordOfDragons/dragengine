/* 
 * Drag[en]gine IGDE Skin Editor
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

#include "seProperty.h"
#include "sePropertyList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class seProperty
/////////////////////

// Constructor, destructor
////////////////////////////

sePropertyList::sePropertyList(){
}

sePropertyList::~sePropertyList(){
}



// Management
///////////////

int sePropertyList::GetCount() const{
	return pProperties.GetCount();
}

seProperty *sePropertyList::GetAt( int index ) const{
	return ( seProperty* )pProperties.GetAt( index );
}

seProperty *sePropertyList::GetNamed( const char *name ) const{
	const int count = pProperties.GetCount();
	seProperty *property;
	int p;
	
	for( p=0; p<count; p++ ){
		property = ( seProperty* )pProperties.GetAt( p );
		
		if( property->GetName().Equals( name ) ){
			return property;
		}
	}
	
	return NULL;
}

int sePropertyList::IndexOf( seProperty *property ) const{
	return pProperties.IndexOf( property );
}

int sePropertyList::IndexOfNamed( const char *name ) const{
	const int count = pProperties.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		if( ( ( seProperty* )pProperties.GetAt( p ) )->GetName().Equals( name ) ){
			return p;
		}
	}
	
	return -1;
}

bool sePropertyList::Has( seProperty *property ) const{
	return pProperties.Has( property );
}

bool sePropertyList::HasNamed( const char *name ) const{
	const int count = pProperties.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		if( ( ( seProperty* )pProperties.GetAt( p ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void sePropertyList::Add( seProperty *property ){
	if( ! property || HasNamed( property->GetName().GetString() ) ) DETHROW( deeInvalidParam );
	
	pProperties.Add( property );
}

void sePropertyList::InsertAt( seProperty *property, int index ){
	if( ! property || HasNamed( property->GetName().GetString() ) ) DETHROW( deeInvalidParam );
	
	pProperties.Insert( property, index );
}

void sePropertyList::MoveTo( seProperty *property, int index ){
	pProperties.Move( property, index );
}

void sePropertyList::Remove( seProperty *property ){
	pProperties.Remove( property );
}

void sePropertyList::RemoveAll(){
	pProperties.RemoveAll();
}



sePropertyList &sePropertyList::operator=( const sePropertyList &list ){
	pProperties = list.pProperties;
	return *this;
}
