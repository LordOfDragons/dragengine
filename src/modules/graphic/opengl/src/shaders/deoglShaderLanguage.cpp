/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
#include "../extensions/deoglExtensions.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTShader.h"

#include <dragengine/common/exceptions.h>



#ifdef ANDROID
//#define PRINT_SHADERS
//#define PRINT_COMPILING
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

static const char * const vPsfFragment = "v130/fragment/defren/light/light.glsl";
static const char * const vPsfVertex = "v130/vertex/defren/light/light.glsl";
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


// Class deoglShaderLanguage
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderLanguage::deoglShaderLanguage( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
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
				break;
				
			case deoglExtensions::evgl3p1:
				pGLSLVersion = "140";
				break;
				
			case deoglExtensions::evgl3p0:
				pGLSLVersion = "130";
				break;
				
			case deoglExtensions::evgl2p1:
				pGLSLVersion = "120";
				break;
				
			default:
				pGLSLVersion = "110";
			}
			
		}else{
			pGLSLVersion.Format( "%d%0d0 core", ext.GetGLVersionMajor(), ext.GetGLVersionMinor() );
		}
		
	}else{
		pGLSLVersion.Format( "%d%0d0 es", ext.GetGLVersionMajor(), ext.GetGLVersionMinor() );
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
	}
}

deoglShaderLanguage::~deoglShaderLanguage(){
	if( pErrorLog ) delete [] pErrorLog;
}



// Management
///////////////

