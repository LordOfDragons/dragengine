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
#include "../deoglCaches.h"
#include "../deGraphicOpenGl.h"
#include "../capabilities/deoglCapabilities.h"
#include "../extensions/deoglExtensions.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTChoices.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/filesystem/deCacheHelper.h>
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


// Class cHasConditionGuard
/////////////////////////////

class cHasConditionGuard{
private:
	bool &pGuard;
	deMutex &pMutex;
	
public:
	cHasConditionGuard( bool &has, bool &guardHas, deMutex &mutex ) :
	pGuard( guardHas ), pMutex( mutex ){
		const deMutexGuard guard( mutex );
		has = true;
		guardHas = true;
	}
	
	~cHasConditionGuard(){
		const deMutexGuard guard( pMutex );
		pGuard = false;
	}
};


// Class deoglShaderLanguage
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderLanguage::deoglShaderLanguage( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pHasLoadingShader( false ),
pGuardHasLoadingShader( false ),
pHasCompilingShader( false ),
pGuardHasCompilingShader( false ),
pPreprocessor( renderThread )
{
	pErrorLog = NULL;
	
	pShaderFileNumber = 1;
	
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
}

deoglShaderLanguage::~deoglShaderLanguage(){
	if( pErrorLog ) delete [] pErrorLog;
}



// Management
///////////////

deoglShaderCompiled *deoglShaderLanguage::CompileShader( deoglShaderProgram &program ){
// 	pRenderThread.GetLogger().LogInfoFormat("CompileShader: cacheId='%s' cacheId.len=%d",
// 		program.GetCacheId().GetString(), program.GetCacheId().GetLength());
	
	deoglShaderCompiled *compiled = pCacheLoadShader( program );
	
	if( ! compiled ){
		compiled = pCompileShader( program );
		
		if( compiled ){
			pCacheSaveShader( program, *compiled );
		}
	}
	
	return compiled;
}

bool deoglShaderLanguage::GetHasLoadingShader(){
	const deMutexGuard guard( pMutexChecks );
	const bool result = pHasLoadingShader;
	if( ! pGuardHasLoadingShader ){
		pHasLoadingShader = false;
	}
	return result;
}

bool deoglShaderLanguage::GetHasCompilingShader(){
	const deMutexGuard guard( pMutexChecks );
	const bool result = pHasCompilingShader;
	if( ! pGuardHasCompilingShader ){
		pHasCompilingShader = false;
	}
	return result;
}



// Private Functions
//////////////////////

deoglShaderCompiled *deoglShaderLanguage::pCompileShader( deoglShaderProgram &program ){
	const cHasConditionGuard guardHas( pHasCompilingShader, pGuardHasCompilingShader, pMutexChecks );
	const deMutexGuard guard( pMutexCompile );
	
	const deoglExtensions &ext = pRenderThread.GetExtensions();
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
	pRenderThread.GetLogger().LogInfo(debugText.GetString());
	#endif
	
	// compile the shader
	pRenderThread.GetLogger().LogInfoFormat(
		"ShaderLanguage.CompileShader: Compile shader for '%.50s...'",
		program.GetCacheId().GetString());
	decTimer timerCompile;
	
	try{
		compiled = new deoglShaderCompiled( pRenderThread );
		
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
			
			if(ext.GetHasExtension(deoglExtensions::ext_ARB_compute_shader)
			&& (ext.GetGLVersion() < deoglExtensions::evgl4p3
			|| ext.GetGLESVersion() < deoglExtensions::evgles3p1)){
				pPreprocessor.SourcesAppend( "#extension GL_ARB_compute_shader : require\n", false );
			}
			
			pAppendPreprocessSourcesBuffer( scCompute->GetFilePath(), scCompute->GetSourceCode() );
			
			#ifdef PRINT_ALL_SHADERS
			pRenderThread.GetLogger().LogInfo("COMPILE COMPUTER IN");
			pRenderThread.GetLogger().LogInfo(pPreprocessor.GetSources());
			#endif
			if( ! pCompileObject( handleC ) ){
				pRenderThread.GetLogger().LogError( "Shader compilation failed:" );
				pRenderThread.GetLogger().LogErrorFormat( "  shader file = %s", sources.GetFilename().GetString() );
				
				pRenderThread.GetLogger().LogErrorFormat(
					"  compute unit source code file = %s", scCompute->GetFilePath().GetString() );
				
				if( pErrorLog ){
					pRenderThread.GetLogger().LogErrorFormat( "  error log: %s", pErrorLog );
				}
				//pOutputShaderToFile( "failed_compute" );
				//pPreprocessor.LogSourceLocationMap();
				pLogFailedShaderSources();
				DETHROW( deeInvalidParam );
			}
			OGL_CHECK( pRenderThread, pglAttachShader( handleShader, handleC ) );
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
			pRenderThread.GetLogger().LogInfo("COMPILE TESSELLATION CONTROL IN");
			pRenderThread.GetLogger().LogInfo(pPreprocessor.GetSources());
			#endif
			if( ! pCompileObject( handleTCP ) ){
				pRenderThread.GetLogger().LogError( "Shader compilation failed:" );
				pRenderThread.GetLogger().LogErrorFormat(
					"  shader file = %s", sources.GetFilename().GetString() );
				
				if( scTessellationControl ){
					pRenderThread.GetLogger().LogErrorFormat(
						"  tessellation control unit source code file = %s",
						scTessellationControl->GetFilePath().GetString() );
				}
				
				if( pErrorLog ){
					pRenderThread.GetLogger().LogErrorFormat( "  error log: %s", pErrorLog );
				}
				//pOutputShaderToFile( "failed_tessellation_control" );
				//pPreprocessor.LogSourceLocationMap();
				pLogFailedShaderSources();
				DETHROW( deeInvalidParam );
			}
			OGL_CHECK( pRenderThread, pglAttachShader( handleShader, handleTCP ) );
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
			pRenderThread.GetLogger().LogInfo("COMPILE TESSELLATION EVALUATION IN");
			pRenderThread.GetLogger().LogInfo(pPreprocessor.GetSources());
			#endif
			if( ! pCompileObject( handleTEP ) ){
				pRenderThread.GetLogger().LogError( "Shader compilation failed:" );
				pRenderThread.GetLogger().LogErrorFormat(
					"  shader file = %s", sources.GetFilename().GetString() );
				
				if( scTessellationEvaluation ){
					pRenderThread.GetLogger().LogErrorFormat(
						"  tessellation evaluation unit source code file = %s",
						scTessellationEvaluation->GetFilePath().GetString() );
				}
				
				if( pErrorLog ){
					pRenderThread.GetLogger().LogErrorFormat( "  error log: %s", pErrorLog );
				}
				//pOutputShaderToFile( "failed_tessellation_evaluation" );
				//pPreprocessor.LogSourceLocationMap();
				pLogFailedShaderSources();
				DETHROW( deeInvalidParam );
			}
			OGL_CHECK( pRenderThread, pglAttachShader( handleShader, handleTEP ) );
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
			pRenderThread.GetLogger().LogInfo("COMPILE GEOMETRY IN");
			pRenderThread.GetLogger().LogInfo(pPreprocessor.GetSources());
			#ifdef PRINT_SHADERS_SPECIAL_MODE
			vSpecialPrintShader.sourceGeometry = pPreprocessor.GetSources();
			#endif
			#endif
			if( ! pCompileObject( handleGP ) ){
				pRenderThread.GetLogger().LogError( "Shader compilation failed:" );
				pRenderThread.GetLogger().LogErrorFormat( "  shader file = %s", sources.GetFilename().GetString() );
				
				if( scGeometry ){
					pRenderThread.GetLogger().LogErrorFormat( "  geometry unit source code file = %s",
						scGeometry->GetFilePath().GetString() );
					
				}else{
					pRenderThread.GetLogger().LogErrorFormat( "  inline geometry unit source code." );
				}
				
				if( pErrorLog ){
					pRenderThread.GetLogger().LogErrorFormat( "  error log: %s", pErrorLog );
				}
				//pOutputShaderToFile( "failed_geometry" );
				//pPreprocessor.LogSourceLocationMap();
				pLogFailedShaderSources();
				DETHROW( deeInvalidParam );
			}
			#ifdef PRINT_ALL_SHADERS
			pRenderThread.GetLogger().LogInfo( "COMPILE GEOMETRY OUT" );
			#endif
			OGL_CHECK( pRenderThread, pglAttachShader( handleShader, handleGP ) );
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
				pRenderThread.GetLogger().LogInfo( "COMPILE VERTEX IN" );
				pRenderThread.GetLogger().LogInfo( pPreprocessor.GetSources() );
				#ifdef PRINT_SHADERS_SPECIAL_MODE
				vSpecialPrintShader.sourceVertex = pPreprocessor.GetSources();
				#endif
			}
			#endif
			#ifdef PRINT_ALL_SHADERS
			pRenderThread.GetLogger().LogInfo("COMPILE VERTEX IN");
			pRenderThread.GetLogger().LogInfo(pPreprocessor.GetSources());
			#endif
			if( ! pCompileObject( handleVP ) ){
				pRenderThread.GetLogger().LogError( "Shader compilation failed:" );
				pRenderThread.GetLogger().LogErrorFormat(
					"  shader file = %s", sources.GetFilename().GetString() );
				
				if( scVertex ){
					pRenderThread.GetLogger().LogErrorFormat( "  vertex unit source code file = %s",
						scVertex->GetFilePath().GetString() );
					
				}else{
					pRenderThread.GetLogger().LogErrorFormat( "  inline vertex unit source code." );
				}
				
				if( pErrorLog ){
					pRenderThread.GetLogger().LogErrorFormat( "  error log: %s", pErrorLog );
				}
				//pOutputShaderToFile( "failed_vertex" );
				//pPreprocessor.LogSourceLocationMap();
				pLogFailedShaderSources();
				DETHROW( deeInvalidParam );
			}
			#ifdef PRINT_SHADERS
			if( psfMatchesVertex( program ) ){
				pRenderThread.GetLogger().LogInfo( "COMPILE VERTEX OUT" );
			}
			#endif
// 			if(scVertex){
// 				decString s(scVertex->GetFilePath());
// 				s = s.GetMiddle(s.FindReverse('/')+1, s.GetLength()-5);
// 				s = decString("vertex_") + s;
// 				pOutputShaderToFile(s);
// 			}
			OGL_CHECK( pRenderThread, pglAttachShader( handleShader, handleVP ) );
		}
		
		// compiled the fragment program if existing
		if( scFragment || ! inlscFragment.IsEmpty() ){
			compiled->CreateFragmentProgram();
			handleFP = compiled->GetHandleFP();
			if( ! handleFP ) DETHROW( deeInvalidAction );
			
			pPreparePreprocessor( program.GetDefines() );
			
			#ifdef OS_ANDROID
			if( scFragment ){
				pAppendPreprocessSourcesBuffer( scFragment->GetFilePath(), scFragment->GetSourceCode(), &outputList );
				
			}else{
				pAppendPreprocessSourcesBuffer( "<inline>", inlscFragment.GetString(), &outputList );
			}
			#else
			if( scFragment ){
				pAppendPreprocessSourcesBuffer( scFragment->GetFilePath(), scFragment->GetSourceCode() );
				
			}else{
				pAppendPreprocessSourcesBuffer( "<inline>", inlscFragment.GetString() );
			}
			#endif
			
			#ifdef PRINT_SHADERS
			if( psfMatchesFragment( program ) ){
				pRenderThread.GetLogger().LogInfo( "COMPILE FRAGMENT IN" );
				pRenderThread.GetLogger().LogInfo( pPreprocessor.GetSources() );
				#ifdef PRINT_SHADERS_SPECIAL_MODE
				vSpecialPrintShader.sourceFragment = pPreprocessor.GetSources();
				#endif
			}
			#endif
			#ifdef PRINT_ALL_SHADERS
			pRenderThread.GetLogger().LogInfo("COMPILE FRAGMENT IN");
			pRenderThread.GetLogger().LogInfo(pPreprocessor.GetSources());
			#endif
			if( ! pCompileObject( handleFP ) ){
				pRenderThread.GetLogger().LogError( "Shader compilation failed:" );
				pRenderThread.GetLogger().LogErrorFormat(
					"  shader file = %s", sources.GetFilename().GetString() );
				
				if( scFragment ){
					pRenderThread.GetLogger().LogErrorFormat( "  fragment unit source code file = %s",
						scFragment->GetFilePath().GetString() );
					
				}else{
					pRenderThread.GetLogger().LogError( "  inline fragment unit source code." );
				}
				
				if( pErrorLog ){
					pRenderThread.GetLogger().LogErrorFormat( "  error log: %s", pErrorLog );
				}
				//pOutputShaderToFile( "failed_fragment" );
				//pPreprocessor.LogSourceLocationMap();
				pLogFailedShaderSources();
				DETHROW( deeInvalidParam );
			}
			#ifdef PRINT_SHADERS
			if( psfMatchesFragment( program ) ){
				pRenderThread.GetLogger().LogInfo( "COMPILE FRAGMENT OUT" );
			}
			#endif
			OGL_CHECK( pRenderThread, pglAttachShader( handleShader, handleFP ) );
		}
		
		// bind attribute locations. this has to be done before linking according to ogl
		// specs. furthermore the name is not required to exist. if not existing the
		// output is simple not linked to anything.
		if( pglBindAttribLocation ){
			count = inputList.GetCount();
			for( i=0; i<count; i++ ){
				OGL_CHECK( pRenderThread, pglBindAttribLocation( handleShader, inputList.GetTargetAt( i ), inputList.GetNameAt( i ) ) );
			}
		}
		
		// bind data locations. this has to be done before linking according to ogl specs.
		// furthermore the name is not required to exist. if not existing the output is
		// simple not linked to anything.
		if( pglBindFragDataLocation ){
			count = outputList.GetCount();
			for( i=0; i<count; i++ ){
				OGL_CHECK( pRenderThread, pglBindFragDataLocation( handleShader, outputList.GetTargetAt( i ), outputList.GetNameAt( i ) ) );
			}
		}
		
		// bind feedback variables. this has to be done before linking according to ogl specs
		if( pglTransformFeedbackVaryings ){
			count = feedbackList.GetCount();
			
			if( count > 0 ){
				const char ** const varnames = new const char *[ count ];
				
				for( i=0; i<count; i++ ){
					varnames[ i ] = feedbackList.GetAt( i ).GetString();
				}
				
				OGL_CHECK( pRenderThread, pglTransformFeedbackVaryings( handleShader, count, varnames,
					feedbackInterleaved ? GL_INTERLEAVED_ATTRIBS : GL_SEPARATE_ATTRIBS ) );
				
				delete [] varnames;
			}
		}
		
		// mark feedback variables active. required only if only the NV transform feedback extension exists.
		// required to be done before linking
		if( ! pglTransformFeedbackVaryings && pglTransformFeedbackVaryingsNV ){
			count = feedbackList.GetCount();
			
			for( i=0; i<count; i++ ){
				OGL_CHECK( pRenderThread, pglActiveVaryingNV( handleShader, feedbackList.GetAt( i ).GetString() ) );
			}
		}
		
		// link the shader
		#ifdef PRINT_SHADERS
		if( psfMatchesLink( program ) ){
			pRenderThread.GetLogger().LogInfo( "COMPILE LINK IN" );
			#ifdef PRINT_SHADERS_SPECIAL_MODE
			vSpecialPrintShader.PrintShader();
			#endif
		}
		#endif
		if( ! pLinkShader( handleShader ) ){
			pRenderThread.GetLogger().LogErrorFormat(
				"Shader linking failed (%s):", sources.GetFilename().GetString() );
			
			if( scCompute ){
				pRenderThread.GetLogger().LogErrorFormat( "  compute unit source code file = %s",
					scCompute->GetFilePath().GetString() );
			}
			if( scTessellationControl ){
				pRenderThread.GetLogger().LogErrorFormat(
					"  tessellation control unit source code file = %s",
					scTessellationControl->GetFilePath().GetString() );
			}
			if( scTessellationEvaluation ){
				pRenderThread.GetLogger().LogErrorFormat(
					"  tessellation evaluation unit source code file = %s",
					scTessellationEvaluation->GetFilePath().GetString() );
			}
			if( scGeometry ){
				pRenderThread.GetLogger().LogErrorFormat( "  geometry unit source code file = %s",
					scGeometry->GetFilePath().GetString() );
			}
			if( scVertex ){
				pRenderThread.GetLogger().LogErrorFormat( "  vertex unit source code file = %s",
					scVertex->GetFilePath().GetString() );
			}
			if( scFragment ){
				pRenderThread.GetLogger().LogErrorFormat( "  fragment unit source code file = %s",
					scFragment->GetFilePath().GetString() );
			}
			if( pErrorLog ){
				pRenderThread.GetLogger().LogErrorFormat( "  error log: %s", pErrorLog );
			}
			
			DETHROW( deeInvalidParam );
		}
		#ifdef PRINT_SHADERS
		if( psfMatchesLink( program ) ){
			pRenderThread.GetLogger().LogInfo( "COMPILE LINK OUT" );
		}
		#endif
		
		pAfterLinkShader( program, *compiled );
		
	}catch( const deException &e ){
		e.PrintError();
		if( compiled ){
			delete compiled;
		}
		throw;
	}
	
	// finished compiling
	pRenderThread.GetLogger().LogInfoFormat(
		"ShaderLanguage.CompileShader: Compiled shader for '%.50s...' in %dms",
		program.GetCacheId().GetString(), (int)(timerCompile.GetElapsedTime() * 1e4f));
	
	return compiled;
}

