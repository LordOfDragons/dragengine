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

#include "meUObjectShapeRotate.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/shape/decShapeList.h>
#include <deigde/codec/igdeCodecPropertyString.h>



// Class meUObjectShapeRotate
//////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectShapeRotate::meUObjectShapeRotate( meObject *object, const char *property,
const meObjectShapeList &list ){
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
	
	SetShortInfo( "Rotate object shapes" );
	SetLongInfo( "Rotate object shapes" );
	
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

meUObjectShapeRotate::~meUObjectShapeRotate(){
	if( pObject ){
		pObject->FreeReference();
	}
}



// Undo and Redo operations
/////////////////////////////

void meUObjectShapeRotate::Undo(){
	meWorld &world = *pObject->GetWorld();
	
	if( pPropertyExists ){
		pObject->SetProperty( pProperty.GetString(), pOldValue.GetString() );
		
	}else{
		pObject->RemoveProperty( pProperty.GetString() );
	}
	
	world.NotifyObjectShapeSelectionChanged();
}

void meUObjectShapeRotate::Redo(){
	const bool modifyPosition = GetModifyPosition();
	const bool modifyOrientation = GetModifyOrientation();
	meWorld &world = *pObject->GetWorld();
	decDMatrix matrixParent, matrixParentInverse, matrixShape;
	decDVector shapePosition, shapeRotation;
	const int count = pList.GetCount();
	igdeCodecPropertyString codec;
	decDVector position, rotation;
	decShapeList shapeList;
	decString newValue;
	int i;
	
	matrixParent = decDMatrix::CreateRT( decDVector( pObject->GetRotation() * DEG2RAD ), pObject->GetPosition() );
	matrixParentInverse = matrixParent.Invert();
	
	codec.DecodeShapeList( pOldValue.GetString(), shapeList );
	
	for( i=0; i<count; i++ ){
		decShape &shape = *shapeList.GetAt( pList.GetAt( i ) );
		
		matrixShape = decDMatrix::CreateWorld( decDVector( shape.GetPosition() ), shape.GetOrientation() ) * matrixParent;
		shapePosition = matrixShape.GetPosition();
		shapeRotation = matrixShape.GetEulerAngles();
		
		position = shapePosition;
		rotation = shapeRotation / DEG2RAD;
		TransformElement( position, rotation );
		
		if( modifyOrientation ){
			shapeRotation = rotation * DEG2RAD;
		}
		if( modifyPosition ){
			shapePosition = position;
		}
		
		matrixShape = decDMatrix::CreateRT( shapeRotation, shapePosition ) * matrixParentInverse;
		shape.SetPosition( matrixShape.GetPosition().ToVector() );
		shape.SetOrientation( matrixShape.ToQuaternion() );
	}
	
	codec.EncodeShapeList( shapeList, newValue );
	
	pObject->SetProperty( pProperty.GetString(), newValue.GetString() );
	
	world.NotifyObjectShapeSelectionChanged();
}

void meUObjectShapeRotate::ProgressiveRedo(){
	Redo(); // redo is enough in this situation
}
