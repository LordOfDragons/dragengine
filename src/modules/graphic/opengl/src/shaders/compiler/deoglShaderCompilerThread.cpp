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
#include "deoglShaderCompileUnitTask.h"
#include "../deoglShaderLanguage.h"
#include "../deoglShaderProgram.h"
#include "../deoglShaderProgramUnit.h"
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

deoglShaderCompilerThread::deoglShaderCompilerThread(deoglShaderLanguage &language,
	int contextIndex, Type type) :
pLanguage(language),
pContextIndex(contextIndex),
pType(type),
pCompiler(nullptr),
pExitThread(false),
pState(State::prepare)
#ifdef OS_UNIX_X11
, pDisplay(None)
#endif
{
	const char *typeName;
	switch(type){
	case Type::single:
		typeName = "single";
		break;
		
	case Type::glparallel:
		typeName ="glparallel";
		break;
		
	default:
		typeName = "?";
	}
	
	language.GetRenderThread().GetLogger().LogInfoFormat(
		"Create shader compiler thread: %d (%s)", contextIndex, typeName);
	
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
	
	switch(pType){
	case Type::single:
		pRunSingle();
		break;
		
	case Type::glparallel:
		pRunGLParallel();
		break;
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
	
#elif defined OS_WEBWASM
	emscripten_webgl_make_context_current(0);
	
#elif defined OS_MACOS
	pGLContextMakeCurrent(nullptr);
	
#elif defined OS_W32
	wglMakeCurrent(NULL, NULL);
	
#elif defined OS_UNIX_X11
	if(pDisplay){
		glXMakeCurrent(pDisplay, None, nullptr);
		XCloseDisplay(pDisplay);
	}
	// glXMakeCurrent(pLanguage.GetRenderThread().GetContext().GetDisplay(), None, nullptr);
#endif
}

void deoglShaderCompilerThread::pActivateContext(){
	if(pContextIndex > -1){
#ifndef OS_BEOS
		deoglRTContext &context = pLanguage.GetRenderThread().GetContext();
#endif
		
#ifdef OS_ANDROID
		DEASSERT_TRUE(eglMakeCurrent(context.GetDisplay(),
			context.GetCompileSurfaceAt(pContextIndex),
			context.GetCompileSurfaceAt(pContextIndex),
			context.GetCompileContextAt(pContextIndex)) == EGL_TRUE)
		
#elif defined OS_WEBWASM
		DEASSERT_TRUE(emscripten_webgl_make_context_current(
			context.GetCompileContextAt(pContextIndex)) == EMSCRIPTEN_RESULT_SUCCESS)
		
#elif defined OS_MACOS
		DETHROW_INFO(deeInvalidAction, "how to do this?")
		//pGLContextMakeCurrent(context.GetActiveRRenderWindow()->GetView());
			
#elif defined OS_W32
		// on windows make current can randomly fail at times. in this case
		// retry the make current call after a short wait to see if it fixes
		// itself. if this goes on for too lang call it quits
		const int retryCounterMax = 10;
		const int retryDelayMs = 100;
		int retryCounter;

		for(retryCounter=0; retryCounter<retryCounterMax; retryCounter++){
			if(wglMakeCurrent(context.GetActiveRRenderWindow()->GetWindowDC(),
			context.GetCompileContextAt(pContextIndex))){
				if(retryCounter > 0){
					pLanguage.GetRenderThread().GetLogger().LogErrorFormat(
						"wglMakeCurrent recovered (%s:%i)[%d]: counter=%d\n",
						__FILE__, __LINE__, pContextIndex, retryCounter);
				}
				break;
			}

			pLanguage.GetRenderThread().GetLogger().LogErrorFormat(
				"wglMakeCurrent failed (%s:%i)[%d]: error=0x%lx, counter=%d\n",
				__FILE__, __LINE__, pContextIndex, GetLastError(), retryCounter);

			SleepEx(retryDelayMs, false);
		}

		if(retryCounter == retryCounterMax){
			DETHROW_INFO(deeInvalidAction, "wglMakeCurrent failed");
		}

#elif defined OS_UNIX_X11
		// on nVidia there can be strange segfaults if using context display connection.
		// to be on the safe side use a unique display connection.
		const char *dispName = getenv("DISPLAY");
		if(!dispName){
			dispName = "";
		}
		pDisplay = XOpenDisplay(dispName);
		DEASSERT_NOTNULL(pDisplay)

		// on nVidia if glXMakeCurrent fails and glGetError is called this can cause a segfault.
		// as a work around for this problem use the return code instead.
		DEASSERT_TRUE(glXMakeCurrent(/*context.GetDisplay()*/ pDisplay,
			context.GetActiveRRenderWindow()->GetWindow(),
			context.GetCompileContextAt(pContextIndex)))

		/*
		OGL_CHECK_WRTC(pLanguage.GetRenderThread(), pContextIndex == -1,
			glXMakeCurrent(context.GetDisplay(),
			context.GetActiveRRenderWindow()->GetWindow(),
			context.GetCompileContextAt(pContextIndex)));
		*/
#endif
	}
	
#ifdef OS_BEOS
	decString threadName;
	if(pContextIndex > -1){
		threadName.Format("OGL-ShaderCompiler-%d", pContextIndex);
	}else{
		threadName = "OGL-ShaderCompiler";
	}
	SetName(threadName);
#endif
	
	pCompiler = new deoglShaderCompiler(pLanguage, pContextIndex);
}

bool deoglShaderCompilerThread::pExitThreadRequested(){
	const deMutexGuard guard(GetMutexState());
	return pExitThread;
}

void deoglShaderCompilerThread::pRunSingle(){
	while(true){
		if(pExitThreadRequested()){
			break;
		}
		
		deoglShaderCompileTask::Ref task;
		deoglShaderCompileUnitTask::Ref unitTask;
		pLanguage.GetNextTask(task, unitTask);
		
		if(unitTask){
			deoglShaderProgramUnit &unit = *unitTask->GetUnit();
			try{
				//const decString &cacheId = unit.GetCacheId();
				//pLanguage.GetRenderThread().GetLogger().LogInfoFormat(
				//	"CompileThread %d: Start task unit '%.50s...'", pContextIndex, cacheId.GetString());
				pCompiler->CompileShaderUnit(unit);
				pCompiler->FinishCompileShaderUnit(unit);
				//pLanguage.GetRenderThread().GetLogger().LogInfoFormat(
				//	"CompileThread %d: Stop task unit '%.50s...'", pContextIndex, cacheId.GetString());
				
			}catch(const deException &){
				// just in case...
				unit.compilingFailed = true;
				unit.DropHandle();
			}
			
			pLanguage.FinishTask(unitTask);
			
		}else if(task){
			deoglShaderProgram &program = *task->GetProgram();
			try{
				//const decString &cacheId = program.GetCacheId();
				//pLanguage.GetRenderThread().GetLogger().LogInfoFormat(
				//	"CompileThread %d: Start task '%.50s...'", pContextIndex, cacheId.GetString());
				pCompiler->CompileShader(program);
				pCompiler->FinishCompileShader(program);
				//pLanguage.GetRenderThread().GetLogger().LogInfoFormat(
				//	"CompileThread %d: Stop task '%.50s...'", pContextIndex, cacheId.GetString());
				
			}catch(const deException &){
				program.SetCompiled(nullptr);
				program.ready = false;
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

void deoglShaderCompilerThread::pRunGLParallel(){
	decObjectList tasks, unitTasks;
	int i, count;
	
	while(true){
		if(pExitThreadRequested()){
			break;
		}
		
		// start compiling all available unit tasks and tasks and store
		while(true){
			deoglShaderCompileTask::Ref task;
			deoglShaderCompileUnitTask::Ref unitTask;
			pLanguage.GetNextTask(task, unitTask);
			
			if(unitTask){
				deoglShaderProgramUnit &unit = *unitTask->GetUnit();
				try{
					//const decString &cacheId = unit.GetCacheId();
					//pLanguage.GetRenderThread().GetLogger().LogInfoFormat(
					//	"CompileThread %d: Start task unit '%.50s...'", pContextIndex, cacheId.GetString());
					pCompiler->CompileShaderUnit(unit);
					unitTasks.Add(unitTask);
					
				}catch(const deException &){
					// just in case...
					unit.compilingFailed = true;
					unit.DropHandle();
					pLanguage.FinishTask(unitTask);
				}
				
			}else if(task){
				deoglShaderProgram &program = *task->GetProgram();
				try{
					//const decString &cacheId = program.GetCacheId();
					//pLanguage.GetRenderThread().GetLogger().LogInfoFormat(
					//	"CompileThread %d: Start task '%.50s...'", pContextIndex, cacheId.GetString());
					pCompiler->CompileShader(program);
					tasks.Add(task);
					
				}catch(const deException &){
					program.SetCompiled(nullptr);
					program.ready = false;
					pLanguage.FinishTask(task);
				}
				
			}else{
				break;
			}
		}
		
		if(pExitThreadRequested()){
			break;
		}
		
		// if no unit tasks or tasks are waiting to finish block on the new task semaphore
		if(unitTasks.GetCount() == 0 && tasks.GetCount() == 0){
			pLanguage.WaitForNewTasks();
			continue;
		}
		
		// process all finished unit tasks
		count = unitTasks.GetCount();
		for(i=0; i<count; i++){
			deoglShaderCompileUnitTask * const unitTask =
				(deoglShaderCompileUnitTask*)unitTasks.GetAt(i);
			deoglShaderProgramUnit &unit = *unitTask->GetUnit();
			
			if(!pCompiler->HasCompileShaderUnitFinished(unit)){
				continue;
			}
			
			try{
				pCompiler->FinishCompileShaderUnit(unit);
				//pLanguage.GetRenderThread().GetLogger().LogInfoFormat(
				//	"CompileThread %d: Stop task unit '%.50s...'", pContextIndex, cacheId.GetString());
				
			}catch(const deException &){
				// just in case...
				unit.compilingFailed = true;
				unit.DropHandle();
			}
			
			deoglShaderCompileUnitTask::Ref guardUnitTask(unitTask);
			unitTasks.RemoveFrom(i);
			i--;
			count--;
			pLanguage.FinishTask(guardUnitTask);
		}
		
		// process all finished tasks
		count = tasks.GetCount();
		for(i=0; i<count; i++){
			deoglShaderCompileTask * const task = (deoglShaderCompileTask*)tasks.GetAt(i);
			deoglShaderProgram &program = *task->GetProgram();
			
			if(!pCompiler->HasCompileShaderFinished(program)){
				continue;
			}
			
			try{
				pCompiler->FinishCompileShader(program);
				//pLanguage.GetRenderThread().GetLogger().LogInfoFormat(
				//	"CompileThread %d: Stop task '%.50s...'", pContextIndex, cacheId.GetString());
				
			}catch(const deException &){
				program.SetCompiled(nullptr);
				program.ready = false;
			}
			
			deoglShaderCompileTask::Ref guardTask(task);
			tasks.RemoveFrom(i);
			i--;
			count--;
			pLanguage.FinishTask(guardTask);
		}
	}
	
	// fail all unit tasks we are still holding to
	count = unitTasks.GetCount();
	for(i=0; i<count; i++){
		deoglShaderCompileUnitTask::Ref unitTask((deoglShaderCompileUnitTask*)unitTasks.GetAt(i));
		deoglShaderProgramUnit &unit = *unitTask->GetUnit();
		unit.compilingFailed = true;
		unit.DropHandle();
		unitTasks.SetAt(i, nullptr);
		pLanguage.FinishTask(unitTask);
	}
	
	// fail all tasks we are still holding to
	count = tasks.GetCount();
	for(i=0; i<count; i++){
		deoglShaderCompileTask::Ref task((deoglShaderCompileTask*)tasks.GetAt(i));
		deoglShaderProgram &program = *task->GetProgram();
		program.SetCompiled(nullptr);
		program.ready = false;
		tasks.SetAt(i, nullptr);
		pLanguage.FinishTask(task);
	}
}
