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

#include "igdeGDCForceField.h"
#include "igdeGDCForceFieldList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCForceField
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCForceFieldList::igdeGDCForceFieldList(){
}

igdeGDCForceFieldList::igdeGDCForceFieldList( const igdeGDCForceFieldList &list ){
	pFields = list.pFields;
}

igdeGDCForceFieldList::~igdeGDCForceFieldList(){
}



// Management
///////////////

int igdeGDCForceFieldList::GetCount() const{
	return pFields.GetCount();
}

igdeGDCForceField *igdeGDCForceFieldList::GetAt( int index ) const{
	return ( igdeGDCForceField* )pFields.GetAt( index );
}

int igdeGDCForceFieldList::IndexOf( igdeGDCForceField *field ) const{
	return pFields.IndexOf( field );
}

bool igdeGDCForceFieldList::Has( igdeGDCForceField *field ) const{
	return pFields.Has( field );
}

void igdeGDCForceFieldList::Add( igdeGDCForceField *field ){
	if( ! field ){
		DETHROW( deeInvalidParam );
	}
	pFields.Add( field );
}

void igdeGDCForceFieldList::InsertAt( igdeGDCForceField *field, int index ){
	if( ! field ){
		DETHROW( deeInvalidParam );
	}
	pFields.Insert( field, index );
}

void igdeGDCForceFieldList::MoveTo( igdeGDCForceField *field, int index ){
	pFields.Move( field, index );
}

void igdeGDCForceFieldList::Remove( igdeGDCForceField *field ){
	pFields.Remove( field );
}

void igdeGDCForceFieldList::RemoveAll(){
	pFields.RemoveAll();
}



void igdeGDCForceFieldList::SetToDeepCopyFrom( const igdeGDCForceFieldList &list ){
	const int count = list.GetCount();
	igdeGDCForceField *field = NULL;
	
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			field = new igdeGDCForceField( *list.GetAt( i ) );
			Add( field );
			field->FreeReference();
			field = NULL;
		}
		
	}catch( const deException & ){
		if( field ){
			field->FreeReference();
		}
		throw;
	}
}



// Operators
//////////////

igdeGDCForceFieldList &igdeGDCForceFieldList::operator=( const igdeGDCForceFieldList &list ){
	pFields = list.pFields;
	return *this;
}
