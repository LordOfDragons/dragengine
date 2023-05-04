/* 
 * Drag[en]gine IGDE Animator Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "aeUAnimatorRemoveVertexPositionSet.h"
#include "../../animator/aeAnimator.h"

#include <dragengine/common/exceptions.h>



// Class aeUAnimatorRemoveVertexPositionSet
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeUAnimatorRemoveVertexPositionSet::aeUAnimatorRemoveVertexPositionSet(
	aeAnimator* animator, const char* pattern ) :
pAnimator( animator )
{
	DEASSERT_NOTNULL( pattern )
	
	const decStringSet &sets = animator->GetListVertexPositionSets();
	const int setCount = sets.GetCount();
	int i;
	
	for( i=0; i<setCount; i++ ){
		const decString &set = sets.GetAt( i );
		
		if( set.MatchesPattern( pattern ) ){
			pVertexPositionSets.Add( set );
		}
	}
	
	if( pVertexPositionSets.GetCount() == 1 ){
		SetShortInfo( "Animator remove vertex position set" );
		
	}else{
		SetShortInfo( "Animator remove vertex position sets" );
	}
}

aeUAnimatorRemoveVertexPositionSet::~aeUAnimatorRemoveVertexPositionSet(){
}



// Management
///////////////

void aeUAnimatorRemoveVertexPositionSet::Undo(){
	const int count = pVertexPositionSets.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pAnimator->AddVertexPositionSet( pVertexPositionSets.GetAt( i ) );
	}
}

void aeUAnimatorRemoveVertexPositionSet::Redo(){
	const int count = pVertexPositionSets.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pAnimator->RemoveVertexPositionSet( pVertexPositionSets.GetAt( i ) );
	}
}
