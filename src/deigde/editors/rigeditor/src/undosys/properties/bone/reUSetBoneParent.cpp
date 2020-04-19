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

#include "reUSetBoneParent.h"
#include "../../../rig/bone/reRigBone.h"

#include <dragengine/common/exceptions.h>



// Class reUSetBoneParent
///////////////////////////

// Constructor, destructor
////////////////////////////

reUSetBoneParent::reUSetBoneParent( reRigBone *bone, reRigBone *parent ){
	if( ! bone ) DETHROW( deeInvalidParam );
	
	pBone = bone;
	pBone->AddReference();
	
	pOldParent = bone->GetParentBone();
	if( pOldParent ) pOldParent->AddReference();
	
	pNewParent = parent;
	if( pNewParent ) pNewParent->AddReference();
	
	try{
		SetShortInfo( "Set Bone Parent" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

reUSetBoneParent::~reUSetBoneParent(){
	pCleanUp();
}



// Management
///////////////

void reUSetBoneParent::Undo(){
	pBone->SetParentBone( pOldParent );
}

void reUSetBoneParent::Redo(){
	pBone->SetParentBone( pNewParent );
}



// Private Functions
//////////////////////

void reUSetBoneParent::pCleanUp(){
	if( pNewParent ) pNewParent->FreeReference();
	if( pOldParent ) pOldParent->FreeReference();
	
	if( pBone ) pBone->FreeReference();
}
