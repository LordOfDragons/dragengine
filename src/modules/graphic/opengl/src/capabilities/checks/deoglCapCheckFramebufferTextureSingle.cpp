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
	
	const deoglCapsTextureFormat &texformat = *pCapabilities.GetFormats()
		.GetUseFBOArrayTexFormatFor( deoglCapsFmtSupport::eutfDepth );
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
