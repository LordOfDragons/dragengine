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

#include "deoglShaderCompiler.h"
#include "../deoglShaderLanguage.h"
#include "../deoglShaderCompiled.h"
#include "../deoglShaderSources.h"
#include "../deoglShaderDefines.h"
#include "../deoglShaderProgram.h"
#include "../deoglShaderUnitSourceCode.h"
#include "../deoglShaderManager.h"
#include "../deoglShaderSourceLocation.h"
#include "../deoglShaderPreprocessorSymbol.h"
#include "../../deoglCaches.h"
#include "../../deGraphicOpenGl.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../extensions/deoglExtensions.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../renderthread/deoglRTShader.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/filesystem/deCacheHelper.h>
#include <dragengine/parallel/deParallelProcessing.h>
#include <dragengine/threading/deMutexGuard.h>


#ifdef OS_ANDROID
// #define PRINT_SHADERS
// #define PRINT_ALL_SHADERS
// #define PRINT_COMPILING
#endif

// #define PRINT_SHADERS
// #define PRINT_ALL_SHADERS
// #define PRINT_SHADERS_SPECIAL_MODE
// #define PRINT_COMPILING



#ifdef PRINT_SHADERS

#ifdef PRINT_SHADERS_SPECIAL_MODE
#include <dragengine/common/file/decDiskFileWriter.h>
class cSpecialPrintShader{
public:
	decBaseFileWriter::Ref writer;
	decString sourceVertex;
	decString sourceGeometry;
	decString sourceFragment;
	int count;
	
	cSpecialPrintShader() : count(0){
		writer.TakeOver(new decDiskFileWriter("specialPrintShader.h", false));
		writer->WriteString("#pragma once\n");
		writer->WriteString("#include <stddef.h>\n");
		writer->WriteString("\n");
		writer->WriteString("struct sShaderSource{\n");
		writer->WriteString("   const char *vertex;\n");
		writer->WriteString("   const char *geometry;\n");
		writer->WriteString("   const char *fragment;\n");
		writer->WriteString("};\n");
		writer->WriteString("\n");
		writer->WriteString("extern const sShaderSource vShaderSources[];\n");
		writer->WriteString("extern const unsigned int vShaderSourceCount;\n");
		
		writer.TakeOver(new decDiskFileWriter("specialPrintShader.cpp", false));
		writer->WriteString("#include \"specialPrintShader.h\"\n");
		writer->WriteString("\n");
		writer->WriteString("const sShaderSource vShaderSources[] = {");
	}
	
	~cSpecialPrintShader(){
		writer->WriteString("};\n");
		decString text;
		text.Format("const unsigned int vShaderSourceCount = %d;\n", count);
		writer->WriteString(text);
	}
	
	void PrintShader(){
		writer->WriteString(count>0 ? ",\n" : "\n");
		writer->WriteString("   sShaderSource{\n");
		writer->WriteString("      .vertex = R\"(");
		writer->WriteString(sourceVertex ? sourceVertex : "");
		writer->WriteString(")\",\n");
		writer->WriteString("      .geometry = R\"(");
		writer->WriteString(sourceGeometry ? sourceGeometry : "");
		writer->WriteString(")\",\n");
		writer->WriteString("      .fragment = R\"(");
		writer->WriteString(sourceFragment ? sourceFragment : "");
		writer->WriteString(")\"\n");
		writer->WriteString("}");
		count++;
		sourceVertex.Empty();
		sourceGeometry.Empty();
		sourceFragment.Empty();
	}
};
static cSpecialPrintShader vSpecialPrintShader;
#endif

static const char * const vPsfFragment = "F DefRen Light";
static const char * const vPsfVertex = "vertex/defren/light/light.glsl";
static const char * const vPsfDefines[] = {
	"HIGH_PRECISION",
	"HIGHP",
	"UBO",
	"POINT_LIGHT",
	"MATERIAL_NORMAL_INTBASIC",
	"HW_DEPTH_COMPARE",
	"SHAMAT2_EQUALS_SHAMAT1",
	"WITH_SUBSURFACE",
	"PCF_9TAP",
	"SMA1_CUBE",
	"SMA2_CUBE",
	NULL
};

static bool psfMatchesDefines(const deoglShaderProgram &program){
	#ifndef PRINT_ALL_SHADERS
	const char **nextDefine = (const char**)&vPsfDefines;
	while(*nextDefine){
		if(!program.GetDefines().HasDefineNamed(*nextDefine)){
			return false;
		}
		nextDefine++;
	}
	
	const int count = program.GetDefines().GetDefineCount();
	int i;
	for(i=0; i<count; i++){
		const char * const defineName = program.GetDefines().GetDefineNameAt(i);
		
		nextDefine = (const char**)&vPsfDefines;
		while(*nextDefine){
			if(strcmp(*nextDefine, defineName) == 0){
				break;
			}
			nextDefine++;
		}
		
		if(!*nextDefine){
			return false;
		}
	}
	#endif
	return true;
}

static bool psfMatchesFragment(const deoglShaderProgram &program){
	if(!program.GetFragmentSourceCode()){
		return false;
	}
	#ifndef PRINT_ALL_SHADERS
	if(strcmp(program.GetFragmentSourceCode()->GetFilePath(), vPsfFragment) != 0){
		return false;
	}
	#endif
	return psfMatchesDefines(program);
}

static bool psfMatchesVertex(const deoglShaderProgram &program){
	if(!program.GetVertexSourceCode()){
		return false;
	}
	#ifndef PRINT_ALL_SHADERS
	if(strcmp(program.GetVertexSourceCode()->GetFilePath(), vPsfVertex) != 0){
		return false;
	}
	#endif
	return psfMatchesDefines(program);
}

static bool psfMatchesLink(const deoglShaderProgram &program){
	#ifndef PRINT_ALL_SHADERS
	if(strlen(vPsfFragment) > 0 && !psfMatchesFragment(program)){
		return false;
	}
	if(strlen(vPsfVertex) > 0 && !psfMatchesVertex(program)){
		return false;
	}
	#endif
	return true;
}

#endif  // PRINT_SHADERS


#define SHADER_CACHE_REVISION 1


class cGuardLoadingShader{
private:
	deoglShaderLanguage &pLanguage;
	
public:
	cGuardLoadingShader(deoglShaderLanguage &language) : pLanguage(language){
		pLanguage.AddLoadingShader();
	}
	
	~cGuardLoadingShader(){
		pLanguage.RemoveLoadingShader();
	}
};


#define SC_OGL_CHECK(renderThread,cmd) OGL_CHECK_WRTC(renderThread, pContextIndex == -1, cmd)


// Class deoglShaderCompiler::cCacheShader
////////////////////////////////////////////

deoglShaderCompiler::cCacheShader::cCacheShader(deoglRenderThread &renderThread,
	deMutex &mutexLogging, int contextIndex, const deoglShaderProgram &program,
	const deoglShaderCompiled &compiled) :
pRenderThread(renderThread),
pMutexLogging(mutexLogging),
pContextIndex(contextIndex),
pCacheId(program.GetCacheId()),
pLength(0),
pFormat(0)
{
	(void)pMutexLogging;
	const GLuint handler = compiled.GetHandleShader();
	SC_OGL_CHECK(renderThread, pglGetProgramiv(handler, GL_PROGRAM_BINARY_LENGTH, &pLength));
	DEASSERT_TRUE(pLength > 0)
	
	pData.Set(' ', pLength);
	
	SC_OGL_CHECK(renderThread, pglGetProgramBinary(handler,
		pLength, nullptr, &pFormat, (void*)pData.GetString()));
}

