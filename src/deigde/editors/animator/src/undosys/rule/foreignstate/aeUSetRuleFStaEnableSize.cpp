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

#include "aeUSetRuleFStaEnableSize.h"
#include "../../../animator/rule/aeRuleForeignState.h"

#include <dragengine/common/exceptions.h>



// Class aeUSetRuleFStaEnableSize
///////////////////////////////////

// Constructor, destructor
////////////////////////////

aeUSetRuleFStaEnableSize::aeUSetRuleFStaEnableSize(aeRuleForeignState *rule){
	if(!rule) DETHROW(deeInvalidParam);
	
	pRule = NULL;
	
	try{
		pRule = rule;
		
		SetShortInfo("Set bone rotator rule enable size");
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

aeUSetRuleFStaEnableSize::~aeUSetRuleFStaEnableSize(){
	pCleanUp();
}



// Management
///////////////

void aeUSetRuleFStaEnableSize::Undo(){
	pRule->SetEnableSize(!pRule->GetEnableSize());
}

void aeUSetRuleFStaEnableSize::Redo(){
	pRule->SetEnableSize(!pRule->GetEnableSize());
}



// Private Functions
//////////////////////

void aeUSetRuleFStaEnableSize::pCleanUp(){
	if(pRule) pRule->FreeReference();
}
