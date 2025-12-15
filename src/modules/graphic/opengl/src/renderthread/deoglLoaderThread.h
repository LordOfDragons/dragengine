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

#ifndef _DEOGLRLOADERTHREAD_H_
#define _DEOGLRLOADERTHREAD_H_

#include "deoglLoaderThreadTask.h"
#include "../deoglBasics.h"

#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/threading/deMutex.h>
#include <dragengine/threading/deSemaphore.h>
#include <dragengine/threading/deThread.h>

class deoglRenderThread;


/**
 * Loader thread. Handles loading and creating object objects outside of the render thread.
 */
class deoglLoaderThread : public deThread{
private:
	deoglRenderThread &pRenderThread;
	
	deSemaphore pSemaphore;
	deMutex pMutex;
	bool pShutdown;
	bool pContextEnabled;
	
	decTThreadSafeObjectOrderedSet<deoglLoaderThreadTask> pTasks;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create loader thread. */
	deoglLoaderThread(deoglRenderThread &renderThread);
	
	/** Clean up loader thread. */
	virtual ~deoglLoaderThread();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Run render thread. */
	virtual void Run();
	
	/** Loader is enabled. */
	bool IsEnabled();
	
	/** Enable context. */
	void EnableContext(bool enable);
	
	/** Add task to process. Returns true if added or false if loader is disabled. */
	bool AddTask(const deoglLoaderThreadTask::Ref &task);
	
	/**
	 * Add task and wait for it to be processed then returns.
	 * Returns true if process or false if loader is disabled.
	 */
	bool AwaitTask(const deoglLoaderThreadTask::Ref &task);
	/*@}*/
	
	
	
private:
	void pInit();
	void pCleanUp();
};

#endif
