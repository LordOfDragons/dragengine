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

static const char * const vPsfFragment = "fragment/defren/light/light.glsl";
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


class cGuardCompilingShader{
private:
	deoglShaderLanguage &pLanguage;
	
public:
	cGuardCompilingShader(deoglShaderLanguage &language) : pLanguage(language){
		pLanguage.AddCompilingShader();
	}
	
	~cGuardCompilingShader(){
		pLanguage.RemoveCompilingShader();
	}
};


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

deoglShaderCompiled *deoglShaderCompiler::CompileShader(const deoglShaderProgram &program){
// 	renderThread.GetLogger().LogInfoFormat("CompileShader: cacheId='%s' cacheId.len=%d",
// 		program.GetCacheId().GetString(), program.GetCacheId().GetLength());
	
	deoglShaderCompiled *compiled = pCacheLoadShader(program);
	
	if(!compiled){
		compiled = pCompileShader(program);
		if(compiled){
			pCacheSaveShader(program, *compiled);
		}
	}
	
	return compiled;
}


// Private Functions
//////////////////////

void deoglShaderCompiler::pCleanUp(){
	if(pErrorLog){
		delete [] pErrorLog;
	}
}

deoglShaderCompiled *deoglShaderCompiler::pCompileShader(const deoglShaderProgram &program){
	const cOptionalMutexGuard guard(pMutexCompile, pContextIndex == -1);
	const cGuardCompilingShader guardCompiling(pLanguage);
	deoglRenderThread &renderThread = pLanguage.GetRenderThread();
	deoglRTLogger &logger = renderThread.GetLogger();
	
	const deoglExtensions &ext = renderThread.GetExtensions();
	const deoglShaderSources &sources = *program.GetSources();
	const deoglShaderUnitSourceCode * const scCompute = program.GetComputeSourceCode();
	const deoglShaderUnitSourceCode * const scTessellationControl = program.GetTessellationControlSourceCode();
	const deoglShaderUnitSourceCode * const scTessellationEvaluation = program.GetTessellationEvaluationSourceCode();
	const deoglShaderUnitSourceCode * const scGeometry = program.GetGeometrySourceCode();
	const deoglShaderUnitSourceCode * const scVertex = program.GetVertexSourceCode();
	const deoglShaderUnitSourceCode * const scFragment = program.GetFragmentSourceCode();
	const decString &inlscGeometry = sources.GetInlineGeometrySourceCode();
	const decString &inlscVertex = sources.GetInlineVertexSourceCode();
	const decString &inlscFragment = sources.GetInlineFragmentSourceCode();
	const deoglShaderBindingList &inputList = sources.GetAttributeList();
	const deoglShaderBindingList &outputList = sources.GetOutputList();
	const decStringList &feedbackList = sources.GetFeedbackList();
	const bool feedbackInterleaved = sources.GetFeedbackInterleaved();
	GLuint handleShader = 0;
	GLuint handleC = 0;
	GLuint handleTCP = 0;
	GLuint handleTEP = 0;
	GLuint handleGP = 0;
	GLuint handleVP = 0;
	GLuint handleFP = 0;
	deoglShaderCompiled *compiled = NULL;
	int i, count;
	
	#ifdef PRINT_COMPILING
	decString debugText("compiling ");
	if(scCompute){
		debugText.AppendFormat(" comp(%s)", scCompute->GetFilePath().GetString());
	}
	if(scTessellationControl){
		debugText.AppendFormat(" tc(%s)", scTessellationControl->GetFilePath().GetString());
	}
	if(scTessellationEvaluation){
		debugText.AppendFormat(" te(%s)", scTessellationEvaluation->GetFilePath().GetString());
	}
	if(scGeometry){
		debugText.AppendFormat(" geom(%s)", scGeometry->GetFilePath().GetString());
	}
	if(scVertex){
		debugText.AppendFormat(" vert(%s)", scVertex->GetFilePath().GetString());
	}
	if(scFragment){
		debugText.AppendFormat(" frag(%s)", scFragment->GetFilePath().GetString());
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
	#endif
	
	// compile the shader
#ifdef WITH_DEBUG
	{
	const deMutexGuard guardLogs(pMutexLogging);
	logger.LogInfoFormat("CompileShader %d: Compile shader for '%.50s...'", pContextIndex,
		program.GetCacheId().GetString());
	}
	decTimer timerCompile;
#endif
	
	try{
		compiled = new deoglShaderCompiled( renderThread );
		
		// retrieve the shader handle
		handleShader = compiled->GetHandleShader();
		
		// compile compute program if existing
		if( scCompute ){
			compiled->CreateComputeProgram();
			handleC = compiled->GetHandleC();
			if( ! handleC ){
				DETHROW( deeInvalidAction );
			}
			
			pPreparePreprocessor( program.GetDefines() );
			
			if(ext.GetGLESVersion() != deoglExtensions::evglesUnsupported){
				if(ext.GetGLESVersion() < deoglExtensions::evgles3p1){
					pPreprocessor.SourcesAppend( "#extension GL_ARB_compute_shader : require\n", false );
				}
				
			}else{
				if(ext.GetGLVersion() < deoglExtensions::evgl4p3){
					pPreprocessor.SourcesAppend( "#extension GL_ARB_compute_shader : require\n", false );
				}
			}
			
			pAppendPreprocessSourcesBuffer( scCompute->GetFilePath(), scCompute->GetSourceCode() );
			
			#ifdef PRINT_ALL_SHADERS
			{
			const deMutexGuard guardLogs(pMutexLogging);
			logger.LogInfo("COMPILE COMPUTER IN");
			logger.LogInfo(pPreprocessor.GetSources());
			}
			#endif
			const bool result = pCompileObject(handleC);
			if(!result || pErrorLog){
				const deMutexGuard guardLogs(pMutexLogging);
				if(!result){
					logger.LogError("Shader compilation failed:");
				}else{
					logger.LogError("Shader compilation succeeded with logs:");
				}
				logger.LogErrorFormat("  shader file = %s", sources.GetFilename().GetString());
				logger.LogErrorFormat("  compute unit source code file = %s",
					scCompute->GetFilePath().GetString());
				if(pErrorLog){
					logger.LogErrorFormat("  compile logs: %s", pErrorLog);
				}
				if(!result){
					//pPreprocessor.LogSourceLocationMap();
					pLogFailedShaderSources();
					DETHROW(deeInvalidParam);
				}
			}
			SC_OGL_CHECK( renderThread, pglAttachShader( handleShader, handleC ) );
			pLogShaderCompute = pPreprocessor.GetSources();
			
		}else{
			pLogShaderCompute.Empty();
		}
		
		// compile the tessellation control program if existing
		if( scTessellationControl ){
			compiled->CreateTessellationControlProgram();
			handleTCP = compiled->GetHandleTCP();
			if( ! handleTCP ){
				DETHROW( deeInvalidAction );
			}
			
			pPreparePreprocessor( program.GetDefines() );
			
			if( scTessellationControl ){
				pAppendPreprocessSourcesBuffer( scTessellationControl->GetFilePath(),
					scTessellationControl->GetSourceCode() );
			}
			
			#ifdef PRINT_ALL_SHADERS
			{
			const deMutexGuard guardLogs(pMutexLogging);
			logger.LogInfo("COMPILE TESSELLATION CONTROL IN");
			logger.LogInfo(pPreprocessor.GetSources());
			}
			#endif
			const bool result = pCompileObject(handleTCP);
			if(!result || pErrorLog){
				const deMutexGuard guardLogs(pMutexLogging);
				if(!result){
					logger.LogError("Shader compilation failed:");
				}else{
					logger.LogError("Shader compilation succeeded with logs:");
				}
				logger.LogErrorFormat("  shader file = %s", sources.GetFilename().GetString());
				if(scTessellationControl){
					logger.LogErrorFormat("  tessellation control unit source code file = %s",
						scTessellationControl->GetFilePath().GetString());
				}
				if(pErrorLog){
					logger.LogErrorFormat("  compile logs: %s", pErrorLog);
				}
				if(!result){
					//pPreprocessor.LogSourceLocationMap();
					pLogFailedShaderSources();
					DETHROW(deeInvalidParam);
				}
			}
			SC_OGL_CHECK( renderThread, pglAttachShader( handleShader, handleTCP ) );
			pLogShaderTessellationControl = pPreprocessor.GetSources();
			
		}else{
			pLogShaderTessellationControl.Empty();
		}
		
		// compile the tessellation evaluation program if existing
		if( scTessellationEvaluation ){
			compiled->CreateTessellationEvaluationProgram();
			handleTEP = compiled->GetHandleTEP();
			if( ! handleTEP ){
				DETHROW( deeInvalidAction );
			}
			
			pPreparePreprocessor( program.GetDefines() );
			
			if( scTessellationEvaluation ){
				pAppendPreprocessSourcesBuffer( scTessellationEvaluation->GetFilePath(),
					scTessellationEvaluation->GetSourceCode() );
			}
			
			#ifdef PRINT_ALL_SHADERS
			{
			const deMutexGuard guardLogs(pMutexLogging);
			logger.LogInfo("COMPILE TESSELLATION EVALUATION IN");
			logger.LogInfo(pPreprocessor.GetSources());
			}
			#endif
			const bool result = pCompileObject(handleTEP);
			if(!result || pErrorLog){
				const deMutexGuard guardLogs(pMutexLogging);
				if(!result){
					logger.LogError("Shader compilation failed:");
				}else{
					logger.LogError("Shader compilation succeeded with logs:");
				}
				logger.LogErrorFormat("  shader file = %s", sources.GetFilename().GetString());
				if(scTessellationEvaluation){
					logger.LogErrorFormat("  tessellation evaluation unit source code file = %s",
						scTessellationEvaluation->GetFilePath().GetString());
				}
				if(pErrorLog){
					logger.LogErrorFormat("  compile logs: %s", pErrorLog);
				}
				if(!result){
					//pPreprocessor.LogSourceLocationMap();
					pLogFailedShaderSources();
					DETHROW(deeInvalidParam);
				}
			}
			SC_OGL_CHECK( renderThread, pglAttachShader( handleShader, handleTEP ) );
			pLogShaderTessellationEvaluation = pPreprocessor.GetSources();
			
		}else{
			pLogShaderTessellationEvaluation.Empty();
		}
		
		// compile the geometry program if existing
		if( scGeometry || ! inlscGeometry.IsEmpty() ){
			compiled->CreateGeometryProgram();
			handleGP = compiled->GetHandleGP();
			if( ! handleGP ) DETHROW( deeInvalidAction );
			
			pPreparePreprocessor( program.GetDefines() );
			
			if( scGeometry ){
				pAppendPreprocessSourcesBuffer( scGeometry->GetFilePath(), scGeometry->GetSourceCode() );
				
			}else{
				pAppendPreprocessSourcesBuffer( "<inline>", inlscGeometry.GetString() );
			}
			
			#ifdef PRINT_ALL_SHADERS
			{
			const deMutexGuard guardLogs(pMutexLogging);
			logger.LogInfo("COMPILE GEOMETRY IN");
			logger.LogInfo(pPreprocessor.GetSources());
			}
			#ifdef PRINT_SHADERS_SPECIAL_MODE
			vSpecialPrintShader.sourceGeometry = pPreprocessor.GetSources();
			#endif
			#endif
			const bool result = pCompileObject(handleGP);
			if(!result || pErrorLog){
				const deMutexGuard guardLogs(pMutexLogging);
				if(!result){
					logger.LogError("Shader compilation failed:");
				}else{
					logger.LogError("Shader compilation succeeded with logs:");
				}
				logger.LogErrorFormat("  shader file = %s", sources.GetFilename().GetString());
				if(scGeometry){
					logger.LogErrorFormat("  geometry unit source code file = %s",
						scGeometry->GetFilePath().GetString());
				}
				if(pErrorLog){
					logger.LogErrorFormat("  compile logs: %s", pErrorLog);
				}
				if(!result){
					//pPreprocessor.LogSourceLocationMap();
					pLogFailedShaderSources();
					DETHROW(deeInvalidParam);
				}
			}
			#ifdef PRINT_ALL_SHADERS
			logger.LogInfo( "COMPILE GEOMETRY OUT" );
			#endif
			SC_OGL_CHECK( renderThread, pglAttachShader( handleShader, handleGP ) );
			pLogShaderGeometry = pPreprocessor.GetSources();
			
		}else{
			pLogShaderGeometry.Empty();
		}
		
		// compile the vertex program if existing
		if( scVertex || ! inlscVertex.IsEmpty() ){
			compiled->CreateVertexProgram();
			handleVP = compiled->GetHandleVP();
			if( ! handleVP ) DETHROW( deeInvalidAction );
			
			pPreparePreprocessor( program.GetDefines() );
			
			if( scVertex ){
				pAppendPreprocessSourcesBuffer( scVertex->GetFilePath(), scVertex->GetSourceCode() );
				
			}else{
				pAppendPreprocessSourcesBuffer( "<inline>", inlscVertex.GetString() );
			}
			
			#ifdef PRINT_SHADERS
			if( psfMatchesVertex( program ) ){
				const deMutexGuard guardLogs(pMutexLogging);
				logger.LogInfo( "COMPILE VERTEX IN" );
				logger.LogInfo( pPreprocessor.GetSources() );
				#ifdef PRINT_SHADERS_SPECIAL_MODE
				vSpecialPrintShader.sourceVertex = pPreprocessor.GetSources();
				#endif
			}
			#endif
			#ifdef PRINT_ALL_SHADERS
			{
			const deMutexGuard guardLogs(pMutexLogging);
			logger.LogInfo("COMPILE VERTEX IN");
			logger.LogInfo(pPreprocessor.GetSources());
			}
			#endif
			const bool result = pCompileObject(handleVP);
			if(!result || pErrorLog){
				const deMutexGuard guardLogs(pMutexLogging);
				if(!result){
					logger.LogError("Shader compilation failed:");
				}else{
					logger.LogError("Shader compilation succeeded with logs:");
				}
				logger.LogErrorFormat("  shader file = %s", sources.GetFilename().GetString());
				if(scVertex){
					logger.LogErrorFormat("  vertex unit source code file = %s",
						scVertex->GetFilePath().GetString());
				}
				if(pErrorLog){
					logger.LogErrorFormat("  compile logs: %s", pErrorLog);
				}
				if(!result){
					//pPreprocessor.LogSourceLocationMap();
					pLogFailedShaderSources();
					DETHROW(deeInvalidParam);
				}
			}
			#ifdef PRINT_SHADERS
			if( psfMatchesVertex( program ) ){
				const deMutexGuard guardLogs(pMutexLogging);
				logger.LogInfo( "COMPILE VERTEX OUT" );
			}
			#endif
// 			if(scVertex){
// 				decString s(scVertex->GetFilePath());
// 				s = s.GetMiddle(s.FindReverse('/')+1, s.GetLength()-5);
// 				s = decString("vertex_") + s;
// 				pOutputShaderToFile(s);
// 			}
			SC_OGL_CHECK( renderThread, pglAttachShader( handleShader, handleVP ) );
			pLogShaderVertex = pPreprocessor.GetSources();
			
		}else{
			pLogShaderVertex.Empty();
		}
		
		// compiled the fragment program if existing
		if( scFragment || ! inlscFragment.IsEmpty() ){
			compiled->CreateFragmentProgram();
			handleFP = compiled->GetHandleFP();
			if( ! handleFP ) DETHROW( deeInvalidAction );
			
			pPreparePreprocessor( program.GetDefines() );
			
			// for OS_ANDROID (aka OpenGL ES) we are required to set the location of output variables
			// in the shader. assigning from source code is not working. for regular OpenGL this works
			// by assigning from code. unfortunately some AMD windows drivers are buggy and assigning
			// the location of output variables from code is not working. most probably the bug prevents
			// the assignement to take any effect. to counter this huge driver bug the location of output
			// variables is always patched into the sources for all systems.
			if(scFragment){
				pAppendPreprocessSourcesBuffer(scFragment->GetFilePath(), scFragment->GetSourceCode(), &outputList);
				
			}else{
				pAppendPreprocessSourcesBuffer("<inline>", inlscFragment.GetString(), &outputList);
			}
			
			#ifdef PRINT_SHADERS
			if( psfMatchesFragment( program ) ){
				const deMutexGuard guardLogs(pMutexLogging);
				logger.LogInfo( "COMPILE FRAGMENT IN" );
				logger.LogInfo( pPreprocessor.GetSources() );
				#ifdef PRINT_SHADERS_SPECIAL_MODE
				vSpecialPrintShader.sourceFragment = pPreprocessor.GetSources();
				#endif
			}
			#endif
			#ifdef PRINT_ALL_SHADERS
			{
			const deMutexGuard guardLogs(pMutexLogging);
			logger.LogInfo("COMPILE FRAGMENT IN");
			logger.LogInfo(pPreprocessor.GetSources());
			}
			#endif
			const bool result = pCompileObject(handleFP);
			if(!result || pErrorLog){
				const deMutexGuard guardLogs(pMutexLogging);
				if(!result){
					logger.LogError("Shader compilation failed:");
				}else{
					logger.LogError("Shader compilation succeeded with logs:");
				}
				logger.LogErrorFormat("  shader file = %s", sources.GetFilename().GetString());
				if(scFragment){
					logger.LogErrorFormat("  fragment unit source code file = %s",
						scFragment->GetFilePath().GetString());
				}
				if(pErrorLog){
					logger.LogErrorFormat("  compile logs: %s", pErrorLog);
				}
				if(!result){
					//pPreprocessor.LogSourceLocationMap();
					pLogFailedShaderSources();
					DETHROW(deeInvalidParam);
				}
			}
			#ifdef PRINT_SHADERS
			if( psfMatchesFragment( program ) ){
				const deMutexGuard guardLogs(pMutexLogging);
				logger.LogInfo( "COMPILE FRAGMENT OUT" );
			}
			#endif
			SC_OGL_CHECK( renderThread, pglAttachShader( handleShader, handleFP ) );
			pLogShaderFragment = pPreprocessor.GetSources();
			
		}else{
			pLogShaderFragment.Empty();
		}
		
		// bind attribute locations. this has to be done before linking according to ogl
		// specs. furthermore the name is not required to exist. if not existing the
		// output is simple not linked to anything.
		if( pglBindAttribLocation ){
			count = inputList.GetCount();
			for( i=0; i<count; i++ ){
				SC_OGL_CHECK( renderThread, pglBindAttribLocation( handleShader,
					inputList.GetTargetAt( i ), inputList.GetNameAt( i ) ) );
			}
		}
		
		// bind data locations. this has to be done before linking according to ogl specs.
		// furthermore the name is not required to exist. if not existing the output is
		// simple not linked to anything.
		
		// for OS_ANDROID (aka OpenGL ES) we are required to set the location of output variables
		// in the shader. assigning from source code is not working. for regular OpenGL this works
		// by assigning from code. unfortunately some AMD windows drivers are buggy and assigning
		// the location of output variables from code is not working. most probably the bug prevents
		// the assignement to take any effect. to counter this huge driver bug the location of output
		// variables is always patched into the sources for all systems. hence to avoid potential
		// bugs-in-bugs do not write the location here
		/*
		if( pglBindFragDataLocation ){
			count = outputList.GetCount();
			for( i=0; i<count; i++ ){
				SC_OGL_CHECK( renderThread, pglBindFragDataLocation( handleShader,
					outputList.GetTargetAt( i ), outputList.GetNameAt( i ) ) );
			}
		}
		*/
		
		// bind feedback variables. this has to be done before linking according to ogl specs
		if( pglTransformFeedbackVaryings ){
			count = feedbackList.GetCount();
			
			if( count > 0 ){
				const char ** const varnames = new const char *[ count ];
				
				for( i=0; i<count; i++ ){
					varnames[ i ] = feedbackList.GetAt( i ).GetString();
				}
				
				SC_OGL_CHECK( renderThread, pglTransformFeedbackVaryings( handleShader, count, varnames,
					feedbackInterleaved ? GL_INTERLEAVED_ATTRIBS : GL_SEPARATE_ATTRIBS ) );
				
				delete [] varnames;
			}
		}
		
		// mark feedback variables active. required only if only the NV transform feedback extension exists.
		// required to be done before linking
		if( ! pglTransformFeedbackVaryings && pglTransformFeedbackVaryingsNV ){
			count = feedbackList.GetCount();
			
			for( i=0; i<count; i++ ){
				SC_OGL_CHECK( renderThread, pglActiveVaryingNV( handleShader, feedbackList.GetAt( i ).GetString() ) );
			}
		}
		
		// link the shader
		#ifdef PRINT_SHADERS
		if( psfMatchesLink( program ) ){
			const deMutexGuard guardLogs(pMutexLogging);
			logger.LogInfo( "COMPILE LINK IN" );
			#ifdef PRINT_SHADERS_SPECIAL_MODE
			vSpecialPrintShader.PrintShader();
			#endif
		}
		#endif
		const bool result = pLinkShader(handleShader);
		if(!result || pErrorLog){
			const deMutexGuard guardLogs(pMutexLogging);
			if(!result){
				logger.LogError("Shader linking failed:");
			}else{
				logger.LogError("Shader linking succeeded with logs:");
			}
			logger.LogErrorFormat("  shader file = %s", sources.GetFilename().GetString());
			if(scCompute){
				logger.LogErrorFormat("  compute unit source code file = %s",
					scCompute->GetFilePath().GetString());
			}
			if(scTessellationControl){
				logger.LogErrorFormat(
					"  tessellation control unit source code file = %s",
					scTessellationControl->GetFilePath().GetString());
			}
			if(scTessellationEvaluation){
				logger.LogErrorFormat("  tessellation evaluation unit source code file = %s",
					scTessellationEvaluation->GetFilePath().GetString());
			}
			if(scGeometry){
				logger.LogErrorFormat("  geometry unit source code file = %s",
					scGeometry->GetFilePath().GetString());
			}
			if(scVertex){
				logger.LogErrorFormat("  vertex unit source code file = %s",
					scVertex->GetFilePath().GetString());
			}
			if(scFragment){
				logger.LogErrorFormat("  fragment unit source code file = %s",
					scFragment->GetFilePath().GetString());
			}
			if(pErrorLog){
				logger.LogErrorFormat("  link logs: %s", pErrorLog);
			}
			pLogFailedShader();
			if(!result){
				DETHROW(deeInvalidParam);
			}
		}
		#ifdef PRINT_SHADERS
		if( psfMatchesLink( program ) ){
			const deMutexGuard guardLogs(pMutexLogging);
			logger.LogInfo( "COMPILE LINK OUT" );
		}
		#endif
		
		pAfterLinkShader( program, *compiled );
		
	}catch( const deException &e ){
		const deMutexGuard guardLogs(pMutexLogging);
		pLanguage.GetRenderThread().GetLogger().LogException(e);
		if( compiled ){
			delete compiled;
		}
		pLogShaderCompute.Empty();
		pLogShaderTessellationControl.Empty();
		pLogShaderTessellationEvaluation.Empty();
		pLogShaderVertex.Empty();
		pLogShaderGeometry.Empty();
		pLogShaderFragment.Empty();
		throw;
	}
	
	// finished compiling
#ifdef WITH_DEBUG
	{
	const deMutexGuard guardLogs(pMutexLogging);
	logger.LogInfoFormat("CompileShader %d: Compiled shader for '%.50s...' in %dms", pContextIndex,
		program.GetCacheId().GetString(), (int)(timerCompile.GetElapsedTime() * 1e3f));
	}
#endif
	
	pLogShaderCompute.Empty();
	pLogShaderTessellationControl.Empty();
	pLogShaderTessellationEvaluation.Empty();
	pLogShaderVertex.Empty();
	pLogShaderGeometry.Empty();
	pLogShaderFragment.Empty();
	
	return compiled;
}

void deoglShaderCompiler::pAfterLinkShader(const deoglShaderProgram& program,
deoglShaderCompiled& compiled){
	deoglRenderThread &renderThread = pLanguage.GetRenderThread();
	const deoglShaderSources &sources = *program.GetSources();
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
		
		// bind uniform blocks
		if( pglGetUniformBlockIndex && pglUniformBlockBinding ){
			const deoglShaderBindingList &uniformBlockList = sources.GetUniformBlockList();
			count = uniformBlockList.GetCount();
			for( i=0; i<count; i++ ){
				location = pglGetUniformBlockIndex( handleShader, uniformBlockList.GetNameAt( i ) );
				if( location != -1 ){
					SC_OGL_CHECK( renderThread, pglUniformBlockBinding(
						handleShader, location, uniformBlockList.GetTargetAt( i ) ) );
				}
			}
		}
		
		// bind shader storage blocks. we do not throw an exception here if the required
		// functions are missing since SSBO usage is often wrapped in if-defs
		const deoglShaderBindingList &shaderStorageBlockList = sources.GetShaderStorageBlockList();
		count = shaderStorageBlockList.GetCount();
		if( count > 0 && pglGetProgramResourceIndex && pglShaderStorageBlockBinding ){
			/*if( ! pglGetProgramResourceIndex ){
				DETHROW_INFO( deeInvalidParam, "missing glGetProgramResourceIndex" );
			}
			if( ! pglShaderStorageBlockBinding ){
				DETHROW_INFO( deeInvalidParam, "missing glShaderStorageBlockBinding" );
			}*/
			for( i=0; i<count; i++ ){
				location = pglGetProgramResourceIndex( handleShader, GL_SHADER_STORAGE_BLOCK,
					shaderStorageBlockList.GetNameAt( i ) );
				if( location != -1 ){ // GL_INVALID_INDEX
					SC_OGL_CHECK( renderThread, pglShaderStorageBlockBinding(
						handleShader, location, shaderStorageBlockList.GetTargetAt( i ) ) );
				}
			}
		}
		
		// bind feedback variables. this is done after the linking if only the NV transform feedback extension exists
		if( ! pglTransformFeedbackVaryings && pglTransformFeedbackVaryingsNV ){
			const bool feedbackInterleaved = sources.GetFeedbackInterleaved();
			const decStringList &feedbackList = sources.GetFeedbackList();
			count = feedbackList.GetCount();
			
			if( count > 0 ){
				int * const locations = new int[ count ];
				
				for( i=0; i<count; i++ ){
					locations[ i ] = pglGetVaryingLocationNV( handleShader, feedbackList.GetAt( i ).GetString() );
					if( locations[ i ] == -1 ){
						DETHROW( deeInvalidParam );
					}
				}
				
				SC_OGL_CHECK( renderThread, pglTransformFeedbackVaryingsNV( handleShader, count, locations,
					feedbackInterleaved ? GL_INTERLEAVED_ATTRIBS : GL_SEPARATE_ATTRIBS ) );
				
				delete [] locations;
			}
		}
		
		SC_OGL_CHECK( renderThread, pglUseProgram( restoreShader ) );
		
	}catch( const deException & ){
		SC_OGL_CHECK( renderThread, pglUseProgram( restoreShader ) );
		throw;
	}
}