void deoglShaderLanguage::pAfterLinkShader( const deoglShaderProgram& program,
deoglShaderCompiled& compiled ){
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
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_CURRENT_PROGRAM, ( GLint* )&restoreShader ) );
	
	try{
		// pRenderThread.GetShader().ActivateShader( nullptr ); // nope, see above comment
		// compiled.Activate();
		OGL_CHECK( pRenderThread, pglUseProgram( compiled.GetHandleShader() ) );
		
		// bind textures
		const deoglShaderBindingList &textureList = sources.GetTextureList();
		count = textureList.GetCount();
		for( i=0; i<count; i++ ){
			location = pglGetUniformLocation( handleShader, textureList.GetNameAt( i ) );
			if( location != -1 ){
				OGL_CHECK( pRenderThread, pglUniform1i( location, textureList.GetTargetAt( i ) ) );
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
					OGL_CHECK( pRenderThread, pglUniformBlockBinding(
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
					OGL_CHECK( pRenderThread, pglShaderStorageBlockBinding(
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
				
				OGL_CHECK( pRenderThread, pglTransformFeedbackVaryingsNV( handleShader, count, locations,
					feedbackInterleaved ? GL_INTERLEAVED_ATTRIBS : GL_SEPARATE_ATTRIBS ) );
				
				delete [] locations;
			}
		}
		
		OGL_CHECK( pRenderThread, pglUseProgram( restoreShader ) );
		
	}catch( const deException & ){
		OGL_CHECK( pRenderThread, pglUseProgram( restoreShader ) );
		throw;
	}
}

deoglShaderCompiled *deoglShaderLanguage::pCacheLoadShader( deoglShaderProgram &program ){
	// NOTE we can not put this decision into deoglRTChoices since shaders are compiled already
	//      during capabilities detection which is before deoglRTChoices is constructed
	if( ! pRenderThread.GetExtensions().GetHasExtension( deoglExtensions::ext_ARB_get_program_binary )
	|| pRenderThread.GetCapabilities().GetNumProgramBinaryFormats() == 0
	|| program.GetCacheId().IsEmpty() ){
		return nullptr;
	}
	
	const cHasConditionGuard guardHas( pHasLoadingShader, pGuardHasLoadingShader, pMutexChecks );
	
	deoglCaches &caches = pRenderThread.GetOgl().GetCaches();
	deCacheHelper &cacheShaders = caches.GetShaders();
	
	caches.Lock();
	
	deoglShaderCompiled *compiled = nullptr;
	
	try{
		decBaseFileReader::Ref reader( decBaseFileReader::Ref::New(
			cacheShaders.Read( program.GetCacheId() ) ) );
		
		// read parameters
		if( ! reader ){
			caches.Unlock();
			pRenderThread.GetLogger().LogInfoFormat(
				"ShaderLanguage.CacheLoadShader: Cached shader not found for '%.50s...'",
				program.GetCacheId().GetString() );
			return nullptr;
		}
		
		if( reader->ReadByte() != SHADER_CACHE_REVISION ){
			// cache file outdated
			reader = nullptr;
			cacheShaders.Delete( program.GetCacheId() );
			caches.Unlock();
			pRenderThread.GetLogger().LogInfoFormat(
				"ShaderLanguage.CacheLoadShader: Cache version changed for '%.50s...'. Cache discarded",
				program.GetCacheId().GetString() );
			return nullptr;
		}
		
		const GLenum format = ( GLenum )reader->ReadUInt();
		const GLint length = ( GLint )reader->ReadUInt();
		
		// read binary data
		decString data;
		data.Set( ' ', length );
		reader->Read( ( void* )data.GetString(), length );
		
		// create program using binary data
		compiled = new deoglShaderCompiled( pRenderThread );
		
		{
		// this has to be mutex protected since this uses opengl call like pCompileShader()
		const deMutexGuard guard( pMutexCompile );
		
		OGL_CHECK( pRenderThread, pglProgramBinary( compiled->GetHandleShader(),
			format, data.GetString(), length ) );
		
		// loading the binary does everything up to the linking step. this also uses
		// opengl calls and thus has to be mutex protected
		pAfterLinkShader( program, *compiled );
		}
		
		// done
		reader = nullptr;
		caches.Unlock();
		
// 		pRenderThread.GetLogger().LogInfoFormat(
// 			"ShaderLanguage.CacheLoadShader: Cached shader loaded for '%.50s...'",
// 			program.GetCacheId().GetString() );
		
	}catch( const deException &e ){
		cacheShaders.Delete( program.GetCacheId() );
		caches.Unlock();
		
		pRenderThread.GetLogger().LogInfoFormat(
			"ShaderLanguage.CacheLoadShader: Failed loading cached shader '%.50s...'. Cache discarded",
			program.GetCacheId().GetString() );
		// pRenderThread.GetLogger().LogException(e); // do not spam logs. slows things down
		
		if( compiled ){
			delete compiled;
			compiled = nullptr;
		}
	}
	
	return compiled;
}

void deoglShaderLanguage::pCacheSaveShader( const deoglShaderProgram &program,
const deoglShaderCompiled &compiled ){
	// NOTE we can not put this decision into deoglRTChoices since shaders are compiled already
	//      during capabilities detection which is before deoglRTChoices is constructed
	if( ! pRenderThread.GetExtensions().GetHasExtension( deoglExtensions::ext_ARB_get_program_binary )
	|| pRenderThread.GetCapabilities().GetNumProgramBinaryFormats() == 0
	|| program.GetCacheId().IsEmpty() ){
		return;
	}
	
	const GLuint handler = compiled.GetHandleShader();
	deoglCaches &caches = pRenderThread.GetOgl().GetCaches();
	deCacheHelper &cacheShaders = caches.GetShaders();
	
	caches.Lock();
	
	try{
		// get length of binary data
		GLint length = 0;
		OGL_CHECK( pRenderThread, pglGetProgramiv( handler, GL_PROGRAM_BINARY_LENGTH, &length ) );
		DEASSERT_TRUE( length > 0 )
		
		// get binary data
		decString data;
		data.Set( ' ', length );
		
		GLenum format = 0;
		OGL_CHECK( pRenderThread, pglGetProgramBinary( handler,
			length, nullptr, &format, ( void* )data.GetString() ) );
		
		// write to cache
		{
		const decBaseFileWriter::Ref writer( decBaseFileWriter::Ref::New(
			cacheShaders.Write( program.GetCacheId() ) ) );
		
		writer->WriteByte( SHADER_CACHE_REVISION );
		writer->WriteUInt( format );
		writer->WriteUInt( length );
		writer->Write( data.GetString(), length );
		}
		
		caches.Unlock();
		
		pRenderThread.GetLogger().LogInfoFormat(
			"ShaderLanguage.CacheSaveShader: Cached shader '%.50s...', length %d bytes",
			program.GetCacheId().GetString(), length );
		
	}catch( const deException &e ){
		caches.Unlock();
		pRenderThread.GetLogger().LogErrorFormat(
			"ShaderLanguage.CacheSaveShader: Failed caching shader '%.50s...'",
			program.GetCacheId().GetString() );
		pRenderThread.GetLogger().LogException( e );
	}
}

void deoglShaderLanguage::pPreparePreprocessor( const deoglShaderDefines &defines ){
	pPreprocessor.Clear();
	
	// add version
	decString line;
	line.Format( "#version %s\n", pGLSLVersion.GetString() );
	pPreprocessor.SourcesAppend( line, false );
	
	// add required extensions
	const int extCount = pGLSLExtensions.GetCount();
	int i;
	
	for( i=0; i<extCount; i++ ){
		line.Format( "#extension %s : require\n", pGLSLExtensions.GetAt( i ).GetString() );
		pPreprocessor.SourcesAppend( line, false );
	}
	
	// add version selection defines
	if( pGLSLVersionNumber >= 450 ){
		pPreprocessor.SetSymbol( "GLSL_450", "1" );
	}
	
	// add symbols
	pPreprocessor.SetSymbolsFromDefines( defines );
	
	if( ! defines.HasDefineNamed( "HIGHP" ) ){
		pPreprocessor.SetSymbol( "HIGHP", "highp" );
	}
	
	#ifdef OS_ANDROID
	pPreprocessor.SetSymbol( "ANDROID", "1" );
	pPreprocessor.SetSymbol( "OPENGLES", "1" );
	pPreprocessor.SetSymbol("ARG_SAMP_HIGHP", "highp");
	pPreprocessor.SetSymbol("ARG_SAMP_MEDP", "medp");
	pPreprocessor.SetSymbol("ARG_SAMP_LOWP", "lowp");
	//pPreprocessor.SourcesAppend( "float modf( in float x, out float i ){ i=floor(x); return fract(x); }\n" );
	
	#else
	pPreprocessor.SetSymbol("ARG_SAMP_HIGHP", "");
	pPreprocessor.SetSymbol("ARG_SAMP_MEDP", "");
	pPreprocessor.SetSymbol("ARG_SAMP_LOWP", "");
	#endif
}



#ifdef OS_ANDROID
void deoglShaderLanguage::pAppendPreprocessSourcesBuffer(
const char *inputFile, const char *data, const deoglShaderBindingList *outputList ){
	if( ! outputList ){
		pPreprocessor.SourcesAppendProcessed( data, inputFile );
		return;
	}
	
	const int datalen = strlen( data );
	int i, start = 0;
	decString token;
	int tokenStart;
	int delimiter;
	
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
			tempTemp.Set( ' ', j - start );
			strncpy( (char*)tempTemp.GetString(), data + start, j - start );
			tempSources += tempTemp;
			
			for( tokenStart=delimiter; tokenStart>j; tokenStart-- ){
				if( data[ tokenStart - 1 ] == ' ' ){
					break;
				}
			}
			
			token.Set( ' ', delimiter - tokenStart );
			strncpy( (char*)token.GetString(), data + tokenStart, delimiter - tokenStart );
			
			const int index = outputList->IndexOfNamed( token );
			
			if( index == -1 ){
				token = "// ";
				
			}else{
				token.Format( "layout(location=%i) ", outputList->GetTargetAt( index ) );
			}
			
			tempSources += token;
			
			tempTemp.Set( ' ', delimiter - j );
			strncpy( (char*)tempTemp.GetString(), data + j, delimiter - j );
			tempSources += tempTemp;
			
			i = delimiter;
			start = i;
		}
	}
	
	decString tempTemp;
	tempTemp.Set( ' ', datalen - start );
	strncpy( (char*)tempTemp.GetString(), data + start, datalen - start );
	tempSources += tempTemp;
	
	pPreprocessor.SourcesAppendProcessed( tempSources.GetString(), inputFile );
}

#else
void deoglShaderLanguage::pAppendPreprocessSourcesBuffer( const char *inputFile, const char *data ){
	pPreprocessor.SourcesAppendProcessed( data, inputFile );
}
#endif

bool deoglShaderLanguage::pCompileObject( GLuint handle ){
	const char *sources = pPreprocessor.GetSources();
	int sourcesLen = pPreprocessor.GetSourcesLength();
	int result;
	
	try{
		// load source
		OGL_CHECK( pRenderThread, pglShaderSource( handle, 1, &sources, &sourcesLen ) );
		
		// compile source
		OGL_CHECK( pRenderThread, pglCompileShader( handle ) );
		OGL_CHECK( pRenderThread, pglGetShaderiv( handle, GL_COMPILE_STATUS, &result ) );
		
		if( ! result ){
			int blen = 0, slen = 0;	
			
			if( pErrorLog ){
				delete [] pErrorLog;
				pErrorLog = NULL;
			}
			
			OGL_CHECK( pRenderThread, pglGetShaderiv( handle, GL_INFO_LOG_LENGTH , &blen ) );
			
			if( blen > 1 ){
				pErrorLog = new char[ blen + 1 ];
				if( ! pErrorLog ) DETHROW( deeOutOfMemory );
				
				OGL_CHECK( pRenderThread, pglGetShaderInfoLog( handle, blen, &slen, pErrorLog ) );
				
				pErrorLog[ blen ] = '\0';
				
				return false;
			}
		}
		
	}catch( const deException & ){
		return false;
	}
	
	return true;
}

bool deoglShaderLanguage::pLinkShader( GLuint handle ){
	int result;
	
	try{
		OGL_CHECK( pRenderThread, pglLinkProgram( handle ) );
		OGL_CHECK( pRenderThread, pglGetProgramiv( handle, GL_LINK_STATUS, &result ) );
		
		if( ! result ){
			int blen = 0, slen = 0;	
			
			if( pErrorLog ){
				delete [] pErrorLog;
				pErrorLog = NULL;
			}
			
			OGL_CHECK( pRenderThread, pglGetProgramiv( handle, GL_INFO_LOG_LENGTH , &blen ) );
			
			if( blen > 1 ){
				pErrorLog = new char[ blen + 1 ];
				if( ! blen ) DETHROW( deeOutOfMemory );
				
				OGL_CHECK( pRenderThread, pglGetProgramInfoLog( handle, blen, &slen, pErrorLog ) );
				pErrorLog[ blen ] = '\0';
				
				return false;
			}
		}
		
	}catch( const deException & ){
		return false;
	}
	
	return true;
}

void deoglShaderLanguage::pOutputShaderToFile( const char *file ){
#ifdef OS_ANDROID
	pRenderThread.GetLogger().LogErrorFormat( "%s_%.3i.shader", file, pShaderFileNumber++ );
	pRenderThread.GetLogger().LogError( pPreprocessor.GetSources() );
	
#else
	char buffer[ 256 ];
	snprintf( &buffer[ 0 ], sizeof( buffer ), "%s_%.3i.shader", file, pShaderFileNumber++ );
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

void deoglShaderLanguage::pLogFailedShaderSources(){
	pRenderThread.GetLogger().LogError( ">>> Sources >>>" );
	
	const decStringList lines( decString( pPreprocessor.GetSources() ).Split( "\n" ) );
	int i, count = lines.GetCount();
	decString lastMapping;
	
	for( i=0; i<count; i++ ){
		const deoglShaderSourceLocation * const location = pPreprocessor.ResolveSourceLocation( i + 1 );
		int mapLine = -1;
		
		if( location ){
			mapLine = location->GetInputLine();
			
			if( location->GetInputFile() != lastMapping ){
				pRenderThread.GetLogger().LogErrorFormat( "@@@ %s", location->GetInputFile().GetString() );
				lastMapping = location->GetInputFile();
			}
		}
		
		pRenderThread.GetLogger().LogErrorFormat( "%d[%d]: %s", i + 1, mapLine, lines.GetAt( i ).GetString() );
	}
	
	pRenderThread.GetLogger().LogError( "<<< End Sources <<<" );
}

void deoglShaderLanguage::pPrintErrorLog(){
	if( pErrorLog ){
		pRenderThread.GetLogger().LogErrorFormat( "  error log: %s", pErrorLog );
	}
}
