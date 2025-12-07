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
#include <stdlib.h>
#include <string.h>

#include "aeULinkRemoveUnused.h"

#include "../../animator/aeAnimator.h"
#include "../../animator/link/aeLink.h"
#include "../../animator/rule/aeRule.h"

#include <dragengine/common/exceptions.h>



// Class aeULinkRemoveUnused
//////////////////////////////

// Constructor, destructor
////////////////////////////

aeULinkRemoveUnused::aeULinkRemoveUnused(aeAnimator *animator) :
pAnimator(NULL)
{
	if(!animator){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Remove unused links");
	
	const int count = animator->GetLinks().GetCount();
	int i;
	
	for(i=0; i<count; i++){
		aeLink * const link = animator->GetLinks().GetAt(i);
		if(animator->CountLinkUsage(link) == 0){
			pLinks.Add(link);
		}
	}
	
	pAnimator = animator;
	pAnimator->AddReference();
}

aeULinkRemoveUnused::~aeULinkRemoveUnused(){
	if(pAnimator){
		pAnimator->FreeReference();
	}
}



// Management
///////////////

void aeULinkRemoveUnused::Undo(){
	const int count = pLinks.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pAnimator->AddLink(pLinks.GetAt(i));
	}
}

void aeULinkRemoveUnused::Redo(){
	const int count = pLinks.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pAnimator->RemoveLink(pLinks.GetAt(i));
	}
}
