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

#include "reUSetBoneIKLimitsLower.h"
#include "../../../rig/bone/reRigBone.h"

#include <dragengine/common/exceptions.h>



// Class reUSetBoneIKLimitsLower
//////////////////////////////////

// Constructor, destructor
////////////////////////////

reUSetBoneIKLimitsLower::reUSetBoneIKLimitsLower( reRigBone *bone, const decVector &ikLimitsLower ){
	if( ! bone ) DETHROW( deeInvalidParam );
	
	pBone = bone;
	pBone->AddReference();
	
	pOldIKLimitsLower = bone->GetIKLimitsLower();
	pNewIKLimitsLower = ikLimitsLower;
	
	try{
		SetShortInfo( "Set Bone IK Limits Lower" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

reUSetBoneIKLimitsLower::~reUSetBoneIKLimitsLower(){
	pCleanUp();
}



// Management
///////////////

void reUSetBoneIKLimitsLower::Undo(){
	pBone->SetIKLimitsLower( pOldIKLimitsLower );
}

void reUSetBoneIKLimitsLower::Redo(){
	pBone->SetIKLimitsLower( pNewIKLimitsLower );
}



// Private Functions
//////////////////////

void reUSetBoneIKLimitsLower::pCleanUp(){
	if( pBone ) pBone->FreeReference();
}