deoglShaderCompiled *deoglShaderCompiler::pCacheLoadShader(const deoglShaderProgram &program){
	deoglRenderThread &renderThread = pLanguage.GetRenderThread();
	
	// NOTE we can not put this decision into deoglRTChoices since shaders are compiled already
	//      during capabilities detection which is before deoglRTChoices is constructed
	if(!renderThread.GetExtensions().GetHasExtension(deoglExtensions::ext_ARB_get_program_binary)
	|| renderThread.GetCapabilities().GetNumProgramBinaryFormats() == 0
	|| program.GetCacheId().IsEmpty()){
		return nullptr;
	}
	
	const cGuardLoadingShader guardLoading(pLanguage);
	
	deoglCaches &caches = renderThread.GetOgl().GetCaches();
	deCacheHelper &cacheShaders = caches.GetShaders();
	deoglRTLogger &logger = renderThread.GetLogger();
	deoglShaderCompiled *compiled = nullptr;
	
	try{
		decBaseFileReader::Ref reader;
		{
		const deMutexGuard guard(caches.GetMutex());
		reader.TakeOver(cacheShaders.Read(program.GetCacheId()));
		}
		
		// read parameters
		if(!reader){
			const deMutexGuard guardLogs(pMutexLogging);
			logger.LogInfoFormat(
				"ShaderLanguage.CacheLoadShader: Cached shader not found for '%.50s...'",
				program.GetCacheId().GetString());
			return nullptr;
		}
		
		if(reader->ReadByte() != SHADER_CACHE_REVISION){
			// cache file outdated
			reader = nullptr;
			{
			const deMutexGuard guard(caches.GetMutex());
			cacheShaders.Delete(program.GetCacheId());
			}
			const deMutexGuard guardLogs(pMutexLogging);
			logger.LogInfoFormat(
				"ShaderLanguage.CacheLoadShader: Cache version changed for '%.50s...'. Cache discarded",
				program.GetCacheId().GetString());
			return nullptr;
		}
		
		const GLenum format = (GLenum)reader->ReadUInt();
		const GLint length = (GLint)reader->ReadUInt();
		
		// read binary data
		decString data;
		data.Set(' ', length);
		reader->Read((void*)data.GetString(), length);
		
		// create program using binary data
		compiled = new deoglShaderCompiled(renderThread);
		
		{
		// this has to be mutex protected since this uses opengl call like pCompileShader()
		const cOptionalMutexGuard guard(pMutexCompile, pContextIndex == -1);
		
		SC_OGL_CHECK(renderThread, pglProgramBinary(compiled->GetHandleShader(),
			format, data.GetString(), length));
		
		// loading the binary does everything up to the linking step. this also uses
		// opengl calls and thus has to be mutex protected
		pAfterLinkShader(program, *compiled);
		}
		
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
		const deMutexGuard guardLogs(pMutexLogging);
		logger.LogInfoFormat(
			"ShaderLanguage.CacheLoadShader: Failed loading cached shader '%.50s...'. Cache discarded",
			program.GetCacheId().GetString());
		// logger.LogException(e); // do not spam logs. slows things down
		}
		
		if(compiled){
			delete compiled;
			compiled = nullptr;
		}
	}
	
	return compiled;
}

