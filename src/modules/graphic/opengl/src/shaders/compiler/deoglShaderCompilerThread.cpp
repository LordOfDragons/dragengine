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
#include "deoglShaderLoadTask.h"
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
}

deoglShaderCompilerThread::~deoglShaderCompilerThread(){
	pCleanUp();
}



// Management
///////////////

void deoglShaderCompilerThread::Run(){
	try{
		pActivateContext();
		pCompiler = new deoglShaderCompiler(pLanguage, pContextIndex);
		
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
	if(pContextIndex > -1){
		pLanguage.GetRenderThread().GetContext().GetBackend()->DeactivateCompileContext(pContextIndex);
	}
}

void deoglShaderCompilerThread::pActivateContext(){
	if(pContextIndex > -1){
		pLanguage.GetRenderThread().GetContext().GetBackend()->ActivateCompileContext(pContextIndex);
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
		deoglShaderLoadTask::Ref loadTask;
		pLanguage.GetNextTask(task, unitTask, loadTask);
		
		if(loadTask){
			deoglShaderProgram &program = *loadTask->GetProgram();
			try{
				pCompiler->LoadCachedShader(program);
				
			}catch(const deException &){
				program.SetCompiled(nullptr);
				program.ready = false;
			}
			
			pLanguage.FinishTask(loadTask);
			
		}else if(unitTask){
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
	decTObjectList<deoglShaderCompileTask> tasks;
	decTObjectList<deoglShaderCompileUnitTask> unitTasks;
	int i, count;
	
	while(true){
		if(pExitThreadRequested()){
			break;
		}
		
		// start compiling all available unit tasks and tasks and store
		while(true){
			deoglShaderCompileTask::Ref task;
			deoglShaderCompileUnitTask::Ref unitTask;
			deoglShaderLoadTask::Ref loadTask;
			pLanguage.GetNextTask(task, unitTask, loadTask);
			
			if(loadTask){
				deoglShaderProgram &program = *loadTask->GetProgram();
				try{
					pCompiler->LoadCachedShader(program);
					
				}catch(const deException &){
					program.SetCompiled(nullptr);
					program.ready = false;
				}
				pLanguage.FinishTask(loadTask);
				
			}else if(unitTask){
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
		deoglShaderCompileUnitTask * const unitTask = unitTasks.GetAt(i);
		deoglShaderProgramUnit &unit = *unitTask->GetUnit();			try{
				if(!pCompiler->HasCompileShaderUnitFinished(unit)){
					continue;
				}
				
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
		deoglShaderCompileTask * const task = tasks.GetAt(i);
		deoglShaderProgram &program = *task->GetProgram();			try{
				if(!pCompiler->HasCompileShaderFinished(program)){
					continue;
				}
				
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
		deoglShaderCompileUnitTask::Ref unitTask(unitTasks.GetAt(i));
		deoglShaderProgramUnit &unit = *unitTask->GetUnit();
		unit.compilingFailed = true;
		unit.DropHandle();
		unitTasks.SetAt(i, nullptr);
		pLanguage.FinishTask(unitTask);
	}
	
	// fail all tasks we are still holding to
	count = tasks.GetCount();
	for(i=0; i<count; i++){
		deoglShaderCompileTask::Ref task(tasks.GetAt(i));
		deoglShaderProgram &program = *task->GetProgram();
		program.SetCompiled(nullptr);
		program.ready = false;
		tasks.SetAt(i, nullptr);
		pLanguage.FinishTask(task);
	}
}
