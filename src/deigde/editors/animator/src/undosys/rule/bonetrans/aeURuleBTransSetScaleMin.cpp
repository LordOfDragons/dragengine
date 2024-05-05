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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "aeURuleBTransSetScaleMin.h"
#include "../../../animator/rule/aeRuleBoneTransformator.h"

#include <dragengine/common/exceptions.h>



// Class aeURuleBTransSetScaleMin
///////////////////////////////////

// Constructor, destructor
////////////////////////////

aeURuleBTransSetScaleMin::aeURuleBTransSetScaleMin( aeRuleBoneTransformator *rule, const decVector &newMin ){
	if( ! rule ){
		DETHROW( deeInvalidParam );
	}
	
	pRule = NULL;
	
	try{
		pRule = rule;
		pRule->AddReference();
		
		pOldMin = rule->GetMinimumScaling();
		pNewMin = newMin;
		
		SetShortInfo( "Bone transformator Set minimum scaling" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

aeURuleBTransSetScaleMin::~aeURuleBTransSetScaleMin(){
	pCleanUp();
}



// Management
///////////////

void aeURuleBTransSetScaleMin::Undo(){
	pRule->SetMinimumScaling( pOldMin );
}

void aeURuleBTransSetScaleMin::Redo(){
	pRule->SetMinimumScaling( pNewMin );
}



// Private Functions
//////////////////////

void aeURuleBTransSetScaleMin::pCleanUp(){
	if( pRule ){
		pRule->FreeReference();
	}
}
