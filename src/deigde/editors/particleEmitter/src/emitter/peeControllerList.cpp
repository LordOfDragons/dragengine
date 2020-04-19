/* 
 * Drag[en]gine IGDE Particle Emitter Editor
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

#include "peeController.h"
#include "peeControllerList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class peeController
////////////////////////

// Constructor, destructor
////////////////////////////

peeControllerList::peeControllerList(){
}

peeControllerList::~peeControllerList(){
}



// Management
///////////////

int peeControllerList::GetCount() const{
	return pControllers.GetCount();
}

peeController *peeControllerList::GetAt( int index ) const{
	return ( peeController* )pControllers.GetAt( index );
}

peeController *peeControllerList::GetNamed( const char *name ) const{
	const int count = pControllers.GetCount();
	peeController *controller;
	int p;
	
	for( p=0; p<count; p++ ){
		controller = ( peeController* )pControllers.GetAt( p );
		
		if( controller->GetName().Equals( name ) ){
			return controller;
		}
	}
	
	return NULL;
}

int peeControllerList::IndexOf( peeController *controller ) const{
	return pControllers.IndexOf( controller );
}

int peeControllerList::IndexOfNamed( const char *name ) const{
	const int count = pControllers.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		if( ( ( peeController* )pControllers.GetAt( p ) )->GetName().Equals( name ) ){
			return p;
		}
	}
	
	return -1;
}

bool peeControllerList::Has( peeController *controller ) const{
	return pControllers.Has( controller );
}

bool peeControllerList::HasNamed( const char *name ) const{
	const int count = pControllers.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		if( ( ( peeController* )pControllers.GetAt( p ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void peeControllerList::Add( peeController *controller ){
	if( ! controller || HasNamed( controller->GetName().GetString() ) ) DETHROW( deeInvalidParam );
	
	pControllers.Add( controller );
}

void peeControllerList::InsertAt( peeController *controller, int index ){
	if( ! controller || HasNamed( controller->GetName().GetString() ) ) DETHROW( deeInvalidParam );
	
	pControllers.Insert( controller, index );
}

void peeControllerList::MoveTo( peeController *controller, int index ){
	pControllers.Move( controller, index );
}

void peeControllerList::Remove( peeController *controller ){
	pControllers.Remove( controller );
}

void peeControllerList::RemoveAll(){
	pControllers.RemoveAll();
}



peeControllerList &peeControllerList::operator=( const peeControllerList &list ){
	pControllers = list.pControllers;
	return *this;
}
