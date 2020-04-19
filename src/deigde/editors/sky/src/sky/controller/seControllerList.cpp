/* 
 * Drag[en]gine IGDE Sky Editor
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

#include "seController.h"
#include "seControllerList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class seController
///////////////////////

// Constructor, destructor
////////////////////////////

seControllerList::seControllerList(){
}

seControllerList::~seControllerList(){
}



// Management
///////////////

int seControllerList::GetCount() const{
	return pControllers.GetCount();
}

seController *seControllerList::GetAt( int index ) const{
	return ( seController* )pControllers.GetAt( index );
}

int seControllerList::IndexOf( seController *controller ) const{
	return pControllers.IndexOf( controller );
}

bool seControllerList::Has( seController *controller ) const{
	return pControllers.Has( controller );
}

void seControllerList::Add( seController *controller ){
	if( ! controller ){
		DETHROW( deeInvalidParam );
	}
	pControllers.Add( controller );
}

void seControllerList::InsertAt( seController *controller, int index ){
	if( ! controller ){
		DETHROW( deeInvalidParam );
	}
	pControllers.Insert( controller, index );
}

void seControllerList::MoveTo( seController *controller, int index ){
	pControllers.Move( controller, index );
}

void seControllerList::Remove( seController *controller ){
	pControllers.Remove( controller );
}

void seControllerList::RemoveAll(){
	pControllers.RemoveAll();
}



seControllerList &seControllerList::operator=( const seControllerList &list ){
	pControllers = list.pControllers;
	return *this;
}
