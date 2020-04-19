/* 
 * Drag[en]gine Bullet Physics Module
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
#include <string.h>
#include <stdlib.h>

#include "debpDebugInformation.h"
#include "debpDebugInformationList.h"

#include <dragengine/common/exceptions.h>



// Class debpDebugInformationList
///////////////////////////////////

// Constructor, destructor
////////////////////////////

debpDebugInformationList::debpDebugInformationList(){
}

debpDebugInformationList::~debpDebugInformationList(){
}



// Management
///////////////

int debpDebugInformationList::GetCount() const{
	return pChildren.GetCount();
}

debpDebugInformation *debpDebugInformationList::GetAt( int index ) const{
	return ( debpDebugInformation* )pChildren.GetAt( index );
}

void debpDebugInformationList::Add( debpDebugInformation *child ){
	pChildren.Add( child );
}

void debpDebugInformationList::Remove( debpDebugInformation *child ){
	pChildren.Remove( child );
}

void debpDebugInformationList::RemoveIfPresent( debpDebugInformation *child ){
	pChildren.RemoveIfPresent( child );
}

void debpDebugInformationList::RemoveAll(){
	pChildren.RemoveAll();
}
