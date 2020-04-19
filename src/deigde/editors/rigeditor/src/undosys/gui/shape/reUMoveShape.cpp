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

#include "reUMoveShape.h"
#include "reUndoDataShape.h"
#include "../../../rig/bone/reRigBone.h"
#include "../../../rig/shape/reRigShape.h"
#include "../../../rig/shape/reRigShapeList.h"

#include <dragengine/common/exceptions.h>



// Class reUMoveShape
///////////////////////

// Constructor, destructor
////////////////////////////

reUMoveShape::reUMoveShape( reRigShapeList &list ){
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
			SetShortInfo( "Move Shapes" );
			
		}else{
			SetShortInfo( "Move Shape" );
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

reUMoveShape::~reUMoveShape(){
	pCleanUp();
}



// Undo and Redo operations
/////////////////////////////

void reUMoveShape::Undo(){
	reRigShape *shape;
	int s;
	
	for( s=0; s<pShapeCount; s++ ){
		shape = pShapes[ s ]->GetShape();
		
		shape->SetPosition( pShapes[ s ]->GetOldPosition() );
	}
}

void reUMoveShape::Redo(){
	reRigShape *shape;
	decMatrix matrix;
	reRigBone *bone;
	int s;
	
	for( s=0; s<pShapeCount; s++ ){
		shape = pShapes[ s ]->GetShape();
		bone = shape->GetRigBone();
		
		if( bone ){
			matrix = bone->GetPoseMatrix().ToMatrix()
				* decMatrix::CreateTranslation( GetDistance() )
				* bone->GetInversePoseMatrix().ToMatrix();
			
		}else{
			matrix.SetTranslation( GetDistance() );
		}
		
		shape->SetPosition( matrix * pShapes[ s ]->GetOldPosition() );
	}
}

void reUMoveShape::ProgressiveRedo(){
	Redo(); // redo is enough in this situation
}



// Private Functions
//////////////////////

void reUMoveShape::pCleanUp(){
	if( pShapes ){
		while( pShapeCount > 0 ){
			pShapeCount--;
			delete pShapes[ pShapeCount ];
		}
		
		delete [] pShapes;
	}
}