void deoglShaderCompiler::cCacheShader::Run(){
#ifdef WITH_DEBUG
	deoglRTLogger &logger = pRenderThread.GetLogger();
#endif
	deoglCaches &caches = pRenderThread.GetOgl().GetCaches();
	deCacheHelper &cacheShaders = caches.GetShaders();
#ifdef WITH_DEBUG
	decTimer timerElapsed;
#endif
	
	{
	decBaseFileWriter::Ref writer;
	{
	const deMutexGuard guard(caches.GetMutex());
	writer.TakeOver(cacheShaders.Write(pCacheId));
	}
	
	writer->WriteByte(SHADER_CACHE_REVISION);
	writer->WriteUInt(pFormat);
	writer->WriteUInt(pLength);
	writer->Write(pData.GetString(), pLength);
	}
	
#ifdef WITH_DEBUG
	const deMutexGuard guardLogging(pMutexLogging);
	logger.LogInfoFormat(
		"CompileShader %d: Cached shader '%.50s...' in %dms, length %d bytes", pContextIndex,
		pCacheId.GetString(), (int)(timerElapsed.GetElapsedTime() * 1e3f), pLength);
#endif
}


// Class deoglShaderCompiler::cCacheShaderTask
////////////////////////////////////////////////

deoglShaderCompiler::cCacheShaderTask::cCacheShaderTask(deoglRenderThread &renderThread,
	deMutex &mutexLogging, int contextIndex, const deoglShaderProgram &program,
	const deoglShaderCompiled &compiled) :
deParallelTask(&renderThread.GetOgl()),
pCacheShader(renderThread, mutexLogging, contextIndex, program, compiled),
pMutexLogging(mutexLogging)
{
	SetLowPriority(true);
}

void deoglShaderCompiler::cCacheShaderTask::Run(){
	try{
		pCacheShader.Run();
		
	}catch(const deException &e){
		const deMutexGuard guardLogging(pMutexLogging);
		deoglRTLogger &logger = pCacheShader.GetRenderThread().GetLogger();
		logger.LogErrorFormat("CompileShader %d: Failed caching shader '%.50s...'",
			pCacheShader.GetContextIndex(), pCacheShader.GetCacheId().GetString());
		logger.LogException(e);
	}
}

void deoglShaderCompiler::cCacheShaderTask::Finished(){
}

decString deoglShaderCompiler::cCacheShaderTask::GetDebugName() const{
	return "OglCacheShader";
}

decString deoglShaderCompiler::cCacheShaderTask::GetDebugDetails() const{
	decString details;
	details.Format("%.50s...", pCacheShader.GetCacheId().GetString());
	return details;
}


// Class deoglShaderCompiler
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderCompiler::deoglShaderCompiler(deoglShaderLanguage &language, int contextIndex) :
pLanguage(language),
pContextIndex(contextIndex),
pErrorLog(nullptr),
pPreprocessor(language.GetRenderThread()){
}

deoglShaderCompiler::~deoglShaderCompiler(){
	pCleanUp();
}



// Management
///////////////

void deoglShaderCompiler::CompileShaderUnit(deoglShaderProgramUnit &unit){
// 	renderThread.GetLogger().LogInfoFormat("CompileShaderUnit: cacheId='%s' cacheId.len=%d",
// 		unit.GetCacheId().GetString(), unit.GetCacheId().GetLength());
	try{
		pLanguage.AddCompilingShader();
		pCompileShaderUnit(unit);
		
	}catch(const deException &){
		unit.compilingFailed = true;
		unit.DropHandle();
		pLanguage.RemoveCompilingShader();
	}
}

bool deoglShaderCompiler::HasCompileShaderUnitFinished(const deoglShaderProgramUnit &unit){
	if(!pglMaxShaderCompilerThreads){
		return true;
	}
	
	GLint result;
	SC_OGL_CHECK(pLanguage.GetRenderThread(),
		pglGetShaderiv(unit.GetHandle(), GL_COMPLETION_STATUS_KHR, &result));
	return result == GL_TRUE;
}

void deoglShaderCompiler::FinishCompileShaderUnit(deoglShaderProgramUnit &unit){
	if(unit.compilingFailed){
		return;
	}
	
	try{
		pFinishCompileShaderUnit(unit);
		
	}catch(const deException &){
		unit.compilingFailed = true;
		unit.DropHandle();
	}
	pLanguage.RemoveCompilingShader();
}

void deoglShaderCompiler::LoadCachedShader(deoglShaderProgram &program){
	pCacheLoadShader(program);
}

void deoglShaderCompiler::CompileShader(deoglShaderProgram &program){
// 	renderThread.GetLogger().LogInfoFormat("CompileShader: cacheId='%s' cacheId.len=%d",
// 		program.GetCacheId().GetString(), program.GetCacheId().GetLength());
	if(program.ready){
		return;
	}
	
	try{
		pLanguage.AddCompilingShader();
		pCompileShader(program);
		
	}catch(const deException &){
		program.SetCompiled(nullptr);
		program.ready = false;
		pLanguage.RemoveCompilingShader();
		throw;
	}
}

bool deoglShaderCompiler::HasCompileShaderFinished(const deoglShaderProgram &program){
	if(!pglMaxShaderCompilerThreads || program.ready){
		return true;
	}
	
	DEASSERT_NOTNULL(program.GetCompiled())
	
	GLint result;
	SC_OGL_CHECK(pLanguage.GetRenderThread(), pglGetProgramiv(
		program.GetCompiled()->GetHandleShader(), GL_COMPLETION_STATUS_KHR, &result));
	return result == GL_TRUE;
}

void deoglShaderCompiler::FinishCompileShader(deoglShaderProgram &program){
	if(program.ready){
		return;
	}
	
	try{
		pFinishCompileShader(program);
		
	}catch(const deException &){
		program.SetCompiled(nullptr);
		program.ready = false;
	}
	pLanguage.RemoveCompilingShader();
	
	if(program.ready){
		pCacheSaveShader(program);
	}
}


// Private Functions
//////////////////////

void deoglShaderCompiler::pCleanUp(){
	if(pErrorLog){
		delete [] pErrorLog;
	}
}

void deoglShaderCompiler::pCompileShaderUnit(deoglShaderProgramUnit &unit){
	const deMutexGuard guard(pMutexCompile);
	const deoglShaderUnitSourceCode * const sources = unit.GetSources();
	deoglRenderThread &renderThread = pLanguage.GetRenderThread();
	#ifdef WITH_DEBUG
	deoglRTLogger &logger = renderThread.GetLogger();
	#endif
	
	#ifdef PRINT_COMPILING
	{
	decString debugText;
	debugText.AppendFormat("compiling '%s'", sources->GetName().GetString());
	debugText.Append(" defines(");
	int i;
	for(i=0; i<unit.GetDefines().GetDefineCount(); i++){
		if(i > 0){
			debugText.Append(" ");
		}
		debugText.AppendFormat("%s=%s", unit.GetDefines().GetDefineNameAt(i).GetString(),
			unit.GetDefines().GetDefineValueAt(i).GetString());
	}
	debugText.Append(")");
	logger.LogInfo(debugText.GetString());
	}
	#endif
	
	#ifdef WITH_DEBUG
	{
	const deMutexGuard guardLogs(renderThread.GetShader().GetShaderManager().GetMutexLogging());
	logger.LogInfoFormat("CompileShaderUnit %d: Compile shader unit for '%s...'", pContextIndex,
		sources->GetName().GetString());
	}
	unit.GetTimerCompile().Reset();
	#endif
	
	unit.CreateHandle();
	GLuint handle = unit.GetHandle();
	DEASSERT_NOTNULL(handle)
	
	PreparePreprocessor(unit);
	AppendPreprocessSourcesBuffer(sources->GetName(), sources->GetSourceCode());
	unit.SetProcessedSources(pPreprocessor.GetSources());
	unit.SetProcessedSourceLocations(pPreprocessor.GetSourceLocations());
	
	const char *ssources = pPreprocessor.GetSources();
	int ssourcesLen = pPreprocessor.GetSourcesLength();
	
	SC_OGL_CHECK(renderThread, pglShaderSource(handle, 1, &ssources, &ssourcesLen));
	SC_OGL_CHECK(renderThread, pglCompileShader(handle));
}

