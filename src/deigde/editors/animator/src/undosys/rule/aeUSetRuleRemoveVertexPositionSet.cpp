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

#include "aeUSetRuleRemoveVertexPositionSet.h"
#include "../../animator/rule/aeRule.h"

#include <dragengine/common/exceptions.h>



// Class aeUSetRuleRemoveVertexPositionSet
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeUSetRuleRemoveVertexPositionSet::aeUSetRuleRemoveVertexPositionSet( aeRule* rule, const char* pattern ){
	DEASSERT_NOTNULL( rule )
	DEASSERT_NOTNULL( pattern )
	
	pRule = nullptr;
	
	const decStringSet &sets = rule->GetListVertexPositionSets();
	const int setCount = sets.GetCount();
	int i;
	
	for( i=0; i<setCount; i++ ){
		const decString &set = sets.GetAt( i );
		
		if( set.MatchesPattern( pattern ) ){
			pVertexPositionSets.Add( set );
		}
	}
	
	if( pVertexPositionSets.GetCount() == 1 ){
		SetShortInfo( "Remove vertex position set from rule" );
		
	}else{
		SetShortInfo( "Remove vertex position sets from rule" );
	}
	
	pRule = rule;
	pRule->AddReference();
}

aeUSetRuleRemoveVertexPositionSet::~aeUSetRuleRemoveVertexPositionSet(){
	if( pRule ){
		pRule->FreeReference();
	}
}



// Management
///////////////

bool aeUSetRuleRemoveVertexPositionSet::HasVertexPositionSets() const{
	return pVertexPositionSets.GetCount() > 0;
}

void aeUSetRuleRemoveVertexPositionSet::Undo(){
	const int count = pVertexPositionSets.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pRule->AddVertexPositionSet( pVertexPositionSets.GetAt( i ) );
	}
}

void aeUSetRuleRemoveVertexPositionSet::Redo(){
	const int count = pVertexPositionSets.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pRule->RemoveVertexPositionSet( pVertexPositionSets.GetAt( i ) );
	}
}
