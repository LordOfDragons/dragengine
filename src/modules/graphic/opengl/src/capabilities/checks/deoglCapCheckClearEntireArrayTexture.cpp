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

#include "deoglCapCheckClearEntireArrayTexture.h"
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



// Class deoglCapCheckClearEntireArrayTexture
///////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCapCheckClearEntireArrayTexture::deoglCapCheckClearEntireArrayTexture(
deoglCapabilities &capabilities ) :
pCapabilities( capabilities ),
pWorking( true ){
}



// Management
///////////////

void deoglCapCheckClearEntireArrayTexture::Check( GLuint fbo ){
	// Summary:
	//    The clear entire array texture bug has been seen on the ATI graphic driver for the linux
	//    system ( 12.6_beta_pre897:legacy ) and nvidia. The presence of this bug prevents all layers
	//    of an array texture to be properly cleared with a single clear call.
	//    
	// Background:
	//    Clearing textures in OpenGL works on attached texture objects. In older OpenGL versions
	//    this has been only single layer textures hence 1D, 2D and 3D textures. With array textures
	//    the clearing operation works on a single layer inside the entire array texture. Since the FBO
	//    extensions it is though possible to attach an entire array texture as a render target instead
	//    of just a single layer. OpenGL demands in the specifications that a clear operation on an array
	//    texture bound as array texture not as array texture layer clears all array texture layers.
	//    
	// Bug Behavior:
	//    In a bugged driver clearing an entire array texture does only clear the first layer leaving the
	//    other layers untouched.
	//    
	// Test Procedure:
	//    An array texture of size 1x1 with 5 levels is filled with a depth value of 0.5 . The array
	//    texture is bound as a array texture not as an array texture layer. The array texture is then
	//    cleared with 1. The array texture is then read back to the CPU memory. If the driver has a correct
	//    implementation all pixels of all layers have the value of 1. If the driver has the bug the pixels
	//    of one or more layers have the value 0.5 .
	
	if( ! pglFramebufferTexture ){
		return;
	}
	
	deoglRenderThread &renderThread = pCapabilities.GetRenderThread();
	
	#ifdef ANDROID
		pWorking = false;
		renderThread.GetLogger().LogWarn(
			"Capabilities: Clear entire array texture: Working (Android)" );
		return;
	#endif
	
	const deoglCapsTextureFormat &texformat = *pCapabilities.GetFormats()
		.GetUseFBOArrayTexFormatFor( deoglCapsFmtSupport::eutfDepth );
	GLfloat pixels[ 5 ] = { 0.5f, 0.5f, 0.5f, 0.5f, 0.5f };
	GLuint texture = 0;
	int i;
	
	try{
		// generate test array texture
		OGL_CHECK( renderThread, glGenTextures( 1, &texture ) );
		if( ! texture ){
			DETHROW( deeOutOfMemory );
		}
		
		OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_2D_ARRAY, texture ) );
		OGL_CHECK( renderThread, glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ) );
		OGL_CHECK( renderThread, pglTexImage3D( GL_TEXTURE_2D_ARRAY, 0,
			texformat.GetFormat(), 1, 1, 5, 0, texformat.GetPixelFormat(),
			texformat.GetPixelType(), ( GLvoid* )&pixels ) );
		OGL_CHECK( renderThread, glPixelStorei( GL_UNPACK_ALIGNMENT, 4 ) );
		
		// bind test array texture to fbo and set render parameters
		OGL_CHECK( renderThread, pglFramebufferTexture( GL_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT, texture, 0 ) );
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
		
		// clear array texture and unbind from fbo
		const GLfloat clearDepth = 1.0f;
		OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
		
		OGL_CHECK( renderThread, pglFramebufferTexture( GL_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT, 0, 0 ) );
		
		// retrieve the results and clean up
		OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_2D_ARRAY, texture ) );
		OGL_CHECK( renderThread, glGetTexImage( GL_TEXTURE_2D_ARRAY, 0,
			GL_DEPTH_COMPONENT, GL_FLOAT, ( GLvoid* )&pixels ) );
		OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_2D_ARRAY, 0 ) );
		
		OGL_CHECK( renderThread, glDeleteTextures( 1, &texture ) );
		texture = 0;
		
	}catch( const deException & ){
		if( texture ){
			glDeleteTextures( 1, &texture );
		}
		
		throw;
	}
	
	// evaluate the results
	//printf( "pTestBugClearEntireArrayTexture: %g %g %g %g %g\n",
	//	pixels[ 0 ], pixels[ 1 ], pixels[ 2 ], pixels[ 3 ], pixels[ 4 ] );
	for( i=0; i<5; i++ ){
		if( fabsf( pixels[ i ] - 1.0f ) > 1e-3f ){
			pWorking = false;
		}
	}
	
	if( pWorking ){
		renderThread.GetLogger().LogInfo( "Capabilities: Clear entire array texture: Working" );
		
	}else{
		renderThread.GetLogger().LogWarn( "Capabilities: Clear entire array texture: Driver Bug!" );
	}
}
