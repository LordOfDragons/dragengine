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

#include "deoglTimerQuery.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>



// Class deoglTimerQuery
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglTimerQuery::deoglTimerQuery(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pQuery(0)
{
	OGL_CHECK(renderThread, pglGenQueries(1, &pQuery));
}

deoglTimerQuery::~deoglTimerQuery(){
	pRenderThread.GetDelayedOperations().DeleteOpenGLQuery(pQuery);
}



// Management
///////////////

void deoglTimerQuery::BeginQuery(){
	OGL_CHECK(pRenderThread, pglBeginQuery(GL_TIME_ELAPSED, pQuery));
}

void deoglTimerQuery::EndQuery(){
	OGL_CHECK(pRenderThread, pglEndQuery(GL_TIME_ELAPSED));
}

bool deoglTimerQuery::HasResult() const{
	GLuint result;
	
	OGL_CHECK(pRenderThread, pglGetQueryObjectuiv(pQuery, GL_QUERY_RESULT_AVAILABLE, &result));
	
	return result == GL_TRUE;
}

unsigned int deoglTimerQuery::GetResult() const{
	GLuint result;
	
	OGL_CHECK(pRenderThread, pglGetQueryObjectuiv(pQuery, GL_QUERY_RESULT, &result));
	
	return (unsigned int)result;
}