void deoglShaderCompiler::pFinishCompileShaderUnit(deoglShaderProgramUnit &unit){
	const deMutexGuard guard(pMutexCompile);
	deoglRenderThread &renderThread = pLanguage.GetRenderThread();
	deoglRTLogger &logger = renderThread.GetLogger();
	
	const bool result = pFinishCompileObject(unit.GetHandle());
	if(result && !pErrorLog){
		#ifdef WITH_DEBUG
		const deMutexGuard guardLogs(renderThread.GetShader().GetShaderManager().GetMutexLogging());
		logger.LogInfoFormat("CompileShaderUnit %d: Compiled shader unit for '%s...' in %.2fms",
			pContextIndex, unit.GetSources()->GetName().GetString(),
			unit.GetTimerCompile().GetElapsedTime() * 1e3f);
		#endif
		return;
	}
	
	const deMutexGuard guardLogs(renderThread.GetShader().GetShaderManager().GetMutexLogging());
	if(!result){
		logger.LogError("Shader unit compilation failed:");
		
	}else{
		logger.LogError("Shader unit compilation succeeded with logs:");
	}
	
	logger.LogErrorFormat("  shader unit = %s", unit.GetSources()->GetName().GetString());
	
	if(pErrorLog){
		logger.LogErrorFormat("  compile logs: %s", pErrorLog);
	}
	
	if(!result){
		//pPreprocessor.LogSourceLocationMap();
		pLogFailedShaderSources(unit);
		unit.DropHandle();
		unit.compilingFailed = true;
		DETHROW(deeInvalidParam);
	}
}

void deoglShaderCompiler::pCompileShader(deoglShaderProgram &program){
	const deMutexGuard guard(pMutexCompile);
	deoglRenderThread &renderThread = pLanguage.GetRenderThread();
	#ifdef WITH_DEBUG
	deoglRTLogger &logger = renderThread.GetLogger();
	#endif
	
	deoglShaderProgramUnit * const units[6] = {
		program.GetUnitCompute(),
		program.GetUnitVertex(),
		program.GetUnitTessellationControl(),
		program.GetUnitTessellationEvaluation(),
		program.GetUnitGeometry(),
		program.GetUnitFragment()};
	GLuint handleShader = 0;
	int i;
	
	#ifdef PRINT_COMPILING
	{
	decString debugText("linking");
	for(i=0; i<6; i++){
		if(units[i]){
			debugText.AppendFormat(" '%s", units[i]->GetSources()->GetName().GetString());
		}
	}
	debugText.Append(" defines(");
	for(i=0; i<program.GetDefines().GetDefineCount(); i++){
		if(i > 0){
			debugText.Append(" ");
		}
		debugText.AppendFormat("%s=%s", program.GetDefines().GetDefineNameAt(i).GetString(),
			program.GetDefines().GetDefineValueAt(i).GetString());
	}
	debugText.Append(")");
	logger.LogInfo(debugText.GetString());
	}
	#endif
	
	#ifdef WITH_DEBUG
	{
	const deMutexGuard guardLogs(renderThread.GetShader().GetShaderManager().GetMutexLogging());
	logger.LogInfoFormat("CompileShader %d: Compile shader for '%.50s...'", pContextIndex,
		program.GetCacheId().GetString());
	}
	program.GetTimerCompile().Reset();
	#endif
	
	try{
		// verify units
		for(i=0; i<6; i++){
			if(units[i]){
				DEASSERT_FALSE(units[i]->compilingFailed)
				DEASSERT_NOTNULL(units[i]->GetHandle())
			}
		}
		
		// create program handle
		program.SetCompiled(new deoglShaderCompiled(renderThread));
		handleShader = program.GetCompiled()->GetHandleShader();
		
		// attach units. mutex protected since multiple threads can attach/detach resource
		for(i=0; i<6; i++){
			if(units[i]){
				const deMutexGuard guardUnit(units[i]->GetMutex());
				SC_OGL_CHECK(renderThread, pglAttachShader(handleShader, units[i]->GetHandle()));
			}
		}
		
		// link the shader
		SC_OGL_CHECK(renderThread, pglLinkProgram(handleShader));
		
	}catch(const deException &e){
		if(handleShader){
			pDetachUnits(program, handleShader);
		}
		
		const deMutexGuard guardLogs(renderThread.GetShader().GetShaderManager().GetMutexLogging());
		pLanguage.GetRenderThread().GetLogger().LogException(e);
		
		program.SetCompiled(nullptr);
		program.ready = false;
		throw;
	}
}

void deoglShaderCompiler::pFinishCompileShader(deoglShaderProgram &program){
	DEASSERT_NOTNULL(program.GetCompiled())
	
	const deMutexGuard guard(pMutexCompile);
	const GLuint handleShader = program.GetCompiled()->GetHandleShader();
	deoglRenderThread &renderThread = pLanguage.GetRenderThread();
	deoglRTLogger &logger = renderThread.GetLogger();
	
	const deoglShaderSources &sources = *program.GetSources();
	deoglShaderProgramUnit * const units[6] = {
		program.GetUnitCompute(),
		program.GetUnitVertex(),
		program.GetUnitTessellationControl(),
		program.GetUnitTessellationEvaluation(),
		program.GetUnitGeometry(),
		program.GetUnitFragment()};
	int i;
	
	try{
		const bool result = pFinishLinkShader(handleShader);
		if(!result || pErrorLog){
			const deMutexGuard guardLogs(renderThread.GetShader().GetShaderManager().GetMutexLogging());
			if(!result){
				logger.LogError("Shader linking failed:");
			}else{
				logger.LogError("Shader linking succeeded with logs:");
			}
			if(sources.GetName().IsEmpty()){
				logger.LogErrorFormat("  shader = %s", program.GetCacheId().GetString());
			}else{
				logger.LogErrorFormat("  shader = %s", sources.GetName().GetString());
			}
			for(i=0; i<6; i++){
				if(units[i]){
					logger.LogErrorFormat("  unit = %s",
						units[i]->GetSources()->GetName().GetString());
				}
			}
			if(pErrorLog){
				logger.LogErrorFormat("  link logs: %s", pErrorLog);
			}
			pLogFailedShader(program);
			if(!result){
				DETHROW(deeInvalidParam);
			}
		}
		
		// after link shader
		pDetachUnits(program, handleShader);
		pAfterLinkShader(program);
		program.ready = true;
		
	}catch(const deException &e){
		pDetachUnits(program, handleShader);
		
		const deMutexGuard guardLogs(renderThread.GetShader().GetShaderManager().GetMutexLogging());
		pLanguage.GetRenderThread().GetLogger().LogException(e);
		program.SetCompiled(nullptr);
		throw;
	}
	
	// finished compiling
	#ifdef WITH_DEBUG
	{
	const deMutexGuard guardLogs(renderThread.GetShader().GetShaderManager().GetMutexLogging());
	logger.LogInfoFormat("CompileShader %d: Compiled shader for '%.50s...' in %.2fms", pContextIndex,
		program.GetCacheId().GetString(), program.GetTimerCompile().GetElapsedTime() * 1e3f);
	}
	#endif
}

