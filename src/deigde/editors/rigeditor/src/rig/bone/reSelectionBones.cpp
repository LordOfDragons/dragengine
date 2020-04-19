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
#include <string.h>

#include "reRigBone.h"
#include "reSelectionBones.h"
#include "../reRig.h"

#include <dragengine/common/exceptions.h>



// Class reSelectionBones
///////////////////////////

// Constructor, destructor
////////////////////////////

reSelectionBones::reSelectionBones( reRig *rig ){
	if( ! rig ) DETHROW( deeInvalidParam );
	
	pRig = rig;
	
	pBones = NULL;
	pBoneCount = 0;
	pBoneSize = 0;
	pActiveBone = NULL;
}

reSelectionBones::~reSelectionBones(){
	Reset();
	if( pBones ) delete [] pBones;
}



// Management
///////////////

reRigBone *reSelectionBones::GetBoneAt( int index ) const{
	if( index < 0 || index >= pBoneCount ) DETHROW( deeOutOfBoundary );
	
	return pBones[ index ];
}

bool reSelectionBones::HasBone( reRigBone *bone ) const{
	if( ! bone ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pBoneCount; i++ ){
		if( bone == pBones[ i ] ){
			return true;
		}
	}
	
	return false;
}
	
int reSelectionBones::IndexOfBone( reRigBone *bone ) const{
	if( ! bone ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pBoneCount; i++ ){
		if( bone == pBones[ i ] ){
			return i;
		}
	}
	
	return -1;
}

int reSelectionBones::IndexOfBoneWith( deColliderVolume *collider ) const{
	if( ! collider ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pBoneCount; i++ ){
		if( collider == pBones[ i ]->GetCollider() ){
			return i;
		}
	}
	
	return -1;
}

void reSelectionBones::AddBone( reRigBone *bone ){
	if( HasBone( bone ) ) DETHROW( deeInvalidParam );
	
	if( pBoneCount == pBoneSize ){
		int newSize = pBoneSize * 3 / 2 + 1;
		reRigBone **newArray = new reRigBone*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pBones ){
			memcpy( newArray, pBones, sizeof( reRigBone* ) * pBoneSize );
			delete [] pBones;
		}
		pBones = newArray;
		pBoneSize = newSize;
	}
	
	pBones[ pBoneCount ] = bone;
	pBoneCount++;
	
	bone->AddReference();
	
	bone->SetSelected( true );
	
	pRig->NotifyBoneSelectedChanged( bone );
	
	if( ! pActiveBone ){
		SetActiveBone( bone );
	}
}

void reSelectionBones::RemoveBone( reRigBone *bone ){
	int i, index = IndexOfBone( bone );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	for( i=index+1; i<pBoneCount; i++ ){
		pBones[ i - 1 ] = pBones[ i ];
	}
	pBones[ pBoneCount - 1 ] = NULL;
	pBoneCount--;
	
	bone->SetSelected( false );
	
	if( bone == pActiveBone ){
		if( pBoneCount > 0 ){
			SetActiveBone( pBones[ 0 ] );
			
		}else{
			SetActiveBone( NULL );
		}
	}
	
	pRig->NotifyBoneSelectedChanged( bone );
	
	bone->FreeReference();
}

void reSelectionBones::RemoveAllBones(){
	SetActiveBone( NULL );
	
	pRig->NotifyAllBonesDeselected();
	
	while( pBoneCount > 0 ){
		pBoneCount--;
		
		pBones[ pBoneCount ]->SetSelected( false );
		pBones[ pBoneCount ]->FreeReference();
	}
}



bool reSelectionBones::HasActiveBone() const{
	return pActiveBone != NULL;
}

void reSelectionBones::SetActiveBone( reRigBone *bone ){
	if( bone != pActiveBone ){
		if( bone && ! HasBone( bone ) ) DETHROW( deeInvalidParam );
		
		if( pActiveBone ){
			pActiveBone->SetActive( false );
		}
		
		pActiveBone = bone;
		
		if( bone ){
			bone->SetActive( true );
		}
		
		pRig->NotifyActiveBoneChanged();
	}
}

void reSelectionBones::Reset(){
	RemoveAllBones();
}
