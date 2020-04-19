/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#include "seControllerTarget.h"
#include "seControllerTargetList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class seControllerTarget
/////////////////////////////

// Constructor, destructor
////////////////////////////

seControllerTargetList::seControllerTargetList(){
}

seControllerTargetList::seControllerTargetList( const seControllerTargetList &copy ) :
pTargets( copy.pTargets ){
}

seControllerTargetList::~seControllerTargetList(){
}



// Management
///////////////

int seControllerTargetList::GetCount() const{
	return pTargets.GetCount();
}

seControllerTarget *seControllerTargetList::GetAt( int index ) const{
	return ( seControllerTarget* )pTargets.GetAt( index );
}

int seControllerTargetList::IndexOf( seControllerTarget *target ) const{
	return pTargets.IndexOf( target );
}

bool seControllerTargetList::Has( seControllerTarget *target ) const{
	return pTargets.Has( target );
}

void seControllerTargetList::Add( seControllerTarget *target ){
	if( ! target || Has( target ) ){
		DETHROW( deeInvalidParam );
	}
	pTargets.Add( target );
}

void seControllerTargetList::Insert( seControllerTarget *target, int index ){
	if( ! target || Has( target ) ){
		DETHROW( deeInvalidParam );
	}
	pTargets.Insert( target, index );
}

void seControllerTargetList::Move( seControllerTarget *target, int index ){
	pTargets.Move( target, index );
}

void seControllerTargetList::Remove( seControllerTarget *target ){
	pTargets.RemoveFrom( pTargets.IndexOf( target ) );
}

void seControllerTargetList::RemoveAll(){
	pTargets.RemoveAll();
}



// Operators
//////////////

seControllerTargetList &seControllerTargetList::operator=( const seControllerTargetList &list ){
	pTargets = list.pTargets;
	return *this;
}
