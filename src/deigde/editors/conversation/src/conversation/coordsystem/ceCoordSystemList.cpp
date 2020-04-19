/* 
 * Drag[en]gine IGDE Conversation Editor
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

#include "ceCoordSystem.h"
#include "ceCoordSystemList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceCoordSystem
////////////////////////

// Constructor, destructor
////////////////////////////

ceCoordSystemList::ceCoordSystemList(){
}

ceCoordSystemList::~ceCoordSystemList(){
}



// Management
///////////////

int ceCoordSystemList::GetCount() const{
	return pCoordSystems.GetCount();
}

ceCoordSystem *ceCoordSystemList::GetAt( int index ) const{
	return ( ceCoordSystem* )pCoordSystems.GetAt( index );
}

ceCoordSystem *ceCoordSystemList::GetWithID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	ceCoordSystem *coordSystem;
	const int count = pCoordSystems.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		coordSystem = ( ceCoordSystem* )pCoordSystems.GetAt( i );
		
		if( coordSystem->GetID().Equals( id ) ){
			return coordSystem;
		}
	}
	
	return NULL;
}

ceCoordSystem *ceCoordSystemList::GetWithAliasID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	ceCoordSystem *coordSystem;
	const int count = pCoordSystems.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		coordSystem = ( ceCoordSystem* )pCoordSystems.GetAt( i );
		
		if( ! coordSystem->GetAliasID().IsEmpty() && coordSystem->GetAliasID().Equals( id ) ){
			return coordSystem;
		}
	}
	
	return NULL;
}

ceCoordSystem *ceCoordSystemList::GetWithIDOrAliasID( const char *id ) const{
	ceCoordSystem *coordSystem = GetWithAliasID( id );
	
	if( ! coordSystem ){
		coordSystem = GetWithID( id );
	}
	
	return coordSystem;
}

int ceCoordSystemList::IndexOf( ceCoordSystem *coordSystem ) const{
	return pCoordSystems.IndexOf( coordSystem );
}

int ceCoordSystemList::IndexWithID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pCoordSystems.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const ceCoordSystem &coordSystem = *( ( ceCoordSystem* )pCoordSystems.GetAt( i ) );
		
		if( coordSystem.GetID().Equals( id ) ){
			return i;
		}
	}
	
	return -1;
}

int ceCoordSystemList::IndexWithAliasID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pCoordSystems.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const ceCoordSystem &coordSystem = *( ( ceCoordSystem* )pCoordSystems.GetAt( i ) );
		
		if( ! coordSystem.GetAliasID().IsEmpty() && coordSystem.GetAliasID().Equals( id ) ){
			return i;
		}
	}
	
	return -1;
}

int ceCoordSystemList::IndexWithIDOrAliasID( const char *id ) const{
	int index = IndexWithAliasID( id );
	
	if( index == -1 ){
		index = IndexWithID( id );
	}
	
	return index;
}

bool ceCoordSystemList::Has( ceCoordSystem *coordSystem ) const{
	return pCoordSystems.Has( coordSystem );
}

bool ceCoordSystemList::HasWithID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pCoordSystems.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const ceCoordSystem &coordSystem = *( ( ceCoordSystem* )pCoordSystems.GetAt( i ) );
		
		if( coordSystem.GetID().Equals( id ) ){
			return true;
		}
	}
	
	return false;
}

bool ceCoordSystemList::HasWithAliasID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pCoordSystems.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const ceCoordSystem &coordSystem = *( ( ceCoordSystem* )pCoordSystems.GetAt( i ) );
		
		if( ! coordSystem.GetAliasID().IsEmpty() && coordSystem.GetAliasID().Equals( id ) ){
			return true;
		}
	}
	
	return false;
}

bool ceCoordSystemList::HasWithIDOrAliasID( const char *id ) const{
	return HasWithAliasID( id ) || HasWithID( id );
}

void ceCoordSystemList::Add( ceCoordSystem *coordSystem ){
	if( ! coordSystem ) DETHROW( deeInvalidParam );
	
	pCoordSystems.Add( coordSystem );
}

void ceCoordSystemList::InsertAt( ceCoordSystem *coordSystem, int index ){
	if( ! coordSystem ) DETHROW( deeInvalidParam );
	
	pCoordSystems.Insert( coordSystem, index );
}

void ceCoordSystemList::MoveTo( ceCoordSystem *coordSystem, int index ){
	pCoordSystems.Move( coordSystem, index );
}

void ceCoordSystemList::Remove( ceCoordSystem *coordSystem ){
	pCoordSystems.Remove( coordSystem );
}

void ceCoordSystemList::RemoveAll(){
	pCoordSystems.RemoveAll();
}



ceCoordSystemList &ceCoordSystemList::operator=( const ceCoordSystemList &list ){
	pCoordSystems = list.pCoordSystems;
	return *this;
}