deoglShaderCompiled *deoglShaderLanguage::CompileShader( deoglShaderProgram &program ){
	const deoglShaderSources &sources = *program.GetSources();
	deoglShaderUnitSourceCode * const scCompute = program.GetComputeSourceCode();
	deoglShaderUnitSourceCode * const scTessellationControl = program.GetTessellationControlSourceCode();
	deoglShaderUnitSourceCode * const scTessellationEvaluation = program.GetTessellationEvaluationSourceCode();
	deoglShaderUnitSourceCode * const scGeometry = program.GetGeometrySourceCode();
	deoglShaderUnitSourceCode * const scVertex = program.GetVertexSourceCode();
	deoglShaderUnitSourceCode * const scFragment = program.GetFragmentSourceCode();
	const decString &inlscGeometry = sources.GetInlineGeometrySourceCode();
	const decString &inlscVertex = sources.GetInlineVertexSourceCode();
	const decString &inlscFragment = sources.GetInlineFragmentSourceCode();
	const deoglShaderBindingList &shaderStorageBlockList = sources.GetShaderStorageBlockList();
	const deoglShaderBindingList &uniformBlockList = sources.GetUniformBlockList();
	const deoglShaderBindingList &textureList = sources.GetTextureList();
	const deoglShaderBindingList &inputList = sources.GetAttributeList();
	const deoglShaderBindingList &outputList = sources.GetOutputList();
	const decStringList &parameterList = sources.GetParameterList();
	const decStringList &feedbackList = sources.GetFeedbackList();
	GLuint handleShader = 0;
	GLuint handleC = 0;
	GLuint handleTCP = 0;
	GLuint handleTEP = 0;
	GLuint handleGP = 0;
	GLuint handleVP = 0;
	GLuint handleFP = 0;
	deoglShaderCompiled *compiled = NULL;
	int i, count, location;
	
	#ifdef PRINT_COMPILING
	decString debugText( "compiling " );
	if( scCompute ){
		debugText.AppendFormat( " comp(%s)", scCompute->GetFilePath() );
	}
	if( scTessellationControl ){
		debugText.AppendFormat( " tc(%s)", scTessellationControl->GetFilePath() );
	}
	if( scTessellationEvaluation ){
		debugText.AppendFormat( " te(%s)", scTessellationEvaluation->GetFilePath() );
	}
	if( scGeometry ){
		debugText.AppendFormat( " geom(%s)", scGeometry->GetFilePath() );
	}
	if( scVertex ){
		debugText.AppendFormat( " vert(%s)", scVertex->GetFilePath() );
	}
	if( scFragment ){
		debugText.AppendFormat( " frag(%s)", scFragment->GetFilePath() );
	}
	debugText.Append( " defines(" );
	for( i=0; i<program.GetDefines().GetDefineCount(); i++ ){
		if( i > 0 ){
			debugText.Append( " " );
		}
		debugText.AppendFormat( "%s=%s", program.GetDefines().GetDefineNameAt( i ),
			program.GetDefines().GetDefineValueAt( i ) );
	}
	debugText.Append( ")" );
	pRenderThread.GetLogger().LogInfo( debugText.GetString() );
	#endif
	
	// compile the shader
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
			
			if( scCompute ){
				pAppendPreprocessSourcesBuffer( scCompute->GetFilePath(), scCompute->GetSourceCode() );
			}
			
			if( ! pCompileObject( handleC ) ){
				pRenderThread.GetLogger().LogError( "Shader compilation failed:" );
				pRenderThread.GetLogger().LogErrorFormat( "  shader file = %s", sources.GetFilename().GetString() );
				
				if( scCompute ){
					pRenderThread.GetLogger().LogErrorFormat( "  compute unit source code file = %s", scCompute->GetFilePath() );
				}
				
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
				pAppendPreprocessSourcesBuffer( scTessellationControl->GetFilePath(), scTessellationControl->GetSourceCode() );
			}
			
			if( ! pCompileObject( handleTCP ) ){
				pRenderThread.GetLogger().LogError( "Shader compilation failed:" );
				pRenderThread.GetLogger().LogErrorFormat( "  shader file = %s", sources.GetFilename().GetString() );
				
				if( scTessellationControl ){
					pRenderThread.GetLogger().LogErrorFormat( "  tessellation control unit source code file = %s", scTessellationControl->GetFilePath() );
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
				pAppendPreprocessSourcesBuffer( scTessellationEvaluation->GetFilePath(), scTessellationEvaluation->GetSourceCode() );
			}
			
			if( ! pCompileObject( handleTEP ) ){
				pRenderThread.GetLogger().LogError( "Shader compilation failed:" );
				pRenderThread.GetLogger().LogErrorFormat( "  shader file = %s", sources.GetFilename().GetString() );
				
				if( scTessellationEvaluation ){
					pRenderThread.GetLogger().LogErrorFormat( "  tessellation evaluation unit source code file = %s", scTessellationEvaluation->GetFilePath() );
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
			pRenderThread.GetLogger().LogInfo( "COMPILE GEOMETRY IN" );
			pRenderThread.GetLogger().LogInfo( pPreprocessor.GetSources() );
			#ifdef PRINT_SHADERS_SPECIAL_MODE
			vSpecialPrintShader.sourceGeometry = pPreprocessor.GetSources();
			#endif
			#endif
			if( ! pCompileObject( handleGP ) ){
				pRenderThread.GetLogger().LogError( "Shader compilation failed:" );
				pRenderThread.GetLogger().LogErrorFormat( "  shader file = %s", sources.GetFilename().GetString() );
				
				if( scGeometry ){
					pRenderThread.GetLogger().LogErrorFormat( "  geometry unit source code file = %s", scGeometry->GetFilePath() );
					
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
			if( ! pCompileObject( handleVP ) ){
				pRenderThread.GetLogger().LogError( "Shader compilation failed:" );
				pRenderThread.GetLogger().LogErrorFormat( "  shader file = %s", sources.GetFilename().GetString() );
				
				if( scVertex ){
					pRenderThread.GetLogger().LogErrorFormat( "  vertex unit source code file = %s", scVertex->GetFilePath() );
					
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
			
			#ifdef ANDROID
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
			if( ! pCompileObject( handleFP ) ){
				pRenderThread.GetLogger().LogError( "Shader compilation failed:" );
				pRenderThread.GetLogger().LogErrorFormat( "  shader file = %s", sources.GetFilename().GetString() );
				
				if( scFragment ){
					pRenderThread.GetLogger().LogErrorFormat( "  fragment unit source code file = %s", scFragment->GetFilePath() );
					
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
				
				OGL_CHECK( pRenderThread, pglTransformFeedbackVaryings( handleShader, count, varnames, GL_INTERLEAVED_ATTRIBS ) );
				
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
			pRenderThread.GetLogger().LogErrorFormat( "Shader linking failed (%s):", sources.GetFilename().GetString() );
			
			if( scCompute ){
				pRenderThread.GetLogger().LogErrorFormat( "  compute unit source code file = %s", scCompute->GetFilePath() );
			}
			
			if( scTessellationControl ){
				pRenderThread.GetLogger().LogErrorFormat( "  tessellation control unit source code file = %s", scTessellationControl->GetFilePath() );
			}
			if( scTessellationEvaluation ){
				pRenderThread.GetLogger().LogErrorFormat( "  tessellation evaluation unit source code file = %s", scTessellationEvaluation->GetFilePath() );
			}
			
			if( scGeometry ){
				pRenderThread.GetLogger().LogErrorFormat( "  geometry unit source code file = %s", scGeometry->GetFilePath() );
				
			}else{
				pRenderThread.GetLogger().LogErrorFormat( "  inline geometry unit source code." );
			}
			
			if( scVertex ){
				pRenderThread.GetLogger().LogErrorFormat( "  vertex unit source code file = %s", scVertex->GetFilePath() );
				
			}else{
				pRenderThread.GetLogger().LogErrorFormat( "  inline vertex unit source code." );
			}
			
			if( scFragment ){
				pRenderThread.GetLogger().LogErrorFormat( "  fragment unit source code file = %s", scFragment->GetFilePath() );
				
			}else{
				pRenderThread.GetLogger().LogError( "  inline fragment unit source code." );
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
		
		// for the rest we have to activate this shader. to avoid upseting the shader tracker the current shader
		// has to be deactivated first. this ensures the next time a shader is activated it is bound as active
		// although the compiled shader is set.
		pRenderThread.GetShader().ActivateShader( NULL );
		compiled->Activate();
		
		// bind textures
		count = textureList.GetCount();
		for( i=0; i<count; i++ ){
			location = pglGetUniformLocation( handleShader, textureList.GetNameAt( i ) );
			if( location != -1 ){
				OGL_CHECK( pRenderThread, pglUniform1i( location, textureList.GetTargetAt( i ) ) );
			}
		}
		
		// resolve parameters
		count = parameterList.GetCount();
		compiled->SetParameterCount( count );
		for( i=0; i<count; i++ ){
			compiled->SetParameterAt( i, pglGetUniformLocation( handleShader, parameterList.GetAt( i ).GetString() ) );
		}
		
		// bind uniform blocks
		if( pglGetUniformBlockIndex && pglUniformBlockBinding ){
			count = uniformBlockList.GetCount();
			for( i=0; i<count; i++ ){
				location = pglGetUniformBlockIndex( handleShader, uniformBlockList.GetNameAt( i ) );
				if( location != -1 ){
					OGL_CHECK( pRenderThread, pglUniformBlockBinding(
						handleShader, location, uniformBlockList.GetTargetAt( i ) ) );
				}
			}
		}
		
		// bind shader storage blocks
		if( pglGetUniformBlockIndex && pglShaderStorageBlockBinding ){
			count = shaderStorageBlockList.GetCount();
			for( i=0; i<count; i++ ){
				location = pglGetUniformBlockIndex( handleShader, shaderStorageBlockList.GetNameAt( i ) );
				if( location != -1 ){
					OGL_CHECK( pRenderThread, pglShaderStorageBlockBinding(
						handleShader, location, shaderStorageBlockList.GetTargetAt( i ) ) );
				}
			}
		}
		
		// bind feedback variables. this is done after the linking if only the NV transform feedback extension exists
		if( ! pglTransformFeedbackVaryings && pglTransformFeedbackVaryingsNV ){
			count = feedbackList.GetCount();
			
			if( count > 0 ){
				int * const locations = new int[ count ];
				
				for( i=0; i<count; i++ ){
					locations[ i ] = pglGetVaryingLocationNV( handleShader, feedbackList.GetAt( i ).GetString() );
					if( locations[ i ] == -1 ){
						DETHROW( deeInvalidParam );
					}
				}
				
				OGL_CHECK( pRenderThread, pglTransformFeedbackVaryingsNV( handleShader, count, locations, GL_INTERLEAVED_ATTRIBS ) );
				
				delete [] locations;
			}
		}
		
		// deactivate the shader is not required since we set the active one to NULL already.
		//OGL_CHECK( pRenderThread, pglUseProgramObject( 0 ) );
		
	}catch( const deException &e ){
		e.PrintError();
		if( compiled ){
			delete compiled;
		}
		throw;
	}
	
	// finished compiling
	return compiled;
}



// Private Functions
//////////////////////

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
	
	// add symbols
	pPreprocessor.SetSymbolsFromDefines( defines );
	
	if( ! defines.HasDefineNamed( "HIGHP" ) ){
		pPreprocessor.SetSymbol( "HIGHP", "highp" );
	}
	
	#ifdef ANDROID
	pPreprocessor.SetSymbol( "ANDROID", "1" );
	//pPreprocessor.SourcesAppend( "float modf( in float x, out float i ){ i=floor(x); return fract(x); }\n" );
	#endif
}



#ifdef ANDROID
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
#ifdef ANDROID
	pRenderThread.GetLogger().LogErrorFormat( "%s_%.3i.shader", file, pShaderFileNumber++ );
	pRenderThread.GetLogger().LogError( pPreprocessor.GetSources() );
	
#else
	char buffer[ 256 ];
	sprintf( &buffer[ 0 ], "%s_%.3i.shader", file, pShaderFileNumber++ );
	FILE *handle = fopen( buffer, "w" );
	if( handle ){
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
