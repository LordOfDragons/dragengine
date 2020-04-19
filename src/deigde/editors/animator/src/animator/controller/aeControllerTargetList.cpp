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

#include "aeControllerTarget.h"
#include "aeControllerTargetList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class aeControllerTarget
/////////////////////////////

// Constructor, destructor
////////////////////////////

aeControllerTargetList::aeControllerTargetList(){
}

aeControllerTargetList::aeControllerTargetList( const aeControllerTargetList &copy ) :
pTargets( copy.pTargets ){
}

aeControllerTargetList::~aeControllerTargetList(){
}



// Management
///////////////

int aeControllerTargetList::GetCount() const{
	return pTargets.GetCount();
}

aeControllerTarget *aeControllerTargetList::GetAt( int index ) const{
	return ( aeControllerTarget* )pTargets.GetAt( index );
}

int aeControllerTargetList::IndexOf( aeControllerTarget *target ) const{
	return pTargets.IndexOf( target );
}

bool aeControllerTargetList::Has( aeControllerTarget *target ) const{
	return pTargets.Has( target );
}

void aeControllerTargetList::Add( aeControllerTarget *target ){
	if( ! target || Has( target ) ){
		DETHROW( deeInvalidParam );
	}
	pTargets.Add( target );
}

void aeControllerTargetList::Insert( aeControllerTarget *target, int index ){
	if( ! target || Has( target ) ){
		DETHROW( deeInvalidParam );
	}
	pTargets.Insert( target, index );
}

void aeControllerTargetList::Move( aeControllerTarget *target, int index ){
	pTargets.Move( target, index );
}

void aeControllerTargetList::Remove( aeControllerTarget *target ){
	pTargets.RemoveFrom( pTargets.IndexOf( target ) );
}

void aeControllerTargetList::RemoveAll(){
	pTargets.RemoveAll();
}



// Operators
//////////////

aeControllerTargetList &aeControllerTargetList::operator=( const aeControllerTargetList &list ){
	pTargets = list.pTargets;
	return *this;
}
