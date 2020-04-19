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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "meNavigationSpace.h"
#include "meNavigationSpaceSelection.h"
#include "../meWorld.h"

#include <dragengine/common/exceptions.h>



// Class meNavigationSpaceSelection
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

meNavigationSpaceSelection::meNavigationSpaceSelection(){
	pActive = NULL;
}

meNavigationSpaceSelection::~meNavigationSpaceSelection(){
	Reset();
}



// Management
///////////////

void meNavigationSpaceSelection::Add( meNavigationSpace *navspaces ){
	if( ! navspaces ){
		DETHROW( deeInvalidParam );
	}
	
	navspaces->SetSelected( true );
	pSelection.AddIfAbsent( navspaces );
}

void meNavigationSpaceSelection::Remove( meNavigationSpace *navspaces ){
	if( ! navspaces ){
		DETHROW( deeInvalidParam );
	}
	
	navspaces->SetSelected( false );
	pSelection.RemoveIfPresent( navspaces );
}

void meNavigationSpaceSelection::RemoveAll(){
	const int count = pSelection.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pSelection.GetAt( i )->SetSelected( false );
	}
	
	pSelection.RemoveAll();
}



bool meNavigationSpaceSelection::HasActive() const{
	return pActive != NULL;
}

void meNavigationSpaceSelection::SetActive( meNavigationSpace *navspaces ){
	if( pActive ){
		pActive->SetActive( false );
		pActive->FreeReference();
	}
	
	pActive = navspaces;
	
	if( navspaces ){
		navspaces->AddReference();
		navspaces->SetActive( true );
	}
}

void meNavigationSpaceSelection::ActivateNext(){
	const int count = pSelection.GetCount();
	meNavigationSpace *next = NULL;
	int i;
	
	for( i=0; i<count; i++ ){
		if( pActive != pSelection.GetAt( i ) ){
			next = pSelection.GetAt( i );
			break;
		}
	}
	
	SetActive( next );
}

void meNavigationSpaceSelection::Reset(){
	RemoveAll();
	SetActive( NULL );
}
