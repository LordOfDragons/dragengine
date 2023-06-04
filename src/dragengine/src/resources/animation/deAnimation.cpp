/* 
 * Drag[en]gine Game Engine
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

#include "deAnimation.h"
#include "deAnimationBone.h"
#include "deAnimationMove.h"
#include "deAnimationManager.h"
#include "../model/deModel.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/file/decPath.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/animator/deBaseAnimatorAnimation.h"



// Class deAnimation
//////////////////////

// Constructor, destructor
////////////////////////////

deAnimation::deAnimation( deAnimationManager *manager, deVirtualFileSystem *vfs,
	const char *filename, TIME_SYSTEM modificationTime ) :
deFileResource( manager, vfs, filename, modificationTime ),
pBones( nullptr ),
pBoneCount( 0 ),
pBoneSize( 0 ),
pMoves( nullptr ),
pMoveCount( 0 ),
pMoveSize( 0 ),
pPeerAnimator( nullptr ){
}

deAnimation::~deAnimation(){
	pCleanUp();
}



// Management
///////////////

bool deAnimation::MatchesModel( deModel *model ) const{
	DEASSERT_NOTNULL( model )
	
	int i;
	for( i=0; i<pBoneCount; i++ ){
		if( ! model->HasBoneNamed( pBones[ i ]->GetName() ) ){
			return false;
		}
	}
	
	const int vpsCount = pVertexPositionSets.GetCount();
	for( i=0; i<vpsCount; i++ ){
		if( ! model->HasVertexPositionSetNamed( pVertexPositionSets.GetAt( i ) ) ){
			return false;
		}
	}
	
	return true;
}



// Bones
//////////

deAnimationBone *deAnimation::GetBone( int index ) const{
	if( index < 0 || index >= pBoneCount ){
		DETHROW( deeOutOfBoundary );
	}
	return pBones[ index ];
}

int deAnimation::FindBone( const char *name ) const{
	int i;
	
	for( i=0; i<pBoneCount; i++ ){
		if( strcmp( pBones[ i ]->GetName(), name ) == 0 ){
			return i;
		}
	}
	
	return -1;
}

void deAnimation::AddBone( deAnimationBone *bone ){
	if( ! bone || FindBone( bone->GetName() ) != -1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pBoneCount == pBoneSize ){
		int i, newSize = pBoneCount * 3 / 2 + 1;
		deAnimationBone **newArray = new deAnimationBone*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pBones ){
			for( i=0; i<pBoneCount; i++ ) newArray[ i ] = pBones[ i ];
			delete [] pBones;
		}
		pBones = newArray;
		pBoneSize = newSize;
	}
	pBones[ pBoneCount ] = bone;
	pBoneCount++;
}



// Moves
//////////

deAnimationMove *deAnimation::GetMove( int index ) const{
	if( index < 0 || index >= pMoveCount ){
		DETHROW( deeOutOfBoundary );
	}
	return pMoves[ index ];
}

int deAnimation::FindMove( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	for( i=0; i<pMoveCount; i++ ){
		if( strcmp( pMoves[ i ]->GetName(), name ) == 0 ){
			return i;
		}
	}
	
	return -1;
}

void deAnimation::AddMove( deAnimationMove *move ){
	if( ! move || FindMove( move->GetName() ) != -1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pMoveCount == pMoveSize ){
		int i, newSize = pMoveCount * 3 / 2 + 1;
		deAnimationMove **newArray = new deAnimationMove*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pMoves ){
			for( i=0; i<pMoveCount; i++ ) newArray[ i ] = pMoves[ i ];
			delete [] pMoves;
		}
		pMoves = newArray;
		pMoveSize = newSize;
	}
	pMoves[ pMoveCount ] = move;
	pMoveCount++;
}



// System Peers
/////////////////

void deAnimation::SetPeerAnimator( deBaseAnimatorAnimation *peer ){
	if( peer == pPeerAnimator ){
		return;
	}
	
	if( pPeerAnimator ){
		delete pPeerAnimator;
	}
	pPeerAnimator = peer;
}



// Private Functions
//////////////////////

void deAnimation::pCleanUp(){
	int i;
	
	if( pPeerAnimator ){
		delete pPeerAnimator;
		pPeerAnimator = NULL;
	}
	
	if( pMoves ){
		for( i=0; i<pMoveCount; i++ ){
			if( pMoves[ i ] ){
				delete pMoves[ i ];
			}
		}
		delete [] pMoves;
	}
	
	if( pBones ){
		for( i=0; i<pBoneCount; i++ ){
			if( pBones[ i ] ){
				delete pBones[ i ];
			}
		}
		delete [] pBones;
	}
}
