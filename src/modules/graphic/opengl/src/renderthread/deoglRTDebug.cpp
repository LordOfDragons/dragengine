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

#include "deoglRTDebug.h"
#include "deoglRenderThread.h"
#include "deoglRTLogger.h"
#include "../deoglBasics.h"
#include "../deGraphicOpenGl.h"
#include "../debug/deoglDebugSaveTexture.h"
#include "../delayedoperation/deoglDelayedFileWrite.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../devmode/deoglDeveloperMode.h"
#include "../extensions/deoglExtensions.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>



// Callbacks
//////////////

void fDebugOutput( GLenum source, GLenum type, GLuint id, GLenum severity,
GLsizei /*length*/, const GLchar *message, const void *userParam ){
	deoglRenderThread &renderThread = *( ( deoglRenderThread* )userParam );
	if( ! renderThread.GetDebug().GetEnableHwDebugOutput() ){
		return;
	}
	
	bool stackTrace = false; //true;
	
	const char *strSource;
	switch( source ){
	case GL_DEBUG_SOURCE_API_ARB:
		strSource = "API";
		break;
		
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		strSource = "Window System";
		break;
		
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		strSource = "Shader Compiler";
		break;
		
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		strSource = "Third Party";
		break;
		
	case GL_DEBUG_SOURCE_APPLICATION:
		strSource = "Application";
		break;
		
	case GL_DEBUG_SOURCE_OTHER:
		strSource = "Other";
		break;
		
	default:
		strSource = "Unknown";
	}
	
	const char *strType;
	switch( type ){
	case GL_DEBUG_TYPE_ERROR_ARB:
		strType = "Error";
		break;
		
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
		strType = "Deprecated Behavior";
		break;
		
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
		strType = "Undefined Behavior";
		break;
		
	case GL_DEBUG_TYPE_PORTABILITY_ARB:
		strType = "Portability";
		break;
		
	case GL_DEBUG_TYPE_PERFORMANCE_ARB:
		strType = "Performance";
		break;
		
	case GL_DEBUG_TYPE_OTHER_ARB:
		strType = "Other";
		break;
		
	default:
		strType = "Unknown";
	}
	
// 	if( strstr( message, "Buffer detailed info:" ) ){
// 		// nvidia spams this like hell
// 		return;
// 	}
	
// 	if( strstr( message, "Program undefined behavior warning: The current GL state uses a sampler" ) ){
// 		return; // nvidia spams this like hell
// 	}
	
	switch( severity ){
	case GL_DEBUG_SEVERITY_HIGH_ARB:
		renderThread.GetLogger().LogErrorFormat( "Source(%s) Type(%s) ID(0x%x): %s",
			strSource, strType, id, message );
		if( stackTrace ){
			renderThread.GetLogger().LogException( deeInvalidAction( __FILE__, __LINE__ ) );
		}
		break;
		
	case GL_DEBUG_SEVERITY_MEDIUM_ARB:
		if( renderThread.GetConfiguration().GetDoLogWarn() ){
			renderThread.GetLogger().LogWarnFormat( "Source(%s) Type(%s) ID(0x%x): %s",
				strSource, strType, id, message );
			if( stackTrace ){
				renderThread.GetLogger().LogException( deeInvalidAction( __FILE__, __LINE__ ) );
			}
		}
		break;
		
	case GL_DEBUG_SEVERITY_LOW_ARB:
		if( renderThread.GetConfiguration().GetDoLogDebug() ){
			renderThread.GetLogger().LogInfoFormat( "Source(%s) Type(%s) ID(0x%x): %s",
				strSource, strType, id, message );
			if( stackTrace ){
				renderThread.GetLogger().LogException( deeInvalidAction( __FILE__, __LINE__ ) );
			}
		}
		break;
		
	default:
		break;
	}
}