void deoglShaderCompiler::pAfterLinkShader(const deoglShaderProgram& program){
	DEASSERT_NOTNULL(program.GetCompiled())
	
	deoglRenderThread &renderThread = pLanguage.GetRenderThread();
	const deoglShaderSources &sources = *program.GetSources();
	deoglShaderCompiled &compiled = *program.GetCompiled();
	const GLuint handleShader = compiled.GetHandleShader();
	int i, count, location;
	
	// for the rest we have to activate this shader. to avoid upseting the shader tracker
	// the current shader is remembered then restored. if this call is done from inside
	// the render thread then this will properly restore the shader avoiding upseting the
	// shader tracker. if this is not the render thread then this has no negative effect
	// 
	// we can not touch the shader tracker here since we do not know if this call has
	// been done from inside the render thread or not
	GLuint restoreShader;
	SC_OGL_CHECK(renderThread, glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&restoreShader));
	
	try{
		// renderThread.GetShader().ActivateShader( nullptr ); // nope, see above comment
		// compiled.Activate();
		SC_OGL_CHECK(renderThread, pglUseProgram(compiled.GetHandleShader()));
		
		// bind textures
		const deoglShaderBindingList &textureList = sources.GetTextureList();
		count = textureList.GetCount();
		for(i=0; i<count; i++){
			location = pglGetUniformLocation(handleShader, textureList.GetNameAt(i));
			if(location != -1){
				SC_OGL_CHECK(renderThread, pglUniform1i(location, textureList.GetTargetAt(i)));
			}
		}
		
		// resolve parameters
		const decStringList &parameterList = sources.GetParameterList();
		count = parameterList.GetCount();
		compiled.SetParameterCount(count);
		
		#ifdef USE_EXPLICIT_UNIFORM_LOCATIONS
			const decIntList &parameterLocations = sources.GetParameterLocations();
			
			for(i=0; i<count; i++){
				compiled.SetParameterAt(i, parameterLocations.GetAt(i));
			}
			
		#else
			for(i=0; i<count; i++){
				compiled.SetParameterAt(i, pglGetUniformLocation(
					handleShader, parameterList.GetAt(i).GetString()));
			}
		#endif
		
		SC_OGL_CHECK(renderThread, pglUseProgram(restoreShader));
		
	}catch(const deException &){
		SC_OGL_CHECK(renderThread, pglUseProgram(restoreShader));
		throw;
	}
}

void deoglShaderCompiler::pCacheLoadShader(deoglShaderProgram &program){
	deoglRenderThread &renderThread = pLanguage.GetRenderThread();
	
	program.SetCompiled(nullptr);
	program.ready = false;
	
	// NOTE we can not put this decision into deoglRTChoices since shaders are compiled already
	//      during capabilities detection which is before deoglRTChoices is constructed
	if(program.GetCacheId().IsEmpty()){
		return;
	}
	
	if(!pglProgramBinary){
		const deMutexGuard guardLogs(renderThread.GetShader().GetShaderManager().GetMutexLogging());
		renderThread.GetLogger().LogInfoFormat(
			"ShaderLanguage.CacheLoadShader: ARB_get_program_binary not support: '%.50s...'",
			program.GetCacheId().GetString());
		return;
	}
	
	if(renderThread.GetCapabilities().GetNumProgramBinaryFormats() == 0){
		const deMutexGuard guardLogs(renderThread.GetShader().GetShaderManager().GetMutexLogging());
		renderThread.GetLogger().LogInfoFormat(
			"ShaderLanguage.CacheLoadShader: Num program binary formats is 0: '%.50s...'",
			program.GetCacheId().GetString());
		return;
	}
	
	const cGuardLoadingShader guardLoading(pLanguage);
	
	deoglCaches &caches = renderThread.GetOgl().GetCaches();
	deCacheHelper &cacheShaders = caches.GetShaders();
	deoglRTLogger &logger = renderThread.GetLogger();
	
	try{
		decBaseFileReader::Ref reader;
		{
		const deMutexGuard guard(caches.GetMutex());
		reader.TakeOver(cacheShaders.Read(program.GetCacheId()));
		}
		
		// read parameters
		if(!reader){
			const deMutexGuard guardLogs(renderThread.GetShader().GetShaderManager().GetMutexLogging());
			logger.LogInfoFormat(
				"ShaderLanguage.CacheLoadShader: Cached shader not found for '%.50s...'",
				program.GetCacheId().GetString());
			return;
		}
		
		if(reader->ReadByte() != SHADER_CACHE_REVISION){
			// cache file outdated
			reader = nullptr;
			{
			const deMutexGuard guard(caches.GetMutex());
			cacheShaders.Delete(program.GetCacheId());
			}
			const deMutexGuard guardLogs(renderThread.GetShader().GetShaderManager().GetMutexLogging());
			logger.LogInfoFormat(
				"ShaderLanguage.CacheLoadShader: Cache version changed for '%.50s...'. Cache discarded",
				program.GetCacheId().GetString());
			return;
		}
		
		const GLenum format = (GLenum)reader->ReadUInt();
		const GLint length = (GLint)reader->ReadUInt();
		
		// read binary data
		decString data;
		data.Set(' ', length);
		reader->Read((void*)data.GetString(), length);
		
		// create program using binary data
		program.SetCompiled(new deoglShaderCompiled(renderThread));
		
		SC_OGL_CHECK(renderThread, pglProgramBinary(program.GetCompiled()->GetHandleShader(),
			format, data.GetString(), length));
		
		// loading the binary does everything up to the linking step. this also uses
		// opengl calls and thus has to be mutex protected
		pAfterLinkShader(program);
		program.ready = true;
		
		// done
// 		logger.LogInfoFormat(
// 			"ShaderLanguage.CacheLoadShader: Cached shader loaded for '%.50s...'",
// 			program.GetCacheId().GetString());
		
	}catch(const deException &){
		{
		const deMutexGuard guard(caches.GetMutex());
		cacheShaders.Delete(program.GetCacheId());
		}
		
		{
		const deMutexGuard guardLogs(renderThread.GetShader().GetShaderManager().GetMutexLogging());
		logger.LogInfoFormat(
			"ShaderLanguage.CacheLoadShader: Failed loading cached shader '%.50s...'. Cache discarded",
			program.GetCacheId().GetString());
		// logger.LogException(e); // do not spam logs. slows things down
		}
		
		program.SetCompiled(nullptr);
		program.ready = false;
	}
}

void deoglShaderCompiler::pCacheSaveShader(const deoglShaderProgram &program){
	deoglRenderThread &renderThread = pLanguage.GetRenderThread();
	deoglRTLogger &logger = renderThread.GetLogger();
	
	// NOTE we can not put this decision into deoglRTChoices since shaders are compiled already
	//      during capabilities detection which is before deoglRTChoices is constructed
	if(!pglGetProgramBinary || renderThread.GetCapabilities().GetNumProgramBinaryFormats() == 0
	|| program.GetCacheId().IsEmpty()){
		return;
	}
	
	try{
		// for some strange reason on android doing cache writing in a parallel task is 50%
		// slower although some of those cache writes have been measured over 1s. on pc there
		// is not much of difference. for these reasons parallel cache writing is disabled
		
		/*
		const cCacheShaderTask::Ref task(cCacheShaderTask::Ref::New(
			new cCacheShaderTask(renderThread, pContextIndex, program, compiled)));
		renderThread.GetOgl().GetGameEngine()->GetParallelProcessing().AddTask(task);
		*/
		
		DEASSERT_NOTNULL(program.GetCompiled())
		cCacheShaderTask(renderThread, renderThread.GetShader().GetShaderManager().GetMutexLogging(),
			pContextIndex, program, *program.GetCompiled()).Run();
		
	}catch(const deException &e){
		const deMutexGuard guardLogs(renderThread.GetShader().GetShaderManager().GetMutexLogging());
		logger.LogErrorFormat("ShaderLanguage.CacheSaveShader: Failed caching shader '%.50s...'",
			program.GetCacheId().GetString());
		logger.LogException(e);
	}
}

enum class SpecDataType{
	Bool,
	Int
};

