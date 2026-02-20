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

#include "deoalRayTraceResult.h"
#include "deoalRayTraceHitElement.h"
#include "../../audiothread/deoalAudioThread.h"
#include "../../audiothread/deoalATLogger.h"
#include "../../component/deoalAComponent.h"
#include "../../model/deoalAModel.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModel.h>



// Class deoalRayTraceResult
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalRayTraceResult::deoalRayTraceResult(){
	pElements.EnlargeCapacity(20);
}

deoalRayTraceResult::~deoalRayTraceResult() = default;



// Manegement
///////////////

void deoalRayTraceResult::Clear(){
	pElements.RemoveAll();
}

void deoalRayTraceResult::AddElement(float distance, const decDVector &point,
const decDVector &normal, deoalAComponent *component, int face, bool frontFacing){
	deoalRayTraceHitElement * const element = pInsert(distance);
	if(element){
		element->SetComponentFace(distance, point, normal, component, face, frontFacing);
	}
}

void deoalRayTraceResult::AddElement(const deoalRayTraceHitElement &element){
	deoalRayTraceHitElement * const insertedElement = pInsert(element.GetDistance());
	if(insertedElement){
		insertedElement->SetComponentFace(element.GetDistance(), element.GetPoint(),
			element.GetNormal(), element.GetComponent(), element.GetComponentFace(),
			element.GetForwardFacing());
	}
}

void deoalRayTraceResult::DebugPrint(deoalAudioThread &audioThread, const char *prefix){
	deoalATLogger &logger = audioThread.GetLogger();
	
	logger.LogInfoFormat("%s) Ray-Trace Result (%d elements):", prefix, pElements.GetCount());
	pElements.Visit([&](const deoalRayTraceHitElement &element){
		const decDVector &point = element.GetPoint();
		const float distance = element.GetDistance();
		
		if(element.GetComponent()){
			const deoalAComponent &component = *element.GetComponent();
			const decDVector &compPos = component.GetPosition();
			const char * const model = component.GetModel() ? component.GetModel()->GetFilename().GetString() : "-";
			const int face = element.GetComponentFace();
			const bool frontFacing = element.GetForwardFacing();
			
			logger.LogInfoFormat("%s) - distance=%.3f point=(%.3f,%.3f,%.3f) "
				"component=(%.3f,%.3f,%.3f) model=%s face=%d frontFacing=%d",
				prefix, distance, point.x, point.y, point.z, compPos.x, compPos.y, compPos.z,
				model, face, frontFacing);
			
		}else{
			logger.LogInfoFormat("%s) - distance=%.3f point=(%.3f,%.3f,%.3f) ??",
				prefix, distance, point.x, point.y, point.z);
		}
	});
}



// Protected functions
////////////////////////

deoalRayTraceHitElement *deoalRayTraceResult::pInsert(float distance){
	if(pElements.IsEmpty()){
		pElements.Add({});
		return &pElements.Last();
	}
	
	int end = pElements.GetCount() - 1;
	int begin = 0;
	
	while(begin < end){
		const int center = (begin + end) / 2;
		
		if(distance < pElements[center].GetDistance()){
			end = center - 1;
			
		}else{
			begin = center + 1;
		}
	}
	
	if(distance >= pElements[begin].GetDistance()){
		begin++;
	}
	if(begin == pElements.GetCount()){
		return nullptr; // element to insert drops out
	}
	
	if(pElements.GetCount() == pElements.GetCapacity()){
		pElements.RemoveFrom(pElements.GetCount() - 1);
	}
	
	pElements.Insert({}, begin);
	return &pElements[begin];
}
