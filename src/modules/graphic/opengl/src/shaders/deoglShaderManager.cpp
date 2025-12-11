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
#include <string.h>
#include <inttypes.h>

#include "deoglShaderManager.h"
#include "deoglShaderDefines.h"
#include "deoglShaderSources.h"
#include "deoglShaderCompiled.h"
#include "deoglShaderLanguage.h"
#include "deoglShaderUnitSourceCode.h"
#include "compiler/deoglShaderCompiler.h"
#include "compiler/deoglShaderCompileListener.h"
#include "../deGraphicOpenGl.h"
#include "../deoglCaches.h"
#include "../configuration/deoglConfiguration.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/deCacheHelper.h>
#include <dragengine/filesystem/deCollectFileSearchVisitor.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/threading/deMutexGuard.h>



// Definitions
////////////////

// cache revision. if anything except *.glsl or *.shader.xml changes increment this
// value to make sure existing caches are invalidate
#define SHADER_CACHE_REVISION 1

// MinGW bug workaround
#if defined OS_W32 && defined __MINGW64__
#undef PRIu64
#define PRIu64 "I64u"
#endif

// try to detect problems with shader define filtering
// #define DEBUG_SHADER_UNIT_DEFINE_FILTERING


// Class deoglShaderManager::cCompilingShader
///////////////////////////////////////////////

deoglShaderManager::cCompileProgram::cCompileProgram(deoglShaderManager &manager,
	const deoglShaderProgram::Ref &program) :
pManager(manager),
pProgram(program)
{
	DEASSERT_NOTNULL(program)
}

void deoglShaderManager::cCompileProgram::AddListener(cGetProgramListener *listener){
	DEASSERT_NOTNULL(listener)
	pListeners.Add(listener);
}

void deoglShaderManager::cCompileProgram::CompileFinished(deoglShaderProgram *program){
	{
	const deMutexGuard guard(pManager.pMutexCompilePrograms);
	const int index = pManager.pCompilePrograms.IndexOf(this);
	if(index != -1){
		pManager.pCompilePrograms.RemoveFrom(index);
	}
	
	deoglShaderProgram *p = nullptr;
	if(program && program->ready){
		p = pManager.pGetProgramWith(program->GetSources(), program->GetDefines());
		if(!p){
			p = program;
			pManager.pPrograms.Add(program);
		}
	}
	pProgram = nullptr;
	
	const int count = pListeners.GetCount();
	int i;
	for(i=0; i<count; i++){
		cGetProgramListener * const listener = (cGetProgramListener*)pListeners.GetAt(i);
		try{
			listener->GetProgramFinished(p);
			
		}catch(const deException &e){
			pManager.pRenderThread.GetLogger().LogException(e);
		}
		delete listener;
	}
	pListeners.RemoveAll();
	}
	
	pManager.pSemaphoreCompileFinished.SignalAll();
}


// Class deoglShaderManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderManager::deoglShaderManager(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pLanguage(nullptr),
pPathShaderSources("/share/shaderSources"),
pPathShaders("/share/shaders")
{
	pLanguage = new deoglShaderLanguage(renderThread);
}

deoglShaderManager::~deoglShaderManager(){
	if(pLanguage){
		delete pLanguage;
	}
}



// Management
///////////////

void deoglShaderManager::ValidateCaches(){
	deGraphicOpenGl &ogl = pRenderThread.GetOgl();
	deCacheHelper &cache = ogl.GetCaches().GetShaders();
	deoglRTLogger &logger = pRenderThread.GetLogger();
	
	// validation string composes of the path and modification times of all shader sources (*.glsl)
	// and shaders (*.shader.xml) in the order they have been loaded. we could also sort the list
	// but running on the same machine with the same operating system and the same file system
	// usually returns identical file listings. in the worst case the cache is invalidate when
	// it could be kept valid
	const decString validationString(pCacheValidationString.Join("\n"));
	
	// if the validation string differs from the cached validation string drop all
	// cached shaders
	deVirtualFileSystem &vfs = ogl.GetVFS(); // accessed from main thread only
	const decPath pathValidationString(decPath::CreatePathUnix("/cache/global/shaderValidation"));
	
	if(vfs.ExistsFile(pathValidationString)){
		decString oldValidationString;
		try{
			const decBaseFileReader::Ref reader(vfs.OpenFileForReading(pathValidationString));
			const int size = reader->GetLength();
			oldValidationString.Set(' ', size);
			reader->Read((void*)oldValidationString.GetString(), size);
			
			if(validationString == oldValidationString){
				logger.LogInfo("ShaderManager Cache: Validated");
				
			}else{
				logger.LogInfo("ShaderManager Cache: Validation failed. Invalidate caches");
				cache.DeleteAll();
			}
			
		}catch(const deException &){
			logger.LogInfo("ShaderManager Cache: Validation file failed reading. Invalidate caches");
			cache.DeleteAll();
		}
		
	}else{
		logger.LogInfo("ShaderManager Cache: Validation file missing. Invalidate caches");
		cache.DeleteAll();
	}
	
	// write validation string
	try{
		vfs.OpenFileForWriting(pathValidationString)->WriteString(validationString);
		
	}catch(const deException &){
		logger.LogInfo("ShaderManager Cache: Writing validation failed");
	}
}

