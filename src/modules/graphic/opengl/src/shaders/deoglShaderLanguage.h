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

#ifndef _DEOGLSHADERLANGUAGE_H_
#define _DEOGLSHADERLANGUAGE_H_

#include "deoglShaderPreprocessor.h"
#include "../deoglBasics.h"

#include <dragengine/common/string/decStringList.h>
#include <dragengine/threading/deMutex.h>

class deoglShaderDefines;
class deoglShaderSources;
class deoglShaderCompiled;
class deoglShaderProgram;
class deoglRenderThread;

#ifdef ANDROID
class deoglShaderBindingList;
#endif



/**
 * Shader Language.
 *
 * A shader language object is responsible for storing all information required
 * for compiling shaders using a specific language as well as handling activation
 * of shaders.
 */
class deoglShaderLanguage{
private:
	deoglRenderThread &pRenderThread;
	char *pErrorLog;
	
	int pShaderFileNumber;
	
	decString pGLSLVersion;
	decStringList pGLSLExtensions;
	
	int pGLSLVersionNumber;
	
	bool pHasLoadingShader;
	bool pGuardHasLoadingShader;
	bool pHasCompilingShader;
	bool pGuardHasCompilingShader;
	
	deoglShaderPreprocessor pPreprocessor;
	deMutex pMutexCompile;
	deMutex pMutexChecks;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new shader language object. */
	deoglShaderLanguage( deoglRenderThread &renderThread );
	/** Cleans up the shader language object. */
	~deoglShaderLanguage();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Compieles a shader from the given sources using the specified defines. */
	deoglShaderCompiled *CompileShader( deoglShaderProgram &program );
	
	/**
	 * Check if shader is loading or has been loaded since the last call.
	 * Resets flag if no loading is in progress right now.
	 */
	bool GetHasLoadingShader();
	
	/**
	 * Check if shader is compiling or has been compiled since the last call.
	 * Resets flag if no compiling is in progress right now.
	 */
	bool GetHasCompilingShader();
	/*@}*/
	
private:
	deoglShaderCompiled *pCompileShader( deoglShaderProgram &program );
	void pAfterLinkShader( const deoglShaderProgram &program, deoglShaderCompiled &compiled );
	deoglShaderCompiled *pCacheLoadShader( deoglShaderProgram &program );
	void pCacheSaveShader( const deoglShaderProgram &program, const deoglShaderCompiled &compiled );
	void pPreparePreprocessor( const deoglShaderDefines &defines );
	
	#ifdef ANDROID
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
