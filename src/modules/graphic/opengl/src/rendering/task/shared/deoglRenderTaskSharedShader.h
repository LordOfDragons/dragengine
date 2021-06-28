/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLRENDERTASKSHAREDSHADER_H_
#define _DEOGLRENDERTASKSHAREDSHADER_H_

class deoglShaderProgram;
class deoglRenderTaskSharedPool;


/**
 * Shared render task shader.
 */
class deoglRenderTaskSharedShader{
private:
	deoglRenderTaskSharedPool &pPool;
	const int pIndex;
	
	deoglShaderProgram *pShader;
	int pSPBInstanceIndexBase;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shared render task shader. */
	deoglRenderTaskSharedShader( deoglRenderTaskSharedPool &pool, int index );
	
	/** Clean up shared render task shader. */
	~deoglRenderTaskSharedShader();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Pool. */
	inline deoglRenderTaskSharedPool &GetPool() const{ return pPool; }
	
	/** Index. */
	inline int GetIndex() const{ return pIndex; }
	
	
	
	/** Shader. */
	inline deoglShaderProgram *GetShader() const{ return pShader; }
	
	/** Set shader. */
	void SetShader( deoglShaderProgram *shader );
	
	/** Shader parameter index of pSPBInstanceIndexBase or -1. */
	inline int GetSPBInstanceIndexBase() const{ return pSPBInstanceIndexBase; }
	
	/** Set shader parameter index of pSPBInstanceIndexBase or -1. */
	void SetSPBInstanceIndexBase( int parameter );
	
	
	
	/** Clear. */
	void Clear();
	
	/** Return to pool. */
	void ReturnToPool();
	/*@}*/
};

#endif