void deoglShaderManager::Update(){
	pLanguage->Update();
}



// Unit Source Codes
//////////////////////

int deoglShaderManager::GetUnitSourceCodeCount() const{
	return pUnitSourceCodes.GetCount();
}

bool deoglShaderManager::HasUnitSourceCodeNamed(const char *name) const{
	return pUnitSourceCodes.Has(name);
}

deoglShaderUnitSourceCode *deoglShaderManager::GetUnitSourceCodeNamed(const char *name) const{
	deObject *o;
	return pUnitSourceCodes.GetAt(name, &o) ? (deoglShaderUnitSourceCode*)o : nullptr;
}

void deoglShaderManager::RemoveAllUnitSourceCodes(){
	pUnitSourceCodes.RemoveAll();
}

void deoglShaderManager::LoadUnitSourceCodes(){
	deGraphicOpenGl &ogl = pRenderThread.GetOgl();
	const int oldCount = pUnitSourceCodes.GetCount();
	const int oldCountIncludable = pIncludableSources.GetCount();
	
	pLoadUnitSourceCodesIn("");
	pLoadIncludableSourcesIn("");
	
	if(ogl.GetConfiguration().GetDoLogInfo()){
		pRenderThread.GetLogger().LogInfoFormat("Loaded %d shader unit source codes.",
			pUnitSourceCodes.GetCount() - oldCount);
		pRenderThread.GetLogger().LogInfoFormat("Loaded %d includable sources.",
			pIncludableSources.GetCount() - oldCountIncludable);
	}
}



// Sources
////////////

int deoglShaderManager::GetSourcesCount() const{
	return pSources.GetCount();
}

bool deoglShaderManager::HasSourcesNamed(const char *name) const{
	return pSources.Has(name);
}

const deoglShaderSources *deoglShaderManager::GetSourcesNamed(const char *name){
	deObject *o;
	return pSources.GetAt(name, &o) ? (const deoglShaderSources *)o : nullptr;
}

void deoglShaderManager::LoadSources(){
	deGraphicOpenGl &ogl = pRenderThread.GetOgl();
	const int oldCount = pSources.GetCount();
	
	pLoadSourcesIn("");
	
	if(ogl.GetConfiguration().GetDoLogInfo()){
		pRenderThread.GetLogger().LogInfoFormat("Loaded %i shaders.",
			pSources.GetCount() - oldCount);
	}
}

decObjectList deoglShaderManager::GetSourcesAsList() const{
	return pSources.GetValues();
}



// Program Units
//////////////////

int deoglShaderManager::GetProgramUnitCount(){
	const deMutexGuard guard(pMutexCompilePrograms);
	return pProgramUnits.GetCount();
}

deoglShaderProgramUnit *deoglShaderManager::GetProgramUnitAt(int index){
	const deMutexGuard guard(pMutexCompilePrograms);
	return (deoglShaderProgramUnit*)pProgramUnits.GetAt(index);
}


// Programs
/////////////

int deoglShaderManager::GetProgramCount(){
	const deMutexGuard guard(pMutexCompilePrograms);
	return pPrograms.GetCount();
}

const deoglShaderProgram *deoglShaderManager::GetProgramAt(int index){
	const deMutexGuard guard(pMutexCompilePrograms);
	return (deoglShaderProgram*)pPrograms.GetAt(index);
}

