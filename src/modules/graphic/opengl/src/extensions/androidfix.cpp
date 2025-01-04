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

#include <dragengine/dragengine_configuration.h>

#ifdef OS_ANDROID

#include <stdlib.h>
#include <string.h>

#include <dragengine/common/exceptions.h>

#include "../deoglGL.h"
#include "androidfix.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"



/**
 * Wraps eglGetProcAddress returning replacement functions if not found.
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
		
	// }else if( strcmp( name, "glTexSubImage1D" ) == 0 ){
	// 	return (__eglMustCastToProperFunctionPointerType)&glTexSubImage1D;
		
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
 * glGetTexImage replacement.
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
 * glPolygonMode replacement.
 * 
 * Method does not exist in EGL. EGL supports only GL_FILL thus the entire method call is missing.
 * No direct solution except using GL_LINES rendering with modified buffers. Since this is currently
 * only used for debug rendering a replacement call is made doing nothing.
 */
void glPolygonMode( GLenum face, GLenum mode ){
}



/**
 * glGetBufferSubData replacement.
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
 * glTexImage1D replacement.
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
 * glTexSubImage1D replacement.
 * 
 * 1D textures are not supported under EGL. Replace with a 2D texture variant.
 * 
 * \note GL_TEXTURE_1D has the same value as _GL_TEXTURE_2D so target can be used as-is.
 */
/*
void glTexSubImage1D( GLenum target, GLint level, GLint xoffset, GLsizei width,
GLenum format, GLenum type, const void *pixels ){
	pglTexSubImage2D( target, level, xoffset, 0, width, 1, format, type, pixels );
}
*/


/**
 * glCompressedTexImage1D replacement.
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
 * glFramebufferTexture replacement.
 * 
 * Use glFramebufferTexture2D instead which fails if not used on a supported type.
 */
/*
void glFramebufferTexture( GLenum target, GLenum attachment, GLuint texture, GLint level ){
	glFramebufferTexture2D( target, attachment, GL_TEXTURE_2D, texture, level );
}
*/



/**
 * glBindFragDataLocation replacement.
 * 
 * EGL supports no custom named and placed output variables.
 * Replacement is a no-op. Shaders fix this.
 */
void glBindFragDataLocation( GLuint program, GLuint color, const GLchar *name ){
}



/**
 * glTexBuffer replacement.
 * 
 * Supported from 3.1 onwards but android has egl 3.0. for the time being the call
 * is silently ignored. problems have to be fixed once known.
 */
void glTexBuffer( GLenum target, GLenum internalformat, GLuint buffer ){
}



/**
 * glDrawElementsBaseVertex replacement.
 * 
 * Supported since 3.2 but android is egl 3.0. this call can not be easily simulated
 * since it requires modifying the VBO data. the shared vbo shifts the indices into
 * place. this works since vbo data blocks can not be moved (for the time being).
 */
void glDrawElementsBaseVertex( GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex ){
	glDrawElements( mode, count, type, indices );
}



/**
 * glDrawRangeElementsBaseVertex replacement.
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
 * glDrawElementsInstancedBaseVertex replacement.
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
 * glQueryCounter replacement.
 * 
 * Not used so simply blanked.
 */
void glQueryCounter( GLuint id, GLenum target ){
}



/**
 * glGetQueryObjectui64v replacement.
 * 
 * Not used so simply blanked.
 */
void glGetQueryObjectui64v( GLuint id, GLenum pname, GLuint64 *params ){
}



/**
 * glShaderStorageBlockBinding replacement.
 * 
 * Set to empty since under OpenGL ES the shader has to set the binding.
 */
void eglShaderStorageBlockBinding( GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding ){
}



static class cTempBuffer{
private:
	uint8_t *pData, *pDataEnd;
	int pSize, pMaxSize;
	
public:
	cTempBuffer() : pData(nullptr), pDataEnd(nullptr), pSize(0), pMaxSize(0){}
	
	~cTempBuffer(){
		if(pData){
			delete [] pData;
		}
	}
	
	inline int GetSize() const{ return pSize; }
	inline uint8_t *GetData() const{ return pData; }
	
	void SetSize(int size){
		DEASSERT_TRUE(size >= 1)
		if(pData && size <= pMaxSize){
			pSize = size;
			pDataEnd = pData + size;
			return;
		}
		
		if(pData){
			delete [] pData;
		}
		pData = new uint8_t[size];
		pMaxSize = pSize = size;
		pDataEnd = pData + size;
	}
	
	void Fill(const uint8_t *data, int size){
		DEASSERT_TRUE(pSize % size == 0)
		uint8_t *d = pData;
		while(d < pDataEnd){
			memcpy(d, data, size);
			d += size;
		}
	}
} vTempBuffer;

/**
 * glClearBufferSubData replacement.
 */
void eglClearBufferSubData(GLenum target, GLenum internalformat, GLintptr offset,
GLsizeiptr size, GLenum format, GLenum type, const void *data){
	vTempBuffer.SetSize((int)size);
	
	switch(format){
	case GL_RGBA:
		DEASSERT_TRUE(type == GL_FLOAT)
		vTempBuffer.Fill((uint8_t*)data, sizeof(GLfloat) * 4);
		break;
		
	case GL_RED:
		DEASSERT_TRUE(type == GL_FLOAT)
		vTempBuffer.Fill((uint8_t*)data, sizeof(GLfloat));
		break;
		
	case GL_RGBA_INTEGER:
		DEASSERT_TRUE(type == GL_UNSIGNED_INT)
		vTempBuffer.Fill((uint8_t*)data, sizeof(GLuint) * 4);
		break;
		
	case GL_RED_INTEGER:
		DEASSERT_TRUE(type == GL_UNSIGNED_INT)
		vTempBuffer.Fill((uint8_t*)data, sizeof(GLuint));
		break;
		
	default:
		DETHROW_INFO(deeInvalidParam, "unsupported format");
	}
	
	glBufferSubData(target, offset, size, vTempBuffer.GetData());
}

void eglMultiDrawElementsBaseVertex(GLenum mode, const GLsizei *count, GLenum type,
const GLvoid *const *indices, GLsizei drawcount, const GLint *basevertex){
	int i;
	for(i=0; i<drawcount; i++){
		if(count[i] > 0){
			pglDrawElementsBaseVertex(mode, count[i], type, indices[i], basevertex[i]);
		}
	}
}

#endif
