/* 
 * Drag[en]gine IGDE Rig Editor
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

#include "reURotateShape.h"
#include "reUndoDataShape.h"
#include "../../../rig/bone/reRigBone.h"
#include "../../../rig/shape/reRigShape.h"
#include "../../../rig/shape/reRigShapeList.h"

#include <dragengine/common/exceptions.h>



// Class reURotateShape
/////////////////////////

// Constructor, destructor
////////////////////////////

reURotateShape::reURotateShape( reRigShapeList &list ){
	int shapeCount = list.GetShapeCount();
	
	if( shapeCount == 0 ) DETHROW( deeInvalidParam );
	
	pShapes = NULL;
	pShapeCount = 0;
	
	try{
		if( shapeCount > 0 ){
			pShapes = new reUndoDataShape*[ shapeCount ];
			if( ! pShapes ) DETHROW( deeOutOfMemory );
			
			while( pShapeCount < shapeCount ){
				pShapes[ pShapeCount ] = new reUndoDataShape( list.GetShapeAt( pShapeCount ) );
				if( ! pShapes[ pShapeCount ] ) DETHROW( deeOutOfMemory );
				pShapeCount++;
			}
		}
		
		if( shapeCount > 1 ){
			SetShortInfo( "Rotate Shapes" );
			
		}else{
			SetShortInfo( "Rotate Shape" );
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

reURotateShape::~reURotateShape(){
	pCleanUp();
}



// Undo and Redo operations
/////////////////////////////

void reURotateShape::Undo(){
	reRigShape *shape;
	int s;
	
	for( s=0; s<pShapeCount; s++ ){
		shape = pShapes[ s ]->GetShape();
		
		shape->SetPosition( pShapes[ s ]->GetOldPosition() );
		shape->SetOrientation( pShapes[ s ]->GetOldOrientation() );
	}
}

void reURotateShape::Redo(){
	bool modifyOrientation = GetModifyOrientation();
	decMatrix rotationMatrix = GetRotationMatrix();
	bool modifyPosition = GetModifyPosition();
	decVector position, view, up, rotation;
	reRigShape *shape;
	decMatrix matrix;
	reRigBone *bone;
	int s;
	
	for( s=0; s<pShapeCount; s++ ){
		shape = pShapes[ s ]->GetShape();
		bone = shape->GetRigBone();
		
		// build matrix
		matrix = decMatrix::CreateRT( pShapes[ s ]->GetOldOrientation() * DEG2RAD, pShapes[ s ]->GetOldPosition() );
		
		if( bone ){
			matrix = matrix * bone->GetPoseMatrix().ToMatrix() * rotationMatrix * bone->GetInversePoseMatrix().ToMatrix();
			
		}else{
			matrix *= rotationMatrix;
		}
		
		// modify orientation
		if( modifyOrientation ){
			view = matrix.TransformView();
			up = matrix.TransformUp();
			view.Normalize();
			up.Normalize();
			rotation = decMatrix::CreateWorld( decVector(), view, up ).GetEulerAngles();
			
			if( fabs( rotation.x ) < 1e-5f ) rotation.x = 0.0f;
			if( fabs( rotation.y ) < 1e-5f ) rotation.y = 0.0f;
			if( fabs( rotation.z ) < 1e-5f ) rotation.z = 0.0f;
			
			shape->SetOrientation( rotation / DEG2RAD );
		}
		
		// modify position
		if( modifyPosition ){
			position = matrix.GetPosition();
			
			if( fabs( position.x ) < 1e-5f ) position.x = 0.0f;
			if( fabs( position.y ) < 1e-5f ) position.y = 0.0f;
			if( fabs( position.z ) < 1e-5f ) position.z = 0.0f;
			
			shape->SetPosition( position );
		}
	}
}

void reURotateShape::ProgressiveRedo(){
	Redo(); // redo is enough in this situation
}



// Private Functions
//////////////////////

void reURotateShape::pCleanUp(){
	if( pShapes ){
		while( pShapeCount > 0 ){
			pShapeCount--;
			delete pShapes[ pShapeCount ];
		}
		
		delete [] pShapes;
	}
}
