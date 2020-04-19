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

#include "igdeGDCComponent.h"
#include "igdeGDCComponentList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCComponent
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCComponentList::igdeGDCComponentList(){
}

igdeGDCComponentList::igdeGDCComponentList( const igdeGDCComponentList &list ){
	pComponents = list.pComponents;
}

igdeGDCComponentList::~igdeGDCComponentList(){
}



// Management
///////////////

int igdeGDCComponentList::GetCount() const{
	return pComponents.GetCount();
}

igdeGDCComponent *igdeGDCComponentList::GetAt( int index ) const{
	return ( igdeGDCComponent* )pComponents.GetAt( index );
}

int igdeGDCComponentList::IndexOf( igdeGDCComponent *component ) const{
	return pComponents.IndexOf( component );
}

bool igdeGDCComponentList::Has( igdeGDCComponent *component ) const{
	return pComponents.Has( component );
}

void igdeGDCComponentList::Add( igdeGDCComponent *component ){
	if( ! component ){
		DETHROW( deeInvalidParam );
	}
	
	pComponents.Add( component );
}

void igdeGDCComponentList::InsertAt( igdeGDCComponent *component, int index ){
	if( ! component ){
		DETHROW( deeInvalidParam );
	}
	
	pComponents.Insert( component, index );
}

void igdeGDCComponentList::MoveTo( igdeGDCComponent *component, int index ){
	pComponents.Move( component, index );
}

void igdeGDCComponentList::Remove( igdeGDCComponent *component ){
	pComponents.Remove( component );
}

void igdeGDCComponentList::RemoveAll(){
	pComponents.RemoveAll();
}



void igdeGDCComponentList::SetToDeepCopyFrom( const igdeGDCComponentList &list ){
	const int count = list.GetCount();
	igdeGDCComponent *component = NULL;
	
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			component = new igdeGDCComponent( *list.GetAt( i ) );
			Add( component );
			component->FreeReference();
			component = NULL;
		}
		
	}catch( const deException & ){
		if( component ){
			component->FreeReference();
		}
		throw;
	}
}



igdeGDCComponentList &igdeGDCComponentList::operator=( const igdeGDCComponentList &list ){
	pComponents = list.pComponents;
	return *this;
}
