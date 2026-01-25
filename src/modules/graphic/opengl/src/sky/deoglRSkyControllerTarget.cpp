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
pLinks(target.GetLinks()){
}

deoglRSkyControllerTarget::~deoglRSkyControllerTarget() = default;



// Management
///////////////

float deoglRSkyControllerTarget::GetValue(const deoglRSkyInstance &instance, float defaultValue) const{
	if(!instance.GetRSky()){
		return defaultValue;
	}
	
	const deoglRSky &sky = *instance.GetRSky();
	float value = defaultValue;
	bool firstValue = true;
	
	pLinks.Visit([&](int linkIndex){
		if(linkIndex == -1){
			return;
		}
		
		const deoglRSkyLink &link = sky.GetLinkAt(linkIndex);
		if(link.IsDisabled()){
			return;
		}
		
		if(firstValue){
			value = link.GetValue(instance);
			firstValue = false;
			
		}else{
			value *= link.GetValue(instance);
		}
	});
	
	return value;
}
