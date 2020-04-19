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

#ifndef _DEOGLSGENERATORUNIT_H_
#define _DEOGLSGENERATORUNIT_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/deObject.h>

class deoglShaderGenerator;



/**
 * @brief Shader Generator Unit.
 */
class deoglSGeneratorUnit : public deObject{
private:
	decString pSourceCode;
	// nodes defining the content of the unit
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shader generator unit. */
	deoglSGeneratorUnit();
	/** Cleans up the shader generator unit. */
	virtual ~deoglSGeneratorUnit();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the source code. */
	inline const decString &GetSourceCode() const{ return pSourceCode; }
	
	/** Generate source code. */
	void GenerateSourceCode( const deoglShaderGenerator &generator );
	/** Delete source code. */
	void DeleteSourceCode();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
