/* 
 * Drag[en]gine IGDE Animator Editor
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

#include "aeController.h"
#include "aeControllerList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class aeController
/////////////////

// Constructor, destructor
////////////////////////////

aeControllerList::aeControllerList(){
}

aeControllerList::aeControllerList( const aeControllerList &copy ) :
pControllers( copy.pControllers ){
}

aeControllerList::~aeControllerList(){
}



// Management
///////////////

int aeControllerList::GetCount() const{
	return pControllers.GetCount();
}

aeController *aeControllerList::GetAt( int index ) const{
	return ( aeController* )pControllers.GetAt( index );
}

aeController *aeControllerList::GetNamed( const char *name ) const{
	const int count = pControllers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		aeController * const controller = ( aeController* )pControllers.GetAt( i );
		if( controller->GetName() == name ){
			return controller;
		}
	}
	
	return NULL;
}

int aeControllerList::IndexOf( aeController *controller ) const{
	return pControllers.IndexOf( controller );
}

int aeControllerList::IndexOfNamed( const char *name ) const{
	const int count = pControllers.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		if( ( ( aeController* )pControllers.GetAt( i ) )->GetName() == name ){
			return i;
		}
	}
	return -1;
}

bool aeControllerList::Has( aeController *controller ) const{
	return pControllers.Has( controller );
}

bool aeControllerList::HasNamed( const char *name ) const{
	return GetNamed( name ) != NULL;
}

void aeControllerList::Add( aeController *controller ){
	if( ! controller || HasNamed( controller->GetName() ) ){
		DETHROW( deeInvalidParam );
	}
	pControllers.Add( controller );
}

void aeControllerList::Insert( aeController *controller, int index ){
	if( ! controller || HasNamed( controller->GetName() ) ){
		DETHROW( deeInvalidParam );
	}
	pControllers.Insert( controller, index );
}

void aeControllerList::Move( aeController *controller, int index ){
	pControllers.Move( controller, index );
}

void aeControllerList::Remove( aeController *controller ){
	pControllers.Remove( controller );
}

void aeControllerList::RemoveAll(){
	pControllers.RemoveAll();
}



// Operators
//////////////

aeControllerList &aeControllerList::operator=( const aeControllerList &list ){
	pControllers = list.pControllers;
	return *this;
}

aeControllerList &aeControllerList::operator+=( const aeControllerList &list ){
	pControllers += list.pControllers;
	return *this;
}
