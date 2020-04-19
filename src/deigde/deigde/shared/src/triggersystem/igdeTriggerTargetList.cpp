/* 
 * Drag[en]gine IGDE
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

#include "igdeTriggerTarget.h"
#include "igdeTriggerTargetList.h"

#include <dragengine/common/exceptions.h>



// Class igdeTriggerTargetList
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTriggerTargetList::igdeTriggerTargetList(){
}

igdeTriggerTargetList::~igdeTriggerTargetList(){
}



// Management
///////////////

int igdeTriggerTargetList::GetCount() const{
	return pTargets.GetCount();
}

igdeTriggerTarget *igdeTriggerTargetList::GetAt( int position ) const{
	return ( igdeTriggerTarget* )pTargets.GetAt( position );
}

igdeTriggerTarget *igdeTriggerTargetList::GetNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pTargets.GetCount();
	igdeTriggerTarget *target;
	int i;
	
	for( i=0; i<count; i++ ){
		target = ( igdeTriggerTarget* )pTargets.GetAt( i );
		if( target->GetName().Equals( name ) ){
			return target;
		}
	}
	
	return NULL;
}

igdeTriggerTarget *igdeTriggerTargetList::GetNamedAddIfMissing( const char *name ){
	igdeTriggerTarget *target = GetNamed( name );
	
	if( ! target ){
		target = new igdeTriggerTarget( name );
		pTargets.Add( target );
		target->FreeReference();
	}
	
	return target;
}

int igdeTriggerTargetList::IndexOf( igdeTriggerTarget *target ) const{
	return pTargets.IndexOf( target );
}

bool igdeTriggerTargetList::Has( igdeTriggerTarget *target ) const{
	return pTargets.Has( target );
}

bool igdeTriggerTargetList::HasNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pTargets.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( igdeTriggerTarget* )pTargets.GetAt( i ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void igdeTriggerTargetList::Add( igdeTriggerTarget *target ){
	if( ! target || HasNamed( target->GetName().GetString() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pTargets.Add( target );
}

void igdeTriggerTargetList::Remove( igdeTriggerTarget *target ){
	if( ! target ){
		DETHROW( deeInvalidParam );
	}
	
	pTargets.Remove( target );
}

void igdeTriggerTargetList::RemoveAll(){
	pTargets.RemoveAll();
}

void igdeTriggerTargetList::RemoveUnused(){
	const int count = pTargets.GetCount();
	igdeTriggerTarget *target;
	int i;
	
	for( i=count-1; i>=0; i-- ){
		target = ( igdeTriggerTarget* )pTargets.GetAt( i );
		
		if( target->GetRefCount() == 1 ){
			pTargets.Remove( target );
		}
	}
}
