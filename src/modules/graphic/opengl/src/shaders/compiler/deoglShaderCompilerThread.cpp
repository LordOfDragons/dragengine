/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deoglShaderCompiler.h"
#include "deoglShaderCompilerThread.h"
#include "deoglShaderCompileTask.h"
#include "../deoglShaderLanguage.h"
#include "../deoglShaderProgram.h"
#include "../../extensions/deoglExtensions.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../renderthread/deoglRTContext.h"
#include "../../window/deoglRRenderWindow.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/threading/deMutexGuard.h>


// Class deoglShaderCompilerThread
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderCompilerThread::deoglShaderCompilerThread(
	deoglShaderLanguage &language, int contextIndex) :
pLanguage(language),
pContextIndex(contextIndex),
pCompiler(nullptr),
pExitThread(false),
pState(State::prepare)
{
	language.GetRenderThread().GetLogger().LogInfoFormat(
		"Create shader compiler thread: %d", contextIndex);
	
	try{
		pCompiler = new deoglShaderCompiler(language, contextIndex);
		
	}catch(const deException &e){
		language.GetRenderThread().GetLogger().LogException(e);
		pExitThread = false;
		pState = State::failed;
	}
}

deoglShaderCompilerThread::~deoglShaderCompilerThread(){
	pCleanUp();
}



// Management
///////////////

void deoglShaderCompilerThread::Run(){
	try{
		pActivateContext();
		
	}catch(const deException &e){
		pLanguage.GetRenderThread().GetLogger().LogException(e);
		const deMutexGuard guard(GetMutexState());
		pExitThread = false;
		pState = State::failed;
		return;
	}
	
	{
	const deMutexGuard guard(GetMutexState());
	pState = State::ready;
	}
	
	deoglShaderCompileTask::Ref task;
	while(true){
		if(pExitThreadRequested()){
			break;
		}
		
		pLanguage.GetNextTask(task);
		
		if(task){
			try{
				const decString &cacheId = ((deoglShaderCompileTask*)task)->GetProgram()->GetCacheId();
				pLanguage.GetRenderThread().GetLogger().LogInfoFormat(
					"CompileThread %d: Start task '%.50s...'", pContextIndex, cacheId.GetString());
				task->SetCompiled(pCompiler->CompileShader(*task->GetProgram()));
				pLanguage.GetRenderThread().GetLogger().LogInfoFormat(
					"CompileThread %d: Stop task '%.50s...'", pContextIndex, cacheId.GetString());
				
			}catch(const deException &e){
				task->SetCompiled(nullptr); // to be on the safe side
			}
			
			pLanguage.FinishTask(task);
			
		}else{
			if(pExitThreadRequested()){
				break;
			}
			
			pLanguage.WaitForNewTasks();
		}
	}
}

void deoglShaderCompilerThread::RequestExit(){
	const deMutexGuard guard(GetMutexState());
	pExitThread = true;
}

deoglShaderCompilerThread::State deoglShaderCompilerThread::GetState(){
	const deMutexGuard guard(GetMutexState());
	return pState;
}


// Private Functions
//////////////////////

void deoglShaderCompilerThread::pCleanUp(){
	if(pCompiler){
		delete pCompiler;
	}
	
#ifdef OS_ANDROID
	eglMakeCurrent(pLanguage.GetRenderThread().GetContext().GetDisplay(),
		EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	
#elif defined OS_BEOS
	
#elif defined OS_MACOS
	pGLContextMakeCurrent(nullptr);
	
#elif defined OS_W32
	wglMakeCurrent(NULL, NULL);
	
#elif defined OS_UNIX
	pLanguage.GetRenderThread(), glXMakeCurrent(
		pLanguage.GetRenderThread().GetContext().GetDisplay(), None, nullptr);
#endif
}

void deoglShaderCompilerThread::pActivateContext(){
	if(pContextIndex > -1){
#ifndef OS_BEOS
		deoglRTContext &context = pLanguage.GetRenderThread().GetContext();
#endif
		
#ifdef OS_ANDROID
		DEASSERT_TRUE(eglMakeCurrent(context.GetDisplay(),
			context.GetCompilerSurfaceAt(pContextIndex),
			context.GetCompilerSurfaceAt(pContextIndex),
			context.GetCompileContextAt(pContextIndex)) == EGL_TRUE)
			
#elif defined OS_BEOS
		
#elif defined OS_MACOS
		DETHROW_INFO(deeInvalidAction, "how to do this?")
		//pGLContextMakeCurrent(context.GetActiveRRenderWindow()->GetView());
			
#elif defined OS_W32
		if(!wglMakeCurrent(context.GetActiveRRenderWindow()->GetWindowDC(),
		context.GetCompileContextAt(pContextIndex))){
			pLanguage.GetRenderThread().GetLogger().LogErrorFormat(
				"wglMakeCurrent failed (%s:%i): error=0x%lx\n",
				__FILE__, __LINE__, GetLastError());
			DETHROW_INFO(deeInvalidAction, "wglMakeCurrent failed");
		}
		
#elif defined OS_UNIX
		OGL_CHECK_WRTC(pLanguage.GetRenderThread(), pContextIndex == -1,
			glXMakeCurrent(context.GetDisplay(),
			context.GetActiveRRenderWindow()->GetWindow(),
			context.GetCompileContextAt(pContextIndex)));
#endif
	}
	
#ifdef OS_BEOS
	decString threadName;
	if(contextIndex > -1){
		threadName.Format("OGL-ShaderCompiler-%d", contextIndex)
	}else{
		threadName = "OGL-ShaderCompiler")
	}
	SetName(threadName);
#endif
	
	pCompiler = new deoglShaderCompiler(pLanguage, pContextIndex);
}

bool deoglShaderCompilerThread::pExitThreadRequested(){
	const deMutexGuard guard(GetMutexState());
	return pExitThread;
}