const deoglShaderProgram *deoglShaderManager::GetProgramWith(
const deoglShaderSources *sources, const deoglShaderDefines &defines){
	DEASSERT_NOTNULL(sources)
	DEASSERT_NOTNULL(pLanguage)
	
	pRenderThread.GetLogger().LogInfoFormat("GetProgramWith(Sync): %s", sources->GetName().GetString());
	
	class cStoreProgram : public cGetProgramListener{
		const deoglShaderProgram **pProgram;
	public:
		cStoreProgram(const deoglShaderProgram **program) : pProgram(program){}
		void GetProgramFinished(const deoglShaderProgram *aprogram) override{
			*pProgram = aprogram;
		}
	};
	
	const deoglShaderProgram *program = nullptr;
	GetProgramWithAsync(sources, defines, new cStoreProgram(&program));
	WaitAllProgramsCompiled();
	return program;
}

void deoglShaderManager::GetProgramWithAsync(const deoglShaderSources *sources,
const deoglShaderDefines &defines, cGetProgramListener *listener){
	DEASSERT_NOTNULL(sources)
	DEASSERT_NOTNULL(pLanguage)
	
	cCompileProgram *compiling = nullptr;
	
#ifdef DEBUG_SHADER_UNIT_DEFINE_FILTERING
	WaitAllProgramsCompiled();
	pLanguage->WaitAllTasksFinished();
#endif
	
	{
	const deMutexGuard guard(pMutexCompilePrograms);
	deoglShaderProgram * const foundProgram = pGetProgramWith(sources, defines);
	if(foundProgram){
		try{
			listener->GetProgramFinished(foundProgram);
			
		}catch(const deException &e){
			pRenderThread.GetLogger().LogException(e);
		}
		delete listener;
		return;
	}
	
	compiling = pGetCompilingWith(sources, defines);
	if(compiling){
		compiling->AddListener(listener);
		return;
	}
	
	try{
		compiling = new cCompileProgram(*this, pCreateProgram(sources, defines));
		compiling->AddListener(listener);
		
	}catch(const deException &e){
		pRenderThread.GetLogger().LogErrorFormat("Prepare shader failed: %s",
			sources->GetName().GetString());
		pRenderThread.GetLogger().LogException(e);
		if(compiling){
			delete compiling;
		}
		delete listener;
		throw;
	}
	pCompilePrograms.Add(compiling);
	}
	
	// has to be outside mutex lock since call can immediately call listener which acquires lock
	pLanguage->CompileShaderAsync(compiling->GetProgram(), compiling);
}

void deoglShaderManager::WaitAllProgramsCompiled(){
	if(!pLanguage->HasCompileThreads()){
		return;
	}
	
	/*
	if(pglMaxShaderCompilerThreads){
		while(pCompilePrograms.GetCount() > 0){
			Update();
		}
		
	}else{
	*/
		while(true){
			{
			const deMutexGuard guard(pMutexCompilePrograms);
			if(pCompilePrograms.GetCount() == 0){
				break;
			}
			}
			pSemaphoreCompileFinished.Wait();
		}
	//}
}

void deoglShaderManager::ResolveProgramUnits(deoglShaderProgram &program){
#ifdef DEBUG_SHADER_UNIT_DEFINE_FILTERING
	WaitAllProgramsCompiled();
	pLanguage->WaitAllTasksFinished();
#endif
	
	const deMutexGuard guard(pMutexCompilePrograms);
	pResolveProgramUnitsLocked(program);
}


// Private Functions
//////////////////////