void fDebugOutputAMD( GLuint id, GLenum category, GLenum severity, GLsizei length,
const GLchar *message, void *userParam ){
	deoglRenderThread &renderThread = *( ( deoglRenderThread* )userParam );
	if( ! renderThread.GetDebug().GetEnableHwDebugOutput() ){
		return;
	}
	
	const char *strCategory;
	switch( category ){
	case GL_DEBUG_CATEGORY_API_ERROR_AMD:
		strCategory = "API";
		break;
		
	case GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD:
		strCategory = "Window System";
		break;
		
	case GL_DEBUG_CATEGORY_DEPRECATION_AMD:
		strCategory = "Deprecation";
		break;
		
	case GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD:
		strCategory = "Undefined Behavior";
		break;
		
	case GL_DEBUG_CATEGORY_PERFORMANCE_AMD:
		strCategory = "Performance";
		break;
		
	case GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD:
		strCategory = "Shader Compiler";
		break;
		
	case GL_DEBUG_CATEGORY_APPLICATION_AMD:
		strCategory = "Application";
		break;
		
	case GL_DEBUG_CATEGORY_OTHER_AMD:
		strCategory = "Other";
		break;
		
	default:
		strCategory = "Unknown";
	}
	
	switch( severity ){
	case GL_DEBUG_SEVERITY_HIGH_AMD:
		renderThread.GetLogger().LogErrorFormat( "Category(%s) ID(0x%x): %s",
			strCategory, id, message );
		break;
		
	case GL_DEBUG_SEVERITY_MEDIUM_AMD:
		if( renderThread.GetConfiguration().GetDoLogWarn() ){
			renderThread.GetLogger().LogWarnFormat( "Category(%s) ID(0x%x): %s",
				strCategory, id, message );
		}
		break;
		
	case GL_DEBUG_SEVERITY_LOW_AMD:
		if( renderThread.GetConfiguration().GetDoLogDebug() ){
			renderThread.GetLogger().LogInfoFormat( "Category(%s) ID(0x%x): %s",
				strCategory, id, message );
		}
		break;
		
	default:
		break;
	}
}



// Class deoglRTDebug
////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTDebug::deoglRTDebug( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pDebugSaveTexture( NULL ),
pDeveloperMode( NULL ),
pEnableHwDebugOutput( true ),
pDebugMemoryConsumption( renderThread ),
pEnableDebugTrace( false )
{
	try{
		if( renderThread.GetConfiguration().GetDebugContext() ){
			// enable debug output
			if( renderThread.GetExtensions().GetHasExtension( deoglExtensions::ext_KHR_debug )
			|| renderThread.GetExtensions().GetHasExtension( deoglExtensions::ext_ARB_debug_output ) ){
				pglDebugMessageCallback( fDebugOutput, &renderThread );
				
				glEnable( GL_DEBUG_OUTPUT );
				
				if( renderThread.GetConfiguration().GetDebugNoMessages() ){
					renderThread.GetLogger().LogInfo( "Suppress hardware debug messages" );
					pglDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_FALSE );
					
				}else{
// 					glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
					
					pglDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE );
					
					
					// disable message not interesting for us but flodding the logs
					GLuint disableMessages[ 20 ];
					int disableMessagesCount = 0;
					
					
					// nVidia:
					//   Source(API) Type(Other) ID(20084): Texture state usage warning:
					//     The texture object (0) bound to texture image unit 0 does not
					//     have a defined base level and cannot be used for texture mapping.
					disableMessages[ disableMessagesCount++ ] = 0x20084;
					
					// nVidia:
					//   Source(API) Type(Other) ID(20061): Framebuffer detailed info:
					//     The driver allocated storage for renderbuffer 0.
					disableMessages[ disableMessagesCount++ ] = 0x20061;
					
					// nVidia:
					//   Source(API) Type(Other) ID(0x20060): Framebuffer info: The drawbuffer
					//     supplied (34854) is currently bound to NONE, no clear will take place.
					disableMessages[ disableMessagesCount++ ] = 0x20060;
					
					// nVidia:
					//   Source(API) Type(Other) ID(0x20081): Texture state detailed info:
					//     GL_TEXTURE_BUFFER_EXT texture 349 was (re)sized to store 145 elements.
					disableMessages[ disableMessagesCount++ ] = 0x20081;
					
					if( disableMessagesCount > 0 ){
						pglDebugMessageControl( GL_DEBUG_SOURCE_API_ARB, GL_DEBUG_TYPE_OTHER_ARB,
							GL_DONT_CARE, disableMessagesCount, &disableMessages[ 0 ], GL_FALSE );
					}
					
					
					disableMessagesCount = 0;
					
					// nVidia:
					//   Source(API) Type(Performance) ID(0x20072): Buffer performance warning:
					//     Buffer object 34 (bound to GL_TRANSFORM_FEEDBACK_BUFFER_NV (0),
					//     GL_ARRAY_BUFFER_ARB, and GL_TRANSFORM_FEEDBACK_BUFFER_NV, usage hint 
					//     is GL_STREAM_READ) is being copied/moved from VIDEO memory to HOST memory.
					disableMessages[ disableMessagesCount++ ] = 0x20072;
					
					// nVidia:
					//   Source(API) Type(Performance) ID(0x20092): Program/shader state performance
					//     warning: Vertex shader in program 15 is being recompiled based on GL state.
					disableMessages[ disableMessagesCount++ ] = 0x20092;
					
					if( disableMessagesCount > 0 ){
						pglDebugMessageControl( GL_DEBUG_SOURCE_API_ARB, GL_DEBUG_TYPE_PERFORMANCE_ARB,
							GL_DONT_CARE, disableMessagesCount, &disableMessages[ 0 ], GL_FALSE );
					}
					
					
					disableMessagesCount = 0;
					
					// nVidia:
					//   Source(API) Type(Undefined Behavior) ID(0x20096): Program undefined
					//     behavior warning: The current GL state uses a sampler (1) that has
					//     depth comparisons disabled, with a texture object (355) with a depth
					//     format, by a shader that samples it with a shadow sampler.
					//     Using this state to sample would result in undefined behavior.
					disableMessages[ disableMessagesCount++ ] = 0x20096;
					
					if( disableMessagesCount > 0 ){
						pglDebugMessageControl( GL_DEBUG_SOURCE_API_ARB, GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB,
							GL_DONT_CARE, disableMessagesCount, &disableMessages[ 0 ], GL_FALSE );
					}
					
					// done
					if( renderThread.GetExtensions().GetHasExtension( deoglExtensions::ext_KHR_debug ) ){
						renderThread.GetLogger().LogInfo( "Debugging callback using KHR_debug activated" );
						
					}else{
						renderThread.GetLogger().LogInfo( "Debugging callback using ARB_debug_output activated" );
					}
				}
				
			}else if( renderThread.GetExtensions().GetHasExtension( deoglExtensions::ext_AMD_debug_output ) ){
				pglDebugMessageCallbackAMD( fDebugOutputAMD, this );
				pglDebugMessageEnableAMD( 0, 0, 0, NULL, GL_TRUE );
				renderThread.GetLogger().LogInfo( "Debugging callback using AMD_debug_output activated" );
			}
			
			// enable debug trace grouping
			pEnableDebugTrace = renderThread.GetExtensions().GetHasExtension( deoglExtensions::ext_KHR_debug );
		}
		
		pDebugSaveTexture = new deoglDebugSaveTexture( renderThread );
		pDebugSaveTexture->SetBasePath( "/capture/debug_save_texture" );
		
		pDeveloperMode = new deoglDeveloperMode( renderThread );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRTDebug::~deoglRTDebug(){
	pCleanUp();
}



