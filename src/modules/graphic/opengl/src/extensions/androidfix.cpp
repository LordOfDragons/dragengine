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

#ifdef WITH_OPENGLES

#include <stdlib.h>
#include <string.h>

#include <dragengine/common/exceptions.h>

#include "../deoglGL.h"
#include "androidfix.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"


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
void glGetTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *img){
	DETHROW(deeInvalidAction);
	
	/*
	char *tempData = NULL;
	
	try{
		tempData = new char[
	(pPixelBuffer->GetFormat(), width, height, 1);
	
	OGL_CHECK(pRenderThread, glReadPixels(0, 0, width, height, pPixelBuffer->GetGLPixelFormat(),
		pPixelBuffer->GetGLPixelType(), tempData.GetPointer()));
	*/
}

/**
 * glPolygonMode replacement.
 * 
 * Method does not exist in EGL. EGL supports only GL_FILL thus the entire method call is missing.
 * No direct solution except using GL_LINES rendering with modified buffers. Since this is currently
 * only used for debug rendering a replacement call is made doing nothing.
 */
void glPolygonMode(GLenum face, GLenum mode){
}

/**
 * glGetBufferSubData replacement.
 * 
 * EGL does not have glGetBuffer*. As replacement glMapBufferRange has to be used.
 */
static void eglGetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, void *data){
	if(size == 0){
		return;
	}
	
	const void * const mapped = glMapBufferRange(target, offset, size, GL_MAP_READ_BIT);
	if(! mapped){
		DETHROW(deeInvalidAction);
	}
	
	memcpy(data, mapped, size);
	
	glUnmapBuffer(target);
}

/**
 * glBindFragDataLocation replacement.
 * 
 * EGL supports no custom named and placed output variables.
 * Replacement is a no-op. Shaders fix this.
 */
static void eglBindFragDataLocation(GLuint program, GLuint color, const GLchar *name){
}

/**
 * glTexBuffer replacement.
 * 
 * Supported from 3.1 onwards but android has egl 3.0. for the time being the call
 * is silently ignored. problems have to be fixed once known.
 */
static void eglTexBuffer(GLenum target, GLenum internalformat, GLuint buffer){
}

/**
 * glClearBufferSubData replacement.
 */
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
	
	inline int GetSize() const{return pSize;}
	inline uint8_t *GetData() const{return pData;}
	
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

static void eglClearBufferSubData(GLenum target, GLenum internalformat, GLintptr offset,
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

/**
 * glMultiDrawElementsBaseVertex replacement.
 */
static void eglMultiDrawElementsBaseVertex(GLenum mode, const GLsizei *count, GLenum type,
const GLvoid *const *indices, GLsizei drawcount, const GLint *basevertex){
	int i;
	for(i=0; i<drawcount; i++){
		if(count[i] > 0){
			pglDrawElementsBaseVertex(mode, count[i], type, indices[i], basevertex[i]);
		}
	}
}

/**
 * glMultiDrawArrays replacement.
 */
static void eglMultiDrawArrays(GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount){
	DEASSERT_NOTNULL(first)
	DEASSERT_NOTNULL(count)
	
	GLsizei i;
	for(i=0; i<drawcount; i++){
		glDrawArrays(mode, first[i], count[i]);
	}
}

/**
 * glMultiDrawElements replacement.
 */
void eglMultiDrawElements(GLenum mode, const GLsizei *count, GLenum type,
const void * const *indices, GLsizei drawcount){
	DEASSERT_NOTNULL(count)
	DEASSERT_NOTNULL(indices)
	
	GLsizei i;
	for(i=0; i<drawcount; i++){
		glDrawElements(mode, count[i], type, indices[i]);
	}
}


/**
 * Wraps eglGetProcAddress returning replacement functions if not found.
 */
__eglMustCastToProperFunctionPointerType androidGetProcAddress(
deoglRenderThread &renderThread, const char *name){
	// find native method
	__eglMustCastToProperFunctionPointerType address = eglGetProcAddress(name);
	if(address){
		// renderThread.GetLogger().LogInfoFormat("androidGetProcAddress(%s): native", name);
		return address;
	}
	
	// return replacement if existing otherwise return NULL
	if(strcmp(name, "glGetBufferSubData") == 0){
		// renderThread.GetLogger().LogInfoFormat("androidGetProcAddress(%s): replacement", name);
		return (__eglMustCastToProperFunctionPointerType)&eglGetBufferSubData;
		
	}else if(strcmp(name, "glBindFragDataLocation") == 0){
		// renderThread.GetLogger().LogInfoFormat("androidGetProcAddress(%s): replacement", name);
		return (__eglMustCastToProperFunctionPointerType)&eglBindFragDataLocation;
		
	}else if(strcmp(name, "glTexBuffer") == 0){
		// renderThread.GetLogger().LogInfoFormat("androidGetProcAddress(%s): replacement", name);
		return (__eglMustCastToProperFunctionPointerType)&eglTexBuffer;
		
	}else if(strcmp(name, "glMultiDrawArrays") == 0){
		// renderThread.GetLogger().LogInfoFormat("androidGetProcAddress(%s): replacement", name);
		return (__eglMustCastToProperFunctionPointerType)&eglMultiDrawArrays;
		
	}else if(strcmp(name, "glMultiDrawElements") == 0){
		// renderThread.GetLogger().LogInfoFormat("androidGetProcAddress(%s): replacement", name);
		return (__eglMustCastToProperFunctionPointerType)&eglMultiDrawElements;
		
	}else if(strcmp(name, "glClearBufferSubData") == 0){
		// renderThread.GetLogger().LogInfoFormat("androidGetProcAddress(%s): replacement", name);
		return (__eglMustCastToProperFunctionPointerType)&eglClearBufferSubData;
		
	}else if(strcmp(name, "glMultiDrawElementsBaseVertex") == 0){
		// renderThread.GetLogger().LogInfoFormat("androidGetProcAddress(%s): replacement", name);
		return (__eglMustCastToProperFunctionPointerType)&eglMultiDrawElementsBaseVertex;
		
	}else{
		// renderThread.GetLogger().LogInfoFormat("androidGetProcAddress(%s): absent", name);
		return nullptr;
	}
}

#endif
