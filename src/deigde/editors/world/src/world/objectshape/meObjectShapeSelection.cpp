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

#include "meObjectShape.h"
#include "meObjectShapeList.h"
#include "meObjectShapeSelection.h"
#include "../meWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decIntList.h>



// Class meObjectShapeSelection
/////////////////////////////////

// Constructor, destructor
////////////////////////////

meObjectShapeSelection::meObjectShapeSelection(){
	pActive = NULL;
}

meObjectShapeSelection::~meObjectShapeSelection(){
	Reset();
}



// Management
///////////////

void meObjectShapeSelection::Add( meObjectShape *objectShape ){
	if( ! objectShape ){
		DETHROW( deeInvalidParam );
	}
	
	objectShape->SetSelected( true );
	pSelection.AddIfAbsent( objectShape );
}

void meObjectShapeSelection::Remove( meObjectShape *objectShape ){
	if( ! objectShape ){
		DETHROW( deeInvalidParam );
	}
	
	objectShape->SetSelected( false );
	pSelection.RemoveIfPresent( objectShape );
}

void meObjectShapeSelection::RemoveAll(){
	const int count = pSelection.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pSelection.GetAt( i )->SetSelected( false );
	}
	
	pSelection.RemoveAll();
}



bool meObjectShapeSelection::HasActive() const{
	return pActive != NULL;
}

void meObjectShapeSelection::SetActive( meObjectShape *objectShape ){
	if( pActive ){
		pActive->SetActive( false );
		pActive->FreeReference();
	}
	
	pActive = objectShape;
	
	if( objectShape ){
		objectShape->AddReference();
		objectShape->SetActive( true );
	}
}

void meObjectShapeSelection::ActivateNext(){
	const int count = pSelection.GetCount();
	meObjectShape *next = NULL;
	int i;
	
	for( i=0; i<count; i++ ){
		if( pActive != pSelection.GetAt( i ) ){
			next = pSelection.GetAt( i );
			break;
		}
	}
	
	SetActive( next );
}

void meObjectShapeSelection::Reset(){
	RemoveAll();
	SetActive( NULL );
}



void meObjectShapeSelection::Backup( const meObjectShapeList &shapeList, decIntList &indicesSelected, int &indexActive ){
	const int count = pSelection.GetCount();
	int i, index;
	
	indicesSelected.RemoveAll();
	
	for( i=0; i<count; i++ ){
		index = shapeList.IndexOf( pSelection.GetAt( i ) );
		
		if( index != -1 ){
			indicesSelected.Add( index );
		}
	}
	
	if( pActive ){
		indexActive = shapeList.IndexOf( pActive );
		
	}else{
		indexActive = -1;
	}
}

void meObjectShapeSelection::Restore( const meObjectShapeList &shapeList, const decIntList &indicesSelected, int indexActive ){
	const int count = indicesSelected.GetCount();
	const int shapeCount = shapeList.GetCount();
	int i, index;
	
	Reset();
	
	for( i=0; i<count; i++ ){
		index = indicesSelected.GetAt( i );
		
		if( index >= 0 && index < shapeCount ){
			Add( shapeList.GetAt( index ) );
		}
	}
	
	if( indexActive >= 0 && indexActive < shapeCount ){
		SetActive( shapeList.GetAt( indexActive ) );
	}
	
	if( ! pActive && pSelection.GetCount() > 0 ){
		ActivateNext();
	}
}
