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

#include "deoglCapCheckClearEntireCubeMap.h"
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



// Class deoglCapCheckClearEntireCubeMap
///////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCapCheckClearEntireCubeMap::deoglCapCheckClearEntireCubeMap(
deoglCapabilities &capabilities ) :
pCapabilities( capabilities ),
pWorking( true ){
}



// Management
///////////////

void deoglCapCheckClearEntireCubeMap::Check( GLuint fbo ){
	// Summary:
	//    The clear entire cube map bug has been seen on the ATI graphic driver for the linux
	//    system ( 12.6_beta_pre897:legacy ) as well as nvidia. The presence of this bug prevents
	//    all faces of a cube map to be properly cleared with a single clear call.
	//    
	// Background:
	//    Clearing textures in OpenGL works on attached texture objects. In older OpenGL versions
	//    this has been only single layer textures hence 1D, 2D and 3D textures. With cube maps
	//    the clearing operation works on a single face inside the cube map. Since the FBO extensions
	//    it is though possible to attach an entire cube map as a render target instead of just a
	//    single face. OpenGL demands in the specifications that a clear operation on a cube map
	//    bound as cube map not as cube map face clears all cube map faces.
	//    
	// Bug Behavior:
	//    In a bugged driver clearing an entire cube map does only clear the first face leaving the
	//    other faces untouched.
	//    
	// Test Procedure:
	//    A cube map of size 1x1 is filled with a depth value of 0.5 . The cube map is bound as a
	//    cube map not as a cube map face. The cube map is then cleared with 1. The cube map is then read
	//    back to the CPU memory. If the driver has a correct implementation all pixels of all faces
	//    have the value of 1. If the driver has the bug the pixels of one or more face have the
	//    value 0.5 .
	
	if( ! pglFramebufferTexture ){
		return;
	}
	
	deoglRenderThread &renderThread = pCapabilities.GetRenderThread();
	
	#ifdef ANDROID
		pWorking = false;
		renderThread.GetLogger().LogWarn(
			"Capabilities: Clear entire cube map: Working (Android)" );
		return;
	#endif
	
	const deoglCapsTextureFormat &texformat = *pCapabilities.GetFormats()
		.GetUseFBOTexCubeFormatFor( deoglCapsFmtSupport::eutfDepth );
	GLfloat pixels[ 6 ] = { 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f };
	GLuint cubemap = 0;
	int i;
	
	try{
		// generate test cube map
		OGL_CHECK( renderThread, glGenTextures( 1, &cubemap ) );
		if( ! cubemap ){
			DETHROW( deeOutOfMemory );
		}
		
		OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_CUBE_MAP, cubemap ) );
		
		OGL_CHECK( renderThread, glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ) );
		for( i=0; i<6; i++ ){
			OGL_CHECK( renderThread, glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, texformat.GetFormat(), 1, 1, 0, texformat.GetPixelFormat(),
				texformat.GetPixelType(), ( GLvoid* )&pixels[ 0 ] ) );
		}
		OGL_CHECK( renderThread, glPixelStorei( GL_UNPACK_ALIGNMENT, 4 ) );
		
		// bind test cube map to fbo and set render parameters
		OGL_CHECK( renderThread, pglFramebufferTexture( GL_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT, cubemap, 0 ) );
		const GLenum buffers[ 1 ] = { GL_NONE };
		OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( renderThread, glReadBuffer( GL_NONE ) );
		
		OGL_CHECK( renderThread, glViewport( 0, 0, 1, 1 ) );
		OGL_CHECK( renderThread, glScissor( 0, 0, 1, 1 ) );
		OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
		
		OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
		OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
		OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
		OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
		OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ) );
		OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
		
		// clear cube map and unbind from fbo
		const GLfloat clearDepth = 1.0f;
		OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
		
		OGL_CHECK( renderThread, pglFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0 ) );
		
		// retrieve the results and clean up
		OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_CUBE_MAP, cubemap ) );
		for( i=0; i<6; i++ ){
			OGL_CHECK( renderThread, glGetTexImage( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_DEPTH_COMPONENT, GL_FLOAT, ( GLvoid* )&pixels[ i ] ) );
		}
		OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_CUBE_MAP, 0 ) );
		
		OGL_CHECK( renderThread, glDeleteTextures( 1, &cubemap ) );
		cubemap = 0;
		
	}catch( const deException & ){
		if( cubemap ){
			glDeleteTextures( 1, &cubemap );
		}
		
		throw;
	}
	
	// evaluate the results
// 	renderThread.GetLogger().LogInfoFormat( "pTestBugClearEntireCubeMap: %g %g %g %g %g %g\n",
// 		pixels[ 0 ], pixels[ 1 ], pixels[ 2 ], pixels[ 3 ], pixels[ 4 ], pixels[ 5 ] );
	for( i=0; i<6; i++ ){
		if( fabsf( pixels[ i ] - 1.0f ) > 1e-3f ){
			pWorking = false;
		}
	}
	
	if( pWorking ){
		renderThread.GetLogger().LogInfo( "Capabilities: Clear entire cube map: Working" );
		
	}else{
		renderThread.GetLogger().LogWarn( "Capabilities: Clear entire cube map: Driver Bug!" );
	}
}
