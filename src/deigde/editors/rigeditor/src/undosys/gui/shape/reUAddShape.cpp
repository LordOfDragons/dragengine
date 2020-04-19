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

#include "reUAddShape.h"
#include "../../../rig/reRig.h"
#include "../../../rig/bone/reRigBone.h"
#include "../../../rig/shape/reRigShape.h"
#include "../../../rig/shape/reSelectionShapes.h"

#include <dragengine/common/exceptions.h>



// Class reUAddShape
//////////////////////

// Constructor, destructor
////////////////////////////

reUAddShape::reUAddShape( reRig *rig, reRigBone *bone, reRigShape *shape ){
	if( ! shape || ( ! rig && ! bone ) ) DETHROW( deeInvalidParam );
	
	pRig = rig;
	if( rig ) rig->AddReference();
	
	pBone = bone;
	if( bone ) bone->AddReference();
	
	pShape = shape;
	shape->AddReference();
	
	try{
		if( bone ){
			SetShortInfo( "Add Bone Shape" );
			
		}else{
			SetShortInfo( "Add Rig Shape" );
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

reUAddShape::~reUAddShape(){
	pCleanUp();
}



// Management
///////////////

void reUAddShape::Undo(){
	reRig *rig = pGetRig();
	
	reSelectionShapes *selection = rig->GetSelectionShapes();
	
	if( pShape->GetSelected() ){
		selection->RemoveShape( pShape );
	}
	
	if( pBone ){
		pBone->RemoveShape( pShape );
		
	}else{
		rig->RemoveShape( pShape );
	}
}

void reUAddShape::Redo(){
	reRig *rig = pGetRig();
	
	reSelectionShapes *selection = rig->GetSelectionShapes();
	
	if( pBone ){
		pBone->AddShape( pShape );
		
	}else{
		rig->AddShape( pShape );
	}
	
	selection->RemoveAllShapes();
	selection->AddShape( pShape );
}



// Private Functions
//////////////////////

void reUAddShape::pCleanUp(){
	if( pShape ) pShape->FreeReference();
	if( pBone ) pBone->FreeReference();
	if( pRig ) pRig->FreeReference();
}

reRig *reUAddShape::pGetRig(){
	if( pRig ) return pRig;
	
	reRig *rig = pBone->GetRig();
	if( ! rig ) DETHROW( deeInvalidParam );
	
	return rig;
}
