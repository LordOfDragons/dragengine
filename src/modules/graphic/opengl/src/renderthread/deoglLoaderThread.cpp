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

#include "deoglLoaderThread.h"
#include "deoglLoaderThreadTask.h"
#include "deoglRenderThread.h"
#include "deoglRTContext.h"
#include "deoglRTLogger.h"
#include "../window/deoglRRenderWindow.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/threading/deMutexGuard.h>


// Class cWaitableTask
////////////////////////

class cWaitableTask : public deoglLoaderThreadTask{
public:
	/** \brief Type holding strong reference. */
	typedef deTThreadSafeObjectReference<cWaitableTask> Ref;
	
private:
	deoglRenderThread &pRenderThread;
	const deoglLoaderThreadTask::Ref pTask;
	deSemaphore pSemaphore;
	bool pSuccess;
	
public:
	cWaitableTask(deoglRenderThread &renderThread, deoglLoaderThreadTask *task) :
	pRenderThread(renderThread),
	pTask(task),
	pSuccess(true){
	}
	
	inline deSemaphore &Semaphore(){ return pSemaphore; }
	inline bool GetSuccess() const{ return pSuccess; }
	
	void Run() override{
		try{
			pTask->Run();
			
		}catch(const deException &e){
			pSuccess = false;
			pRenderThread.GetLogger().LogException(e);
		}
		pSemaphore.Signal();
	}
};



