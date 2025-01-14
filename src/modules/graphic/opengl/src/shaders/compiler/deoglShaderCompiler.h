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

#ifndef _DEOGLSHADERCOMPILER_H_
#define _DEOGLSHADERCOMPILER_H_

#include "../deoglShaderPreprocessor.h"
#include "../../deoglBasics.h"

#include <dragengine/threading/deMutex.h>

class deoglShaderLanguage;

class deoglShaderDefines;
class deoglShaderSources;
class deoglShaderCompiled;
class deoglShaderProgram;
class deoglRenderThread;

#ifdef OS_ANDROID
	class deoglShaderBindingList;
#endif



/**
 * Shader compiler.
 */
class deoglShaderCompiler{
private:
	deoglShaderLanguage &pLanguage;
	int pContextIndex;
	char *pErrorLog;
	deoglShaderPreprocessor pPreprocessor;
	
	deMutex pMutexCompile;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shader compiler. */
	deoglShaderCompiler(deoglShaderLanguage &language, int contextIndex);
	
	/** Clean up shader compiler. */
	~deoglShaderCompiler();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Compile shader from given sources using specified defines. */
	deoglShaderCompiled *CompileShader(const deoglShaderProgram &program);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	deoglShaderCompiled *pCompileShader(const deoglShaderProgram &program);
	void pAfterLinkShader(const deoglShaderProgram &program, deoglShaderCompiled &compiled);
	deoglShaderCompiled *pCacheLoadShader(const deoglShaderProgram &program);
	void pCacheSaveShader(const deoglShaderProgram &program, const deoglShaderCompiled &compiled);
	void pPreparePreprocessor(const deoglShaderDefines &defines);
	
	#ifdef OS_ANDROID
	void pAppendPreprocessSourcesBuffer( const char *inputFile, const char *data, const deoglShaderBindingList *outputList = NULL );
	#else
	void pAppendPreprocessSourcesBuffer( const char *inputFile, const char *data );
	#endif
	
	bool pCompileObject( GLuint handle );
	bool pLinkShader( GLuint handle );
	
	void pOutputShaderToFile( const char *file );
	void pLogFailedShaderSources();
	void pPrintErrorLog();
};

#endif
