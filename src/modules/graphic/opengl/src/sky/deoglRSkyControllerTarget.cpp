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

#include "deoglRSky.h"
#include "deoglRSkyControllerTarget.h"
#include "deoglRSkyInstance.h"
#include "deoglRSkyLink.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sky/deSkyControllerTarget.h>



// Class deoglRSkyControllerTarget
////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoglRSkyControllerTarget::deoglRSkyControllerTarget(const deSkyControllerTarget &target) :
pLinks(NULL),
pLinkCount(0)
{
	const int linkCount = target.GetLinkCount();
	if(linkCount == 0){
		return;
	}
	
	pLinks = new int[linkCount];
	for(pLinkCount=0; pLinkCount<linkCount; pLinkCount++){
		pLinks[pLinkCount] = target.GetLinkAt(pLinkCount);
	}
}

deoglRSkyControllerTarget::~deoglRSkyControllerTarget(){
	if(pLinks){
		delete [] pLinks;
	}
}



// Management
///////////////

int deoglRSkyControllerTarget::GetLinkAt(int index) const{
	if(index < 0 || index >= pLinkCount){
		DETHROW(deeInvalidParam);
	}
	return pLinks[index];
}

float deoglRSkyControllerTarget::GetValue(const deoglRSkyInstance &instance, float defaultValue) const{
	if(! instance.GetRSky()){
		return defaultValue;
	}
	
	const deoglRSky &sky = *instance.GetRSky();
	float value = defaultValue;
	bool firstValue = true;
	int i;
	
	for(i=0; i<pLinkCount; i++){
		if(pLinks[i] == -1){
			continue;
		}
		
		const deoglRSkyLink &link = sky.GetLinkAt(pLinks[i]);
		if(link.IsDisabled()){
			continue;
		}
		
		if(firstValue){
			value = link.GetValue(instance);
			firstValue = false;
			
		}else{
			value *= link.GetValue(instance);
		}
	}
	
	return value;
}
