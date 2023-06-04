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

#include "aeURuleMirrorVertexPositionSets.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/aeAnimator.h"

#include <dragengine/common/exceptions.h>



// Class aeURuleMirrorVertexPositionSets
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeURuleMirrorVertexPositionSets::aeURuleMirrorVertexPositionSets( aeRule *rule ) :
pRule( nullptr )
{
	DEASSERT_NOTNULL( rule )
	
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
	
	pRule = rule;
	pRule->AddReference();
}

aeURuleMirrorVertexPositionSets::~aeURuleMirrorVertexPositionSets(){
	if( pRule ){
		pRule->FreeReference();
	}
}



// Management
///////////////

bool aeURuleMirrorVertexPositionSets::HasAnyEffect() const{
	const decStringSet &sets = pRule->GetListVertexPositionSets();
	const int patternCount = pPatterns.GetCount();
	const int setCount = sets.GetCount();
	int i, j;
	
	for( i=0; i<setCount; i++ ){
		const decString &set = sets.GetAt( i );
		
		for( j=0; j<patternCount; j++ ){
			const decString &pattern = pPatterns.GetAt( j );
			
			if( set.GetRight( pattern.GetLength() ) == pattern ){
				return true;
			}
		}
	}
	
	return false;
}



void aeURuleMirrorVertexPositionSets::Undo(){
	Redo();
}

void aeURuleMirrorVertexPositionSets::Redo(){
	const decStringSet &sets = pRule->GetListVertexPositionSets();
	const int patternCount = pPatterns.GetCount();
	const int setCount = sets.GetCount();
	decStringSet mirroredVertexPositionSets;
	int i, j;
	
	for( i=0; i<setCount; i++ ){
		const decString &set = sets.GetAt( i );
		
		for( j=0; j<patternCount; j++ ){
			const decString &pattern = pPatterns.GetAt( j );
			
			if( set.GetRight( pattern.GetLength() ) == pattern ){
				const decString &replace = pPatterns.GetAt( ( ( j % 2 ) == 0 ) ? j + 1 : j - 1 );
				mirroredVertexPositionSets.Add( set.GetLeft( set.GetLength() - pattern.GetLength() ) + replace );
				break;
			}
		}
		
		if( j == patternCount ){
			mirroredVertexPositionSets.Add( set );
		}
	}
	
	pRule->SetListVertexPositionSets( mirroredVertexPositionSets );
}
