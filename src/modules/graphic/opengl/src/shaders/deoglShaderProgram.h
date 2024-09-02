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

#ifndef _DEOGLSHADERPROGRAM_H_
#define _DEOGLSHADERPROGRAM_H_

#include "deoglShaderDefines.h"

#include <dragengine/deObject.h>

class deoglRenderThread;
class deoglShaderUnitSourceCode;
class deoglShaderSources;
class deoglShaderCompiled;



/**
 * A shader program. Shader programs are compiled from a base code loaded
 * from disk using defines to produce a specialized program.
 */
class deoglShaderProgram : public deObject{
public:
	typedef deTObjectReference<deoglShaderProgram> Ref;
	
	
	
private:
	deoglRenderThread &pRenderThread;
	const deoglShaderSources *pSources;
	deoglShaderDefines pDefines;
	decString pCacheId;
	
	deoglShaderUnitSourceCode *pSCCompute;
	deoglShaderUnitSourceCode *pSCTessellationControl;
	deoglShaderUnitSourceCode *pSCTessellationEvaluation;
	deoglShaderUnitSourceCode *pSCGeometry;
	deoglShaderUnitSourceCode *pSCVertex;
	deoglShaderUnitSourceCode *pSCFragment;
	
	deoglShaderCompiled *pCompiled;
	
	unsigned int pUniqueKey;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shader program. */
	deoglShaderProgram( deoglRenderThread &renderThread, const deoglShaderSources *sources );
	
	/** Create shader program. */
	deoglShaderProgram( deoglRenderThread &renderThread,
		const deoglShaderSources *sources, const deoglShaderDefines &defines );
	
protected:
	/** Clean up shader program. */
	virtual ~deoglShaderProgram();
	/*@}*/
	
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Retrieves the sources. */
	inline const deoglShaderSources *GetSources() const{ return pSources; }
	
	/** Retrieves the defines. */
	inline const deoglShaderDefines &GetDefines() const{ return pDefines; }
	
	/** Cache identifier. */
	inline const decString &GetCacheId() const{ return pCacheId; }
	
	/** Set cache identifier. */
	void SetCacheId( const decString &id );
	
	/** Compute source code or NULL. */
	inline deoglShaderUnitSourceCode *GetComputeSourceCode() const{ return pSCCompute; }
	
	/** Set compute source code or NULL. */
	void SetComputeSourceCode( deoglShaderUnitSourceCode *sourceCode );
	
	/** Retrieves the tessellation control source code or NULL if not used. */
	inline deoglShaderUnitSourceCode *GetTessellationControlSourceCode() const{ return pSCTessellationControl; }
	
	/** Sets the tessellation control source code or NULL if not used. */
	void SetTessellationControlSourceCode( deoglShaderUnitSourceCode *sourceCode );
	
	/** Retrieves the tessellation evaluation source code or NULL if not used. */
	inline deoglShaderUnitSourceCode *GetTessellationEvaluationSourceCode() const{ return pSCTessellationEvaluation; }
	
	/** Sets the tessellation evaluation source code or NULL if not used. */
	void SetTessellationEvaluationSourceCode( deoglShaderUnitSourceCode *sourceCode );
	
	/** Retrieves the geometry source code or NULL if not used. */
	inline deoglShaderUnitSourceCode *GetGeometrySourceCode() const{ return pSCGeometry; }
	
	/** Sets the geometry source code or NULL if not used. */
	void SetGeometrySourceCode( deoglShaderUnitSourceCode *sourceCode );
	
	/** Retrieves the vertex source code or NULL if not used. */
	inline deoglShaderUnitSourceCode *GetVertexSourceCode() const{ return pSCVertex; }
	
	/** Sets the vertex source code or NULL if not used. */
	void SetVertexSourceCode( deoglShaderUnitSourceCode *sourceCode );
	
	/** Retrieves the fragment source code or NULL if not used. */
	inline deoglShaderUnitSourceCode *GetFragmentSourceCode() const{ return pSCFragment; }
	
	/** Sets the fragment source code or NULL if not used. */
	void SetFragmentSourceCode( deoglShaderUnitSourceCode *sourceCode );
	
	/** Retrieves the compiled shader. */
	inline deoglShaderCompiled *GetCompiled() const{ return pCompiled; }
	
	/** Sets the compiled shader. */
	void SetCompiled( deoglShaderCompiled *compiled );
	
	/** Unique key for use with dictionaries. */
	inline unsigned int GetUniqueKey() const{ return pUniqueKey; }
	/*@}*/
};

#endif
