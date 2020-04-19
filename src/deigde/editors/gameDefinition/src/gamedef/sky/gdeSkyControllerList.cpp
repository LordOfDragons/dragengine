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

#include "gdeSkyController.h"
#include "gdeSkyControllerList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeSkyController
/////////////////

// Constructor, destructor
////////////////////////////

gdeSkyControllerList::gdeSkyControllerList(){
}

gdeSkyControllerList::gdeSkyControllerList( const gdeSkyControllerList &list ) :
pControllers( list.pControllers ){
}

gdeSkyControllerList::~gdeSkyControllerList(){
}



// Management
///////////////

int gdeSkyControllerList::GetCount() const{
	return pControllers.GetCount();
}

gdeSkyController *gdeSkyControllerList::GetAt( int index ) const{
	return ( gdeSkyController* )pControllers.GetAt( index );
}

gdeSkyController *gdeSkyControllerList::GetNamed( const char *name ) const{
	const int count = pControllers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		gdeSkyController * const controller = ( gdeSkyController* )pControllers.GetAt( i );
		if( controller->GetName() == name ){
			return controller;
		}
	}
	
	return NULL;
}

int gdeSkyControllerList::IndexOf( gdeSkyController *controller ) const{
	return pControllers.IndexOf( controller );
}

bool gdeSkyControllerList::Has( gdeSkyController *controller ) const{
	return pControllers.Has( controller );
}

bool gdeSkyControllerList::HasNamed( const char *name ) const{
	return GetNamed( name ) != NULL;
}

void gdeSkyControllerList::Add( gdeSkyController *controller ){
	if( ! controller || HasNamed( controller->GetName() ) ){
		DETHROW( deeInvalidParam );
	}
	pControllers.Add( controller );
}

void gdeSkyControllerList::Remove( gdeSkyController *controller ){
	pControllers.Remove( controller );
}

void gdeSkyControllerList::RemoveAll(){
	pControllers.RemoveAll();
}



gdeSkyControllerList &gdeSkyControllerList::operator=( const gdeSkyControllerList &list ){
	pControllers = list.pControllers;
	return *this;
}
