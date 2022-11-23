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

#include "ceActorController.h"
#include "ceActorControllerList.h"

#include <dragengine/common/exceptions.h>



// Class ceActorController
////////////////////////////

// Constructor, destructor
////////////////////////////

ceActorControllerList::ceActorControllerList(){
}

ceActorControllerList::ceActorControllerList( const ceActorControllerList &list ) :
pControllers( list.pControllers ){
}

ceActorControllerList::~ceActorControllerList(){
}



// Management
///////////////

int ceActorControllerList::GetCount() const{
	return pControllers.GetCount();
}

ceActorController *ceActorControllerList::GetAt( int index ) const{
	return ( ceActorController* )pControllers.GetAt( index );
}

ceActorController *ceActorControllerList::GetNamed( const char *name ) const{
	const int count = pControllers.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		ceActorController * const controller = ( ceActorController* )pControllers.GetAt( i );
		if( controller->GetName() == name ){
			return controller;
		}
	}
	return nullptr;
}

int ceActorControllerList::IndexOf( ceActorController *controller ) const{
	return pControllers.IndexOf( controller );
}

bool ceActorControllerList::Has( ceActorController *controller ) const{
	return pControllers.Has( controller );
}

bool ceActorControllerList::HasNamed( const char *name ) const{
	return GetNamed( name ) != nullptr;
}

void ceActorControllerList::Add( ceActorController *controller ){
	if( ! controller || Has( controller ) ){
		DETHROW( deeInvalidParam );
	}
	
	pControllers.Add( controller );
}

void ceActorControllerList::Remove( ceActorController *controller ){
	pControllers.Remove( controller );
}

void ceActorControllerList::RemoveAll(){
	pControllers.RemoveAll();
}



ceActorControllerList &ceActorControllerList::operator=( const ceActorControllerList &list ){
	pControllers = list.pControllers;
	return *this;
}
