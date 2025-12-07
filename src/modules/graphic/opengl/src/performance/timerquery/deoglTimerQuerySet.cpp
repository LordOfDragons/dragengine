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

#include "deoglTimerQuerySet.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>



// Class deoglTimerQuerySet
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglTimerQuerySet::deoglTimerQuerySet(deoglRenderThread &renderthread) :
pRenderThread(renderthread),
pQueries(NULL),
pQueryCount(0){
}

deoglTimerQuerySet::~deoglTimerQuerySet(){
	if(pQueries){
		deoglDelayedOperations &dops = pRenderThread.GetDelayedOperations();
		int i;
		for(i=0; i<pQueryCount; i++){
			dops.DeleteOpenGLQuery(pQueries[i]);
		}
		
		delete [] pQueries;
		pQueries = NULL;
	}
}



// Management
///////////////

void deoglTimerQuerySet::SetQueryCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count != pQueryCount){
		if(pQueryCount > 0){
			pglDeleteQueries(pQueryCount, pQueries);
			pQueryCount = 0;
		}
		if(pQueries){
			delete [] pQueries;
			pQueries = NULL;
		}
		
		if(count > 0){
			pQueries = new GLuint[count];
			
			OGL_CHECK(pRenderThread, pglGenQueries(count, pQueries));
			
			for(pQueryCount=0; pQueryCount<count; pQueryCount++){
				if(! pQueries[pQueryCount]){
					DETHROW(deeOutOfMemory);
				}
			}
		}
	}
}

void deoglTimerQuerySet::BeginQuery(int index){
	if(index < 0 || index >= pQueryCount){
		DETHROW(deeInvalidParam);
	}
	
	OGL_CHECK(pRenderThread, pglBeginQuery(GL_TIME_ELAPSED, pQueries[index]));
}

void deoglTimerQuerySet::EndQuery(){
	OGL_CHECK(pRenderThread, pglEndQuery(GL_TIME_ELAPSED));
}

bool deoglTimerQuerySet::HasResult(int index) const{
	if(index < 0 || index >= pQueryCount){
		DETHROW(deeInvalidParam);
	}
	
	GLuint result;
	
	OGL_CHECK(pRenderThread, pglGetQueryObjectuiv(pQueries[index], GL_QUERY_RESULT_AVAILABLE, &result));
	
	return result == GL_TRUE;
}

unsigned int deoglTimerQuerySet::GetResult(int index) const{
	if(index < 0 || index >= pQueryCount){
		DETHROW(deeInvalidParam);
	}
	
	GLuint result;
	
	OGL_CHECK(pRenderThread, pglGetQueryObjectuiv(pQueries[index], GL_QUERY_RESULT, &result));
	
	return (unsigned int)result;
}
