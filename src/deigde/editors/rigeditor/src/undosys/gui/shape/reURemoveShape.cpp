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

#include "reURemoveShape.h"
#include "../../../rig/reRig.h"
#include "../../../rig/bone/reRigBone.h"
#include "../../../rig/shape/reRigShape.h"
#include "../../../rig/shape/reRigShapeList.h"
#include "../../../rig/shape/reSelectionShapes.h"

#include <dragengine/common/exceptions.h>



// Class reURemoveShape
/////////////////////////

// Constructor, destructor
////////////////////////////

reURemoveShape::reURemoveShape( reRigShapeList &list ){
	int shapeCount = list.GetShapeCount();
	reRigShape *shape;
	reRigBone *bone;
	
	if( shapeCount == 0 ) DETHROW( deeInvalidParam );
	
	pRig = list.GetShapeAt( 0 )->GetRig();
	if( ! pRig ) DETHROW( deeInvalidParam );
	pRig->AddReference();
	
	pEntries = NULL;
	pEntryCount = 0;
	
	try{
		pEntries = new sEntry[ shapeCount ];
		if( ! pEntries ) DETHROW( deeOutOfMemory );
		
		while( pEntryCount < shapeCount ){
			shape = list.GetShapeAt( pEntryCount );
			bone = shape->GetRigBone();
			
			if( shape->GetRig() != pRig ) DETHROW( deeInvalidParam );
			
			pEntries[ pEntryCount ].shape = shape;
			shape->AddReference();
			
			pEntries[ pEntryCount ].bone = bone;
			if( bone ) bone->AddReference();
			
			pEntryCount++;
		}
		
		SetShortInfo( "Remove Shapes" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

reURemoveShape::~reURemoveShape(){
	pCleanUp();
}



// Management
///////////////

void reURemoveShape::Undo(){
	reSelectionShapes *selection = pRig->GetSelectionShapes();
	int e;
	
	selection->RemoveAllShapes();
	
	for( e=0; e<pEntryCount; e++ ){
		if( pEntries[ e ].bone ){
			pEntries[ e ].bone->AddShape( pEntries[ e ].shape );
			
		}else{
			pRig->AddShape( pEntries[ e ].shape );
		}
		
		selection->AddShape( pEntries[ e ].shape );
	}
	
}

void reURemoveShape::Redo(){
	reSelectionShapes *selection = pRig->GetSelectionShapes();
	int e;
	
	for( e=0; e<pEntryCount; e++ ){
		if( pEntries[ e ].shape->GetSelected() ){
			selection->RemoveShape( pEntries[ e ].shape );
		}
		
		if( pEntries[ e ].bone ){
			pEntries[ e ].bone->RemoveShape( pEntries[ e ].shape );
			
		}else{
			pRig->RemoveShape( pEntries[ e ].shape );
		}
	}
}



// Private Functions
//////////////////////

void reURemoveShape::pCleanUp(){
	reRigBone *bone;
	
	if( pEntries ){
		while( pEntryCount > 0 ){
			pEntryCount--;
			
			bone = pEntries[ pEntryCount ].bone;
			
			if( bone ) bone->FreeReference();
			pEntries[ pEntryCount ].shape->FreeReference();
		}
		
		delete [] pEntries;
	}
	
	if( pRig ){
		pRig->FreeReference();
	}
}
