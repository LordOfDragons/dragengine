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
