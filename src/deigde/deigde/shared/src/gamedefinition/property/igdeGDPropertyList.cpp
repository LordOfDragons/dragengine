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

#include "igdeGDProperty.h"
#include "igdeGDPropertyReference.h"
#include "igdeGDPropertyList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDProperty
//////////////////////

// Constructor, destructor
////////////////////////////

igdeGDPropertyList::igdeGDPropertyList(){
}

igdeGDPropertyList::igdeGDPropertyList( const igdeGDPropertyList &list ) :
pProperties( list.pProperties ){
}

igdeGDPropertyList::~igdeGDPropertyList(){
}



// Management
///////////////

int igdeGDPropertyList::GetCount() const{
	return pProperties.GetCount();
}

igdeGDProperty *igdeGDPropertyList::GetAt( int index ) const{
	return ( igdeGDProperty* )pProperties.GetAt( index );
}

igdeGDProperty *igdeGDPropertyList::GetNamed( const char *name ) const{
	const int count = pProperties.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		igdeGDProperty * const property = ( igdeGDProperty* )pProperties.GetAt( i );
		if( property->GetName() == name ){
			return property;
		}
	}
	
	return NULL;
}

int igdeGDPropertyList::IndexOf( igdeGDProperty *property ) const{
	return pProperties.IndexOf( property );
}

int igdeGDPropertyList::IndexOfNamed( const char *name ) const{
	const int count = pProperties.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( igdeGDProperty* )pProperties.GetAt( i ) )->GetName() == name ){
			return i;
		}
	}
	
	return -1;
}

bool igdeGDPropertyList::Has( igdeGDProperty *property ) const{
	return pProperties.Has( property );
}

bool igdeGDPropertyList::HasNamed( const char *name ) const{
	const int count = pProperties.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( igdeGDProperty* )pProperties.GetAt( i ) )->GetName() == name ){
			return true;
		}
	}
	
	return false;
}

void igdeGDPropertyList::Add( igdeGDProperty *property ){
	if( ! property || HasNamed( property->GetName() ) ){
		DETHROW( deeInvalidParam );
	}
	pProperties.Add( property );
}

void igdeGDPropertyList::Remove( igdeGDProperty *property ){
	pProperties.Remove( property );
}

void igdeGDPropertyList::RemoveAll(){
	pProperties.RemoveAll();
}

void igdeGDPropertyList::SetToDeepCopyFrom( const igdeGDPropertyList &list ){
	const int count = list.pProperties.GetCount();
	igdeGDPropertyReference property;
	int i;
	
	for( i=0; i<count; i++ ){
		property.TakeOver( new igdeGDProperty( *list.GetAt( i ) ) );
		Add( property );
	}
}



igdeGDPropertyList &igdeGDPropertyList::operator=( const igdeGDPropertyList &list ){
	pProperties = list.pProperties;
	return *this;
}
