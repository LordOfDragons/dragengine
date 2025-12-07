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

#include "../deoglBasics.h"

#include "deoglRTLeakTracker.h"
#include "deoglRenderThread.h"
#include "deoglRTLogger.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/deObject.h>



// Class deoglRTLeakTracker
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTLeakTracker::deoglRTLeakTracker(deoglRenderThread &renderThread) :
pRenderThread(renderThread){
}

deoglRTLeakTracker::~deoglRTLeakTracker(){
}



// Management
///////////////

void deoglRTLeakTracker::ReportLeaks(const char *name, const decPointerSet &tracker){
	pMutex.Lock();
	
	if(tracker.GetCount() > 0){
		try{
			deoglRTLogger &logger = pRenderThread.GetLogger();
			const int count = tracker.GetCount();
			const int entriesPerLine = 8; //10
			decString line;
			int i;
			
			logger.LogInfoFormat("%i Leaks for %s", count, name);
			for(i=0; i<count; i++){
				line.AppendFormat(" %p(%i)", tracker.GetAt(i), ((deObject*)tracker.GetAt(i))->GetRefCount());
				if(((i + 1) % entriesPerLine) == 0){
					logger.LogInfo(line);
					line.Empty();
				}
			}
			
			if(! line.IsEmpty()){
				logger.LogInfo(line);
			}
			
		}catch(const deException &){
			pMutex.Unlock();
			throw;
		}
	}
	
	pMutex.Unlock();
}



#ifdef ENABLE_LEAK_TRACKING
void deoglRTLeakTracker::AddTracked(decPointerSet &list, void *object){
	pMutex.Lock();
	
	try{
		list.Add(object);
		
	}catch(const deException &){
		pMutex.Unlock();
		throw;
	}
	
	pMutex.Unlock();
}

void deoglRTLeakTracker::RemoveTracked(decPointerSet &list, void *object) {
	pMutex.Lock();
	
	try{
		list.Remove(object);
		
	}catch(const deException &){
		pMutex.Unlock();
		throw;
	}
	
	pMutex.Unlock();
}
#endif
