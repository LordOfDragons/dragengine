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

#include "seBody.h"
#include "seBodyList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class seBody
////////////////////

// Constructor, destructor
////////////////////////////

seBodyList::seBodyList(){
}

seBodyList::~seBodyList(){
}



// Management
///////////////

int seBodyList::GetCount() const{
	return pBodies.GetCount();
}

seBody *seBodyList::GetAt( int index ) const{
	return ( seBody* )pBodies.GetAt( index );
}

int seBodyList::IndexOf( seBody *body ) const{
	return pBodies.IndexOf( body );
}

bool seBodyList::Has( seBody *body ) const{
	return pBodies.Has( body );
}

void seBodyList::Add( seBody *body ){
	if( ! body ){
		DETHROW( deeInvalidParam );
	}
	pBodies.Add( body );
}

void seBodyList::InsertAt( seBody *body, int index ){
	if( ! body ){
		DETHROW( deeInvalidParam );
	}
	pBodies.Insert( body, index );
}

void seBodyList::MoveTo( seBody *body, int index ){
	pBodies.Move( body, index );
}

void seBodyList::Remove( seBody *body ){
	pBodies.Remove( body );
}

void seBodyList::RemoveAll(){
	pBodies.RemoveAll();
}



seBodyList &seBodyList::operator=( const seBodyList &list ){
	pBodies = list.pBodies;
	return *this;
}