void deoglShaderManager::pResolveProgramUnitsLocked(deoglShaderProgram &program){
	const deoglShaderSources &sources = *program.GetSources();
	const deoglShaderDefines &defines = program.GetDefines();
	
	if(!sources.GetPathGeometrySourceCode().IsEmpty()){
		program.SetUnitGeometry(pGetProgramUnitWithLocked(
			sources.GetPathGeometrySourceCode(), defines));
		
		if(!program.GetUnitGeometry()){
			pRenderThread.GetLogger().LogErrorFormat(
				"Shader(%s): Geometry source '%s' not found", sources.GetFilename().GetString(),
				sources.GetPathGeometrySourceCode().GetString());
			DETHROW(deeInvalidParam);
		}
	}
	
	if(!sources.GetPathVertexSourceCode().IsEmpty()){
		program.SetUnitVertex(pGetProgramUnitWithLocked(
			sources.GetPathVertexSourceCode(), defines));
		
		if(!program.GetUnitVertex()){
			pRenderThread.GetLogger().LogErrorFormat(
				"Shader(%s): Vertex source '%s' not found", sources.GetFilename().GetString(),
				sources.GetPathVertexSourceCode().GetString());
			DETHROW(deeInvalidParam);
		}
	}
	
	if(!sources.GetPathFragmentSourceCode().IsEmpty()){
		program.SetUnitFragment(pGetProgramUnitWithLocked(
			sources.GetPathFragmentSourceCode(), defines));
		
		if(!program.GetUnitFragment()){
			pRenderThread.GetLogger().LogErrorFormat(
				"Shader(%s): Fragment source '%s' not found", sources.GetFilename().GetString(),
				sources.GetPathFragmentSourceCode().GetString());
			DETHROW(deeInvalidParam);
		}
	}
	
	if(!sources.GetPathComputeSourceCode().IsEmpty()){
		program.SetUnitCompute(pGetProgramUnitWithLocked(
			sources.GetPathComputeSourceCode(), defines));
		
		if(!program.GetUnitCompute()){
			pRenderThread.GetLogger().LogErrorFormat(
				"Shader(%s): Compute source '%s' not found", sources.GetFilename().GetString(),
				sources.GetPathComputeSourceCode().GetString());
			DETHROW(deeInvalidParam);
		}
	}
	
	if(!sources.GetPathTessellationControlSourceCode().IsEmpty()){
		program.SetUnitTessellationControl(pGetProgramUnitWithLocked(
			sources.GetPathTessellationControlSourceCode(), defines));
		
		if(!program.GetUnitTessellationControl()){
			pRenderThread.GetLogger().LogErrorFormat(
				"Shader(%s): Tessellation control source '%s' not found",
				sources.GetFilename().GetString(),
				sources.GetPathTessellationControlSourceCode().GetString());
			DETHROW(deeInvalidParam);
		}
	}
	
	if(!sources.GetPathTessellationEvaluationSourceCode().IsEmpty()){
		program.SetUnitTessellationEvaluation(pGetProgramUnitWithLocked(
			sources.GetPathTessellationEvaluationSourceCode(), defines));
		
		if(!program.GetUnitTessellationEvaluation()){
			pRenderThread.GetLogger().LogErrorFormat(
				"Shader(%s): Tessellation evaluation source '%s' not found",
				sources.GetFilename().GetString(),
				sources.GetPathTessellationEvaluationSourceCode().GetString());
			DETHROW(deeInvalidParam);
		}
	}
}

deoglShaderProgramUnit *deoglShaderManager::pGetProgramUnitWithLocked(
const char *name, const deoglShaderDefines &defines){
	const deoglShaderUnitSourceCode * const sources = GetUnitSourceCodeNamed(name);
	if(!sources){
		return nullptr;
	}
	
	deoglShaderProgramUnit * const foundUnit = pGetProgramUnitWith(sources, defines);
	if(foundUnit){
#ifdef DEBUG_SHADER_UNIT_DEFINE_FILTERING
		{
		deoglShaderCompiler sc(*pLanguage, -1);
		sc.PreparePreprocessor(defines);
		sc.AppendPreprocessSourcesBuffer(sources->GetName(), sources->GetSourceCode());
		
		decStringList l(decString(sc.GetPreprocessor().GetSources()).Split('\n'));
		int i=0; bool f=false;
		for(i=0; i<l.GetCount(); i++){
			if(l.GetAt(i).BeginsWith("#define ")){
				l.RemoveFrom(i); i--; f=true;
			}else if(f) break;
		}
		const decString s1(l.Join("\n"));
		
		l = foundUnit->GetProcessedSources().Split('\n');
		i=0; f=false;
		for(i=0; i<l.GetCount(); i++){
			if(l.GetAt(i).BeginsWith("#define ")){
				l.RemoveFrom(i); i--; f=true;
			}else if(f) break;
		}
		const decString s2(l.Join("\n"));
		
		if(s1 != s2){
			const deMutexGuard guardLogging(pMutexLogging);
			pRenderThread.GetLogger().LogErrorFormat("Shader Unit Problem: %s", name);
			
			decString text;
			int d, defineCount = defines.GetDefineCount();
			for(d=0; d<defineCount; d++){
				text.AppendFormat("%s %s=%s", d == 0 ? "" : ",",
					defines.GetDefineNameAt(d).GetString(), defines.GetDefineValueAt(d).GetString());
			}
			pRenderThread.GetLogger().LogErrorFormat("Defines =>%s", text.GetString());
			
			text.Empty();
			defineCount = foundUnit->GetDefines().GetDefineCount();
			for(d=0; d<defineCount; d++){
				text.AppendFormat("%s %s=%s", d == 0 ? "" : ",",
					foundUnit->GetDefines().GetDefineNameAt(d).GetString(),
					foundUnit->GetDefines().GetDefineValueAt(d).GetString());
			}
			pRenderThread.GetLogger().LogErrorFormat("Defines <=%s", text.GetString());
			
			text.Empty();
			defineCount = sources->GetDefines().GetCount();
			for(d=0; d<defineCount; d++){
				text.AppendFormat("%s %s", d == 0 ? "" : ",", sources->GetDefines().GetAt(d).GetString());
			}
			pRenderThread.GetLogger().LogErrorFormat("Filter:%s", text.GetString());
			
			pRenderThread.GetLogger().LogErrorFormat("Sources =>%s", s1.GetString());
			pRenderThread.GetLogger().LogErrorFormat("Sources <=%s", s2.GetString());
		}
		}
#endif
		
		return foundUnit;
	}
	
	const deoglShaderProgramUnit::Ref unit(deoglShaderProgramUnit::Ref::NewWith(
		pRenderThread, sources, defines));
	pProgramUnits.Add(unit);
	return unit;
}

