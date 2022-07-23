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

#include "deoglCapCheckRasterizerDiscard.h"
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
#include "../../shaders/deoglShaderProgramUsage.h"
#include "../../shaders/deoglShaderSources.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shaders/paramblock/deoglSPBParameter.h"

#include <dragengine/common/exceptions.h>



// Class deoglCapCheckRasterizerDiscard
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCapCheckRasterizerDiscard::deoglCapCheckRasterizerDiscard(
deoglCapabilities &capabilities ) :
pCapabilities( capabilities ),
pWorking( true ){
}



// Management
///////////////

void deoglCapCheckRasterizerDiscard::Check( GLuint fbo ){
	// Summary:
	//    The GL_RASTERIZER_DISCARD bug has been seen on the ATI graphic driver for the linux
	//    system ( 12.6_beta_pre897:legacy ). The presence of this bug writes pixels to the
	//    active FBO although GL_RASTERIZER_DISCARD is enabled which it should not do.
	//    
	// Background:
	//    Transform Feedback allows to apply a vertex shader on a set of input vertex data and to
	//    write the transfomed vertex attributes to a VBO. This allows to apply calculations on
	//    vertex data without leaving the GPU. By default the fragment shader is also run and the
	//    resulting output written to the active FBO. In some situations this is desirable but in
	//    others we just want the transformed result written to the VBO but no updates done to the
	//    FBO. If GL_RASTERIZER_DISCARD is enabled the processing stops after the vertex shader
	//    producing no fragments to be written to the active FBO speeding up the process.
	//    
	// Bug Behavior:
	//    In a bugged driver GL_RASTERIZER_DISCARD has no effect and although enabled fragments
	//    are produced and written to the active FBO.
	//    
	// Test Procedure:
	//    A texture of size 3x3 is filled with white pixels. A transform feedback shader is activated
	//    writing a constant value to the VBO. No fragment shader is attached which equals to a black
	//    fragment written. A transform feedback is carried out with GL_RASTERIZER_DISCARD enabled.
	//    The texture is then read back. If the driver has a correct implementation all pixels are
	//    left unchange and have a value of 255. If the driver has the bug one or more pixels have a
	//    value not equal to 255. It is not possible to test only the middle pixel since different
	//    implementations render the point not necessarily in the middle pixel but potentially one
	//    pixel to the right of it. Testing all pixels catches all potential scenarios.
	
	if( ! pglUniformBlockBinding ){
		return;
	}
	
	deoglRenderThread &renderThread = pCapabilities.GetRenderThread();
	
	#ifdef ANDROID
		pWorking = true;
		renderThread.GetLogger().LogWarn(
			"Capabilities: Rasterizer Discard: Working (Android)" );
		return;
	#endif
	
	const deoglCapsTextureFormat &texformat = *pCapabilities.GetFormats()
		.GetUseFBOTex2DFormatFor( deoglCapsFmtSupport::eutfR8 );
	GLubyte pixels[ 9 ] = { 255, 255, 255, 255, 255, 255, 255, 255, 255 };
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	deoglShaderSources *sources;
	deoglShaderDefines defines;
	GLuint texture = 0;
	GLuint vbo = 0;
	int i;
	
	try{
		// load shader
		sources = shaderManager.GetSourcesNamed( "Test Bug Rasterizer Discard" );
		if( ! sources ){
			DETHROW( deeInvalidParam );
		}
		deoglShaderProgramUsage shader( shaderManager.GetProgramWith( sources, defines ) );
		
		// generate test texture
		OGL_CHECK( renderThread, glGenTextures( 1, &texture ) );
		if( ! texture ){
			DETHROW( deeOutOfMemory );
		}
		
		OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_2D, texture ) );
		OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) );
		OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) );
		OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) );
		OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) );
		
		OGL_CHECK( renderThread, glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ) );
		OGL_CHECK( renderThread, glTexImage2D( GL_TEXTURE_2D, 0, texformat.GetFormat(), 3, 3, 0,
			texformat.GetPixelFormat(), texformat.GetPixelType(), ( GLvoid* )&pixels[ 0 ] ) );
		OGL_CHECK( renderThread, glPixelStorei( GL_UNPACK_ALIGNMENT, 4 ) );
		
		// bind test texture to fbo and set render parameters
		OGL_CHECK( renderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0 ) );
		const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
		OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( renderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		
		OGL_CHECK( renderThread, glViewport( 0, 0, 3, 3 ) );
		OGL_CHECK( renderThread, glScissor( 0, 0, 3, 3 ) );
		OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
		
		OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
		OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
		OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
		OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
		OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
		OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
		
		// set shader
		renderThread.GetShader().ActivateShader( shader );
		
		// create feedback VBO
		OGL_CHECK( renderThread, pglGenBuffers( 1, &vbo ) );
		if( ! vbo ){
			DETHROW( deeOutOfMemory );
		}
		OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, vbo ) );
		OGL_CHECK( renderThread, pglBufferData( GL_ARRAY_BUFFER, 4, NULL, GL_STREAM_READ ) );
		
		// render and unbind from fbo
		OGL_CHECK( renderThread, glEnable( GL_RASTERIZER_DISCARD ) );
		OGL_CHECK( renderThread, pglBindBufferBase( GL_TRANSFORM_FEEDBACK_BUFFER, 0, vbo ) );
		OGL_CHECK( renderThread, pglBindVertexArray( pCapabilities.GetFSQuadVAO() ) );
		OGL_CHECK( renderThread, pglBeginTransformFeedback( GL_POINTS ) );
		OGL_CHECK( renderThread, glDrawArrays( GL_POINTS, 0, 1 ) );
		OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
		OGL_CHECK( renderThread, pglEndTransformFeedback() );
		OGL_CHECK( renderThread, glDisable( GL_RASTERIZER_DISCARD ) );
		
		OGL_CHECK( renderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0 ) );
		
		// retrieve the results and clean up
		OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_2D, texture ) );
		OGL_CHECK( renderThread, glPixelStorei( GL_PACK_ALIGNMENT, 1 ) );
		OGL_CHECK( renderThread, glGetTexImage( GL_TEXTURE_2D, 0, GL_RED,
			GL_UNSIGNED_BYTE, ( GLvoid* )&pixels[ 0 ] ) );
		OGL_CHECK( renderThread, glPixelStorei( GL_PACK_ALIGNMENT, 4 ) );
		OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_2D, 0 ) );
		
		pglDeleteBuffers( 1, &vbo );
		vbo = 0;
		
		OGL_CHECK( renderThread, glDeleteTextures( 1, &texture ) );
		texture = 0;
		
	}catch( const deException & ){
		if( vbo ){
			pglDeleteBuffers( 1, &vbo );
		}
		if( texture ){
			glDeleteTextures( 1, &texture );
		}
		throw;
	}
	
	// evaluate the results
	//printf( "pixels: %i %i %i | %i %i %i | %i %i %i\n", pixels[ 0 ], pixels[ 1 ], pixels[ 2 ],
	//	pixels[ 3 ], pixels[ 4 ], pixels[ 5 ], pixels[ 6 ], pixels[ 7 ], pixels[ 8 ] );
	for( i=0; i<9; i++ ){
		if( pixels[ i ] < 255 ){
			pWorking = false;
		}
	}
	
	if( pWorking ){
		renderThread.GetLogger().LogInfo( "Capabilities: Rasterizer Discard: Working" );
		
	}else{
		renderThread.GetLogger().LogWarn( "Capabilities: Rasterizer Discard: Driver Bug!" );
	}
}
