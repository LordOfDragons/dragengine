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

#ifndef _DEOGLSHADERGENERATOR_H_
#define _DEOGLSHADERGENERATOR_H_

#include <dragengine/common/string/decString.h>

class deoglSGeneratorUnit;
class deoglShaderSources;
class deoglShaderProgram;



/**
 * @brief Shader Generator.
 */
class deoglShaderGenerator{
private:
	decString pName;
	
	deoglSGeneratorUnit *pUnitVertex;
	deoglSGeneratorUnit *pUnitGeometry;
	deoglSGeneratorUnit *pUnitFragment;
	
	deoglShaderSources *pSources;
	deoglShaderProgram *pShader;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shader generator. */
	deoglShaderGenerator( const char *name );
	/** Cleans up the shader generator. */
	~deoglShaderGenerator();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Retrieves the vertex unit or NULL if not used. */
	inline deoglSGeneratorUnit *GetUnitVertex() const{ return pUnitVertex; }
	/** Sets the vertex unit or NULL if not used. */
	void SetUnitVertex( deoglSGeneratorUnit *unit );
	/** Retrieves the geometry unit or NULL if not used. */
	inline deoglSGeneratorUnit *GetUnitGeometry() const{ return pUnitGeometry; }
	/** Sets the geometry unit or NULL if not used. */
	void SetUnitGeometry( deoglSGeneratorUnit *unit );
	/** Retrieves the fragment unit or NULL if not used. */
	inline deoglSGeneratorUnit *GetUnitFragment() const{ return pUnitFragment; }
	/** Sets the fragment unit or NULL if not used. */
	void SetUnitFragment( deoglSGeneratorUnit *unit );
	
	/** Generate shader. */
	void GenerateShader();
	/** Free shader. */
	void FreeShader();
	/** Retrieves the shader or NULL if not generated yet. */
	inline deoglShaderProgram *GetShader() const{ return pShader; }
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
