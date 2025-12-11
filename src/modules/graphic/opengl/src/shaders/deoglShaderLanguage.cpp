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
#include <ctype.h>

#include "deoglShaderLanguage.h"
#include "deoglShaderCompiled.h"
#include "deoglShaderSources.h"
#include "deoglShaderDefines.h"
#include "deoglShaderProgram.h"
#include "deoglShaderProgramUnit.h"
#include "deoglShaderUnitSourceCode.h"
#include "deoglShaderManager.h"
#include "deoglShaderSourceLocation.h"
#include "compiler/deoglShaderCompiler.h"
#include "compiler/deoglShaderCompileListener.h"
#include "compiler/deoglShaderCompilerThread.h"
#include "../deoglCaches.h"
#include "../deGraphicOpenGl.h"
#include "../capabilities/deoglCapabilities.h"
#include "../extensions/deoglExtensions.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTChoices.h"
#include "../renderthread/deoglRTContext.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/filesystem/deCacheHelper.h>
#include <dragengine/threading/deMutexGuard.h>


class cOptionalMutexGuard{
private:
	deMutex &pMutex;
	bool pEnable;
	
public:
	cOptionalMutexGuard(deMutex &mutex, bool enable) :
	pMutex(mutex), pEnable(enable){
		if(enable){
			mutex.Lock();
		}
	}
	
	~cOptionalMutexGuard(){
		if(pEnable){
			pMutex.Unlock();
		}
	}
};

bool deoglShaderLanguage::sCompileTotals::operator==(const sCompileTotals &other) const{
	return units == other.units
		&& stage[0] == other.stage[0]
		&& stage[1] == other.stage[1]
		&& stage[2] == other.stage[2]
		&& stage[3] == other.stage[3]
		&& stage[4] == other.stage[4]
		&& stage[5] == other.stage[5]
		&& shaders == other.shaders;
}

deoglShaderLanguage::sCompileTotals &deoglShaderLanguage::sCompileTotals::operator=(const sCompileTotals &other){
	units = other.units;
	stage[0] = other.stage[0];
	stage[1] = other.stage[1];
	stage[2] = other.stage[2];
	stage[3] = other.stage[3];
	stage[4] = other.stage[4];
	stage[5] = other.stage[5];
	shaders = other.shaders;
	return *this;
}


