/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifdef OS_WEBWASM

#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decPointerDictionary.h>

#include "../deoglGL.h"
#include "wasmfix.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"

#include <emscripten/html5.h>
#include <emscripten/threading.h>


void wasm_glBindFragDataLocation(GLuint program, GLuint color, const GLchar *name){}

void *wasmGetProcAddress(deoglRenderThread &renderThread, const char *name){
	static decPointerDictionary functions;
	
	if(functions.GetCount() == 0){
		functions.SetAt("glPolygonOffset", (void*)&glPolygonOffset);
		functions.SetAt("glTexImage3D", (void*)&glTexImage3D);
		functions.SetAt("glTexSubImage3D", (void*)&glTexSubImage3D);
		functions.SetAt("glBlendColor", (void*)&glBlendColor);
		functions.SetAt("glBlendEquation", (void*)&glBlendEquation);
		functions.SetAt("glActiveTexture", (void*)&glActiveTexture);
		functions.SetAt("glCompressedTexImage3D", (void*)&glCompressedTexImage3D);
		functions.SetAt("glCompressedTexImage2D", (void*)&glCompressedTexImage2D);
		functions.SetAt("glCompressedTexSubImage3D", (void*)&glCompressedTexSubImage3D);
		functions.SetAt("glCompressedTexSubImage2D", (void*)&glCompressedTexSubImage2D);
		functions.SetAt("glBindBuffer", (void*)&glBindBuffer);
		functions.SetAt("glDeleteBuffers", (void*)&glDeleteBuffers);
		functions.SetAt("glGenBuffers", (void*)&glGenBuffers);
		functions.SetAt("glBufferData", (void*)&glBufferData);
		functions.SetAt("glBufferSubData", (void*)&glBufferSubData);
		functions.SetAt("glMapBufferRange", (void*)&glMapBufferRange);
		functions.SetAt("glUnmapBuffer", (void*)&glUnmapBuffer);
		functions.SetAt("glGenQueries", (void*)&glGenQueries);
		functions.SetAt("glDeleteQueries", (void*)&glDeleteQueries);
		functions.SetAt("glBeginQuery", (void*)&glBeginQuery);
		functions.SetAt("glEndQuery", (void*)&glEndQuery);
		functions.SetAt("glGetQueryObjectuiv", (void*)&glGetQueryObjectuiv);
		functions.SetAt("glStencilOpSeparate", (void*)&glStencilOpSeparate);
		functions.SetAt("glVertexAttribPointer", (void*)&glVertexAttribPointer);
		functions.SetAt("glEnableVertexAttribArray", (void*)&glEnableVertexAttribArray);
		functions.SetAt("glBindAttribLocation", (void*)&glBindAttribLocation);
		functions.SetAt("glDrawBuffers", (void*)&glDrawBuffers);
		functions.SetAt("glDeleteShader", (void*)&glDeleteShader);
		functions.SetAt("glDeleteProgram", (void*)&glDeleteProgram);
		functions.SetAt("glCreateShader", (void*)&glCreateShader);
		functions.SetAt("glCreateProgram", (void*)&glCreateProgram);
		functions.SetAt("glAttachShader", (void*)&glAttachShader);
		functions.SetAt("glDetachShader", (void*)&glDetachShader);
		functions.SetAt("glUseProgram", (void*)&glUseProgram);
		functions.SetAt("glGetShaderiv", (void*)&glGetShaderiv);
		functions.SetAt("glGetProgramiv", (void*)&glGetProgramiv);
		functions.SetAt("glGetShaderInfoLog", (void*)&glGetShaderInfoLog);
		functions.SetAt("glGetProgramInfoLog", (void*)&glGetProgramInfoLog);
		functions.SetAt("glShaderSource", (void*)&glShaderSource);
		functions.SetAt("glCompileShader", (void*)&glCompileShader);
		functions.SetAt("glLinkProgram", (void*)&glLinkProgram);
		functions.SetAt("glUniform1f", (void*)&glUniform1f);
		functions.SetAt("glUniform2f", (void*)&glUniform2f);
		functions.SetAt("glUniform3f", (void*)&glUniform3f);
		functions.SetAt("glUniform4f", (void*)&glUniform4f);
		functions.SetAt("glUniform1i", (void*)&glUniform1i);
		functions.SetAt("glUniform2i", (void*)&glUniform2i);
		functions.SetAt("glUniform3i", (void*)&glUniform3i);
		functions.SetAt("glUniform4i", (void*)&glUniform4i);
		functions.SetAt("glUniformMatrix3fv", (void*)&glUniformMatrix3fv);
		functions.SetAt("glUniformMatrix4fv", (void*)&glUniformMatrix4fv);
		functions.SetAt("glGetUniformLocation", (void*)&glGetUniformLocation);
		functions.SetAt("glUniformMatrix3x2fv", (void*)&glUniformMatrix3x2fv);
		functions.SetAt("glUniformMatrix4x3fv", (void*)&glUniformMatrix4x3fv);
		functions.SetAt("glClearBufferiv", (void*)&glClearBufferiv);
		functions.SetAt("glClearBufferfv", (void*)&glClearBufferfv);
		functions.SetAt("glClearBufferfi", (void*)&glClearBufferfi);
		functions.SetAt("glGetIntegeri_v", (void*)&glGetIntegeri_v);
		functions.SetAt("glBindBufferRange", (void*)&glBindBufferRange);
		functions.SetAt("glBindBufferBase", (void*)&glBindBufferBase);
		functions.SetAt("glTransformFeedbackVaryings", (void*)&glTransformFeedbackVaryings);
		functions.SetAt("glBindFragDataLocation", (void*)&wasm_glBindFragDataLocation);
		functions.SetAt("glUniform1ui", (void*)&glUniform1ui);
		functions.SetAt("glUniform2ui", (void*)&glUniform2ui);
		functions.SetAt("glUniform3ui", (void*)&glUniform3ui);
		functions.SetAt("glUniform4ui", (void*)&glUniform4ui);
		functions.SetAt("glFramebufferTextureLayer", (void*)&glFramebufferTextureLayer);
		functions.SetAt("glDrawArraysInstanced", (void*)&glDrawArraysInstanced);
		functions.SetAt("glDrawElementsInstanced", (void*)&glDrawElementsInstanced);
		// functions.SetAt("glTexBuffer", (void*)&glTexBuffer); // not supported since this is ES 3.2 only
		functions.SetAt("glGetUniformBlockIndex", (void*)&glGetUniformBlockIndex);
		functions.SetAt("glUniformBlockBinding", (void*)&glUniformBlockBinding);
		functions.SetAt("glFenceSync", (void*)&glFenceSync);
		functions.SetAt("glDeleteSync", (void*)&glDeleteSync);
		functions.SetAt("glClientWaitSync", (void*)&glClientWaitSync);
		functions.SetAt("glGenSamplers", (void*)&glGenSamplers);
		functions.SetAt("glDeleteSamplers", (void*)&glDeleteSamplers);
		functions.SetAt("glBindSampler", (void*)&glBindSampler);
		functions.SetAt("glSamplerParameteri", (void*)&glSamplerParameteri);
		functions.SetAt("glBindFramebuffer", (void*)&glBindFramebuffer);
		functions.SetAt("glDeleteFramebuffers", (void*)&glDeleteFramebuffers);
		functions.SetAt("glGenFramebuffers", (void*)&glGenFramebuffers);
		functions.SetAt("glCheckFramebufferStatus", (void*)&glCheckFramebufferStatus);
		functions.SetAt("glFramebufferTexture2D", (void*)&glFramebufferTexture2D);
		functions.SetAt("glGenerateMipmap", (void*)&glGenerateMipmap);
		functions.SetAt("glBlitFramebuffer", (void*)&glBlitFramebuffer);
	}
	
	void *pointer = nullptr;
	return functions.GetAt(name, &pointer) ? pointer : nullptr;
}

#endif