// Class deoglLoaderThread
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglLoaderThread::deoglLoaderThread(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pShutdown(true),
pContextEnabled(false)
{
	#ifdef OS_BEOS
	SetName("OpenGL-LoaderThread");
	#endif
}

deoglLoaderThread::~deoglLoaderThread(){
}



// Management
///////////////

// #define DO_DEBUG_LOG

void deoglLoaderThread::Run(){
	OGL_INIT_LOADER_THREAD_CHECK
	pRenderThread.GetLogger().LogInfo("LoaderThread: Starting");
	try{
		pInit();
		
	}catch(const deException &e){
		pRenderThread.GetLogger().LogException(e);
		pShutdown = true;
	}
	
	deoglLoaderThreadTask::Ref task;
	
	while(!pShutdown){
		{
		const deMutexGuard guard(pMutex);
		if(pTasks.IsNotEmpty()){
			task = pTasks.First();
			pTasks.RemoveFrom(0);
		}
		}
		
		if(task){
			#ifdef DO_DEBUG_LOG
			pRenderThread.GetLogger().LogInfoFormat("LoaderThread: Run: Process task %p", task);
			#endif
			
			try{
				task->Run();
				
			}catch(const deException &e){
				pRenderThread.GetLogger().LogException(e);
			}
			
			#ifdef DO_DEBUG_LOG
			pRenderThread.GetLogger().LogInfoFormat("LoaderThread: Run: Done task %p", task);
			#endif
			task = nullptr;
			
		}else{
			try{
				#ifdef DO_DEBUG_LOG
				pRenderThread.GetLogger().LogInfo("LoaderThread: Run: Wait on semaphore");
				#endif
				
				pSemaphore.Wait();
				
				#ifdef DO_DEBUG_LOG
				pRenderThread.GetLogger().LogInfo("LoaderThread: Run: Woke up from semaphore");
				#endif
				
			}catch(const deException &e){
				pRenderThread.GetLogger().LogException(e);
			}
		}
	}
	
	try{
		pCleanUp();
		
	}catch(const deException &e){
		pRenderThread.GetLogger().LogException(e);
	}
	pRenderThread.GetLogger().LogInfo("LoaderThread: Exiting");
	OGL_EXIT_LOADER_THREAD_CHECK
}

bool deoglLoaderThread::IsEnabled(){
	const deMutexGuard guard(pMutex);
	return pContextEnabled;
}

void deoglLoaderThread::EnableContext(bool enable){
	#ifdef OS_BEOS
	return; // no context so far
	#endif
	
	if(enable == pContextEnabled){
		return;
	}
	
	if(enable){
		{
		const deMutexGuard guard(pMutex);
		pContextEnabled = true;
		}
		pShutdown = false;
		
		Start();
		
	}else{
		if(IsRunning()){
			pShutdown = true;
			pSemaphore.Signal();
		}
		WaitForExit();
		pShutdown = true;
		
		const deMutexGuard guard(pMutex);
		pContextEnabled = false;
	}
}

bool deoglLoaderThread::AddTask(const deoglLoaderThreadTask::Ref &task){
	DEASSERT_NOTNULL(task);
	
	const deMutexGuard guard(pMutex);
	if(!pContextEnabled){
		return false;
	}
	
	#ifdef DO_DEBUG_LOG
	pRenderThread.GetLogger().LogInfoFormat("LoaderThread: AddTask %p", task);
	#endif
	
	pTasks.Add(task);
	pSemaphore.Signal();
	return true;
}

bool deoglLoaderThread::AwaitTask(const deoglLoaderThreadTask::Ref &task){
	DEASSERT_NOTNULL(task)
	
	const cWaitableTask::Ref waitableTask(cWaitableTask::Ref::New(pRenderThread, task));
	
	#ifdef DO_DEBUG_LOG
	pRenderThread.GetLogger().LogInfoFormat("LoaderThread: AwaitTask: Add task %p", task);
	#endif
	
	if(!AddTask(waitableTask)){
		return false;
	}
	
	#ifdef DO_DEBUG_LOG
	pRenderThread.GetLogger().LogInfoFormat("LoaderThread: AwaitTask: Finished task %p", task);
	#endif
	waitableTask->Semaphore().Wait();
	
	DEASSERT_TRUE(waitableTask->GetSuccess())
	return true;
}



// Private Functions
//////////////////////

void deoglLoaderThread::pInit(){
	#ifndef OS_BEOS
	deoglRTContext &context = pRenderThread.GetContext();
	#endif
	
	#ifdef OS_UNIX_X11
	OGLX_CHECK(pRenderThread, glXMakeCurrent(context.GetDisplay(),
		context.GetActiveRRenderWindow()->GetWindow(), context.GetLoaderContext()));
	
#elif defined OS_ANDROID
	DEASSERT_TRUE(eglMakeCurrent(context.GetDisplay(), context.GetLoaderSurface(),
		context.GetLoaderSurface(), context.GetLoaderContext()) == EGL_TRUE)
	
#elif defined OS_WEBWASM
	DEASSERT_TRUE(emscripten_webgl_make_context_current(
		context.GetLoaderContext()) == EMSCRIPTEN_RESULT_SUCCESS)
	
#elif defined OS_MACOS
	pGLContextMakeCurrent(context.GetActiveRRenderWindow()->GetView());
	
#elif defined OS_W32
	if(!wglMakeCurrent(context.GetActiveRRenderWindow()->GetWindowDC(), context.GetLoaderContext())){
		pRenderThread.GetLogger().LogErrorFormat("wglMakeCurrent failed (%s:%i): error=0x%lx\n",
			__FILE__, __LINE__, GetLastError());
		DETHROW_INFO(deeInvalidAction, "wglMakeCurrent failed");
	}
#endif
}

void deoglLoaderThread::pCleanUp(){
#ifdef OS_UNIX_X11
	OGLX_CHECK(pRenderThread, glXMakeCurrent(pRenderThread.GetContext().GetDisplay(), None, nullptr));
	
#elif defined OS_ANDROID
	eglMakeCurrent(pRenderThread.GetContext().GetDisplay(), EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	
#elif defined OS_WEBWASM
	emscripten_webgl_make_context_current(0);
	
#elif defined OS_MACOS
	pGLContextMakeCurrent(nullptr);
	
#elif defined OS_W32
	wglMakeCurrent(NULL, NULL);
#endif
}