deoglShaderProgram *deoglShaderManager::pGetProgramWith(
const deoglShaderSources *sources, const deoglShaderDefines &defines) const{
	const int count = pPrograms.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deoglShaderProgram * const program = (deoglShaderProgram*)pPrograms.GetAt(i);
		if(sources == program->GetSources() && defines.Equals(program->GetDefines())){
			return program;
		}
	}
	
	return nullptr;
}

deoglShaderManager::cCompileProgram *deoglShaderManager::pGetCompilingWith(
const deoglShaderSources *sources, const deoglShaderDefines &defines) const{
	const int count = pCompilePrograms.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		cCompileProgram * const compiling = (cCompileProgram*)pCompilePrograms.GetAt(i);
		if(sources == compiling->GetProgram()->GetSources()
		&& defines.Equals(compiling->GetProgram()->GetDefines())){
			return compiling;
		}
	}
	
	return nullptr;
}

deoglShaderProgram::Ref deoglShaderManager::pCreateProgram(
const deoglShaderSources *sources, const deoglShaderDefines &defines){
	const deoglShaderProgram::Ref program(deoglShaderProgram::Ref::NewWith(
		pRenderThread, sources, defines));
	
	pResolveProgramUnitsLocked(program);
	
	decString cacheId;
	cacheId.Format("shader%d;%s;%s", SHADER_CACHE_REVISION,
		sources->GetName().GetString(), defines.CalcCacheId().GetString());
	program->SetCacheId(cacheId);
	
	return program;
}

deoglShaderProgramUnit *deoglShaderManager::pGetProgramUnitWith(
const deoglShaderUnitSourceCode *sources, const deoglShaderDefines &defines) const{
	const int count = pProgramUnits.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deoglShaderProgramUnit * const unit = (deoglShaderProgramUnit*)pProgramUnits.GetAt(i);
		if(sources == unit->GetSources() && defines.Equals(unit->GetDefines(), sources->GetDefines())){
			return unit;
		}
	}
	
	return nullptr;
}

void deoglShaderManager::pLoadUnitSourceCodesIn(const char *directory){
	deGraphicOpenGl &ogl = pRenderThread.GetOgl();
	deVirtualFileSystem &vfs = ogl.GetVFS(); // accessed from main thread only
	decPath searchPath, basePath;
	decString filename;
	int i;
	
	basePath.SetFromUnix(pPathShaderSources);
	const int basePathLen = basePath.GetPathUnix().GetLength() + 1;
	
	try{
		deCollectFileSearchVisitor collect("*.shaderSource.xml", true);
		searchPath.SetFrom(basePath);
		searchPath.AddUnixPath(directory);
		vfs.SearchFiles(searchPath, collect);
		
		const dePathList &pathList = collect.GetFiles();
		const int count = pathList.GetCount();
		decString validationString;
		
		for(i=0; i<count; i++){
			const decPath &path = pathList.GetAt(i);
			filename = path.GetPathUnix().GetMiddle(basePathLen);
			
			/*if(ogl.GetConfiguration()->GetDoLogDebug()){
				pRenderThread.GetLogger().LogInfoFormat("Loading shader unit source code %s...", filename);
			}*/
			
			const deoglShaderUnitSourceCode::Ref sources(
				deoglShaderUnitSourceCode::Ref::NewWith(ogl, path));
			
			DEASSERT_FALSE(pUnitSourceCodes.Has(sources->GetName()))
			
			pUnitSourceCodes.SetAt(sources->GetName(), sources);
			pCacheValidationString.Add(sources->GetValidationString());
		}
		
	}catch(const deException &){
		pRenderThread.GetLogger().LogInfoFormat(
			"Loading shader unit source code %s failed!", filename.GetString());
		throw;
	}
}