static const int vSpecializationCount = 167;
static const struct sSpecialization{
	int index;
	SpecDataType dataType;
	const char *constantName, *defineName, *defaultValue;
} vSpecializations[vSpecializationCount] = {
	// 0-2: LOCAL_SIZE_{XYZ}
	{3, SpecDataType::Bool, "TransformInPlace", "TRANSFORM_INPLACE", "false"},
	{4, SpecDataType::Bool, "WithRayCache", "WITH_RAY_CACHE", "false"},
	{5, SpecDataType::Bool, "GIUseRayCache", "GI_USE_RAY_CACHE", "false"},
	{6, SpecDataType::Bool, "GIRayCastDistanceOnly", "GI_RAYCAST_DISTANCE_ONLY", "false"},
	{7, SpecDataType::Bool, "GIRayCastOccMeshOnly", "GI_RAYCAST_OCCMESH_ONLY", "false"},
	{8, SpecDataType::Bool, "RenderDocDebugGI", "RENDER_DOC_DEBUG_GI", "false"},
	{9, SpecDataType::Int, "GIClearProbesCount", "GI_CLEAR_PROBES_COUNT", "64"},
	{10, SpecDataType::Bool, "MapIrradiance", "MAP_IRRADIANCE", "false"},
	{11, SpecDataType::Int, "RenderPass", "RENDER_PASS", "0"},
	{12, SpecDataType::Bool, "DualOccMap", "DUAL_OCCMAP", "false"},
	{13, SpecDataType::Bool, "EnsureMinSize", "ENSURE_MIN_SIZE", "false"},
	{14, SpecDataType::Bool, "FrustumTest", "FRUSTUM_TEST", "false"},
	{15, SpecDataType::Bool, "RenderDocDebugOccTest", "RENDER_DOC_DEBUG_OCCTEST", "false"},
	{16, SpecDataType::Bool, "WithComputeRenderTask", "WITH_COMPUTE_RENDER_TASK", "false"},
	{17, SpecDataType::Bool, "CullViewFrustum", "CULL_VIEW_FRUSTUM", "false"},
	{18, SpecDataType::Bool, "CullSkyLightFrustum", "CULL_SKY_LIGHT_FRUSTUM", "false"},
	{19, SpecDataType::Bool, "CullSkyLightGIBox", "CULL_SKY_LIGHT_GIBOX", "false"},
	{20, SpecDataType::Bool, "CullTooSmall", "CULL_TOO_SMALL", "false"},
	{21, SpecDataType::Bool, "WriteCullResult", "WRITE_CULL_RESULT", "false"},
	{22, SpecDataType::Bool, "WithOcclusion", "WITH_OCCLUSION", "false"},
	{23, SpecDataType::Bool, "ClearCullResult", "CLEAR_CULL_RESULT", "false"},
	{24, SpecDataType::Bool, "WithCalcLod", "WITH_CALC_LOD", "false"},
	{25, SpecDataType::Bool, "WithTexture", "WITH_TEXTURE", "false"},
	{26, SpecDataType::Bool, "WithRenderWorld", "WITH_RENDER_WORLD", "false"},
	{27, SpecDataType::Bool, "WithMask", "WITH_MASK", "false"},
	{28, SpecDataType::Bool, "InverseDepth", "INVERSE_DEPTH", "false"},
	{29, SpecDataType::Bool, "WithDepth", "WITH_DEPTH", "false"},
	{30, SpecDataType::Bool, "DepthDifferenceWeighting", "DEPTH_DIFFERENCE_WEIGHTING", "false"},
	{31, SpecDataType::Bool, "InputArrayTextures", "INPUT_ARRAY_TEXTURES", "false"},
	{32, SpecDataType::Int, "OutDataSize", "OUT_DATA_SIZE", "4"},
	{33, SpecDataType::Int, "OutDataSwizzle", "OUT_DATA_SWIZZLE", "0"},
	{34, SpecDataType::Int, "TapCount", "TAP_COUNT", "1"},
	{35, SpecDataType::Int, "TexDataSize", "TEX_DATA_SIZE", "1"},
	{36, SpecDataType::Int, "TexDataSwizzle", "TEX_DATA_SWIZZLE", "0"},
	{37, SpecDataType::Bool, "TextureLevel", "TEXTURELEVEL", "false"},
	{38, SpecDataType::Bool, "NoTexCoord", "NO_TEXCOORD", "false"},
	{39, SpecDataType::Bool, "VSRenderLayer", "VS_RENDER_LAYER", "false"},
	{40, SpecDataType::Int, "LayeredRendering", "LAYERED_RENDERING", "0"},
	{41, SpecDataType::Bool, "FullScreenQuad", "FULLSCREENQUAD", "false"},
	{42, SpecDataType::Bool, "FullScreenQuadSCTransform", "FULLSCREENQUAD_SCTRANSFORM", "false"},
	{43, SpecDataType::Bool, "FullScreenQuadTCTransform", "FULLSCREENQUAD_TCTRANSFORM", "false"},
	{44, SpecDataType::Bool, "NoPosTransform", "NO_POSTRANSFORM", "false"},
	{45, SpecDataType::Bool, "NoTCTransform", "NO_TCTRANSFORM", "false"},
	{46, SpecDataType::Bool, "TexCoordFlipY", "TEXCOORD_FLIP_Y", "false"},
	{47, SpecDataType::Int, "LightMode", "LIGHT_MODE", "0"},
	{48, SpecDataType::Bool, "NoiseTap", "NOISE_TAP", "false"},
	{49, SpecDataType::Int, "PcfMode", "PCF_MODE", "0"},
	{50, SpecDataType::Bool, "AmbientLighting", "AMBIENT_LIGHTING", "false"},
	{51, SpecDataType::Bool, "GIRay", "GI_RAY", "false"},
	{52, SpecDataType::Int, "Shadow1Mode", "SHADOW1_MODE", "0"},
	{53, SpecDataType::Int, "Shadow2Mode", "SHADOW2_MODE", "0"},
	{54, SpecDataType::Bool, "ShadowInverseDepth", "SHADOW_INVERSE_DEPTH", "false"},
	{55, SpecDataType::Bool, "ShaMat2EqualsShaMat1", "SHAMAT2_EQUALS_SHAMAT1", "false"},
	{56, SpecDataType::Int, "TextureLightColor", "TEXTURE_LIGHT_COLOR", "0"},
	{57, SpecDataType::Bool, "TextureShadow1Solid", "TEXTURE_SHADOW1_SOLID", "false"},
	{58, SpecDataType::Bool, "TextureShadow1Transparent", "TEXTURE_SHADOW1_TRANSPARENT", "false"},
	{59, SpecDataType::Bool, "TextureShadow1Ambient", "TEXTURE_SHADOW1_AMBIENT", "false"},
	{60, SpecDataType::Bool, "TextureShadow2Solid", "TEXTURE_SHADOW2_SOLID", "false"},
	{61, SpecDataType::Bool, "TextureShadow2Transparent", "TEXTURE_SHADOW2_TRANSPARENT", "false"},
	{62, SpecDataType::Bool, "TextureShadow2Ambient", "TEXTURE_SHADOW2_AMBIENT", "false"},
	{63, SpecDataType::Bool, "WithSubsurface", "WITH_SUBSURFACE", "false"},
	{64, SpecDataType::Int, "MaterialNormalDec", "MATERIAL_NORMAL_DEC", "0"},
	{65, SpecDataType::Int, "MaterialNormalEnc", "MATERIAL_NORMAL_ENC", "0"},
	{66, SpecDataType::Bool, "DepthDistance", "DEPTH_DISTANCE", "false"},
	{67, SpecDataType::Int, "ParticleMode", "PARTICLE_MODE", "0"},
	{68, SpecDataType::Int, "SSAOResolutionCount", "SSAO_RESOLUTION_COUNT", "1"},
	{69, SpecDataType::Bool, "DepthCubeMap", "DEPTH_CUBEMAP", "false"},
	{70, SpecDataType::Bool, "DepthInput", "DEPTH_INPUT", "false"},
	{71, SpecDataType::Bool, "AmbientMap", "AMBIENT_MAP", "false"},
	{72, SpecDataType::Bool, "CopyColor", "COPY_COLOR", "false"},
	{73, SpecDataType::Bool, "EncodedDepth", "ENCODED_DEPTH", "false"},
	{74, SpecDataType::Int, "DepthTest", "DEPTH_TEST", "0"},
	{75, SpecDataType::Bool, "DepthOrthogonal", "DEPTH_ORTHOGONAL", "false"},
	{76, SpecDataType::Bool, "WithShadowMap", "WITH_SHADOWMAP", "false"},
	{77, SpecDataType::Bool, "DepthOffset", "DEPTH_OFFSET", "false"},
	{78, SpecDataType::Bool, "UseClipPlane", "USE_CLIP_PLANE", "false"},
	{79, SpecDataType::Bool, "PerspectiveToLinear", "PERSPECTIVE_TO_LINEAR", "false"},
	{80, SpecDataType::Int, "GeometryMode", "GEOMETRY_MODE", "0"},
	{81, SpecDataType::Bool, "TextureColor", "TEXTURE_COLOR", "false"},
	{82, SpecDataType::Bool, "TextureColorTintMask", "TEXTURE_COLOR_TINT_MASK", "false"},
	{83, SpecDataType::Bool, "TextureTransparency", "TEXTURE_TRANSPARENCY", "false"},
	{84, SpecDataType::Bool, "TextureSolidity", "TEXTURE_SOLIDITY", "false"},
	{85, SpecDataType::Bool, "TextureNormal", "TEXTURE_NORMAL", "false"},
	{86, SpecDataType::Bool, "TextureHeight", "TEXTURE_HEIGHT", "false"},
	{87, SpecDataType::Bool, "TextureReflectivity", "TEXTURE_REFLECTIVITY", "false"},
	{88, SpecDataType::Bool, "TextureRoughness", "TEXTURE_ROUGHNESS", "false"},
	{89, SpecDataType::Bool, "TextureEnvMap", "TEXTURE_ENVMAP", "false"},
	{90, SpecDataType::Bool, "TextureEnvMapEqui", "TEXTURE_ENVMAP_EQUI", "false"},
	{91, SpecDataType::Bool, "TextureEnvMapFade", "TEXTURE_ENVMAP_FADE", "false"},
	{92, SpecDataType::Bool, "TextureEmissivity", "TEXTURE_EMISSIVITY", "false"},
	{93, SpecDataType::Bool, "TextureAbsorption", "TEXTURE_ABSORPTION", "false"},
	{94, SpecDataType::Bool, "TextureRenderColor", "TEXTURE_RENDERCOLOR", "false"},
	{95, SpecDataType::Bool, "TextureRefractionDistort", "TEXTURE_REFRACTION_DISTORT", "false"},
	{96, SpecDataType::Bool, "TextureAO", "TEXTURE_AO", "false"},
	{97, SpecDataType::Bool, "TextureEnvRoom", "TEXTURE_ENVROOM", "false"},
	{98, SpecDataType::Bool, "TextureEnvRoomMask", "TEXTURE_ENVROOM_MASK", "false"},
	{99, SpecDataType::Bool, "TextureEnvRoomEmissivity", "TEXTURE_ENVROOM_EMISSIVITY", "false"},
	{100, SpecDataType::Bool, "TextureRimEmissivity", "TEXTURE_RIM_EMISSIVITY", "false"},
	{101, SpecDataType::Bool, "TextureNonPbrAlbedo", "TEXTURE_NONPBR_ALBEDO", "false"},
	{102, SpecDataType::Bool, "TextureNonPbrMetalness", "TEXTURE_NONPBR_METALNESS", "false"},
	{103, SpecDataType::Bool, "WithVariations", "WITH_VARIATIONS", "false"},
	{104, SpecDataType::Bool, "MaskedSolidity", "MASKED_SOLIDITY", "false"},
	{105, SpecDataType::Bool, "ClipPlane", "CLIP_PLANE", "false"},
	{106, SpecDataType::Bool, "EncodeOutDepth", "ENCODE_OUT_DEPTH", "false"},
	{107, SpecDataType::Bool, "GSRenderCubeCulling", "GS_RENDER_CUBE_CULLING", "false"},
	{108, SpecDataType::Int, "TessellationMode", "TESSELLATION_MODE", "0"},
	{109, SpecDataType::Int, "OutputMode", "OUTPUT_MODE", "0"},
	{110, SpecDataType::Bool, "TPNormalStrength", "TP_NORMAL_STRENGTH", "false"},
	{111, SpecDataType::Bool, "TPRoughnessRemap", "TP_ROUGHNESS_REMAP", "false"},
	{112, SpecDataType::Bool, "UseNormalRoughnessCorrection", "USE_NORMAL_ROUGHNESS_CORRECTION", "false"},
	{113, SpecDataType::Bool, "AmbientLightProbe", "AMBIENT_LIGHT_PROBE", "false"},
	{114, SpecDataType::Bool, "SkinReflections", "SKIN_REFLECTIONS", "false"},
	{115, SpecDataType::Bool, "FadeOutRange", "FADEOUT_RANGE", "false"},
	{116, SpecDataType::Bool, "WithOutline", "WITH_OUTLINE", "false"},
	{117, SpecDataType::Bool, "WithOutlineThicknessScreen", "WITH_OUTLINE_THICKNESS_SCREEN", "false"},
	{118, SpecDataType::Bool, "SkinClipPlane", "SKIN_CLIP_PLANE", "false"},
	{119, SpecDataType::Bool, "DynamicColorTint", "DYNAMIC_COLOR_TINT", "false"},
	{120, SpecDataType::Bool, "DynamicColorGamma", "DYNAMIC_COLOR_GAMMA", "false"},
	{121, SpecDataType::Bool, "DynamicColorSolidityMultiplier", "DYNAMIC_COLOR_SOLIDITY_MULTIPLIER", "false"},
	{122, SpecDataType::Bool, "DynamicAOSolidityMultiplier", "DYNAMIC_AO_SOLIDITY_MULTIPLIER", "false"},
	{123, SpecDataType::Bool, "DynamicTransparencyMultiplier", "DYNAMIC_TRANSPARENCY_MULTIPLIER", "false"},
	{124, SpecDataType::Bool, "DynamicSolidityRemap", "DYNAMIC_SOLIDITY_REMAP", "false"},
	{125, SpecDataType::Bool, "DynamicHeightRemap", "DYNAMIC_HEIGHT_REMAP", "false"},
	{126, SpecDataType::Bool, "DynamicNormalStrength", "DYNAMIC_NORMAL_STRENGTH", "false"},
	{127, SpecDataType::Bool, "DynamicNormalStrengthMultiplier", "DYNAMIC_NORMAL_SOLIDITY_MULTIPLIER", "false"},
	{128, SpecDataType::Bool, "DynamicRoughnessRemap", "DYNAMIC_ROUGHNESS_REMAP", "false"},
	{129, SpecDataType::Bool, "DynamicRoughnessGamma", "DYNAMIC_ROUGHNESS_GAMMA", "false"},
	{130, SpecDataType::Bool, "DynamicRoughnessSolidityMultiplier", "DYNAMIC_ROUGHNESS_SOLIDITY_MULTIPLIER", "false"},
	{131, SpecDataType::Bool, "DynamicReflectivitySolidityMultiplier", "DYNAMIC_REFLECTIVITY_SOLIDITY_MULTIPLIER", "false"},
	{132, SpecDataType::Bool, "DynamicReflectivityMultiplier", "DYNAMIC_REFLECTIVITY_MULTIPLIER", "false"},
	{133, SpecDataType::Bool, "DynamicRefractionDistortStrength", "DYNAMIC_REFRACTION_DISTORT_STRENGTH", "false"},
	{134, SpecDataType::Bool, "DynamicEmissivityIntensity", "DYNAMIC_EMISSIVITY_INTENSITY", "false"},
	{135, SpecDataType::Bool, "DynamicEnvRoomTint", "DYNAMIC_ENVROOM_TINT", "false"},
	{136, SpecDataType::Bool, "DynamicEnvRoomSize", "DYNAMIC_ENVROOM_SIZE", "false"},
	{137, SpecDataType::Bool, "DynamicEnvRoomOffset", "DYNAMIC_ENVROOM_OFFSET", "false"},
	{138, SpecDataType::Bool, "DynamicEnvRoomEmissivityIntensity", "DYNAMIC_ENVROOM_EMISSIVITY_INTENSITY", "false"},
	{139, SpecDataType::Bool, "DynamicVariation", "DYNAMIC_VARIATION", "false"},
	{140, SpecDataType::Bool, "DynamicRimEmissivityIntensity", "DYNAMIC_RIM_EMISSIVITY_INTENSITY", "false"},
	{141, SpecDataType::Bool, "DynamicRimAngle", "DYNAMIC_RIM_ANGLE", "false"},
	{142, SpecDataType::Bool, "DynamicRimExponent", "DYNAMIC_RIM_EXPONENT", "false"},
	{143, SpecDataType::Bool, "DynamicOutlineColor", "DYNAMIC_OUTLINE_COLOR", "false"},
	{144, SpecDataType::Bool, "DynamicOutlineColorTint", "DYNAMIC_OUTLINE_COLOR_TINT", "false"},
	{145, SpecDataType::Bool, "DynamicOutlineThickness", "DYNAMIC_OUTLINE_THICKNESS", "false"},
	{146, SpecDataType::Bool, "DynamicOutlineSolidity", "DYNAMIC_OUTLINE_SOLIDITY", "false"},
	{147, SpecDataType::Bool, "DynamicOutlineEmissivity", "DYNAMIC_OUTLINE_EMISSIVITY", "false"},
	{148, SpecDataType::Bool, "DynamicOutlineEmissivityTint", "DYNAMIC_OUTLINE_EMISSIVITY_TINT", "false"},
	{149, SpecDataType::Bool, "DynamicSkinClipPlane", "DYNAMIC_SKIN_CLIP_PLANE", "false"},
	{150, SpecDataType::Bool, "DynamicSkinClipPlaneBorder", "DYNAMIC_SKIN_CLIP_PLANE_BORDER", "false"},
	
	{151, SpecDataType::Bool, "WithSelector", "WITH_SELECTOR", "false"},
	{152, SpecDataType::Bool, "MipMap", "MIPMAP", "false"},
	{153, SpecDataType::Bool, "ClampTC", "CLAMP_TC", "false"},
	{154, SpecDataType::Bool, "UseMinFunction", "USE_MIN_FUNCTION", "false"},
	{155, SpecDataType::Bool, "SplitLayers", "SPLIT_LAYERS", "false"},
	{156, SpecDataType::Bool, "WithGI", "WITH_GI", "false"},
	{157, SpecDataType::Int, "EnvMapMode", "ENVMAP_MODE", "0"},
	{158, SpecDataType::Bool, "SplitVersion", "SPLIT_VERSION", "false"},
	{159, SpecDataType::Bool, "SplitShiftTC", "SPLIT_SHIFT_TC", "false"},
	{160, SpecDataType::Bool, "UseDepthMipMap", "USE_DEPTH_MIPMAP", "false"},
	{161, SpecDataType::Bool, "ResultAfterFirstLoop", "RESULT_AFTER_FIRST_LOOP", "false"},
	{162, SpecDataType::Bool, "RoughnessTapping", "ROUGHNESS_TAPPING", "false"},
	{163, SpecDataType::Int, "SSRVersion", "SSR_VERSION", "0"},
	{164, SpecDataType::Bool, "Multistepping", "MULTISTEPPING", "false"},
	{165, SpecDataType::Bool, "IntegratedThresholdTest", "INTEGRATED_THRESHOLD_TEST", "false"},
	{166, SpecDataType::Bool, "NestedLoop", "NESTED_LOOP", "false"},
	{167, SpecDataType::Bool, "WithToneMapCurve", "WITH_TONEMAP_CURVE", "false"},
	{168, SpecDataType::Bool, "SampleStereo", "SAMPLE_STEREO", "false"},
	{169, SpecDataType::Bool, "WithReflection", "WITH_REFLECTION", "false"}
};