void deoglShaderCompiler::pCacheSaveShader(const deoglShaderProgram &program,
const deoglShaderCompiled &compiled){
	deoglRenderThread &renderThread = pLanguage.GetRenderThread();
	deoglRTLogger &logger = renderThread.GetLogger();
	
	// NOTE we can not put this decision into deoglRTChoices since shaders are compiled already
	//      during capabilities detection which is before deoglRTChoices is constructed
	if(!renderThread.GetExtensions().GetHasExtension(deoglExtensions::ext_ARB_get_program_binary)
	|| renderThread.GetCapabilities().GetNumProgramBinaryFormats() == 0
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
		
		cCacheShaderTask(renderThread, pMutexLogging, pContextIndex, program, compiled).Run();
		
	}catch(const deException &e){
		const deMutexGuard guardLogs(pMutexLogging);
		logger.LogErrorFormat("ShaderLanguage.CacheSaveShader: Failed caching shader '%.50s...'",
			program.GetCacheId().GetString());
		logger.LogException(e);
	}
}

void deoglShaderCompiler::pPreparePreprocessor(const deoglShaderDefines &defines){
	deoglRenderThread &renderThread = pLanguage.GetRenderThread();
	pPreprocessor.Clear();
	
	// add version
	decString line;
	line.Format( "#version %s\n", pLanguage.GetGLSLVersion().GetString() );
	pPreprocessor.SourcesAppend( line, false );
	
	// add required extensions
	const int extCount = pLanguage.GetGLSLExtensions().GetCount();
	int i;
	
	for( i=0; i<extCount; i++ ){
		line.Format( "#extension %s : require\n", pLanguage.GetGLSLExtensions().GetAt( i ).GetString() );
		pPreprocessor.SourcesAppend( line, false );
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
}



void deoglShaderCompiler::pAppendPreprocessSourcesBuffer(
const char *inputFile, const char *data, const deoglShaderBindingList *outputList ){
	if(!outputList){
		pPreprocessor.SourcesAppendProcessed(data, inputFile);
		return;
	}
	
	const int datalen = (int)strlen(data);
	int tokenStart, delimiter, count, i, start = 0;
	decString token;
	
	decString tempSources;
	
	for( i=0; i<datalen; i++ ){
		bool matches = false;
		int j = i + 1;
		if( data[ i ] == '\n' ){
			while( data[ j ] == ' ' || data[ j ] == '\t' ){
				j++;
			}
			matches = ( strncmp( data + j, "out ", 4 ) == 0 );
		}
		
		if( matches ){
			for( delimiter=j+4; delimiter<datalen; delimiter++ ){
				if( data[ delimiter ] == ','
				||  data[ delimiter ] == ')'
				||  data[ delimiter ] == '{' ){
					// most probably a function declaration wrapped to a new line. ignore it
					matches = false;
					break;
					
				}else if( data[ delimiter ] == ';' ){
					break;
				}
			}
		}
		
		if( matches ){
			decString tempTemp;
			count = j - start;
			tempTemp.Set(' ', count);
			#ifdef OS_W32_VS
				strncpy_s((char*)tempTemp.GetString(), count + 1, data + start, count);
			#else
				strncpy((char*)tempTemp.GetString(), data + start, count);
			#endif

			tempSources += tempTemp;
			
			for( tokenStart=delimiter; tokenStart>j; tokenStart-- ){
				if( data[ tokenStart - 1 ] == ' ' ){
					break;
				}
			}
			
			count = delimiter - tokenStart;
			token.Set(' ', count);
			#ifdef OS_W32_VS
				strncpy_s((char*)token.GetString(), count + 1, data + tokenStart, count);
			#else
				strncpy((char*)token.GetString(), data + tokenStart, count);
			#endif
			
			const int index = outputList->IndexOfNamed( token );
			
			if( index == -1 ){
				token = "// ";
				
			}else{
				token.Format( "layout(location=%i) ", outputList->GetTargetAt( index ) );
			}
			
			tempSources += token;
			
			count = delimiter - j;
			tempTemp.Set(' ', count);
			#ifdef OS_W32_VS
				strncpy_s((char*)tempTemp.GetString(), count + 1, data + j, count);
			#else
				strncpy((char*)tempTemp.GetString(), data + j, count);
			#endif
			tempSources += tempTemp;
			
			i = delimiter;
			start = i;
		}
	}
	
	decString tempTemp;
	count = datalen - start;
	tempTemp.Set(' ', count);
	#ifdef OS_W32_VS
		strncpy_s((char*)tempTemp.GetString(), count + 1, data + start, count);
	#else
		strncpy((char*)tempTemp.GetString(), data + start, count);
	#endif
	tempSources += tempTemp;
	
	pPreprocessor.SourcesAppendProcessed( tempSources.GetString(), inputFile );
}

bool deoglShaderCompiler::pCompileObject( GLuint handle ){
	deoglRenderThread &renderThread = pLanguage.GetRenderThread();
	const char *sources = pPreprocessor.GetSources();
	int sourcesLen = pPreprocessor.GetSourcesLength();
	GLint result;
	
	SC_OGL_CHECK( renderThread, pglShaderSource( handle, 1, &sources, &sourcesLen ) );
	
	SC_OGL_CHECK( renderThread, pglCompileShader( handle ) );
	SC_OGL_CHECK( renderThread, pglGetShaderiv( handle, GL_COMPILE_STATUS, &result ) );
	
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

bool deoglShaderCompiler::pLinkShader( GLuint handle ){
	deoglRenderThread &renderThread = pLanguage.GetRenderThread();
	GLint result;
	
	SC_OGL_CHECK( renderThread, pglLinkProgram( handle ) );
	SC_OGL_CHECK( renderThread, pglGetProgramiv( handle, GL_LINK_STATUS, &result ) );
	
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

void deoglShaderCompiler::pOutputShaderToFile(const char *file){
	const int number = pLanguage.NextShaderFileNumber();
	
#ifdef WITH_OPENGLES
	deoglRenderThread &renderThread = pLanguage.GetRenderThread();
	deoglRTLogger &logger = renderThread.GetLogger();
	{
	const deMutexGuard guardLogs(pMutexLogging);
	logger.LogErrorFormat("%s_%.3i.shader", file, number);
	logger.LogError(pPreprocessor.GetSources());
	}
	
#else
	char buffer[ 256 ];
	snprintf(&buffer[0], sizeof(buffer), "%s_%.3i.shader", file, number);
	FILE *handle = nullptr;
	#ifdef OS_W32
		if( fopen_s( &handle, buffer, "w" ) )
	#else
		handle = fopen( buffer, "w" );
		if( handle )
	#endif
	{
		// this looks now like useless code but unfortunately it's required. older gcc versions complain about the
		// return value not being used failing compilation. newer gcc versions on the other hand don't complain about
		// the unused result but about the dummy variable not being used causing compilation to fail too. only solution
		// to keep all types of compilers happy no matter they crazy idea of handling this function is to do a dummy
		// test which is useless just to get it to work properly. this unused flag on fwrite is one of the most stupid
		// ideas the gcc developers ever had. it's useless and causes tons of pain
		size_t dummy = fwrite( pPreprocessor.GetSources(), 1, pPreprocessor.GetSourcesLength(), handle );
		if( dummy > 0 ){
			fclose( handle );
		}else{
			fclose( handle );
		}
	}
#endif
}

void deoglShaderCompiler::pLogFailedShaderSources(){
	// do not mutex guard pMutexLogging. caller guards it already
	deoglRenderThread &renderThread = pLanguage.GetRenderThread();
	deoglRTLogger &logger = renderThread.GetLogger();
	logger.LogErrorFormat("CompileShader %d Failed: >>> Sources >>>", pContextIndex);
	
	const decStringList lines( decString( pPreprocessor.GetSources() ).Split( "\n" ) );
	int i, count = lines.GetCount();
	decString lastMapping;
	
	for( i=0; i<count; i++ ){
		const deoglShaderSourceLocation * const location = pPreprocessor.ResolveSourceLocation( i + 1 );
		int mapLine = -1;
		
		if( location ){
			mapLine = location->GetInputLine();
			
			if( location->GetInputFile() != lastMapping ){
				logger.LogErrorFormat( "@@@ %s", location->GetInputFile().GetString() );
				lastMapping = location->GetInputFile();
			}
		}
		
		logger.LogErrorFormat( "%d[%d]: %s", i + 1, mapLine, lines.GetAt( i ).GetString() );
	}
	
	logger.LogError( "<<< End Sources <<<" );
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

void deoglShaderCompiler::pLogFailedShader(){
	// do not mutex guard pMutexLogging. caller guards it already
	deoglRenderThread &renderThread = pLanguage.GetRenderThread();
	deoglRTLogger &logger = renderThread.GetLogger();
	
	const struct sData{
		const char *name;
		const decString *sources;
	} data[6] = {
		{"Compute", &pLogShaderCompute},
		{"Tessellation Control", &pLogShaderTessellationControl},
		{"Tessellation Evaluation", &pLogShaderTessellationEvaluation},
		{"Vertex", &pLogShaderVertex},
		{"Geometry", &pLogShaderGeometry},
		{"Fragment", &pLogShaderFragment}};
	
	logger.LogErrorFormat("CompileShader %d Failed: >>> All Sources >>>", pContextIndex);
	
	int i;
	for(i=0; i<6; i++){
		if(data[i].sources->IsEmpty()){
			continue;
		}
		
		logger.LogErrorFormat(">>> %s >>>", data[i].name);
		logger.LogError(data[i].sources->GetString());
		logger.LogErrorFormat("<<< End %s <<<", data[i].name);
	}
	
	logger.LogError("<<< End All Sources <<<");
}
