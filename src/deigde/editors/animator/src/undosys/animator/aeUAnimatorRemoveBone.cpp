/* 
 * Drag[en]gine IGDE Animator Editor
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
#include <string.h>
#include <stdlib.h>

#include "aeUAnimatorRemoveBone.h"
#include "../../animator/aeAnimator.h"

#include <dragengine/common/exceptions.h>



// Class aeUAnimatorRemoveBone
////////////////////////////////

// Constructor, destructor
////////////////////////////

aeUAnimatorRemoveBone::aeUAnimatorRemoveBone( aeAnimator* animator, const char* pattern ) :
pAnimator( animator )
{
	if( ! pattern ){
		DETHROW( deeInvalidParam );
	}
	
	const decStringSet &ruleBoneList = animator->GetListBones();
	const int boneCount = ruleBoneList.GetCount();
	int i;
	
	for( i=0; i<boneCount; i++ ){
		const decString &bone = ruleBoneList.GetAt( i );
		
		if( bone.MatchesPattern( pattern ) ){
			pBones.Add( bone );
		}
	}
	
	if( pBones.GetCount() == 1 ){
		SetShortInfo( "Animator remove bone" );
		
	}else{
		SetShortInfo( "Animator remove bones" );
	}
}

aeUAnimatorRemoveBone::~aeUAnimatorRemoveBone(){
}



// Management
///////////////

void aeUAnimatorRemoveBone::Undo(){
	const int count = pBones.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pAnimator->AddBone( pBones.GetAt( i ) );
	}
}

void aeUAnimatorRemoveBone::Redo(){
	const int count = pBones.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pAnimator->RemoveBone( pBones.GetAt( i ) );
	}
}
