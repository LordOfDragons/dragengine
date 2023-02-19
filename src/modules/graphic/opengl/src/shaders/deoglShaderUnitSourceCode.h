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

#ifndef _DEOGLSHADERUNITSOURCECODE_H_
#define _DEOGLSHADERUNITSOURCECODE_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/deObject.h>

class decBaseFileReader;


/**
 * Shader Unit Source Code.
 *
 * Stores the source code of a shader unit program. The shader unit
 * source code can be compiled into a specialized shader unit program
 * using a set of defines. Each shader unit source object is identified
 * by the file path relative to the shader unit source code directory.
 */
class deoglShaderUnitSourceCode : public deObject{
public:
	typedef deTObjectReference<deoglShaderUnitSourceCode> Ref;
	
	
	
private:
	decString pFilePath;
	decString pSourceCode;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new source code object reading source code from a file. */
	deoglShaderUnitSourceCode( const char *filePath, decBaseFileReader &fileReader );
	
protected:
	/** Cleans up the source code object. */
	virtual ~deoglShaderUnitSourceCode();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Retrieves the file path. */
	inline const decString &GetFilePath() const{ return pFilePath; }

	/** Retrieves the source code. */
	inline const decString &GetSourceCode() const{ return pSourceCode; }
	/*@}*/
};

#endif
