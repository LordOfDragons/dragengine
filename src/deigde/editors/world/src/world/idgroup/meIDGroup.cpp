/* 
 * Drag[en]gine IGDE World Editor
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

#include <stdlib.h>

#include "meIDGroup.h"
#include "meIDGroupID.h"

#include <dragengine/common/exceptions.h>



// Class meIDGroup
////////////////////

// Constructor, destructor
////////////////////////////

meIDGroup::meIDGroup( const char *name ) :
pName( name ){
}

meIDGroup::~meIDGroup(){
	RemoveAll();
}



// Management
///////////////

int meIDGroup::GetCount() const{
	return pIDs.GetCount();
}

decStringList meIDGroup::GetIDList() const{
	return pIDs.GetKeys();
}

int meIDGroup::GetUsageCountFor( const char *id ) const{
	deObject *object;
	
	if( pIDs.GetAt( id, &object ) ){
		return ( ( meIDGroupID* )object )->GetUsageCount();
		
	}else{
		return 0;
	}
}

bool meIDGroup::Has( const char *id ) const{
	return pIDs.Has( id );
}

void meIDGroup::Add( const char *id ){
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	deObject *object;
	
	if( pIDs.GetAt( id, &object ) ){
		( ( meIDGroupID* )object )->Increment();
		return;
	}
	
	meIDGroupID *groupID = NULL;
	
	try{
		groupID = new meIDGroupID( id );
		pIDs.SetAt( id, groupID );
		groupID->FreeReference();
		
	}catch( const deException & ){
		if( groupID ){
			groupID->FreeReference();
		}
		throw;
	}
}

void meIDGroup::Remove( const char *id ){
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	deObject *object;
	
	if( ! pIDs.GetAt( id, &object ) ){
		DETHROW( deeInvalidParam );
	}
	
	meIDGroupID &groupID = *( ( meIDGroupID* )object );
	groupID.Decrement();
	if( groupID.GetUsageCount() == 0 ){
		pIDs.Remove( id );
	}
}

void meIDGroup::RemoveAll(){
	pIDs.RemoveAll();
}
