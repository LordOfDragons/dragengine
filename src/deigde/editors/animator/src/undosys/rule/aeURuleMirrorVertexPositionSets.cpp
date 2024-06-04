/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
