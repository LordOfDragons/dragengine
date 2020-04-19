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

#include "reUSetBoneOrientation.h"
#include "../../../rig/bone/reRigBone.h"

#include <dragengine/common/exceptions.h>



// Class reUSetBoneOrientation
/////////////////////////////

// Constructor, destructor
////////////////////////////

reUSetBoneOrientation::reUSetBoneOrientation( reRigBone *bone, const decVector &orientation ){
	if( ! bone ) DETHROW( deeInvalidParam );
	
	pBone = bone;
	pBone->AddReference();
	
	pOldOrientation = bone->GetOrientation();
	pNewOrientation = orientation;
	
	try{
		SetShortInfo( "Set Bone Orientation" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

reUSetBoneOrientation::~reUSetBoneOrientation(){
	pCleanUp();
}



// Management
///////////////

void reUSetBoneOrientation::Undo(){
	pBone->SetOrientation( pOldOrientation );
}

void reUSetBoneOrientation::Redo(){
	pBone->SetOrientation( pNewOrientation );
}



// Private Functions
//////////////////////

void reUSetBoneOrientation::pCleanUp(){
	if( pBone ) pBone->FreeReference();
}
