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
