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

#include "deoglVAO.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../vbo/deoglVBOLayout.h"
#include "../rendering/task/shared/deoglRenderTaskSharedPool.h"
#include "../rendering/task/shared/deoglRenderTaskSharedVAO.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTUniqueKey.h"

#include <dragengine/common/exceptions.h>



// Class deoglVAO
///////////////////

// Constructor, destructor
////////////////////////////

deoglVAO::deoglVAO(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pVAO(0),
pIndexSize(0),
pIndexGLType(GL_NONE),
pRTSVAO(NULL)
{
	OGL_CHECK(renderThread, pglGenVertexArrays(1, &pVAO));
	if(!pVAO){
		DETHROW(deeOutOfMemory);
	}
	
	pUniqueKey = renderThread.GetUniqueKey().Get();
}

deoglVAO::~deoglVAO(){
	if(pRTSVAO){
		pRTSVAO->ReturnToPool();
	}
	
	pRenderThread.GetDelayedOperations().DeleteOpenGLVertexArray(pVAO);
	pRenderThread.GetUniqueKey().Return(pUniqueKey);
}



// Management
///////////////

void deoglVAO::SetIndexType(deoglVBOLayout::eIndexTypes indexType){
	pIndexType = indexType;
	
	switch(indexType){
	case deoglVBOLayout::eitUnsignedInt:
		pIndexSize = 4;
		pIndexGLType = GL_UNSIGNED_INT;
		break;
		
	case deoglVBOLayout::eitUnsignedShort:
		pIndexSize = 2;
		pIndexGLType = GL_UNSIGNED_SHORT;
		break;
		
	case deoglVBOLayout::eitUnsignedByte:
		pIndexSize = 1;
		pIndexGLType = GL_UNSIGNED_BYTE;
		break;
		
	default:
		// the values here are chosen this way to prevent a crash if by mistake indices are used although
		// eitNone has been specified in the layout. an index size of 0 causes allocation to fail should
		// a check be missing for pIndexSize equal to 0. also GL_NONE causes rendering to fail with an
		// opengl error instead of crashing
		pIndexSize = 0;
		pIndexGLType = GL_NONE;
	}
}



void deoglVAO::EnsureRTSVAO(){
	if(pRTSVAO){
		return;
	}
	
	pRTSVAO = pRenderThread.GetRenderTaskSharedPool().GetVAO();
	pRTSVAO->SetVAO(this);
}