// Management
///////////////

decBaseFileWriter *deoglRTDebug::OpenFileForWriting( const decPath &path ){
	deoglDelayedFileWrite *fileWrite = NULL;
	
	try{
		fileWrite = new deoglDelayedFileWrite( path );
		pRenderThread.GetDelayedOperations().AddFileWrite( fileWrite );
		
	}catch( const deException & ){
		if( fileWrite ){
			delete fileWrite;
		}
		throw;
	}
	
	return fileWrite->GetFileWriter();
}

void deoglRTDebug::SetEnableHwDebugOutput( bool enable ){
	pEnableHwDebugOutput = enable;
}

void deoglRTDebug::BeginDebugGroup( const char *name, int id ){
	if( pEnableDebugTrace ){
		pglPushDebugGroup( GL_DEBUG_SOURCE_APPLICATION, id, strlen( name ), name );
	}
}

void deoglRTDebug::EndDebugGroup(){
	if( pEnableDebugTrace ){
		pglPopDebugGroup();
	}
}

void deoglRTDebug::SetDebugObjectLabel( GLenum type, GLuint object, const char *name ){
	if( pEnableDebugTrace ){
		pglObjectLabel( type, object, strlen( name ), name );
	}
}



// Private Functions
//////////////////////

void deoglRTDebug::pCleanUp(){
	if( pDeveloperMode ){
		delete pDeveloperMode;
	}
	if( pDebugSaveTexture ){
		delete pDebugSaveTexture;
	}
	
	if( pglDebugMessageCallback ){
		pglDebugMessageCallback( NULL, NULL );
		
	}else if( pglDebugMessageCallbackAMD ){
		pglDebugMessageCallbackAMD( NULL, NULL );
	}
}
