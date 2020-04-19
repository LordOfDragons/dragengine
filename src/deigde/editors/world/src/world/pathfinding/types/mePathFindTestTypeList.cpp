/* 
 * Drag[en]gine IGDE World Editor
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

#include "mePathFindTestType.h"
#include "mePathFindTestTypeList.h"

#include <dragengine/common/exceptions.h>



// Class mePathFindTestTypeList
/////////////////////////////////

// Constructor, destructor
////////////////////////////

mePathFindTestTypeList::mePathFindTestTypeList(){
}

mePathFindTestTypeList::~mePathFindTestTypeList(){
}



// Management
///////////////

int mePathFindTestTypeList::GetCount() const{
	return pTypes.GetCount();
}

mePathFindTestType *mePathFindTestTypeList::GetAt( int position ) const{
	return ( mePathFindTestType* )pTypes.GetAt( position );
}

mePathFindTestType *mePathFindTestTypeList::GetWith( int typeNumber ) const{
	const int count = pTypes.GetCount();
	mePathFindTestType *type;
	int i;
	
	for( i=0; i<count; i++ ){
		type = ( mePathFindTestType* )pTypes.GetAt( i );
		
		if( type->GetTypeNumber() == typeNumber ){
			return type;
		}
	}
	
	return NULL;
}

int mePathFindTestTypeList::IndexOf( mePathFindTestType *type ) const{
	return pTypes.IndexOf( type );
}

int mePathFindTestTypeList::IndexWith( int typeNumber ) const{
	const int count = pTypes.GetCount();
	mePathFindTestType *type;
	int i;
	
	for( i=0; i<count; i++ ){
		type = ( mePathFindTestType* )pTypes.GetAt( i );
		
		if( type->GetTypeNumber() == typeNumber ){
			return i;
		}
	}
	
	return -1;
}

bool mePathFindTestTypeList::Has( mePathFindTestType *type ) const{
	return pTypes.Has( type );
}

bool mePathFindTestTypeList::HasWith( int typeNumber ) const{
	const int count = pTypes.GetCount();
	mePathFindTestType *type;
	int i;
	
	for( i=0; i<count; i++ ){
		type = ( mePathFindTestType* )pTypes.GetAt( i );
		
		if( type->GetTypeNumber() == typeNumber ){
			return true;
		}
	}
	
	return false;
}

void mePathFindTestTypeList::Add( mePathFindTestType *type ){
	if( ! type || HasWith( type->GetTypeNumber() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pTypes.Add( type );
}

void mePathFindTestTypeList::AddIfAbsent( mePathFindTestType *type ){
	if( ! type || HasWith( type->GetTypeNumber() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pTypes.AddIfAbsent( type );
}

void mePathFindTestTypeList::Remove( mePathFindTestType *type ){
	pTypes.Remove( type );
}

void mePathFindTestTypeList::RemoveIfPresent( mePathFindTestType *type ){
	pTypes.RemoveIfPresent( type );
}

void mePathFindTestTypeList::RemoveAll(){
	pTypes.RemoveAll();
}
