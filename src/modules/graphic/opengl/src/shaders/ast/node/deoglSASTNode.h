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

#ifndef _DEOGLSASTNODE_H_
#define _DEOGLSASTNODE_H_

#include <dragengine/deObject.h>

class deoglSASTGenerateCode;



/**
 * @brief Shader Abstract Syntax Tree Node.
 */
class deoglSASTNode : public deObject{
public:
	/** Node types. */
	enum eNodeTypes{
		entOperator,
		entVariable,
		entFunctionCall,
		entIfElse,
		entReturn,
		entConstant,
		entDiscard,
		entBlock,
		entDeclareCariable,
		ENT_COUNT
	};
	
private:
	int pNodeType;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shader abstract syntax tree node. */
	deoglSASTNode( int nodeType );
	/** Cleans up the shader abstract syntax tree node. */
	virtual ~deoglSASTNode();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the node type. */
	inline int GetNodeType() const{ return pNodeType; }
	
	/** Generate source code. */
	virtual void GenerateSourceCode( deoglSASTGenerateCode &gencode ) = 0;
	/*@}*/
};

#endif
