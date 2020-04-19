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

#include "igdeGameDefinition.h"
#include "igdeGameDefinitionList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGameDefinition
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGameDefinitionList::igdeGameDefinitionList(){
}

igdeGameDefinitionList::igdeGameDefinitionList( const igdeGameDefinitionList &list ) :
pGameDefinitions( list.pGameDefinitions ){
}

igdeGameDefinitionList::~igdeGameDefinitionList(){
}



// Management
///////////////

int igdeGameDefinitionList::GetCount() const{
	return pGameDefinitions.GetCount();
}

igdeGameDefinition *igdeGameDefinitionList::GetAt( int index ) const{
	return ( igdeGameDefinition* )pGameDefinitions.GetAt( index );
}

igdeGameDefinition *igdeGameDefinitionList::GetWithID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pGameDefinitions.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		igdeGameDefinition * const gd = ( igdeGameDefinition* )pGameDefinitions.GetAt( i );
		if( gd->GetID() == id ){
			return gd;
		}
	}
	
	return NULL;
}

int igdeGameDefinitionList::IndexOf( igdeGameDefinition *gameDefinition ) const{
	return pGameDefinitions.IndexOf( gameDefinition );
}

bool igdeGameDefinitionList::Has( igdeGameDefinition *gameDefinition ) const{
	return pGameDefinitions.Has( gameDefinition );
}

bool igdeGameDefinitionList::HasWithID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pGameDefinitions.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( igdeGameDefinition* )pGameDefinitions.GetAt( i ) )->GetID() == id ){
			return true;
		}
	}
	
	return false;
}

void igdeGameDefinitionList::Add( igdeGameDefinition *gameDefinition ){
	if( ! gameDefinition || HasWithID( gameDefinition->GetID() ) ){
		DETHROW( deeInvalidParam );
	}
	pGameDefinitions.Add( gameDefinition );
}

void igdeGameDefinitionList::Remove( igdeGameDefinition *gameDefinition ){
	pGameDefinitions.Remove( gameDefinition );
}

void igdeGameDefinitionList::RemoveAll(){
	pGameDefinitions.RemoveAll();
}



// Operators
//////////////

igdeGameDefinitionList &igdeGameDefinitionList::operator=( const igdeGameDefinitionList &list ){
	pGameDefinitions = list.pGameDefinitions;
	return *this;
}
