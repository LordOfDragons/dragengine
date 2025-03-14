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

#include "deoglBatchedShaderLoading.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/threading/deMutexGuard.h>


// Class deoglBatchedShaderLoading::cBaseGetProgramListener
/////////////////////////////////////////////////////////////

deoglBatchedShaderLoading::cBaseGetProgramListener::cBaseGetProgramListener(
	deoglBatchedShaderLoading &batched, const deoglPipelineConfiguration &config) :
pBatched(batched),
pConfig(config){
}


// Class deoglBatchedShaderLoading
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglBatchedShaderLoading::deoglBatchedShaderLoading(
	deoglRenderThread &renderThread, float timeout, bool asyncCompile) :
pRenderThread(renderThread),
pInitialTimeout(timeout),
pTimeout(timeout),
pFirst(true),
pTimedOut(false),
pAsyncCompile(asyncCompile),
pPendingCompiles(0),
pFailedCompiles(0){
}

deoglBatchedShaderLoading::~deoglBatchedShaderLoading(){
}




// Management
///////////////
bool deoglBatchedShaderLoading::CanLoad(){
	if(pFirst || pAsyncCompile){
		return true;
	}
	if(pTimedOut){
		return false;
	}
	
	pTimeout -= pTimer.GetElapsedTime();
	pTimedOut = pTimeout <= 0.0f;
	return !pTimedOut;
}

void deoglBatchedShaderLoading::Loaded(){
	pFirst = false;
}

void deoglBatchedShaderLoading::AddPendingCompile(){
	const deMutexGuard guard(pMutexCompiles);
	pPendingCompiles++;
}

void deoglBatchedShaderLoading::WaitAllCompileFinished(){
	if(!pAsyncCompile){
		return;
	}
	
	while(true){
		{
		const deMutexGuard guard(pMutexCompiles);
		if(pPendingCompiles == 0){
			DEASSERT_TRUE(pFailedCompiles == 0)
			return;
		}
		}
		pSemaphoreCompileFinished.Wait();
	}
}

void deoglBatchedShaderLoading::FinishCompile(bool success){
	const deMutexGuard guard(pMutexCompiles);
	if(!success){
		pFailedCompiles++;
	}
	
	pPendingCompiles--;
	
	if(pPendingCompiles == 0){
		pSemaphoreCompileFinished.SignalAll();
	}
}
