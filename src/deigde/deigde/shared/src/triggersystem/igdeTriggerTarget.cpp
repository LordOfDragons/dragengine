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

#include "igdeTriggerTarget.h"
#include "igdeTriggerListener.h"

#include <dragengine/common/exceptions.h>



// Class igdeTriggerTarget
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeTriggerTarget::igdeTriggerTarget(const char *name){
	if(!name){
		DETHROW(deeInvalidParam);
	}
	
	pName = name;
	pFired = false;
	pHasFired = false;
}

igdeTriggerTarget::~igdeTriggerTarget(){
	pListeners.RemoveAll();
}



// Management
///////////////

void igdeTriggerTarget::SetFired(bool fired){
	pFired = fired;
}

void igdeTriggerTarget::SetHasFired(bool hasFired){
	pHasFired = hasFired;
}

void igdeTriggerTarget::Fire(){
	if(!pFired){
		pFired = true;
		pHasFired = true;
		NotifyListeners();
	}
}

void igdeTriggerTarget::Reset(){
	if(pFired){
		pFired = false;
		NotifyListeners();
	}
}

void igdeTriggerTarget::FullReset(){
	if(pHasFired || pFired){
		pHasFired = false;
		pFired = false;
		NotifyListeners();
	}
}



void igdeTriggerTarget::AddListener(igdeTriggerListener *listener){
	if(!listener){
		DETHROW(deeInvalidParam);
	}
	
	pListeners.AddIfAbsent(listener);
}

void igdeTriggerTarget::RemoveListener(igdeTriggerListener *listener){
	pListeners.RemoveIfPresent(listener);
}

void igdeTriggerTarget::NotifyListeners(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((igdeTriggerListener*)pListeners.GetAt(i))->TriggerTargetChanged(this);
	}
}