void deoglShaderManager::pLoadIncludableSourcesIn(const char *directory){
	deGraphicOpenGl &ogl = pRenderThread.GetOgl();
	deVirtualFileSystem &vfs = ogl.GetVFS(); // accessed from main thread only
	decBaseFileReader::Ref reader;
	decPath searchPath, basePath;
	decString filename, source;
	int i;
	
	basePath.SetFromUnix(pPathShaderSources);
	const int basePathLen = basePath.GetPathUnix().GetLength() + 1;
	
	try{
		deCollectFileSearchVisitor collect("*.glsl", true);
		searchPath.SetFrom(basePath);
		searchPath.AddUnixPath(directory);
		vfs.SearchFiles(searchPath, collect);
		
		const dePathList &pathList = collect.GetFiles();
		const int count = pathList.GetCount();
		
		for(i=0; i<count; i++){
			const decPath &path = pathList.GetAt(i);
			filename = path.GetPathUnix().GetMiddle(basePathLen);
			
			/*if(ogl.GetConfiguration()->GetDoLogDebug()){
				pRenderThread.GetLogger().LogInfoFormat("Loading includable source %s...", filename);
			}*/
			
			reader = vfs.OpenFileForReading(path);
			const int length = reader->GetLength();
			source.Set(' ', length);
			reader->Read((char*)source.GetString(), length);
			pIncludableSources.SetAt(filename, source);
		}
		
	}catch(const deException &){
		pRenderThread.GetLogger().LogInfoFormat(
			"Loading includable source %s failed!", filename.GetString());
		throw;
	}
}

void deoglShaderManager::pLoadSourcesIn(const char *directory){
	deGraphicOpenGl &ogl = pRenderThread.GetOgl();
	deLogger &logger = *ogl.GetGameEngine()->GetLogger();
	deVirtualFileSystem &vfs = ogl.GetVFS(); // accessed from main thread only
	decString filename;
	decPath searchPath;
	decPath basePath;
	int i;
	
	// search for shaders in the current directory
	basePath.SetFromUnix(pPathShaders.GetString());
	const int basePathLen = (int)strlen(basePath.GetPathUnix()) + 1;
	
	try{
		deCollectFileSearchVisitor collect("*.shader.xml", true);
		searchPath.SetFrom(basePath);
		searchPath.AddUnixPath(directory);
		vfs.SearchFiles(searchPath, collect);
		
		const dePathList &pathList = collect.GetFiles();
		const int count = pathList.GetCount();
		decString validationString;
		
		for(i=0; i<count; i++){
			const decPath &path = pathList.GetAt(i);
			filename = path.GetPathUnix().GetMiddle(basePathLen);
			/*if( ogl.GetConfiguration()->GetDoLogDebug() ){
				pRenderThread.GetLogger().LogInfoFormat("Loading shader %s...", filename);
			}*/
			
			const decBaseFileReader::Ref reader(vfs.OpenFileForReading(path));
			
			const deoglShaderSources::Ref sources(deoglShaderSources::Ref::NewWith(logger, reader));
			
			if(pSources.Has(sources->GetName())){
				ogl.LogErrorFormat("Shader file '%s' defines a shader named '%s' but"
					" a shader with this name already exists!",
					sources->GetFilename().GetString(), sources->GetName().GetString());
				DETHROW(deeInvalidParam);
			}
			
			pSources.SetAt(sources->GetName(), sources);
			
			validationString.Format("%s: %" PRIu64, filename.GetString(),
				(uint64_t)reader->GetModificationTime());
			pCacheValidationString.Add(validationString);
		}
		
	}catch(const deException &){
		pRenderThread.GetLogger().LogInfoFormat("Loading shader %s failed!", filename.GetString());
		throw;
	}
}