// Special:
// - GI_RAYCAST_USE_SSBO
// - GS_INSTANCING
// - LAYERED_RENDERING
// - HW_DEPTH_COMPARE
// - SHARED_SPB
// - SHARED_SPB_USE_SSBO
// - SHARED_SPB_PADDING
// - SHARED_SPB_TEXTURE_PADDING
// - SHARED_SPB_TEXTURE_ARRAY_SIZE
// - SPB_SSBO_INSTANCE_ARRAY
// - SPB_INSTANCE_ARRAY_SIZE
// - RESTRICTED_IMAGE_BUFFER_FORMATS
// - BROKEN_R16F_RG16F_IMAGE_FORMAT

// Problem:
// - SHARED_SPB_PADDING: can cause 0 length arrays which is not allowed

void deoglShaderCompiler::PreparePreprocessor(const deoglShaderProgramUnit &unit){
	deoglRenderThread &renderThread = pLanguage.GetRenderThread();
	const deoglExtensions &ext = renderThread.GetExtensions();
	const deoglShaderDefines &defines = unit.GetDefines();
	pPreprocessor.Clear();
	
	// add version
	decString line;
	line.Format("#version %s\n", pLanguage.GetGLSLVersion().GetString());
	pPreprocessor.SourcesAppend(line, false);
	
	// add required extensions
	const int extCount = pLanguage.GetGLSLExtensions().GetCount();
	int i;
	
	for(i=0; i<extCount; i++){
		line.Format("#extension %s : require\n", pLanguage.GetGLSLExtensions().GetAt(i).GetString());
		pPreprocessor.SourcesAppend(line, false);
	}
	
	switch(unit.GetSources()->GetStage()){
	case GL_VERTEX_SHADER:
		// OpenGL extensions would define symbols for these extentions which would work in
		// shaders but our pre-processor does not know about them. so add them manually.
		// this is mainly required for broken intel and nvidia drivers
		if(ext.GetHasExtension(deoglExtensions::ext_ARB_shader_viewport_layer_array)){
			pPreprocessor.SourcesAppend("#extension GL_ARB_shader_viewport_layer_array : require\n", false);
			
		}else if(ext.GetHasExtension(deoglExtensions::ext_AMD_vertex_shader_layer)){
			pPreprocessor.SourcesAppend("#extension GL_AMD_vertex_shader_layer : require\n", false);
		}
		
		if(ext.GetHasExtension(deoglExtensions::ext_ARB_shader_draw_parameters)){
			pPreprocessor.SourcesAppend("#extension GL_ARB_shader_draw_parameters : require\n", false);
		}
		break;
		
	case GL_GEOMETRY_SHADER:
		#if !defined OS_ANDROID && !defined WITH_OPENGLES
		if(ext.SupportsGSInstancing()){
			pPreprocessor.SourcesAppend("#extension GL_ARB_gpu_shader5 : require\n", false);
		}
		#endif
		break;
	}
	
	// add version selection defines
	if(pLanguage.GetGLSLVersionNumber() >= 450){
		pPreprocessor.SetSymbol("GLSL_450", "1");
	}
	
	// add symbols
	pPreprocessor.SetSymbolsFromDefines(defines);
	
	if(!defines.HasDefineNamed("HIGHP")){
		pPreprocessor.SetSymbol("HIGHP", "highp");
	}
	
	if(renderThread.GetCapabilities().GetRestrictedImageBufferFormats()){
		pPreprocessor.SetSymbol("RESTRICTED_IMAGE_BUFFER_FORMATS", "1");
	}
	
	#ifdef OS_ANDROID
	pPreprocessor.SetSymbol("ANDROID", "1");
	pPreprocessor.SetSymbol("OPENGLES", "1");
	pPreprocessor.SetSymbol("ARG_SAMP_HIGHP", "highp");
	pPreprocessor.SetSymbol("ARG_SAMP_MEDP", "mediump");
	pPreprocessor.SetSymbol("ARG_SAMP_LOWP", "lowp");
	//pPreprocessor.SourcesAppend( "float modf( in float x, out float i ){ i=floor(x); return fract(x); }\n" );
	
	#elif defined WITH_OPENGLES
	pPreprocessor.SetSymbol("OPENGLES", "1");
	pPreprocessor.SetSymbol("ARG_SAMP_HIGHP", "highp");
	pPreprocessor.SetSymbol("ARG_SAMP_MEDP", "highp");
	pPreprocessor.SetSymbol("ARG_SAMP_LOWP", "highp");
	
	#else
	pPreprocessor.SetSymbol("ARG_SAMP_HIGHP", "");
	pPreprocessor.SetSymbol("ARG_SAMP_MEDP", "");
	pPreprocessor.SetSymbol("ARG_SAMP_LOWP", "");
	#endif
	
	pPreprocessor.SetSymbol("ARG_CONST", "const");
	
	if(unit.GetSources()->GetStage() == GL_VERTEX_SHADER){
		if(ext.SupportsVSLayer()){
			pPreprocessor.SetSymbol("SUPPORTS_VSLAYER", "1");
		}
		if(ext.GetHasExtension(deoglExtensions::ext_ARB_shader_draw_parameters)){
			pPreprocessor.SetSymbol("SUPPORTS_VSDRAWPARAM", "1");
		}
	}
	
	// specializations
	char specBuf[256];
	
	for(i=0; i<vSpecializationCount; i++){
		const sSpecialization &s = vSpecializations[i];
		const char * const v = defines.GetDefineValueFor(s.defineName, nullptr);
		
		switch(s.dataType){
		case SpecDataType::Bool:
			snprintf(specBuf, sizeof(specBuf), "const bool %s = %s;\n", s.constantName,
				v ? (strcmp(v, "1") == 0 ? "true" : "false") : s.defaultValue);
			break;
			
		case SpecDataType::Int:
			snprintf(specBuf, sizeof(specBuf), "const int %s = %s;\n", s.constantName,
				v ? v : s.defaultValue);
			break;
		}
		pPreprocessor.SourcesAppend(specBuf, false);
	}
}

