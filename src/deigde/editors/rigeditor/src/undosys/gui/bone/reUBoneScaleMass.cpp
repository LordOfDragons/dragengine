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

#include "reUBoneScaleMass.h"
#include "../../../rig/reRig.h"
#include "../../../rig/bone/reRigBone.h"
#include "../../../rig/bone/reSelectionBones.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>



// Class reUBoneScaleMass
///////////////////////////

// Constructor, destructor
////////////////////////////

reUBoneScaleMass::reUBoneScaleMass( reRig *rig, const decObjectOrderedSet &bones, float newMass ){
	if( ! rig || bones.GetCount() == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( newMass < 0.0f ){
		newMass = 0.0f;
	}
	
	const int boneCount = bones.GetCount();
	float oldMass = 0.0f;
	int i;
	
	pRig = NULL;
	
	pBones = NULL;
	pBoneCount = 0;
	
	SetShortInfo( "Scale bone mass" );
	
	try{
		if( boneCount > 0 ){
			// create bones array
			pBones = new sBone[ boneCount ];
			
			// store bones summing up the their mass. ignore bones without a shape
			for( i=0; i<boneCount; i++ ){
				pBones[ i ].bone = ( reRigBone* )bones.GetAt( i );
				
				if( pBones[ i ].bone->GetShapeCount() == 0 ){
					pBones[ i ].bone = NULL;
					pBones[ i ].oldMass = 0.0f;
					
				}else{
					pBones[ i ].bone->AddReference();
					pBones[ i ].oldMass = pBones[ i ].bone->GetMass();
					oldMass += pBones[ i ].oldMass;
				}
			}
			
			pBoneCount = boneCount;
			
			// calculate the new mass for each bone
			if( oldMass < FLOAT_SAFE_EPSILON ){
				for( i=0; i<boneCount; i++ ){
					pBones[ i ].newMass = pBones[ i ].oldMass;
				}
				
			}else{
				const float factor = newMass / oldMass;
				
				for( i=0; i<boneCount; i++ ){
					pBones[ i ].newMass = pBones[ i ].oldMass * factor;
				}
			}
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	
	pRig = rig;
	rig->AddReference();
}

reUBoneScaleMass::~reUBoneScaleMass(){
	pCleanUp();
}



// Undo and Redo operations
/////////////////////////////

void reUBoneScaleMass::Undo(){
	int i;
	
	for( i=0; i<pBoneCount; i++ ){
		if( pBones[ i ].bone ){
			pBones[ i ].bone->SetMass( pBones[ i ].oldMass );
		}
	}
}

void reUBoneScaleMass::Redo(){
	int i;
	
	for( i=0; i<pBoneCount; i++ ){
		if( pBones[ i ].bone ){
			pBones[ i ].bone->SetMass( pBones[ i ].newMass );
		}
	}
}



// Private Functions
//////////////////////

void reUBoneScaleMass::pCleanUp(){
	if( pBones ){
		while( pBoneCount > 0 ){
			pBoneCount--;
			if( pBones[ pBoneCount ].bone ){
				pBones[ pBoneCount ].bone->FreeReference();
			}
		}
		
		delete [] pBones;
	}
	
	if( pRig ){
		pRig->FreeReference();
	}
}
