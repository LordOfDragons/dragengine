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

#include "igdeGDCLight.h"
#include "igdeGDCLightList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCLight
///////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCLightList::igdeGDCLightList(){
}

igdeGDCLightList::igdeGDCLightList( const igdeGDCLightList &list ){
	pLights = list.pLights;
}

igdeGDCLightList::~igdeGDCLightList(){
}



// Management
///////////////

int igdeGDCLightList::GetCount() const{
	return pLights.GetCount();
}

igdeGDCLight *igdeGDCLightList::GetAt( int index ) const{
	return ( igdeGDCLight* )pLights.GetAt( index );
}

int igdeGDCLightList::IndexOf( igdeGDCLight *light ) const{
	return pLights.IndexOf( light );
}

bool igdeGDCLightList::Has( igdeGDCLight *light ) const{
	return pLights.Has( light );
}

void igdeGDCLightList::Add( igdeGDCLight *light ){
	if( ! light ){
		DETHROW( deeInvalidParam );
	}
	
	pLights.Add( light );
}

void igdeGDCLightList::InsertAt( igdeGDCLight *light, int index ){
	if( ! light ){
		DETHROW( deeInvalidParam );
	}
	
	pLights.Insert( light, index );
}

void igdeGDCLightList::MoveTo( igdeGDCLight *light, int index ){
	pLights.Move( light, index );
}

void igdeGDCLightList::Remove( igdeGDCLight *light ){
	pLights.Remove( light );
}

void igdeGDCLightList::RemoveAll(){
	pLights.RemoveAll();
}



void igdeGDCLightList::SetToDeepCopyFrom( const igdeGDCLightList &list ){
	const int count = list.GetCount();
	igdeGDCLight *light = NULL;
	
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			light = new igdeGDCLight( *list.GetAt( i ) );
			Add( light );
			light->FreeReference();
			light = NULL;
		}
		
	}catch( const deException & ){
		if( light ){
			light->FreeReference();
		}
		throw;
	}
}



// Operators
//////////////

igdeGDCLightList &igdeGDCLightList::operator=( const igdeGDCLightList &list ){
	pLights = list.pLights;
	return *this;
}
