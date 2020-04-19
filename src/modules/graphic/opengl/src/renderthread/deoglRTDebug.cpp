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
GLsizei length, const GLchar *message, const void *userParam ){
	deoglRenderThread &renderThread = *( ( deoglRenderThread* )userParam );
	if( ! renderThread.GetDebug().GetEnableHwDebugOutput() ){
		return;
	}
	
	const char *strSource, *strType;
	
	if( source == GL_DEBUG_SOURCE_API ){
		strSource = "GL";
		
	}else if( source == GL_DEBUG_SOURCE_SHADER_COMPILER ){
		strSource = "GLSL Compiler";
		
	}else if( source == GL_DEBUG_SOURCE_WINDOW_SYSTEM ){
		strSource = "Window System";
		
	}else if( source == GL_DEBUG_SOURCE_THIRD_PARTY ){
		strSource = "Third Party";
		
	}else if( source == GL_DEBUG_SOURCE_APPLICATION ){
		strSource = "Application";
		
	}else{ // GL_DEBUG_SOURCE_OTHER
		strSource = "Other";
	}
	
	if( type == GL_DEBUG_TYPE_ERROR ){
		strType = "Event Error";
		
	}else if( type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR ){
		strType = "Deprecated";
		
	}else if( type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR ){
		strType = "Undefined";
		
	}else if( type == GL_DEBUG_TYPE_PERFORMANCE ){
		strType = "Performance";
		return; // nVidia spams this like hell
		
	}else if( type == GL_DEBUG_TYPE_PORTABILITY ){
		strType = "Portability";
		
	}else{ // GL_DEBUG_TYPE_OTHER
		strType = "Other";
		return; // nVidia spams this like hell
	}
	
// 	if( strstr( message, "Buffer detailed info:" ) ){
// 		// nvidia spams this like hell
// 		return;
// 	}
	
	if( strstr( message, "Program undefined behavior warning: The current GL state uses a sampler" ) ){
		return; // nvidia spams this like hell
	}
	
	if( severity == GL_DEBUG_SEVERITY_HIGH ){
		renderThread.GetLogger().LogErrorFormat( "Source(%s) Type(%s): %s", strSource, strType, message );
		
	}else{ // GL_DEBUG_SEVERITY_MEDIUM or GL_DEBUG_SEVERITY_LOW
		// the configuration parameter has to be stored in the render thread for safe access
		if( renderThread.GetConfiguration().GetDoLogWarn() ){
			renderThread.GetLogger().LogWarnFormat( "Source(%s) Type(%s): %s", strSource, strType, message );
		}
	}
}

void fDebugOutputAMD( GLuint id, GLenum category, GLenum severity, GLsizei length, const GLchar *message, void *userParam ){
	deoglRenderThread &renderThread = *( ( deoglRenderThread* )userParam );
	if( ! renderThread.GetDebug().GetEnableHwDebugOutput() ){
		return;
	}
	
	const char *strCategory;
	
	if( category == GL_DEBUG_CATEGORY_API_ERROR_AMD ){
		strCategory = "GL";
		
	}else if( category == GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD ){
		strCategory = "Window System";
		
	}else if( category == GL_DEBUG_CATEGORY_DEPRECATION_AMD ){
		strCategory = "Deprecated";
		
	}else if( category == GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD ){
		strCategory = "Undefined";
		
	}else if( category == GL_DEBUG_CATEGORY_PERFORMANCE_AMD ){
		strCategory = "Performance";
		
	}else if( category == GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD ){
		strCategory = "GLSL";
		
	}else if( category == GL_DEBUG_CATEGORY_APPLICATION_AMD ){
		strCategory = "Application";
		
	}else{ // GL_DEBUG_CATEGORY_OTHER_AMD
		strCategory = "Other";
	}
	
	if( severity == GL_DEBUG_SEVERITY_HIGH_AMD ){
		renderThread.GetLogger().LogErrorFormat( "Category(%s) ID(%u): %s", strCategory, id, message );
		
	}else{ // GL_DEBUG_SEVERITY_MEDIUM_AMD or GL_DEBUG_SEVERITY_LOW_AMD
		// the configuration parameter has to be stored in the render thread for safe access
		if( renderThread.GetConfiguration().GetDoLogWarn() ){
			renderThread.GetLogger().LogWarnFormat( "Category(%s) ID(%u): %s", strCategory, id, message );
		}
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
pEnableHwDebugOutput( true )
{
	try{
		if( renderThread.GetConfiguration().GetDebugContext() ){
			if( renderThread.GetExtensions().GetHasExtension( deoglExtensions::ext_KHR_debug ) ){
				pglDebugMessageCallback( fDebugOutput, &renderThread );
				glEnable( GL_DEBUG_OUTPUT );
				pglDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true );
				renderThread.GetLogger().LogInfo( "Debugging callback using KHR_debug activated" );
				
			}else if( renderThread.GetExtensions().GetHasExtension( deoglExtensions::ext_ARB_debug_output ) ){
				pglDebugMessageCallback( fDebugOutput, &renderThread );
				glEnable( GL_DEBUG_OUTPUT );
				pglDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true );
				renderThread.GetLogger().LogInfo( "Debugging callback using ARB_debug_output activated" );
				
			}else if( renderThread.GetExtensions().GetHasExtension( deoglExtensions::ext_AMD_debug_output ) ){
				pglDebugMessageCallbackAMD( fDebugOutputAMD, this );
				pglDebugMessageEnableAMD( 0, 0, 0, NULL, true );
				renderThread.GetLogger().LogInfo( "Debugging callback using AMD_debug_output activated" );
			}
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
		pglDebugMessageCallback( ( GLDEBUGPROCARB )fDebugOutput, NULL );
		
	}else if( pglDebugMessageCallbackAMD ){
		pglDebugMessageCallbackAMD( ( GLDEBUGPROCAMD )fDebugOutputAMD, NULL );
	}
}

