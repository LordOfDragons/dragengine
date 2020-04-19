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

#include "meUObjectShapeMove.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/shape/decShapeList.h>
#include <deigde/codec/igdeCodecPropertyString.h>



// Class meUObjectShapeMove
/////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectShapeMove::meUObjectShapeMove( meObject *object, const char *property, const meObjectShapeList &list ){
	if( list.GetCount() == 0 ){
		DETHROW( deeInvalidParam );
	}
	if( ! object || ! property ){
		DETHROW( deeInvalidParam );
	}
	if( ! object->GetWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	const meObjectShapeList &shapeList = object->GetWorld()->GetObjectShapes();
	const int count = list.GetCount();
	int i;
	
	pObject = NULL;
	
	SetShortInfo( "Move object shapes" );
	SetLongInfo( "Move object shapes" );
	
	pPropertyExists = object->GetProperties().Has( property );
	if( pPropertyExists ){
		pOldValue = object->GetProperties().GetAt( property );
	}
	
	for( i=0; i<count; i++ ){
		pList.Add( shapeList.IndexOf( list.GetAt( i ) ) );
	}
	
	pProperty = property;
	pObject = object;
	object->AddReference();
}

meUObjectShapeMove::~meUObjectShapeMove(){
	if( pObject ){
		pObject->FreeReference();
	}
}



// Management
///////////////

void meUObjectShapeMove::Undo(){
	meWorld &world = *pObject->GetWorld();
	
	if( pPropertyExists ){
		pObject->SetProperty( pProperty.GetString(), pOldValue.GetString() );
		
	}else{
		pObject->RemoveProperty( pProperty.GetString() );
	}
	
	world.NotifyObjectShapeSelectionChanged();
}

void meUObjectShapeMove::Redo(){
	meWorld &world = *pObject->GetWorld();
	const int count = pList.GetCount();
	decDMatrix matrixParentInverse;
	igdeCodecPropertyString codec;
	decShapeList shapeList;
	decVector distance;
	decString newValue;
	int i;
	
	matrixParentInverse = decDMatrix::CreateRotation( decDVector( pObject->GetRotation() * DEG2RAD ) ).Invert();
	distance = ( matrixParentInverse * GetDistance() ).ToVector();
	
	codec.DecodeShapeList( pOldValue.GetString(), shapeList );
	
	for( i=0; i<count; i++ ){
		decShape &shape = *shapeList.GetAt( pList.GetAt( i ) );
		
		shape.SetPosition( shape.GetPosition() + distance );
	}
	
	codec.EncodeShapeList( shapeList, newValue );
	
	pObject->SetProperty( pProperty.GetString(), newValue.GetString() );
	
	world.NotifyObjectShapeSelectionChanged();
}

void meUObjectShapeMove::ProgressiveRedo(){
	Redo();
}
