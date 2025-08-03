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
#include <dragengine/common/file/decBaseFileWriterReference.h>
class cSpecialPrintShader{
public:
	decBaseFileWriterReference writer;
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
		writer->WriteString("   }");
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

static bool psfMatchesDefines( const deoglShaderProgram &program ){
	#ifndef PRINT_ALL_SHADERS
	const char **nextDefine = ( const char** )&vPsfDefines;
	while( *nextDefine ){
		if( ! program.GetDefines().HasDefineNamed( *nextDefine ) ){
			return false;
		}
		nextDefine++;
	}
	
	const int count = program.GetDefines().GetDefineCount();
	int i;
	for( i=0; i<count; i++ ){
		const char * const defineName = program.GetDefines().GetDefineNameAt( i );
		
		nextDefine = ( const char** )&vPsfDefines;
		while( *nextDefine ){
			if( strcmp( *nextDefine, defineName ) == 0 ){
				break;
			}
			nextDefine++;
		}
		
		if( ! *nextDefine ){
			return false;
		}
	}
	#endif
	return true;
}

static bool psfMatchesFragment( const deoglShaderProgram &program ){
	if( ! program.GetFragmentSourceCode() ){
		return false;
	}
	#ifndef PRINT_ALL_SHADERS
	if( strcmp( program.GetFragmentSourceCode()->GetFilePath(), vPsfFragment ) != 0 ){
		return false;
	}
	#endif
	return psfMatchesDefines( program );
}

static bool psfMatchesVertex( const deoglShaderProgram &program ){
	if( ! program.GetVertexSourceCode() ){
		return false;
	}
	#ifndef PRINT_ALL_SHADERS
	if( strcmp( program.GetVertexSourceCode()->GetFilePath(), vPsfVertex ) != 0 ){
		return false;
	}
	#endif
	return psfMatchesDefines( program );
}

static bool psfMatchesLink( const deoglShaderProgram &program ){
	#ifndef PRINT_ALL_SHADERS
	if( strlen( vPsfFragment ) > 0 && ! psfMatchesFragment( program ) ){
		return false;
	}
	if( strlen( vPsfVertex ) > 0 && ! psfMatchesVertex( program ) ){
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
	deoglRTLogger &logger = renderThread.GetLogger();
	
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
	deoglRTLogger &logger = renderThread.GetLogger();
	
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
	SC_OGL_CHECK( renderThread, glGetIntegerv( GL_CURRENT_PROGRAM, ( GLint* )&restoreShader ) );
	
	try{
		// renderThread.GetShader().ActivateShader( nullptr ); // nope, see above comment
		// compiled.Activate();
		SC_OGL_CHECK( renderThread, pglUseProgram( compiled.GetHandleShader() ) );
		
		// bind textures
		const deoglShaderBindingList &textureList = sources.GetTextureList();
		count = textureList.GetCount();
		for( i=0; i<count; i++ ){
			location = pglGetUniformLocation( handleShader, textureList.GetNameAt( i ) );
			if( location != -1 ){
				SC_OGL_CHECK( renderThread, pglUniform1i( location, textureList.GetTargetAt( i ) ) );
			}
		}
		
		// resolve parameters
		const decStringList &parameterList = sources.GetParameterList();
		count = parameterList.GetCount();
		compiled.SetParameterCount( count );
		for( i=0; i<count; i++ ){
			compiled.SetParameterAt( i, pglGetUniformLocation( handleShader, parameterList.GetAt( i ).GetString() ) );
		}
		
		SC_OGL_CHECK( renderThread, pglUseProgram( restoreShader ) );
		
	}catch( const deException & ){
		SC_OGL_CHECK( renderThread, pglUseProgram( restoreShader ) );
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

static const int vSpecializationCount = 23;
static const struct sSpecialization{
	int index;
	bool isBool;
	const char *constantName, *defineName;
} vSpecializations[vSpecializationCount] = {
	// 0-2: LOCAL_SIZE_{XYZ}
	{3, true, "TransformInPlace", "TRANSFORM_INPLACE"},
	{4, true, "WithRayCache", "WITH_RAY_CACHE"},
	{5, true, "GIUseRayCache", "GI_USE_RAY_CACHE"},
	{6, true, "GIRayCastDistanceOnly", "GI_RAYCAST_DISTANCE_ONLY"},
	{7, true, "GIRayCastOccMeshOnly", "GI_RAYCAST_OCCMESH_ONLY"},
	{8, true, "RenderDocDebugGI", "RENDER_DOC_DEBUG_GI"},
	{9, false, "GIClearProbesCount", "GI_CLEAR_PROBES_COUNT"},
	{10, true, "MapIrradiance", "MAP_IRRADIANCE"},
	{11, true, "BlurPass2", "BLUR_PASS_2"},
	{12, true, "DecodeInDepth", "DECODE_IN_DEPTH"},
	{13, true, "DualOccMap", "DUAL_OCCMAP"},
	{14, true, "EnsureMinSize", "ENSURE_MIN_SIZE"},
	{15, true, "FrustumTest", "FRUSTUM_TEST"},
	{16, true, "RenderDocDebugOccTest", "RENDER_DOC_DEBUG_OCCTEST"},
	{17, true, "WithComputeRenderTask", "WITH_COMPUTE_RENDER_TASK"},
	{18, true, "CullViewFrustum", "CULL_VIEW_FRUSTUM"},
	{19, true, "CullSkyLightFrustum", "CULL_SKY_LIGHT_FRUSTUM"},
	{20, true, "CullSkyLightGIBox", "CULL_SKY_LIGHT_GIBOX"},
	{21, true, "CullTooSmall", "CULL_TOO_SMALL"},
	{22, true, "WriteCullResult", "WRITE_CULL_RESULT"},
	{23, true, "WithOcclusion", "WITH_OCCLUSION"},
	{24, true, "ClearCullResult", "CLEAR_CULL_RESULT"},
	{25, true, "WithCalcLod", "WITH_CALC_LOD"}
};

// Special:
// - GI_RAYCAST_USE_SSBO

void deoglShaderCompiler::PreparePreprocessor(const deoglShaderProgramUnit &unit){
	deoglRenderThread &renderThread = pLanguage.GetRenderThread();
	const deoglExtensions &ext = renderThread.GetExtensions();
	const deoglShaderDefines &defines = unit.GetDefines();
	pPreprocessor.Clear();
	
	// add version
	decString line;
	line.Format( "#version %s\n", pLanguage.GetGLSLVersion().GetString() );
	pPreprocessor.SourcesAppend( line, false );
	
	// add required extensions
	const int extCount = pLanguage.GetGLSLExtensions().GetCount();
	int i;
	
	for(i=0; i<extCount; i++){
		line.Format("#extension %s : require\n", pLanguage.GetGLSLExtensions().GetAt( i ).GetString());
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
		#if ! defined OS_ANDROID && ! defined WITH_OPENGLES
		if(ext.SupportsGSInstancing()){
			pPreprocessor.SourcesAppend("#extension GL_ARB_gpu_shader5 : require\n", false);
		}
		#endif
		break;
	}
	
	// add version selection defines
	if( pLanguage.GetGLSLVersionNumber() >= 450 ){
		pPreprocessor.SetSymbol( "GLSL_450", "1" );
	}
	
	// add symbols
	pPreprocessor.SetSymbolsFromDefines( defines );
	
	if( ! defines.HasDefineNamed( "HIGHP" ) ){
		pPreprocessor.SetSymbol( "HIGHP", "highp" );
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
	
	// specializations
	static const decString notDefined("0");
	char specBuf[256];
	
	for(i=0; i<vSpecializationCount; i++){
		const sSpecialization &s = vSpecializations[i];
		if(s.isBool){
			snprintf(specBuf, sizeof(specBuf), "const bool %s = %s;\n", s.constantName,
				defines.GetDefineValueFor(s.defineName, notDefined) == "1" ? "true" : "false");
			
		}else{
			snprintf(specBuf, sizeof(specBuf), "const int %s = %s;\n", s.constantName,
				defines.GetDefineValueFor(s.defineName, notDefined).GetString());
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
			pPreprocessor.ResolveSourceLocation(unit.GetProcessedSourceLocations(), i + 1 );
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
