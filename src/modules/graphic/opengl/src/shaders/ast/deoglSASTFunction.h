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

#ifndef _DEOGLSASTFUNCTION_H_
#define _DEOGLSASTFUNCTION_H_

#include "deoglSASTDeclarationList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class deoglSASTNodeBlock;



/**
 * @brief Shader Abstract Syntax Tree Function.
 */
class deoglSASTFunction : public deObject{
private:
	decString pName;
	int pDataType;
	deoglSASTDeclarationList pParameterList;
	deoglSASTNodeBlock *pNodeBody;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shader abstract syntax tree function. */
	deoglSASTFunction( const char *name );
	/** Cleans up the shader abstract syntax tree function. */
	virtual ~deoglSASTFunction();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Retrieves the data type. */
	inline int GetDataType() const{ return pDataType; }
	/** Sets the data type. */
	void SetDataType( int dataType );
	/** Retrieves the parameter list. */
	inline deoglSASTDeclarationList &GetParameterList(){ return pParameterList; }
	inline const deoglSASTDeclarationList &GetParameterList() const{ return pParameterList; }
	/** Retrieves the body node. */
	inline deoglSASTNodeBlock *GetNodeBody() const{ return pNodeBody; }
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
