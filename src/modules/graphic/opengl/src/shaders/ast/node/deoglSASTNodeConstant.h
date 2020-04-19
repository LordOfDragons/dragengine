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

#ifndef _DEOGLSASTNODECONSTANT_H_
#define _DEOGLSASTNODECONSTANT_H_

#include "deoglSASTNode.h"



/**
 * @brief Shader Abstract Syntax Tree Node Constant.
 */
class deoglSASTNodeConstant : public deoglSASTNode{
public:
	/** Constant types. */
	enum eConstantTypes{
		/** Constant is a float. */
		ectFloat,
		/** Constant is an integer. */
		ectInteger,
		/** Constant is the keyword true. */
		ectTrue,
		/** Constant is the keaword false. */
		ectFalse,
		
		ECT_COUNT
	};
	
private:
	int pConstantType;
	float pValueFloat;
	int pValueInt;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shader abstract syntax tree node. */
	deoglSASTNodeConstant();
	deoglSASTNodeConstant( float value );
	deoglSASTNodeConstant( int value );
	deoglSASTNodeConstant( bool value );
	/** Cleans up the shader abstract syntax tree node. */
	virtual ~deoglSASTNodeConstant();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the constant type. */
	inline int GetConstantType() const{ return pConstantType; }
	/** Sets the constant type. */
	void SetConstantType( int constantType );
	/** Retrieves the float value. */
	inline float GetValueFloat() const{ return pValueFloat; }
	/** Sets the float value. */
	void SetValueFloat( float value );
	/** Retrieves the integer value. */
	inline int GetValueInt() const{ return pValueInt; }
	/** Sets the integer value. */
	void SetValueInt( int value );
	
	/** Generate source code. */
	virtual void GenerateSourceCode( deoglSASTGenerateCode &gencode );
	/*@}*/
};

#endif
