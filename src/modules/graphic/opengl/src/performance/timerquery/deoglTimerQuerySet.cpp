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

#include "deoglTimerQuerySet.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>



// Class deoglTimerQuerySet
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglTimerQuerySet::deoglTimerQuerySet(deoglRenderThread &renderthread) :
pRenderThread(renderthread){
}

deoglTimerQuerySet::~deoglTimerQuerySet(){
	deoglDelayedOperations &dops = pRenderThread.GetDelayedOperations();
	pQueries.Visit([&](GLuint query){
		dops.DeleteOpenGLQuery(query);
	});
}



// Management
///////////////

void deoglTimerQuerySet::SetQueryCount(int count){
	DEASSERT_TRUE(count >= 0)
	
	if(count == pQueries.GetCount()){
		return;
	}
	
	if(pQueries.IsNotEmpty()){
		pglDeleteQueries(pQueries.GetCount(), pQueries.GetArrayPointer());
		pQueries.RemoveAll();
	}
	
	if(count > 0){
		pQueries.SetAll(count, 0);
		OGL_CHECK(pRenderThread, pglGenQueries(count, pQueries.GetArrayPointer()));
	}
}

void deoglTimerQuerySet::BeginQuery(int index){
	OGL_CHECK(pRenderThread, pglBeginQuery(GL_TIME_ELAPSED, pQueries[index]));
}

void deoglTimerQuerySet::EndQuery(){
	OGL_CHECK(pRenderThread, pglEndQuery(GL_TIME_ELAPSED));
}

bool deoglTimerQuerySet::HasResult(int index) const{
	GLuint result;
	OGL_CHECK(pRenderThread, pglGetQueryObjectuiv(pQueries[index], GL_QUERY_RESULT_AVAILABLE, &result));
	
	return result == GL_TRUE;
}

unsigned int deoglTimerQuerySet::GetResult(int index) const{
	GLuint result;
	OGL_CHECK(pRenderThread, pglGetQueryObjectuiv(pQueries[index], GL_QUERY_RESULT, &result));
	
	return (unsigned int)result;
}
