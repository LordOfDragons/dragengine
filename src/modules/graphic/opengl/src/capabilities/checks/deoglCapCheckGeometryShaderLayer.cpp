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

#include "deoglCapCheckGeometryShaderLayer.h"
#include "../deoglCapabilities.h"
#include "../deoglCapsTextureFormat.h"
#include "../../extensions/deoglExtensions.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../shaders/deoglShaderCompiled.h"
#include "../../shaders/deoglShaderDefines.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderSources.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shaders/paramblock/deoglSPBParameter.h"

#include <dragengine/common/exceptions.h>



// Class deoglCapCheckGeometryShaderLayer
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCapCheckGeometryShaderLayer::deoglCapCheckGeometryShaderLayer(
deoglCapabilities &capabilities ) :
pCapabilities( capabilities ),
pWorking( true ){
}



// Management
///////////////

void deoglCapCheckGeometryShaderLayer::Check( GLuint fbo ){
	// Summary:
	//    The geometry shader layer bug has been seen on the ATI and nVidia graphic driver for the
	//    linux system ( 12.6_beta_pre897:legacy for ATI and the most recent drivers for nVidia ).
	//    The presence of this bug prevents using geometry shaders to render to different layers
	//    in a framebuffer by assigning a different layer to different primitives generated in a
	//    single geometry shader run.
	//    
	// Background:
	//    Using geometry shaders in OpenGL generated primitives can be redirected to a specific layer
	//    in the attached textures of a framebuffer using the glLayer variable. For this glLayer has
	//    to be set to the layer to emit a primitive to before the EndPrimitive method is called.
	//    The important point is that for multiple primitives emitted in a single geometry shader run
	//    different layers can be specified. This allows for multi-pass rendering to be collapsed into
	//    a single run version where primitives are rendered multiple times (duplication in the
	//    geometry shader) but each copy redirected to a different layer in the framebuffer textures.
	//    This is typically used to render to cube maps in a single run for example.
	//    
	// Bug Behavior:
	//    In a bugged driver glLayer can be set but nevertheless the first value assigned to glLayer
	//    is used for all emitted primitives breaking the rendering algorithm. In a correct driver
	//    the last set value for glLayer is used for each call to EndPrimitive
	//    
	// Test Procedure:
	//    A cube map of size 1x1 is cleared to all black. Then it is attached to a framebuffer as
	//    entire cube map. A single full screen quad is rendered using a geometry shader duplicating
	//    the quad 6 times each writing to a different layer. The cube map is then read back to the
	//    CPU memory. If the driver has a correct implementation all pixels have a value that is not
	//    black. If the driver has the bug all pixels but the one of the first face in the cube map
	//    have still the value of black.
	
	if( ! pglFramebufferTexture ){
		return;
	}
	
	deoglRenderThread &renderThread = pCapabilities.GetRenderThread();
	
	#ifdef ANDROID
		pWorking = false;
		renderThread.GetLogger().LogWarn(
			"Capabilities: Geometry Shader Layer: Working (Android)" );
		return;
	#endif
	
	const deoglCapsTextureFormat &texformat = *pCapabilities.GetFormats()
		.GetUseFBOTexCubeFormatFor( deoglCapsFmtSupport::eutfR8 );
	GLubyte pixels[ 6 ] = { 0, 0, 0, 0, 0, 0 };
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	deoglShaderProgram *shader = NULL;
	deoglShaderSources *sources;
	deoglShaderDefines defines;
	GLuint cubemap = 0;
	int i;
	
	try{
		// load shader
		sources = shaderManager.GetSourcesNamed( "Test Geometry Shader Layer Bug" );
		if( ! sources ){
			DETHROW( deeInvalidParam );
		}
		shader = shaderManager.GetProgramWith( sources, defines );
		
		// generate test cube map
		OGL_CHECK( renderThread, glGenTextures( 1, &cubemap ) );
		if( ! cubemap ){
			DETHROW( deeOutOfMemory );
		}
		
		OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_CUBE_MAP, cubemap ) );
		
		OGL_CHECK( renderThread, glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ) );
		for( i=0; i<6; i++ ){
			OGL_CHECK( renderThread, glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
				texformat.GetFormat(), 1, 1, 0, texformat.GetPixelFormat(),
				texformat.GetPixelType(), ( GLvoid* )&pixels[ i ] ) );
		}
		OGL_CHECK( renderThread, glPixelStorei( GL_UNPACK_ALIGNMENT, 4 ) );
		
		OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_CUBE_MAP, cubemap ) );
		OGL_CHECK( renderThread, glTexParameteri(
			GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) );
		OGL_CHECK( renderThread, glTexParameteri(
			GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) );
		OGL_CHECK( renderThread, glTexParameteri(
			GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT ) );
		OGL_CHECK( renderThread, glTexParameteri(
			GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT ) );
		OGL_CHECK( renderThread, glTexParameteri(
			GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT ) );
		
		// bind test cube map to fbo and set render parameters
		OGL_CHECK( renderThread, pglFramebufferTexture( GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0, cubemap, 0 ) );
		const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
		OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( renderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		
		OGL_CHECK( renderThread, glViewport( 0, 0, 1, 1 ) );
		OGL_CHECK( renderThread, glScissor( 0, 0, 1, 1 ) );
		OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
		
		OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
		OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
		OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
		OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
		OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
		OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
		
		// set shader
		renderThread.GetShader().ActivateShader( shader );
		
		// render and unbind from fbo
		OGL_CHECK( renderThread, pglBindVertexArray( pCapabilities.GetFSQuadVAO() ) );
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLES, 0, 6 ) );
		OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
		
		OGL_CHECK( renderThread, pglFramebufferTexture( GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0, 0, 0 ) );
		
		// retrieve the results and clean up
		OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_CUBE_MAP, cubemap ) );
		for( i=0; i<6; i++ ){
			OGL_CHECK( renderThread, glGetTexImage( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
				texformat.GetPixelFormat(), texformat.GetPixelType(), ( GLvoid* )&pixels[ i ] ) );
		}
		OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_CUBE_MAP, 0 ) );
		
		OGL_CHECK( renderThread, glDeleteTextures( 1, &cubemap ) );
		cubemap = 0;
		
		shader->RemoveUsage();
		
	}catch( const deException & ){
		if( shader ){
			shader->RemoveUsage();
		}
		if( cubemap ){
			glDeleteTextures( 1, &cubemap );
		}
		
		throw;
	}
	
	// evaluate the results
// 	renderThread.GetLogger().LogInfoFormat( "pTestBugGeometryShaderLayer: %d %d %d %d %d %d\n",
// 		pixels[ 0 ], pixels[ 1 ], pixels[ 2 ], pixels[ 3 ], pixels[ 4 ], pixels[ 5 ] );
	for( i=0; i<6; i++ ){
		if( pixels[ i ] == 0 ){
			pWorking = false;
		}
	}
	
	if( pWorking ){
		renderThread.GetLogger().LogInfo( "Capabilities: Geometry Shader Layer: Working" );
		
	}else{
		renderThread.GetLogger().LogWarn( "Capabilities: Geometry Shader Layer: Driver Bug!" );
	}
}
