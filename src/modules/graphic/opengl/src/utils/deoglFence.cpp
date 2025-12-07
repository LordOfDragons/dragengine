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

#include "deoglFence.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>


// Class deoglFence
/////////////////////

// Constructor, destructor
////////////////////////////

deoglFence::deoglFence(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pFence(nullptr){
}

deoglFence::~deoglFence(){
	pRenderThread.GetDelayedOperations().DeleteOpenGLSync(pFence);
}



// Management
///////////////

void deoglFence::Arm(){
	Reset();
	pFence = pglFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	
	// ensure the fence is processed by the GPU. AMD goes to work quickly but nVidia delays
	// processing the queue causing the signal to not be processed causing glClientWaitSync()
	// to wait forever. we could use GL_SYNC_FLUSH_COMMANDS_BIT for glClientWaitSync() but
	// according to docs it is safer to do an explicit glFlush() due to driver peculiarities
	glFlush();
}

bool deoglFence::HasFired(){
	if(! pFence){
		return true;
	}
	
	oglClearError();
	
	switch(pglClientWaitSync(pFence, 0, 0)){ // 0s timeout
	case GL_ALREADY_SIGNALED:
	case GL_CONDITION_SATISFIED:
		pFence = nullptr;
		return true;
		
	case GL_TIMEOUT_EXPIRED:
		return false;
		
	case GL_WAIT_FAILED:
		pFence = nullptr;
		dbgCheckOglError(pRenderThread, __FILE__, __LINE__);
		return true;
		
	default:
		pFence = nullptr;
		DETHROW_INFO(deeInvalidAction, "Unknown return value while waiting for fence");
	}
}

void deoglFence::Wait(){
	if(! pFence){
		return;
	}
	
	oglClearError();
	
	switch(pglClientWaitSync(pFence, 0, 1000000000)){ // 1s timeout
	case GL_ALREADY_SIGNALED:
	case GL_CONDITION_SATISFIED:
		pFence = nullptr;
		return;
		
	case GL_TIMEOUT_EXPIRED:
		pFence = nullptr;
		DETHROW_INFO(deeInvalidAction, "Timeout while waiting for fence");
		
	case GL_WAIT_FAILED:
		pFence = nullptr;
		dbgCheckOglError(pRenderThread, __FILE__, __LINE__);
		return;
		
	default:
		pFence = nullptr;
		DETHROW_INFO(deeInvalidAction, "Unknown return value while waiting for fence");
	}
}

void deoglFence::Reset(){
	if(pFence){
		pglDeleteSync(pFence);
		pFence = nullptr;
	}
}
