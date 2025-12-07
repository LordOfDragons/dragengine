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

#include "dearAnimatorInstance.h"
#include "dearControllerTarget.h"
#include "dearControllerStates.h"
#include "dearLink.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/controller/deAnimatorControllerTarget.h>



// Class dearControllerTarget
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dearControllerTarget::dearControllerTarget(const deAnimatorControllerTarget &target, int firstLink) :
pLinks(NULL),
pLinkCount(0){
	const int linkCount = target.GetLinkCount();
	
	if(linkCount > 0){
		pLinks = new int[linkCount];
		
		for(pLinkCount=0; pLinkCount<linkCount; pLinkCount++){
			pLinks[pLinkCount] = firstLink + target.GetLinkAt(pLinkCount);
		}
	}
}

dearControllerTarget::~dearControllerTarget(){
	if(pLinks){
		delete [] pLinks;
	}
}



// Management
///////////////

int dearControllerTarget::GetLinkAt(int index) const{
	if(index < 0 || index >= pLinkCount){
		DETHROW(deeInvalidParam);
	}
	return pLinks[index];
}



float dearControllerTarget::GetValue(const dearAnimatorInstance &instance, float defaultValue) const{
	float value = defaultValue;
	bool firstValue = true;
	int i;
	
	for(i=0; i<pLinkCount; i++){
		if(pLinks[i] == -1){
			continue;
		}
		
		const dearLink &link = *instance.GetLinkAt(pLinks[i]);
		if(!link.HasController() && !link.HasBone()){
			continue;
		}
		
		if(firstValue){
			value = link.GetValue(1.0f);
			firstValue = false;
			
		}else{
			value *= link.GetValue(1.0f);
		}
	}
	
	return value;
}

void dearControllerTarget::GetVector(const dearAnimatorInstance &instance, decVector &vector) const{
	int i;
	
	for(i=0; i<pLinkCount; i++){
		if(pLinks[i] == -1){
			continue;
		}
		
		const dearLink &link = *instance.GetLinkAt(pLinks[i]);
		if(!link.HasController()){
			continue;
		}
		
		vector = instance.GetControllerStates().GetVectorAt(link.GetController());
		break;
	}
}

void dearControllerTarget::GetQuaternion(const dearAnimatorInstance &instance, decQuaternion &quaternion) const{
	int i;
	
	for(i=0; i<pLinkCount; i++){
		if(pLinks[i] == -1){
			continue;
		}
		
		const dearLink &link = *instance.GetLinkAt(pLinks[i]);
		if(!link.HasController()){
			continue;
		}
		
		quaternion.SetFromEuler(instance.GetControllerStates().GetVectorAt(link.GetController()));
		break;
	}
}
