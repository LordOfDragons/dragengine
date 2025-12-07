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

#include "../deoalBasics.h"

#include "deoalATLeakTracker.h"
#include "deoalAudioThread.h"
#include "deoalATLogger.h"

#include <dragengine/deObject.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/threading/deMutexGuard.h>



// Class deoalATLeakTracker
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoalATLeakTracker::deoalATLeakTracker(deoalAudioThread &audioThread) :
pAudioThread(audioThread){
}

deoalATLeakTracker::~deoalATLeakTracker(){
}



// Management
///////////////

void deoalATLeakTracker::ReportLeaks(const char *name, const decPointerSet &tracker){
	deMutexGuard lock(pMutex);
	
	if(tracker.GetCount() == 0){
		return;
	}
	
	deoalATLogger &logger = pAudioThread.GetLogger();
	const int count = tracker.GetCount();
	const int entriesPerLine = 8; //10
	decString line;
	int i;
	
	logger.LogInfoFormat("%i Leaks for %s", count, name);
	for(i=0; i<count; i++){
		line.AppendFormat(" %p(%i)", tracker.GetAt(i),
			((deObject*)tracker.GetAt(i))->GetRefCount());
		
		if(((i + 1) % entriesPerLine) == 0){
			logger.LogInfo(line);
			line.Empty();
		}
	}
	
	if(! line.IsEmpty()){
		logger.LogInfo(line);
	}
}



#ifdef ENABLE_LEAK_TRACKING
void deoalATLeakTracker::AddTracked(decPointerSet &list, void *object){
	deMutexGuard lock(pMutex);
	list.Add(object);
}

void deoalATLeakTracker::RemoveTracked(decPointerSet &list, void *object) {
	deMutexGuard lock(pMutex);
	list.Remove(object);
}
#endif
