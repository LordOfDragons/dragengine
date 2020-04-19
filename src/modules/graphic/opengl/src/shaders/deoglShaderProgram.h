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

class deoglShaderUnitSourceCode;
class deoglShaderSources;
class deoglShaderCompiled;
class deoglRenderTaskShader;



/**
 * @brief Shader Program.
 * A shader program. Shader programs are compiled from a base code loaded
 * from disk using defines to produce a specialized program.
 */
class deoglShaderProgram{
private:
	deoglShaderSources *pSources;
	deoglShaderDefines pDefines;
	
	deoglShaderUnitSourceCode *pSCTessellationControl;
	deoglShaderUnitSourceCode *pSCTessellationEvaluation;
	deoglShaderUnitSourceCode *pSCGeometry;
	deoglShaderUnitSourceCode *pSCVertex;
	deoglShaderUnitSourceCode *pSCFragment;
	
	deoglShaderCompiled *pCompiled;
	
	deoglRenderTaskShader *pRenderTaskShader;
	unsigned int pRenderTaskTrackingNumber;
	
	int pUsageCount;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shader program object. */
	deoglShaderProgram( deoglShaderSources *sources );
	/** Creates a new shader program object. */
	deoglShaderProgram( deoglShaderSources *sources, const deoglShaderDefines &defines );
	/** Cleans up the shader program object. */
	~deoglShaderProgram();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the sources. */
	inline deoglShaderSources *GetSources() const{ return pSources; }
	/** Retrieves the defines. */
	inline const deoglShaderDefines &GetDefines() const{ return pDefines; }
	
	/** \brief Retrieves the tessellation control source code or NULL if not used. */
	inline deoglShaderUnitSourceCode *GetTessellationControlSourceCode() const{ return pSCTessellationControl; }
	/** \brief Sets the tessellation control source code or NULL if not used. */
	void SetTessellationControlSourceCode( deoglShaderUnitSourceCode *sourceCode );
	/** \brief Retrieves the tessellation evaluation source code or NULL if not used. */
	inline deoglShaderUnitSourceCode *GetTessellationEvaluationSourceCode() const{ return pSCTessellationEvaluation; }
	/** \brief Sets the tessellation evaluation source code or NULL if not used. */
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
	
	/** Retrieves the render task shader pointer. */
	inline deoglRenderTaskShader *GetRenderTaskShader() const{ return pRenderTaskShader; }
	/** Sets the render task shader pointer. */
	void SetRenderTaskShader( deoglRenderTaskShader *renderTaskShader );
	/** Retrieves the render task tracking number. */
	inline unsigned int GetRenderTaskTrackingNumber() const{ return pRenderTaskTrackingNumber; }
	/** Sets the render task tracking number. */
	void SetRenderTaskTrackingNumber( unsigned int trackingNumber );
	
	/** Retrieves the usage count. */
	inline int GetUsageCount() const{ return pUsageCount; }
	/** Add usage increases the usage count by one. */
	void AddUsage();
	/** Removes a usage decreasing the usage count by one. */
	void RemoveUsage();
	/*@}*/
};

#endif
