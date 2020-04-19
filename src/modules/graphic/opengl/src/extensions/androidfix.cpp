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

#ifdef ANDROID

#include <stdlib.h>
#include <string.h>

#include <dragengine/common/exceptions.h>

#include "deoglExtResult.h"



/**
 * \brief Wraps eglGetProcAddress returning replacement functions if not found.
 */
__eglMustCastToProperFunctionPointerType androidGetProcAddress( const char *name ){
	// find native method
	__eglMustCastToProperFunctionPointerType address = eglGetProcAddress( name );
	if( address ){
		return address;
	}
	
	// return replacement if existing otherwise return NULL
	if( strcmp( name, "glGetBufferSubData" ) == 0 ){
		return (__eglMustCastToProperFunctionPointerType)&glGetBufferSubData;
		
	}else if( strcmp( name, "glTexImage1D" ) == 0 ){
		return (__eglMustCastToProperFunctionPointerType)&glTexImage1D;
		
	}else if( strcmp( name, "glTexSubImage1D" ) == 0 ){
		return (__eglMustCastToProperFunctionPointerType)&glTexSubImage1D;
		
	}else if( strcmp( name, "glCompressedTexImage1D" ) == 0 ){
		return (__eglMustCastToProperFunctionPointerType)&glCompressedTexImage1D;
		
// 	}else if( strcmp( name, "glFramebufferTexture" ) == 0 ){
// 		return (__eglMustCastToProperFunctionPointerType)&glFramebufferTexture;
		
	}else if( strcmp( name, "glBindFragDataLocation" ) == 0 ){
		return (__eglMustCastToProperFunctionPointerType)&glBindFragDataLocation;
		
	}else if( strcmp( name, "glTexBuffer" ) == 0 ){
		return (__eglMustCastToProperFunctionPointerType)&glTexBuffer;
		
	}else if( strcmp( name, "glDrawElementsBaseVertex" ) == 0 ){
		return (__eglMustCastToProperFunctionPointerType)&glDrawElementsBaseVertex;
		
	}else if( strcmp( name, "glDrawRangeElementsBaseVertex" ) == 0 ){
		return (__eglMustCastToProperFunctionPointerType)&glDrawRangeElementsBaseVertex;
		
	}else if( strcmp( name, "glDrawElementsInstancedBaseVertex" ) == 0 ){
		return (__eglMustCastToProperFunctionPointerType)&glDrawElementsInstancedBaseVertex;
		
	}else if( strcmp( name, "glQueryCounter" ) == 0 ){
		return (__eglMustCastToProperFunctionPointerType)&glQueryCounter;
		
	}else if( strcmp( name, "glGetQueryObjectui64v" ) == 0 ){
		return (__eglMustCastToProperFunctionPointerType)&glGetQueryObjectui64v;
		
	}else{
		return NULL;
	}
}



/**
 * \brief glGetTexImage replacement.
 * 
 * Method is not existing in EGL. Potentially simulate it using glBlitFramebuffer.
 * 
 * void glBlitFramebuffer(GLint srcX0,  GLint srcY0,  GLint srcX1,  GLint srcY1,
 *    GLint dstX0,  GLint dstY0,  GLint dstX1,  GLint dstY1,  GLbitfield mask,  GLenum filter);
 * 
 * Until then an exception is thrown to keep things compiling for the moment.
 */
void glGetTexImage( GLenum target, GLint level, GLenum format, GLenum type, GLvoid *img ){
	DETHROW( deeInvalidAction );
	
	/*
	char *tempData = NULL;
	
	try{
		tempData = new char[ 
	( pPixelBuffer->GetFormat(), width, height, 1 );
	
	OGL_CHECK( pRenderThread, glReadPixels( 0, 0, width, height, pPixelBuffer->GetGLPixelFormat(),
		pPixelBuffer->GetGLPixelType(), tempData.GetPointer() ) );
	*/
}



/**
 * \brief glPolygonMode replacement.
 * 
 * Method does not exist in EGL. EGL supports only GL_FILL thus the entire method call is missing.
 * No direct solution except using GL_LINES rendering with modified buffers. Since this is currently
 * only used for debug rendering a replacement call is made doing nothing.
 */
void glPolygonMode( GLenum face, GLenum mode ){
}



/**
 * \brief glGetBufferSubData replacement.
 * 
 * EGL does not have glGetBuffer*. As replacement glMapBufferRange has to be used.
 */
void glGetBufferSubData( GLenum target, GLintptr offset, GLsizeiptr size, void *data ){
	if( size == 0 ){
		return;
	}
	
	const void * const mapped = glMapBufferRange( target, offset, size, GL_MAP_READ_BIT );
	if( ! mapped ){
		DETHROW( deeInvalidAction );
	}
	
	memcpy( data, mapped, size );
	
	glUnmapBuffer( target );
}



