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

#ifndef _DEOGLSASTNODEDECLAREVARIABLE_H_
#define _DEOGLSASTNODEDECLAREVARIABLE_H_

#include "deoglSASTNode.h"
#include "deoglSASTNodeList.h"

#include <dragengine/common/string/decString.h>



/**
 * @brief Shader Abstract Syntax Tree Node Declare Variable.
 */
class deoglSASTNodeDeclareVariable : public deoglSASTNode{
private:
	decString pName;
	int pDataType;
	deoglSASTNode *pNodeInit;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shader abstract syntax tree node. */
	deoglSASTNodeDeclareVariable();
	deoglSASTNodeDeclareVariable( const char *name );
	deoglSASTNodeDeclareVariable( const char *name, int dataType );
	deoglSASTNodeDeclareVariable( const char *name, int dataType, deoglSASTNode *nodeInit );
	/** Cleans up the shader abstract syntax tree node. */
	virtual ~deoglSASTNodeDeclareVariable();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the name. */
	void SetName( const char *name );
	/** Retrieves the data type. */
	inline int GetDataType() const{ return pDataType; }
	/** Sets the data type. */
	void SetDataType( int dataType );
	/** Retrieves the initialization node or NULL if not used. */
	inline deoglSASTNode *GetNodeInit() const{ return pNodeInit; }
	/** Sets the initialization node or NULL if not used. */
	void SetNodeInit( deoglSASTNode *node );
	
	/** Generate source code. */
	virtual void GenerateSourceCode( deoglSASTGenerateCode &gencode );
	/*@}*/
};

#endif
