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

#ifndef _DEOGLSHADERPREPROCESSORSYMBOL_H_
#define _DEOGLSHADERPREPROCESSORSYMBOL_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/deObject.h>



/**
 * GLSL shader preprocessor symbol.
 */
class deoglShaderPreprocessorSymbol : public deObject{
private:
	decString pName;
	decString pValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shader preprocessor symbol. */
	deoglShaderPreprocessorSymbol();
	
	/** Create shader preprocessor symbol. */
	deoglShaderPreprocessorSymbol( const char *name, const char *value );
	
	/** Clean up shader preprocessor. */
	virtual ~deoglShaderPreprocessorSymbol();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Value. */
	inline const decString &GetValue() const{ return pValue; }
	/*@}*/
};

#endif
