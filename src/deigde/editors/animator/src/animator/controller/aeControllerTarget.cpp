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

#include "aeControllerTarget.h"
#include "../aeAnimator.h"
#include "../link/aeLink.h"

#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorLink.h>
#include <dragengine/resources/animator/controller/deAnimatorControllerTarget.h>
#include <dragengine/common/exceptions.h>



// Class aeControllerTarget
/////////////////////////////

// Constructor, destructor
////////////////////////////

aeControllerTarget::aeControllerTarget(){
}

aeControllerTarget::aeControllerTarget(const aeControllerTarget &copy) :
pLinks(copy.pLinks){
}

aeControllerTarget::~aeControllerTarget(){
	RemoveAllLinks();
}



// Management
///////////////

void aeControllerTarget::AddLink(aeLink *link){
	DEASSERT_NOTNULL(link)
	pLinks.Add(link);
}

void aeControllerTarget::RemoveLink(aeLink *link){
	pLinks.Remove(link);
}

void aeControllerTarget::RemoveAllLinks(){
	pLinks.RemoveAll();
}



void aeControllerTarget::UpdateEngineTarget(aeAnimator *animator, deAnimatorControllerTarget &target) const{
	if(!animator){
		DETHROW(deeInvalidParam);
	}
	
	target.RemoveAllLinks();
	
	deAnimator * const engAnimator = animator->GetEngineAnimator();
	if(engAnimator){
		const int linkCount = pLinks.GetCount();
		int i;
		
		for(i=0; i<linkCount; i++){
			deAnimatorLink * const engLink = ((aeLink*)pLinks.GetAt(i))->GetEngineLink();
			
			if(engLink){
				const int indexLink = engAnimator->IndexOfLink(engLink);
				
				if(indexLink != -1){
					target.AddLink(indexLink);
				}
			}
		}
	}
}



void aeControllerTarget::AddLinksToList(aeLink::List &list){
	list += pLinks;
}



// Operators
//////////////

aeControllerTarget &aeControllerTarget::operator=(const aeControllerTarget &copy){
	pLinks = copy.pLinks;
	return *this;
}