// Class deoglShaderLanguage
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderLanguage::deoglShaderLanguage(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pShaderFileNumber(1),
pLoadingShaderCount(0),
pHasLoadingShader(false),
pCompilingShaderCount(0),
pHasCompilingShader(false),
pCompiler(nullptr),
pCompilerThreads(nullptr),
pCompilerThreadCount(0),
pCompilingTaskCount(0)
{
	const deoglExtensions &ext = renderThread.GetExtensions();
	
	// determine the required shader language version
	if(ext.GetGLESVersion() == deoglExtensions::evglesUnsupported){
		if(ext.GetGLVersion() < deoglExtensions::evgl3p3){
			switch(ext.GetGLVersion()){
			case deoglExtensions::evgl3p2:
				pGLSLVersion = "150";
				pGLSLVersionNumber = 150;
				break;
				
			case deoglExtensions::evgl3p1:
				pGLSLVersion = "140";
				pGLSLVersionNumber = 140;
				break;
				
			case deoglExtensions::evgl3p0:
				pGLSLVersion = "130";
				pGLSLVersionNumber = 130;
				break;
				
			case deoglExtensions::evgl2p1:
				pGLSLVersion = "120";
				pGLSLVersionNumber = 120;
				break;
				
			default:
				pGLSLVersion = "110";
				pGLSLVersionNumber = 110;
			}
			
		}else{
			pGLSLVersion.Format("%d%0d0 core", ext.GetGLVersionMajor(), ext.GetGLVersionMinor());
			pGLSLVersionNumber = 100 * ext.GetGLVersionMajor() + 10 * ext.GetGLVersionMinor();
		}
		
	}else{
		pGLSLVersion.Format("%d%0d0 es", ext.GetGLVersionMajor(), ext.GetGLVersionMinor());
		pGLSLVersionNumber = 100 * ext.GetGLVersionMajor() + 10 * ext.GetGLVersionMinor();
	}
	
	// some extensions provide functionality which is not present in the supported GLSL
	// version. add the required extension declarations
	if(ext.GetGLESVersion() == deoglExtensions::evglesUnsupported){
		// opengl extensions have a "in core" and "core since" version. some drivers seem to
		// fail if "core since" version is used. using thus "in core" to be on the safe side.
		// 
		// and again nVidia kills the fun. if "in core" is used extensions which are present
		// cause shader compilation to fail. looks like nVidia needs "core since" while stuff
		// like Intel needs "in core". what a huge mess
		const bool useCoreSince = true;
		
		#define GLSL_EXT_CHECK(v,cs,ci) ((v) < (useCoreSince ? deoglExtensions:: cs : deoglExtensions:: ci))
		
		// core since: 3.1 , in core: 4.6
		if(ext.GetHasExtension(deoglExtensions::ext_ARB_uniform_buffer_object)
		&& GLSL_EXT_CHECK(ext.GetGLVersion(), evgl3p1, evgl4p6)){
			// ext.GetGLESVersion() < deoglExtensions::evgles3p0
			pGLSLExtensions.Add("GL_ARB_uniform_buffer_object");
		}
		
		// core since: 3.1 , in core: 4.6
		if(ext.GetHasExtension(deoglExtensions::ext_ARB_texture_buffer_object)
		&& GLSL_EXT_CHECK(ext.GetGLVersion(), evgl3p1, evgl4p6)){
			pGLSLExtensions.Add("GL_ARB_texture_buffer_object");
		}
		
		// core since: 4.3 , in core: 4.6
		if(ext.GetHasExtension(deoglExtensions::ext_ARB_shader_storage_buffer_object)
		&& GLSL_EXT_CHECK(ext.GetGLVersion(), evgl4p3, evgl4p6)){
			// ext.GetGLESVersion() < deoglExtensions::evgles3p2
			pGLSLExtensions.Add("GL_ARB_shader_storage_buffer_object");
		}
		
		// required for intel drivers. keyword "readonly" is added in
		// GL_ARB_shader_image_load_store extension. if extension is not
		// included intel drivers can fail to compile shader
		// core since: 4.2 , in core: 4.6
		if(ext.GetHasExtension(deoglExtensions::ext_ARB_shader_image_load_store)
		&& GLSL_EXT_CHECK(ext.GetGLVersion(), evgl4p2, evgl4p6)){
			// ext.GetGLESVersion() < deoglExtensions::evgles3p2
			pGLSLExtensions.Add("GL_ARB_shader_image_load_store");
		}
		
		if(ext.GetHasExtension(deoglExtensions::ext_ARB_shading_language_420pack)
		&& GLSL_EXT_CHECK(ext.GetGLVersion(), evgl4p2, evgl4p6)){
			// ext.GetGLESVersion() < deoglExtensions::evgles3p2
			pGLSLExtensions.Add("GL_ARB_shading_language_420pack");
		}
		
		if(ext.GetHasExtension(deoglExtensions::ext_ARB_shader_atomic_counters)
		&& GLSL_EXT_CHECK(ext.GetGLVersion(), evgl4p2, evgl4p6)){
			pGLSLExtensions.Add("GL_ARB_shader_atomic_counters");
		}
	}
	
	try{
		pCompiler = new deoglShaderCompiler(*this, -1);
		pCreateCompileThreads();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglShaderLanguage::~deoglShaderLanguage(){
	pLogTotalsLocked();
	pCleanUp();
}



// Management
///////////////

void deoglShaderLanguage::CompileShader(deoglShaderProgram &program){
	// pRenderThread.GetLogger().LogInfoFormat("CompileShader: cacheId='%s' cacheId.len=%d",
	// 	program.GetCacheId().GetString(), program.GetCacheId().GetLength());
	const deMutexGuard guard(pMutexTasks);
	
	pCompileTotals.shaders++;
	pCompiler->LoadCachedShader(program);
	if(!program.ready){
		pCompiler->CompileShader(program);
	}
	pCompiler->FinishCompileShader(program);
	pLogTotalsLocked();
}

void deoglShaderLanguage::CompileShaderAsync(deoglShaderProgram *program,
deoglShaderCompileListener *listener){
	DEASSERT_NOTNULL(listener)
	DEASSERT_NOTNULL(program)
	
	// pRenderThread.GetLogger().LogInfoFormat("CompileShaderAsync: cacheId='%s' cacheId.len=%d",
	// 	program->GetCacheId().GetString(), program->GetCacheId().GetLength());
	
	deoglShaderProgramUnit * const units[6] = {
		program->GetUnitCompute(),
		program->GetUnitVertex(),
		program->GetUnitTessellationControl(),
		program->GetUnitTessellationEvaluation(),
		program->GetUnitGeometry(),
		program->GetUnitFragment()};
	int i;
	
	if(pCompilerThreadCount > 0 /*|| pglMaxShaderCompilerThreads*/){
		{
		const deMutexGuard guard(pMutexTasks);
		pLoadTasksPending.Add(deoglShaderLoadTask::Ref::NewWith(program, listener));
		}
		pSemaphoreNewTasks.Signal();
		
	}else{
		try{
			pCompiler->LoadCachedShader(*program);
			if(!program->ready){
				for(i=0; i<6; i++){
					deoglShaderProgramUnit * const u = units[i];
					if(!u || u->compilingFailed || u->IsCompiled()){
						continue;
					}
					
					pCompileTotals.units++;
					pCompileTotals.stage[i]++;
					
					pCompiler->CompileShaderUnit(*u);
					pCompiler->FinishCompileShaderUnit(*u);
				}
				
				pCompileTotals.shaders++;
				pCompiler->CompileShader(*program);
			}
			pCompiler->FinishCompileShader(*program);
			pLogTotalsLocked();
			
		}catch(const deException &e){
			pRenderThread.GetLogger().LogException(e);
		}
		
		try{
			listener->CompileFinished(program);
		}catch(const deException &e){
			pRenderThread.GetLogger().LogException(e);
		}
		
		try{
			delete listener;
		}catch(const deException &e){
			pRenderThread.GetLogger().LogException(e);
		}
	}
}

int deoglShaderLanguage::NextShaderFileNumber(){
	const deMutexGuard guard(pMutex);
	return pShaderFileNumber++;
}

bool deoglShaderLanguage::GetHasLoadingShader(){
	const deMutexGuard guard(pMutex);
	const bool result = pHasLoadingShader;
	if(pLoadingShaderCount == 0){
		pHasLoadingShader = false;
	}
	return result;
}

bool deoglShaderLanguage::GetHasCompilingShader(){
	const deMutexGuard guard(pMutex);
	const bool result = pHasCompilingShader;
	if(pCompilingShaderCount == 0){
		pHasCompilingShader = false;
	}
	return result;
}

void deoglShaderLanguage::AddLoadingShader(){
	const deMutexGuard guard(pMutex);
	pLoadingShaderCount++;
	pHasLoadingShader = true;
}

void deoglShaderLanguage::RemoveLoadingShader(){
	const deMutexGuard guard(pMutex);
	DEASSERT_TRUE(pLoadingShaderCount > 0)
	pLoadingShaderCount--;
}

void deoglShaderLanguage::AddCompilingShader(){
	const deMutexGuard guard(pMutex);
	pCompilingShaderCount++;
	pHasCompilingShader = true;
}

void deoglShaderLanguage::RemoveCompilingShader(){
	const deMutexGuard guard(pMutex);
	DEASSERT_TRUE(pCompilingShaderCount > 0)
	pCompilingShaderCount--;
}

void deoglShaderLanguage::GetNextTask(deoglShaderCompileTask::Ref &task,
deoglShaderCompileUnitTask::Ref &unitTask, deoglShaderLoadTask::Ref &loadTask){
	task = nullptr;
	unitTask = nullptr;
	loadTask = nullptr;
	
	const deMutexGuard guard(pMutexTasks);
	
	if(pLoadTasksPending.GetCount() > 0){
		loadTask = (deoglShaderLoadTask*)pLoadTasksPending.GetAt(0);
		pLoadTasksPending.RemoveFrom(0);
		pCompilingTaskCount++;
		return;
	}
	
	if(pUnitTasksPending.GetCount() > 0){
		unitTask = (deoglShaderCompileUnitTask*)pUnitTasksPending.GetAt(0);
		pUnitTasksPending.RemoveFrom(0);
		pCompilingTaskCount++;
		return;
	}
	
	const int count = pTasksPending.GetCount();
	int i;
	for(i=0; i<count; i++){
		deoglShaderCompileTask * const candidate = (deoglShaderCompileTask*)pTasksPending.GetAt(i);
		if(candidate->GetProgram()->CanCompile()){
			task = candidate;
			pTasksPending.RemoveFrom(i);
			pCompilingTaskCount++;
			return;
		}
	}
}

void deoglShaderLanguage::FinishTask(deoglShaderCompileTask::Ref &task){
	try{
		task->GetListener()->CompileFinished(task->GetProgram());
		
	}catch(const deException &e){
		pRenderThread.GetLogger().LogException(e);
	}
	
	{
	const deMutexGuard guard(pMutexTasks);
	
	DEASSERT_TRUE(pCompilingTaskCount > 0)
	pCompilingTaskCount--;
	
	task = nullptr;
	
	if(pUnitTasksPending.GetCount() == 0 && pTasksPending.GetCount() == 0
	&& pLoadTasksPending.GetCount() == 0){
		pLogTotalsLocked();
	}
	}
	
	pSemaphoreTasksFinished.Signal();
}

void deoglShaderLanguage::FinishTask(deoglShaderCompileUnitTask::Ref &task){
	{
	const deMutexGuard guard(pMutexTasks);
	
	task->GetUnit()->isCompiling = false;
	
	DEASSERT_TRUE(pCompilingTaskCount > 0)
	pCompilingTaskCount--;
	
	task = nullptr;
	
	if(pUnitTasksPending.GetCount() == 0 && pTasksPending.GetCount() == 0
	&& pLoadTasksPending.GetCount() == 0){
		pLogTotalsLocked();
	}
	}
	
	pSemaphoreTasksFinished.Signal();
	pSemaphoreNewTasks.Signal();
}

void deoglShaderLanguage::FinishTask(deoglShaderLoadTask::Ref &task){
	const bool ready = task->GetProgram()->ready;
	
	if(ready){
		try{
			task->GetListener()->CompileFinished(task->GetProgram());
			
		}catch(const deException &e){
			pRenderThread.GetLogger().LogException(e);
		}
	}
	
	{
	const deMutexGuard guard(pMutexTasks);
	
	if(!ready){
		deoglShaderProgram * const program = task->GetProgram();
		
		deoglShaderProgramUnit * const units[6] = {
			program->GetUnitCompute(),
			program->GetUnitVertex(),
			program->GetUnitTessellationControl(),
			program->GetUnitTessellationEvaluation(),
			program->GetUnitGeometry(),
			program->GetUnitFragment()};
		int i;
		
		for(i=0; i<6; i++){
			deoglShaderProgramUnit * const u = units[i];
			if(!u || u->isCompiling || u->compilingFailed || u->IsCompiled()){
				continue;
			}
			
			pCompileTotals.units++;
			pCompileTotals.stage[i]++;
			
			pUnitTasksPending.Add(deoglShaderCompileUnitTask::Ref::NewWith(u));
			
			// this method can be called from the resource loader thread instead of the render
			// thread. for this reason CompileShaderUnit() can not be called here. the task
			// sets a pending flag which causes the compile to be done in the Update() call
			/*
			if(pglMaxShaderCompilerThreads){
				pCompiler->CompileShaderUnit(*u);
			}
			*/
		}
		
		pCompileTotals.shaders++;
		pTasksPending.Add(deoglShaderCompileTask::Ref::NewWith(program, task->TakeOutListener()));
	}
	
	DEASSERT_TRUE(pCompilingTaskCount > 0)
	pCompilingTaskCount--;
	
	task = nullptr;
	
	if(pUnitTasksPending.GetCount() == 0 && pTasksPending.GetCount() == 0
	&& pLoadTasksPending.GetCount() == 0){
		pLogTotalsLocked();
	}
	}
	
	pSemaphoreTasksFinished.Signal();
	pSemaphoreNewTasks.Signal();
}

void deoglShaderLanguage::WaitForNewTasks(){
	pSemaphoreNewTasks.Wait();
}

void deoglShaderLanguage::WaitAllTasksFinished(){
	/*
	if(pglMaxShaderCompilerThreads){
		int i, count = pUnitTasksPending.GetCount();
		for(i=0; i<count; i++){
			deoglShaderCompileUnitTask &task = *((deoglShaderCompileUnitTask*)pUnitTasksPending.GetAt(i));
			pCompiler->FinishCompileShaderUnit(*task.GetUnit());
			task.GetUnit()->isCompiling = false;
		}
		pUnitTasksPending.RemoveAll();
		
		count = pTasksPending.GetCount();
		for(i=0; i<count; i++){
			deoglShaderCompileTask &task = *((deoglShaderCompileTask*)pTasksPending.GetAt(i));
			
			try{
				pCompiler->FinishCompileShader(*task.GetProgram());
				
			}catch(const deException &e){
				pRenderThread.GetLogger().LogException(e);
			}
			
			try{
				task.GetListener()->CompileFinished(task.GetProgram());
				
			}catch(const deException &e){
				pRenderThread.GetLogger().LogException(e);
			}
		}
		pTasksPending.RemoveAll();
		
		pLogTotalsLocked();
		
	}else{
	*/
		while(true){
			{
			const deMutexGuard guard(pMutexTasks);
			if(pCompilingTaskCount == 0 && pUnitTasksPending.GetCount() == 0
			&& pTasksPending.GetCount() == 0 && pLoadTasksPending.GetCount() == 0){
				break;
			}
			}
			pSemaphoreTasksFinished.Wait();
		}
	//}
}

void deoglShaderLanguage::Update(){
	return; // yes, this is here on purpose
	if(!pglMaxShaderCompilerThreads){
		return;
	}
	if(pUnitTasksPending.GetCount() == 0 && pTasksPending.GetCount() == 0
	&& pLoadTasksPending.GetCount() == 0){
		return;
	}
	
	int i, count = pLoadTasksPending.GetCount();
	for(i=0; i<count; i++){
		deoglShaderLoadTask &task = *((deoglShaderLoadTask*)pLoadTasksPending.GetAt(i));
		deoglShaderProgram * const program = task.GetProgram();
		
		try{
			pCompiler->LoadCachedShader(*program);
			if(program->ready){
				task.GetListener()->CompileFinished(program);
				
			}else{
				deoglShaderProgramUnit * const units[6] = {
					program->GetUnitCompute(),
					program->GetUnitVertex(),
					program->GetUnitTessellationControl(),
					program->GetUnitTessellationEvaluation(),
					program->GetUnitGeometry(),
					program->GetUnitFragment()};
				int j;
				
				for(j=0; j<6; j++){
					deoglShaderProgramUnit * const u = units[j];
					if(!u || u->isCompiling || u->compilingFailed || u->IsCompiled()){
						continue;
					}
					
					pCompileTotals.units++;
					pCompileTotals.stage[j]++;
					
					pUnitTasksPending.Add(deoglShaderCompileUnitTask::Ref::NewWith(u));
				}
				
				pCompileTotals.shaders++;
				pTasksPending.Add(deoglShaderCompileTask::Ref::NewWith(program, task.TakeOutListener()));
			}
			
		}catch(const deException &e){
			pRenderThread.GetLogger().LogException(e);
		}
		
		pLoadTasksPending.RemoveFrom(i);
		count--;
		i--;
	}
	
	count = pUnitTasksPending.GetCount();
	for(i=0; i<count; i++){
		deoglShaderCompileUnitTask &task = *((deoglShaderCompileUnitTask*)pUnitTasksPending.GetAt(i));
		
		if(task.pending){
			task.pending = false;
			pCompiler->CompileShaderUnit(*task.GetUnit());
		}
		
		if(!pCompiler->HasCompileShaderUnitFinished(*task.GetUnit())){
			continue;
		}
		
		pCompiler->FinishCompileShaderUnit(*task.GetUnit());
		task.GetUnit()->isCompiling = false;
		pUnitTasksPending.RemoveFrom(i);
		count--;
		i--;
	}
	
	count = pTasksPending.GetCount();
	for(i=0; i<count; i++){
		deoglShaderCompileTask &task = *((deoglShaderCompileTask*)pTasksPending.GetAt(i));
		deoglShaderProgram &program = *task.GetProgram();
		
		if(!program.isCompiling){
			if(!program.CanCompile()){
				continue;
			}
			
			program.isCompiling = true;
			try{
				pCompiler->LoadCachedShader(program);
				if(!program.ready){
					pCompiler->CompileShader(program);
				}
				
			}catch(const deException &e){
				pRenderThread.GetLogger().LogException(e);
				program.isCompiling = false;
			}
			
			if(program.isCompiling){
				continue;
			}
		}
		
		if(!pCompiler->HasCompileShaderFinished(*task.GetProgram())){
			continue;
		}
		
		try{
			pCompiler->FinishCompileShader(*task.GetProgram());
			
		}catch(const deException &e){
			pRenderThread.GetLogger().LogException(e);
		}
		
		try{
			task.GetListener()->CompileFinished(task.GetProgram());
			
		}catch(const deException &e){
			pRenderThread.GetLogger().LogException(e);
		}
		
		pTasksPending.RemoveFrom(i);
		count--;
		i--;
	}
	
	if(pUnitTasksPending.GetCount() == 0 && pTasksPending.GetCount() == 0
	&& pLoadTasksPending.GetCount() == 0){
		pLogTotalsLocked();
	}
}


// Private Functions
//////////////////////

void deoglShaderLanguage::pCleanUp(){
	if(pCompilerThreads){
		int i;
		for(i=0; i<pCompilerThreadCount; i++){
			pCompilerThreads[i]->RequestExit();
		}
		
		pSemaphoreNewTasks.SignalAll();
		pSemaphoreTasksFinished.SignalAll();
		
		for(i=0; i<pCompilerThreadCount; i++){
			pRenderThread.GetLogger().LogInfoFormat("Wait exit shader compile thread %d", i);
			pCompilerThreads[i]->WaitForExit();
			pRenderThread.GetLogger().LogInfoFormat("Shader compile thread %d exited", i);
			delete pCompilerThreads[i];
		}
		
		delete [] pCompilerThreads;
	}
	
	if(pCompiler){
		delete pCompiler;
	}
}

void deoglShaderLanguage::pCreateCompileThreads(){
	deoglRTContext &context = pRenderThread.GetContext();
	int count = context.GetCompileContextCount();
	if(count == 0){
		return;
	}
	
	if(pglMaxShaderCompilerThreads){
		count = 1;
	}
#ifdef OS_ANDROID_QUEST
	// count = 1;
#endif
	
	pCompilerThreads = new deoglShaderCompilerThread*[count];
	
	int i;
	if(pglMaxShaderCompilerThreads){
		pCompilerThreads[0] = new deoglShaderCompilerThread(
			*this, 0, deoglShaderCompilerThread::Type::glparallel);
		pCompilerThreadCount = 1;
		pCompilerThreads[0]->Start();
		pRenderThread.GetContext().DropCompileContexts(1);
		
	}else{
		for(i=0; i<count; i++){
			pCompilerThreads[i] = new deoglShaderCompilerThread(
				*this, i, deoglShaderCompilerThread::Type::single);
			pCompilerThreadCount = i + 1;
			pCompilerThreads[i]->Start();
		}
	}
	
	for(i=0; i<count; i++){
		while(true){
			const deoglShaderCompilerThread::State state = pCompilerThreads[i]->GetState();
			DEASSERT_FALSE(state == deoglShaderCompilerThread::State::failed)
			if(state == deoglShaderCompilerThread::State::ready){
				break;
			}
			
			decTimer timer;
			while(timer.PeekElapsedTime() < 0.001f);
		}
	}
}

void deoglShaderLanguage::pLogTotalsLocked(){
	if(pCompileTotals == pLastCompileTotals){
		return;
	}
	
	pLastCompileTotals = pCompileTotals;
	
	pRenderThread.GetLogger().LogInfoFormat(
		"ShaderCompileTotal: shaders=%d units=%d (c=%d v=%d tc=%d te=%d g=%d f=%d)",
		pCompileTotals.shaders, pCompileTotals.units, pCompileTotals.stage[0],
		pCompileTotals.stage[1], pCompileTotals.stage[2], pCompileTotals.stage[3],
		pCompileTotals.stage[4], pCompileTotals.stage[5]);
}
