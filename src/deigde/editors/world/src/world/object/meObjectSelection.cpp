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

#include "meObject.h"
#include "meObjectList.h"
#include "meObjectSelection.h"
#include "../meWorld.h"

#include <dragengine/common/exceptions.h>



// Class meObjectSelection
//////////////////////////////

// Constructor, destructor
////////////////////////////

meObjectSelection::meObjectSelection(){
	pActive = NULL;
}

meObjectSelection::~meObjectSelection(){
	Reset();
}



// Management
///////////////

void meObjectSelection::Add( meObject *object ){
	if( ! object ){
		DETHROW( deeInvalidParam );
	}
	
	object->SetSelected( true );
	pSelection.AddIfAbsent( object );
}

void meObjectSelection::Remove( meObject *object ){
	if( ! object ){
		DETHROW( deeInvalidParam );
	}
	
	object->SetSelected( false );
	pSelection.RemoveIfPresent( object );
}

void meObjectSelection::RemoveAll(){
	const int count = pSelection.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pSelection.GetAt( i )->SetSelected( false );
	}
	
	pSelection.RemoveAll();
}



bool meObjectSelection::HasActive() const{
	return pActive != NULL;
}

void meObjectSelection::SetActive( meObject *object ){
	if( pActive ){
		pActive->SetActive( false );
		pActive->FreeReference();
	}
	
	pActive = object;
	
	if( object ){
		object->AddReference();
		object->SetActive( true );
	}
}

void meObjectSelection::ActivateNext(){
	const int count = pSelection.GetCount();
	meObject *next = NULL;
	int i;
	
	for( i=0; i<count; i++ ){
		if( pActive != pSelection.GetAt( i ) ){
			next = pSelection.GetAt( i );
			break;
		}
	}
	
	SetActive( next );
}

void meObjectSelection::Reset(){
	RemoveAll();
	SetActive( NULL );
}