void deoglShaderCompiler::pDetachUnits(const deoglShaderProgram &program, GLuint handleShader){
	deoglShaderProgramUnit * const units[6] = {
		program.GetUnitCompute(),
		program.GetUnitVertex(),
		program.GetUnitTessellationControl(),
		program.GetUnitTessellationEvaluation(),
		program.GetUnitGeometry(),
		program.GetUnitFragment()};
	
	int i;
	for(i=0; i<6; i++){
		if(units[i]){
			const deMutexGuard guardUnit(units[i]->GetMutex());
			pglDetachShader(handleShader, units[i]->GetHandle());
		}
	}
}



void deoglShaderCompiler::AppendPreprocessSourcesBuffer(const char *inputFile, const char *data){
	pPreprocessor.SourcesAppendProcessed(data, inputFile);
	// layout(location=0) in
}

bool deoglShaderCompiler::pFinishCompileObject(GLuint handle){
	deoglRenderThread &renderThread = pLanguage.GetRenderThread();
	GLint result;
	SC_OGL_CHECK(renderThread, pglGetShaderiv(handle, GL_COMPILE_STATUS, &result));
	
	if(pErrorLog){
		delete [] pErrorLog;
		pErrorLog = nullptr;
	}
	
	GLint blen = 0;
	SC_OGL_CHECK(renderThread, pglGetShaderiv(handle, GL_INFO_LOG_LENGTH , &blen));
	
	if(blen > 1){
		pErrorLog = new char[blen + 1];
		SC_OGL_CHECK(renderThread, pglGetShaderInfoLog(handle, blen, nullptr, pErrorLog));
		pErrorLog[blen] = 0;
		
		if(result == GL_TRUE && strncmp(pErrorLog, "Success", 7) == 0){
			delete [] pErrorLog;
			pErrorLog = nullptr;
		}
	}
	
	return result == GL_TRUE;
}