/**
 * \brief glTexImage1D replacement.
 * 
 * 1D textures are not supported under EGL. Replace with a 2D texture variant.
 * 
 * \note GL_TEXTURE_1D has the same value as _GL_TEXTURE_2D so target can be used as-is.
 */
void glTexImage1D( GLenum target, GLint level, GLint internalFormat, GLsizei width,
GLint border, GLenum format, GLenum type, const GLvoid *data ){
	glTexImage2D( target, level, internalFormat, width, 1, border, format, type, data );
}



/**
 * \brief glTexSubImage1D replacement.
 * 
 * 1D textures are not supported under EGL. Replace with a 2D texture variant.
 * 
 * \note GL_TEXTURE_1D has the same value as _GL_TEXTURE_2D so target can be used as-is.
 */
void glTexSubImage1D( GLenum target, GLint level, GLint xoffset, GLsizei width,
GLenum format, GLenum type, const void *pixels ){
	pglTexSubImage2D( target, level, xoffset, 0, width, 1, format, type, pixels );
}



/**
 * \brief glCompressedTexImage1D replacement.
 * 
 * 1D textures are not supported under EGL. Replace with a 2D texture variant.
 * 
 * \note GL_TEXTURE_1D has the same value as _GL_TEXTURE_2D so target can be used as-is.
 */
void glCompressedTexImage1D( GLenum target, GLint level, GLenum internalFormat,
GLsizei width, GLint border, GLsizei imageSize, const void *data ){
	pglCompressedTexImage2D( target, level, internalFormat, width, 1, border, imageSize, data );
}



/**
 * \brief glFramebufferTexture replacement.
 * 
 * Use glFramebufferTexture2D instead which fails if not used on a supported type.
 */
/*
void glFramebufferTexture( GLenum target, GLenum attachment, GLuint texture, GLint level ){
	glFramebufferTexture2D( target, attachment, GL_TEXTURE_2D, texture, level );
}
*/



/**
 * \brief glBindFragDataLocation replacement.
 * 
 * EGL supports no custom named and placed output variables.
 * Replacement is a no-op. Shaders fix this.
 */
void glBindFragDataLocation( GLuint program, GLuint color, const GLchar *name ){
}



/**
 * \brief glTexBuffer replacement.
 * 
 * Supported from 3.1 onwards but android has egl 3.0. for the time being the call
 * is silently ignored. problems have to be fixed once known.
 */
void glTexBuffer( GLenum target, GLenum internalformat, GLuint buffer ){
}



/**
 * \brief glDrawElementsBaseVertex replacement.
 * 
 * Supported since 3.2 but android is egl 3.0. this call can not be easily simulated
 * since it requires modifying the VBO data. the shared vbo shifts the indices into
 * place. this works since vbo data blocks can not be moved (for the time being).
 */
void glDrawElementsBaseVertex( GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex ){
	glDrawElements( mode, count, type, indices );
}



/**
 * \brief glDrawRangeElementsBaseVertex replacement.
 * 
 * Supported since 3.2 but android is egl 3.0. this call can not be easily simulated
 * since it requires modifying the VBO data. the shared vbo shifts the indices into
 * place. this works since vbo data blocks can not be moved (for the time being).
 */
void glDrawRangeElementsBaseVertex( GLenum mode, GLuint start, GLuint end, GLsizei count,
GLenum type, const void *indices, GLint basevertex ){
	glDrawRangeElements( mode, start, end, count, type, indices );
}



/**
 * \brief glDrawElementsInstancedBaseVertex replacement.
 * 
 * Supported since 3.2 but android is egl 3.0. this call can not be easily simulated
 * since it requires modifying the VBO data. the shared vbo shifts the indices into
 * place. this works since vbo data blocks can not be moved (for the time being).
 */
void glDrawElementsInstancedBaseVertex( GLenum mode, GLsizei count, GLenum type,
const void *indices, GLsizei instancecount, GLint basevertex ){
	glDrawElementsInstanced( mode, count, type, indices, instancecount );
}



/**
 * \brief glQueryCounter replacement.
 * 
 * Not used so simply blanked.
 */
void glQueryCounter( GLuint id, GLenum target ){
}



/**
 * \brief glGetQueryObjectui64v replacement.
 * 
 * Not used so simply blanked.
 */
void glGetQueryObjectui64v( GLuint id, GLenum pname, GLuint64 *params ){
}



/**
 * \brief glShaderStorageBlockBinding replacement.
 * 
 * Set to empty since under OpenGL ES the shader has to set the binding.
 */
void eglShaderStorageBlockBinding( GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding ){
}

#endif
