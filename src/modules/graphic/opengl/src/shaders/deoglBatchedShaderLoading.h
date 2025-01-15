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

#ifndef _DEOGLBATCHEDSHADERLOADING_H_
#define _DEOGLBATCHEDSHADERLOADING_H_

#include "deoglShaderManager.h"
#include "../pipeline/deoglPipelineConfiguration.h"

#include <dragengine/common/utils/decTimer.h>
#include <dragengine/threading/deMutex.h>
#include <dragengine/threading/deSemaphore.h>

class deoglRenderThread;
class deoglPipeline;


/**
 * Batch shader loading.
 * 
 * Top level object creates instance on stack and hands down the reference.
 * 
 * \par Timeout Handling
 * 
 * The lowest level shader loading code has to first call CanLoad() to check if the
 * timeout elapsed. This function returns true if either this is the first shader to
 * be loaded or the timeout has not elapsed. This ensures at least one shader is
 * loaded without the timeout being checked.
 * 
 * After loading the shader the lowest level shader loading code has to call Loaded().
 * This clears the first flag. At this point TimedOut() still returns true. This
 * ensures loading one shader can not cause a timeout to be noticed up the call chain.
 * If this is not done functions up the call chain can potentially exit and another
 * try is done at creating shaders.
 * 
 * If later a lowest level shader loader calls CanLoad() and the timeout has elapsed
 * the TimedOut() function will return true. This way a timeout is noticed after
 * the first shader loading that has been prevented due to the timeout after at least
 * one shader has been loaded. This ensure loading never can stall because at least
 * one shader is loaded and no timeout is noticed then.
 */
class deoglBatchedShaderLoading{
public:
	class cBaseGetProgramListener : public deoglShaderManager::cGetProgramListener{
	protected:
		deoglBatchedShaderLoading &pBatched;
		deoglPipelineConfiguration pConfig;
		
	public:
		cBaseGetProgramListener(deoglBatchedShaderLoading &batched,
			const deoglPipelineConfiguration &config);
	};
	
	
private:
	deoglRenderThread &pRenderThread;
	
	decTimer pTimer;
	float pInitialTimeout;
	float pTimeout;
	bool pFirst;
	bool pTimedOut;
	
	bool pAsyncCompile;
	
	int pPendingCompiles, pFailedCompiles;
	deMutex pMutexCompiles;
	deSemaphore pSemaphoreCompileFinished;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shader loading timeout. */
	deoglBatchedShaderLoading(deoglRenderThread &renderThread, float timeout, bool asyncCompile);
	
	/** Clean up shader loading timeout. */
	~deoglBatchedShaderLoading();
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Initial timeout. */
	inline float GetInitialTimeout() const{ return pInitialTimeout; }
	
	/** Remaining timeout. */
	inline float GetTimeout() const{ return pTimeout; }
	
	/** Timeout elapsed. */
	inline bool TimedOut() const{ return pTimedOut; }
	
	/** Timeout not elapsed. */
	inline bool NotTimedOut() const{ return !pTimedOut; }
	
	/** Use asynchronous compile. */
	inline bool GetAsyncCompile() const{ return pAsyncCompile; }
	
	/** First. */
	inline bool First() const{ return pFirst; }
	
	/** Check if shader can be loaded. If false is returned exit. */
	bool CanLoad();
	
	/** Shader loaded. */
	void Loaded();
	
	
	/** Increment pending compile by one. */
	void AddPendingCompile();
	
	/**
	 * Wait until all compile have finished or async is false.
	 * Throws exception if any compile failed.
	 */
	void WaitAllCompileFinished();
	
	/**
	 * Finish pending compile. Decrements pending compile count by one. If success is false
	 * also increments failed compile count by one.
	 */
	void FinishCompile(bool success);
	/*@}*/
};

#endif