bool deoglShaderCompiler::pFinishLinkShader(GLuint handle){
	deoglRenderThread &renderThread = pLanguage.GetRenderThread();
	GLint result;
	
	SC_OGL_CHECK(renderThread, pglGetProgramiv(handle, GL_LINK_STATUS, &result));
	
	if(pErrorLog){
		delete [] pErrorLog;
		pErrorLog = nullptr;
	}
	
	GLint blen = 0;
	SC_OGL_CHECK(renderThread, pglGetProgramiv(handle, GL_INFO_LOG_LENGTH, &blen));
	
	if(blen > 1){
		pErrorLog = new char[blen + 1];
		SC_OGL_CHECK(renderThread, pglGetProgramInfoLog(handle, blen, nullptr, pErrorLog));
		pErrorLog[blen] = 0;
		
		if(result == GL_TRUE && strncmp(pErrorLog, "Success", 7) == 0){
			delete [] pErrorLog;
			pErrorLog = nullptr;
		}
	}
	
	return result == GL_TRUE;
}

void deoglShaderCompiler::pLogFailedShaderSources(const deoglShaderProgramUnit &unit){
	// do not mutex guard pMutexLogging. caller guards it already
	deoglRenderThread &renderThread = pLanguage.GetRenderThread();
	deoglRTLogger &logger = renderThread.GetLogger();
	logger.LogErrorFormat("CompileShader %d Failed: >>> Sources >>>", pContextIndex);
	
	const decStringList lines(unit.GetProcessedSources().Split("\n"));
	int i, count = lines.GetCount();
	decString lastMapping;
	
	for(i=0; i<count; i++){
		const deoglShaderSourceLocation * const location =
			pPreprocessor.ResolveSourceLocation(unit.GetProcessedSourceLocations(), i + 1);
		int mapLine = -1;
		
		if(location){
			mapLine = location->GetInputLine();
			
			if(location->GetInputFile() != lastMapping){
				logger.LogErrorFormat("@@@ %s", location->GetInputFile().GetString());
				lastMapping = location->GetInputFile();
			}
		}
		
		logger.LogErrorFormat("%d[%d]: %s", i + 1, mapLine, lines.GetAt(i).GetString());
	}
	
	logger.LogError("<<< End Sources <<<");
}

void deoglShaderCompiler::pLogFailedSymbols(){
	// do not mutex guard pMutexLogging. caller guards it already
	deoglRenderThread &renderThread = pLanguage.GetRenderThread();
	deoglRTLogger &logger = renderThread.GetLogger();
	logger.LogErrorFormat("CompileShader %d Failed: >>> Symbols >>>", pContextIndex);
	
	const decStringList names(pPreprocessor.GetSymbolNames());
	const int count = names.GetCount();
	int i;
	for(i=0; i<count; i++){
		const decString &name = names.GetAt(i);
		logger.LogErrorFormat("- %s: '%s'", name.GetString(),
			pPreprocessor.GetSymbolNamed(name)->GetValue().GetString());
	}
}

void deoglShaderCompiler::pLogFailedShader(const deoglShaderProgram &program){
	// do not mutex guard pMutexLogging. caller guards it already
	deoglRenderThread &renderThread = pLanguage.GetRenderThread();
	deoglRTLogger &logger = renderThread.GetLogger();
	
	const deoglShaderProgramUnit *unit[6] = {
		program.GetUnitCompute(),
		program.GetUnitTessellationControl(),
		program.GetUnitTessellationEvaluation(),
		program.GetUnitVertex(),
		program.GetUnitGeometry(),
		program.GetUnitFragment()};
	
	logger.LogErrorFormat("CompileShader %d Failed: >>> All Sources >>>", pContextIndex);
	
	int i;
	for(i=0; i<6; i++){
		if(!unit[i] || unit[i]->GetProcessedSources().IsEmpty()){
			continue;
		}
		
		logger.LogErrorFormat(">>> %s >>>", unit[i]->GetSources()->GetName().GetString());
		logger.LogError(unit[i]->GetProcessedSources());
		logger.LogErrorFormat("<<< End %s <<<", unit[i]->GetSources()->GetName().GetString());
	}
	
	logger.LogError("<<< End All Sources <<<");
}
