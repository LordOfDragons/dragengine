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
	deoglExtensions &ext = renderThread.GetExtensions();
	
	// determine the required shader language version
	if( ext.GetGLESVersion() == deoglExtensions::evglesUnsupported ){
		if( ext.GetGLVersion() < deoglExtensions::evgl3p3 ){
			switch( ext.GetGLVersion() ){
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
			pGLSLVersion.Format( "%d%0d0 core", ext.GetGLVersionMajor(), ext.GetGLVersionMinor() );
			pGLSLVersionNumber = 100 * ext.GetGLVersionMajor() + 10 * ext.GetGLVersionMinor();
		}
		
	}else{
		pGLSLVersion.Format( "%d%0d0 es", ext.GetGLVersionMajor(), ext.GetGLVersionMinor() );
		pGLSLVersionNumber = 100 * ext.GetGLVersionMajor() + 10 * ext.GetGLVersionMinor();
	}
	
	// some extensions provide functionality which is not present in the supported GLSL
	// version. add the required extension declarations
	if( ext.GetGLESVersion() == deoglExtensions::evglesUnsupported ){
		// opengl extensions have a "in core" and "core since" version. some drivers seem to
		// fail if "core since" version is used. using thus "in core" to be on the safe side.
		// 
		// and again nVidia kills the fun. if "in core" is used extensions which are present
		// cause shader compilation to fail. looks like nVidia needs "core since" while stuff
		// like Intel needs "in core". what a huge mess
		const bool useCoreSince = true;
		
		#define GLSL_EXT_CHECK(v,cs,ci) ( (v) < ( useCoreSince ? deoglExtensions:: cs : deoglExtensions:: ci ) )
		
		// core since: 3.1 , in core: 4.6
		if( ext.GetHasExtension( deoglExtensions::ext_ARB_uniform_buffer_object )
		&& GLSL_EXT_CHECK( ext.GetGLVersion(), evgl3p1, evgl4p6 ) ){
			// ext.GetGLESVersion() < deoglExtensions::evgles3p0
			pGLSLExtensions.Add( "GL_ARB_uniform_buffer_object" );
		}
		
		// core since: 3.1 , in core: 4.6
		if( ext.GetHasExtension( deoglExtensions::ext_ARB_texture_buffer_object )
		&& GLSL_EXT_CHECK( ext.GetGLVersion(), evgl3p1, evgl4p6 ) ){
			pGLSLExtensions.Add( "GL_ARB_texture_buffer_object" );
		}
		
		// core since: 4.3 , in core: 4.6
		if( ext.GetHasExtension( deoglExtensions::ext_ARB_shader_storage_buffer_object )
		&& GLSL_EXT_CHECK( ext.GetGLVersion(), evgl4p3, evgl4p6 ) ){
			// ext.GetGLESVersion() < deoglExtensions::evgles3p2
			pGLSLExtensions.Add( "GL_ARB_shader_storage_buffer_object" );
		}
		
		// required for intel drivers. keyword "readonly" is added in
		// GL_ARB_shader_image_load_store extension. if extension is not
		// included intel drivers can fail to compile shader
		// core since: 4.2 , in core: 4.6
		if( ext.GetHasExtension( deoglExtensions::ext_ARB_shader_image_load_store )
		&& GLSL_EXT_CHECK( ext.GetGLVersion(), evgl4p2, evgl4p6 ) ){
			// ext.GetGLESVersion() < deoglExtensions::evgles3p2
			pGLSLExtensions.Add( "GL_ARB_shader_image_load_store" );
		}
		
		if( ext.GetHasExtension( deoglExtensions::ext_ARB_shading_language_420pack )
		&& GLSL_EXT_CHECK( ext.GetGLVersion(), evgl4p2, evgl4p6 ) ){
			// ext.GetGLESVersion() < deoglExtensions::evgles3p2
			pGLSLExtensions.Add( "GL_ARB_shading_language_420pack" );
		}
		
		if( ext.GetHasExtension( deoglExtensions::ext_ARB_shader_atomic_counters )
		&& GLSL_EXT_CHECK( ext.GetGLVersion(), evgl4p2, evgl4p6 ) ){
			pGLSLExtensions.Add( "GL_ARB_shader_atomic_counters" );
		}
	}
	
	try{
		pCreateCompileThreads();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglShaderLanguage::~deoglShaderLanguage(){
	pCleanUp();
}



// Management
///////////////

deoglShaderCompiled *deoglShaderLanguage::CompileShader(const deoglShaderProgram &program){
// 	pRenderThread.GetLogger().LogInfoFormat("CompileShader: cacheId='%s' cacheId.len=%d",
// 		program.GetCacheId().GetString(), program.GetCacheId().GetLength());
	
	if(!pCompiler){
		pCompiler = new deoglShaderCompiler(*this, -1);
	}
	
	return pCompiler->CompileShader(program);
}

void deoglShaderLanguage::CompileShaderAsync(const deoglShaderProgram *program,
deoglShaderCompileListener *listener){
	DEASSERT_NOTNULL(listener)
	DEASSERT_NOTNULL(program)
	
	if(pCompilerThreadCount > 0){
		{
		const deMutexGuard guard(pMutexTasks);
		pTasksPending.Add(deObject::Ref::New(new deoglShaderCompileTask(program, listener)));
		}
		pSemaphoreNewTasks.Signal();
		
	}else{
		deoglShaderCompiled *compiled = nullptr;
		try{
			compiled = CompileShader(*program);
		}catch(const deException &e){
			pRenderThread.GetLogger().LogException(e);
		}
		
		try{
			listener->CompileFinished(compiled);
		}catch(const deException &e){
			if(compiled){
				delete compiled;
			}
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

void deoglShaderLanguage::GetNextTask(deoglShaderCompileTask::Ref &task){
	const deMutexGuard guard(pMutexTasks);
	if(pTasksPending.GetCount() > 0){
		task = (deoglShaderCompileTask*)pTasksPending.GetAt(0);
		pTasksPending.RemoveFrom(0);
		pCompilingTaskCount++;
		
	}else{
		task = nullptr;
	}
}

void deoglShaderLanguage::FinishTask(deoglShaderCompileTask::Ref &task){
	try{
		task->GetListener()->CompileFinished(task->GetCompiled());
		
	}catch(const deException &e){
		pRenderThread.GetLogger().LogException(e);
	}
	
	{
	const deMutexGuard guard(pMutexTasks);
	DEASSERT_TRUE(pCompilingTaskCount > 0)
	pCompilingTaskCount--;
	task = nullptr;
	}
	
	pSemaphoreTasksFinished.Signal();
}

void deoglShaderLanguage::WaitForNewTasks(){
	pSemaphoreNewTasks.Wait();
}

void deoglShaderLanguage::WaitAllTasksFinished(){
	while(true){
		{
		const deMutexGuard guard(pMutexTasks);
		if(pCompilingTaskCount == 0 && pTasksPending.GetCount() == 0){
			break;
		}
		}
		pSemaphoreTasksFinished.Wait();
	}
}


// Private Functions
//////////////////////

void deoglShaderLanguage::pCleanUp(){
	int i;
	for(i=0; i<pCompilerThreadCount; i++){
		pCompilerThreads[i]->RequestExit();
	}
	
	pSemaphoreNewTasks.SignalAll();
	
	for(i=0; i<pCompilerThreadCount; i++){
		pRenderThread.GetLogger().LogInfoFormat("Wait exit shader compile thread %d", i);
		pCompilerThreads[i]->WaitForExit();
		pRenderThread.GetLogger().LogInfoFormat("Shader compile thread %d exited", i);
		delete pCompilerThreads[i];
	}
	
	if(pCompiler){
		delete pCompiler;
	}
}

void deoglShaderLanguage::pCreateCompileThreads(){
	deoglRTContext &context = pRenderThread.GetContext();
	const int count = context.GetCompileContextCount();
	if(count == 0){
		return;
	}
	
	pCompilerThreads = new deoglShaderCompilerThread*[count];
	
	int i;
	for(i=0; i<count; i++){
		pCompilerThreads[i] = new deoglShaderCompilerThread(*this, i);
		pCompilerThreadCount = i + 1;
		pCompilerThreads[i]->Start();
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
