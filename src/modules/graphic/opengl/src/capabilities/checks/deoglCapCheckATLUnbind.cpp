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

#include "deoglCapCheckATLUnbind.h"
#include "../deoglCapabilities.h"
#include "../../extensions/deoglExtensions.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>



// Class deoglCapCheckATLUnbind
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCapCheckATLUnbind::deoglCapCheckATLUnbind( deoglCapabilities &capabilities ) :
pCapabilities( capabilities ),
pResult( 0 ){
}



// Management
///////////////

void deoglCapCheckATLUnbind::Check( GLuint fbo ){
	deoglRTLogger &logger = pCapabilities.GetRenderThread().GetLogger();
	
	if( ! pglFramebufferTexture ){
		pResult = 1; // hm...
		return;
	}
	
	// this version works on ati but causes invalid_operation on nvidia
	glGetError(); // clear error code
	pglFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0 );
	if( glGetError() == GL_NO_ERROR ){
		pResult = 1;
	}
	
	// this version works on nvidia but causes invalid_operation on ati
	if( pResult == 0 ){
		glGetError(); // clear error code
		pglFramebufferTextureLayer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0, 0 );
		if( glGetError() == GL_NO_ERROR ){
			pResult = 2;
		}
	}
	
	if( pResult == 0 ){
		logger.LogError( "Capabilities: ATL Unbind: No working version found!" );
		DETHROW( deeInvalidAction );
	}
	
	logger.LogInfoFormat( "Capabilities: ATL Unbind: Method %d", pResult );
}

#if 0
// Example bug test.
void deoglCapCheckATLUnbind::pTestBugExample( GLuint fbo ){
	const deoglCapsTextureFormat &texformat = *pFormats.GetUseFBOTex2DFormatFor( deoglCapsFmtSupport::eutfR16 );
	GLuint texture = 0;
	
	// generate test texture
	OGL_CHECK( pRenderThread, glGenTextures( 1, &texture ) );
	if( ! texture ){
		DETHROW( deeOutOfMemory );
	}
	
	OGL_CHECK( pRenderThread, glBindTexture( GL_TEXTURE_2D, texture ) );
	OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) );
	OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) );
	OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) );
	OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) );
	
	OGL_CHECK( pRenderThread, glTexImage2D( GL_TEXTURE_2D, 0, texformat.GetFormat(), 512, 1, 0, texformat.GetPixelFormat(), texformat.GetPixelType(), NULL ) );
	
	// bind test texture to fbo and set render parameters
	OGL_CHECK( pRenderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0 ) );
	const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
	OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	
	OGL_CHECK( pRenderThread, glViewport( 0, 0, 512, 1 ) );
	OGL_CHECK( pRenderThread, glScissor( 0, 0, 512, 1 ) );
	OGL_CHECK( pRenderThread, glEnable( GL_SCISSOR_TEST ) );
	
	OGL_CHECK( pRenderThread, glDisable( GL_DEPTH_TEST ) );
	//OGL_CHECK( pRenderThread, glDepthFunc( GL_ALWAYS ) );
	OGL_CHECK( pRenderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( pRenderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( pRenderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( pRenderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( pRenderThread, glDepthMask( GL_FALSE ) );
	
	// set shader and parameters
	pRenderThread.GetLogg().GetRenderThread().GetShader().ActivateShader( pShaderBugExample );
	
	// render and unbind from fbo
	OGL_CHECK( pRenderThread, oglBindVertexArray( pFSQuadVAO ) );
	OGL_CHECK( pRenderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	OGL_CHECK( pRenderThread, oglBindVertexArray( 0 ) );
	
	OGL_CHECK( pRenderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0 ) );
	
	// retrieve the results and clean up
	GLubyte *result = NULL;
	
	result = new GLubyte[ 512 ];
	
	OGL_CHECK( pRenderThread, glBindTexture( GL_TEXTURE_2D, texture ) );
	OGL_CHECK( pRenderThread, glPixelStorei( GL_PACK_ALIGNMENT, 1 ) );
	OGL_CHECK( pRenderThread, glGetTexImage( GL_TEXTURE_2D, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, ( GLvoid* )result ) );
	OGL_CHECK( pRenderThread, glPixelStorei( GL_PACK_ALIGNMENT, 4 ) );
	OGL_CHECK( pRenderThread, glBindTexture( GL_TEXTURE_2D, 0 ) );
	OGL_CHECK( pRenderThread, glDeleteTextures( 1, &texture ) );
	texture = 0;
	
	// evaluate the results
	int i;
	
	pBugGLSLSmoothStep = false;
	
	for( i=0; i<512; i++ ){
		if( result[ i ] > 0 ){
			pBugExample = true;
			pRenderThread.GetLogg().LogWarn( "Example Bug detected!" );
			break;
		}
	}
	
	delete [] result;
}
#endif
