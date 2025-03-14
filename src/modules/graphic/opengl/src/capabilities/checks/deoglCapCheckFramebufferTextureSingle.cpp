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

#include "deoglCapCheckFramebufferTextureSingle.h"
#include "../deoglCapabilities.h"
#include "../deoglCapsTextureFormat.h"
#include "../../extensions/deoglExtensions.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>



// Class deoglCapCheckFramebufferTextureSingle
////////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCapCheckFramebufferTextureSingle::deoglCapCheckFramebufferTextureSingle(
deoglCapabilities &capabilities ) :
pCapabilities( capabilities ),
pWorking( true ){
}



// Management
///////////////

void deoglCapCheckFramebufferTextureSingle::Check( GLuint fbo ){
	// Summary:
	//    Checks if glFramebufferTexture accepts single textures like 2D texture in addition
	//    to cube-maps and array-textures.
	//    
	// Background:
	//    glFramebufferTexture is used to attach entire cube-maps and array-textures to
	//    enable layered rendering. This function also accepts single textures like 2D
	//    textures similar to glFramebufferTexture2D. The OpenGL Specification does not
	//    rule out this usage of this function.
	//    
	// Bug Behavior:
	//    In a bugged driver calling glFramebufferTexture with a 2D texture results in
	//    GL_INVALID_OPERATION. This wrong behavior is found on MESA 18.0.5 and is confirmed
	//    to exist up to 18.0.10 . The wrong behavior is exhibited on all possible
	//    attachments of a framebuffer.
	//    
	// Test Procedure:
	//    A 2D texture is attached using glFramebufferTexture. If GL_INVALID_OPERATION is
	//    thrown then the driver is broken
	
	if( ! pglFramebufferTexture ){
		return;
	}
	
	deoglRenderThread &renderThread = pCapabilities.GetRenderThread();
	
	const deoglCapsTextureFormat &texformat = pCapabilities.GetFormats()
		.RequireUseFBOArrayTexFormatFor(deoglCapsFmtSupport::eutfDepth);
	GLfloat pixels[ 1 ] = { 0.0f };
	GLuint texture = 0;
	
	try{
		// generate test array texture
		OGL_CHECK( renderThread, glGenTextures( 1, &texture ) );
		if( ! texture ){
			DETHROW( deeOutOfMemory );
		}
		
		OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_2D, texture ) );
		OGL_CHECK( renderThread, glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ) );
		OGL_CHECK( renderThread, glTexImage2D( GL_TEXTURE_2D, 0,
			texformat.GetFormat(), 1, 1, 0, texformat.GetPixelFormat(),
			texformat.GetPixelType(), ( GLvoid* )&pixels ) );
		OGL_CHECK( renderThread, glPixelStorei( GL_UNPACK_ALIGNMENT, 4 ) );
		
		// bind test texture to fbo
		oglClearError();
		pglFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0 );
		
		if( glGetError() == GL_NO_ERROR ){
			// unbind texture
			OGL_CHECK( renderThread, pglFramebufferTexture( GL_FRAMEBUFFER,
				GL_DEPTH_ATTACHMENT, 0, 0 ) );
			
		}else{
			pWorking = false;
		}
		
		// clean up
		OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_2D, 0 ) );
		OGL_CHECK( renderThread, glDeleteTextures( 1, &texture ) );
		
	}catch( const deException & ){
		if( texture ){
			glDeleteTextures( 1, &texture );
		}
		throw;
	}
	
	if( pWorking ){
		renderThread.GetLogger().LogInfo( "Capabilities: Framebuffer texture single: Working" );
		
	}else{
		renderThread.GetLogger().LogWarn( "Capabilities: Framebuffer texture single: Driver Bug!" );
	}
}
