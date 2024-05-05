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

#include "aeUSetRuleFStaScaleVertexPositionSet.h"
#include "../../../animator/rule/aeRuleForeignState.h"

#include <dragengine/common/exceptions.h>



// Class aeUSetRuleFStaScaleVertexPositionSet
///////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeUSetRuleFStaScaleVertexPositionSet::aeUSetRuleFStaScaleVertexPositionSet(
	aeRuleForeignState *rule, float newScale ) :
pRule( nullptr )
{
	DEASSERT_NOTNULL( rule )
	
	try{
		pRule = rule;
		pRule->AddReference();
		
		pOldScale = rule->GetScaleVertexPositionSet();
		pNewScale = newScale;
		
		SetShortInfo( "Set foreign state rule vertex position set scale" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

aeUSetRuleFStaScaleVertexPositionSet::~aeUSetRuleFStaScaleVertexPositionSet(){
	pCleanUp();
}



// Management
///////////////

void aeUSetRuleFStaScaleVertexPositionSet::Undo(){
	pRule->SetScaleVertexPositionSet( pOldScale );
}

void aeUSetRuleFStaScaleVertexPositionSet::Redo(){
	pRule->SetScaleVertexPositionSet( pNewScale );
}



// Private Functions
//////////////////////

void aeUSetRuleFStaScaleVertexPositionSet::pCleanUp(){
	if( pRule ){
		pRule->FreeReference();
	}
}
