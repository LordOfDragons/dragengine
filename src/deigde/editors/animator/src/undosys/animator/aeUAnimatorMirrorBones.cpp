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

#include "aeUAnimatorMirrorBones.h"
#include "../../animator/aeAnimator.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/component/deComponent.h>



// Class aeUAnimatorMirrorBones
/////////////////////////////////

// Constructor, destructor
////////////////////////////

aeUAnimatorMirrorBones::aeUAnimatorMirrorBones( aeAnimator *animator ) :
pAnimator( animator )
{
	if( ! animator ){
		DETHROW( deeInvalidParam );
	}
	
	pPatterns.Add( ".l" );
	pPatterns.Add( ".r" );
	
	pPatterns.Add( ".L" );
	pPatterns.Add( ".R" );
	
	pPatterns.Add( " l" );
	pPatterns.Add( " r" );
	
	pPatterns.Add( " L" );
	pPatterns.Add( " R" );
	
	pPatterns.Add( ".left" );
	pPatterns.Add( ".right" );
	
	pPatterns.Add( " left" );
	pPatterns.Add( " right" );
	
	SetShortInfo( "Animator mirror bones" );
}

aeUAnimatorMirrorBones::~aeUAnimatorMirrorBones(){
}



// Management
///////////////

bool aeUAnimatorMirrorBones::HasAnyEffect() const{
	const decStringSet &bones = pAnimator->GetListBones();
	const int patternCount = pPatterns.GetCount();
	const int boneCount = bones.GetCount();
	int i, j;
	
	for( i=0; i<boneCount; i++ ){
		const decString &bone = bones.GetAt( i );
		
		for( j=0; j<patternCount; j++ ){
			const decString &pattern = pPatterns.GetAt( j );
			
			if( bone.GetRight( pattern.GetLength() ) == pattern ){
				return true;
			}
		}
	}
	
	return false;
}



void aeUAnimatorMirrorBones::Undo(){
	Redo();
}

void aeUAnimatorMirrorBones::Redo(){
	const decStringSet &bones = pAnimator->GetListBones();
	const int patternCount = pPatterns.GetCount();
	const int boneCount = bones.GetCount();
	decStringSet mirroredBones;
	int i, j;
	
	for( i=0; i<boneCount; i++ ){
		const decString &bone = bones.GetAt( i );
		
		for( j=0; j<patternCount; j++ ){
			const decString &pattern = pPatterns.GetAt( j );
			
			if( bone.GetRight( pattern.GetLength() ) == pattern ){
				const decString &replace = pPatterns.GetAt( ( ( j % 2 ) == 0 ) ? j + 1 : j - 1 );
				mirroredBones.Add( bone.GetLeft( bone.GetLength() - pattern.GetLength() ) + replace );
				break;
			}
		}
		
		if( j == patternCount ){
			mirroredBones.Add( bone );
		}
	}
	
	pAnimator->SetListBones( mirroredBones );
}
