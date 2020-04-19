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

#include "meUObjectShapeDelete.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectShapesDelete
////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectShapesDelete::meUObjectShapesDelete( meObject *object, const char *property, const meObjectShapeList &list ){
	if( list.GetCount() == 0 ){
		DETHROW( deeInvalidParam );
	}
	if( ! object || ! property ){
		DETHROW( deeInvalidParam );
	}
	if( ! object->GetWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	const meObjectShapeList &shapeList1 = object->GetWorld()->GetObjectShapes();
	const int count = shapeList1.GetCount();
	meObjectShapeList shapeList2;
	int i;
	
	pObject = NULL;
	
	SetShortInfo( "Object-Shape delete" );
	SetLongInfo( "Object-Shape delete" );
	
	pPropertyExists = object->GetProperties().Has( property );
	if( pPropertyExists ){
		pOldValue = object->GetProperties().GetAt( property );
	}
	
	for( i=0; i<count; i++ ){
		if( ! list.Has( shapeList1.GetAt( i ) ) ){
			shapeList2.Add( shapeList1.GetAt( i ) );
		}
	}
	
	shapeList2.CreatePropertyString( pNewValue );
	shapeList2.RemoveAll();
	
	pProperty = property;
	pObject = object;
	object->AddReference();
}

meUObjectShapesDelete::~meUObjectShapesDelete(){
	if( pObject ){
		pObject->FreeReference();
	}
}



// Management
///////////////

void meUObjectShapesDelete::Undo(){
	meWorld &world = *pObject->GetWorld();
	
	if( pPropertyExists ){
		pObject->SetProperty( pProperty.GetString(), pOldValue.GetString() );
		
	}else{
		pObject->RemoveProperty( pProperty.GetString() );
	}
	
	world.GetSelectionObjectShape().Reset();
	world.NotifyObjectShapeSelectionChanged();
}

void meUObjectShapesDelete::Redo(){
	meWorld &world = *pObject->GetWorld();
	
	pObject->SetProperty( pProperty.GetString(), pNewValue.GetString() );
	
	world.GetSelectionObjectShape().Reset();
	world.NotifyObjectShapeSelectionChanged();
}
