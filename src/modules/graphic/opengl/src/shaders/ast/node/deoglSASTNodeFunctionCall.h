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

#ifndef _DEOGLSASTNODEFUNCTIONCALL_H_
#define _DEOGLSASTNODEFUNCTIONCALL_H_

#include "deoglSASTNode.h"
#include "deoglSASTNodeList.h"

#include <dragengine/common/string/decString.h>



/**
 * @brief Shader Abstract Syntax Tree Node Function Call.
 */
class deoglSASTNodeFunctionCall : public deoglSASTNode{
private:
	decString pName;
	short pSwizzle[ 4 ];
	deoglSASTNodeList pNodeListParams;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shader abstract syntax tree node. */
	deoglSASTNodeFunctionCall();
	deoglSASTNodeFunctionCall( const char *name );
	deoglSASTNodeFunctionCall( const char *name, deoglSASTNode *nodeParam1 );
	deoglSASTNodeFunctionCall( const char *name, deoglSASTNode *nodeParam1, deoglSASTNode *nodeParam2 );
	deoglSASTNodeFunctionCall( const char *name, deoglSASTNode *nodeParam1, deoglSASTNode *nodeParam2,
		deoglSASTNode *nodeParam3 );
	deoglSASTNodeFunctionCall( const char *name, deoglSASTNode *nodeParam1, deoglSASTNode *nodeParam2,
		deoglSASTNode *nodeParam3, deoglSASTNode *nodeParam4 );
	/** Cleans up the shader abstract syntax tree node. */
	virtual ~deoglSASTNodeFunctionCall();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the function name. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the function name. */
	void SetName( const char *name );
	/** Retrieves the parameter node list. */
	inline deoglSASTNodeList &GetNodeParamsList(){ return pNodeListParams; }
	inline const deoglSASTNodeList &GetNodeParamsList() const{ return pNodeListParams; }
	/** Retrieves the swizzle for a position. */
	int GetSwizzleAt( int position ) const;
	/** Sets the swizzle for a position. */
	void SetSwizzleAt( int position, int swizzle );
	/** Sets all swizzles. */
	void SetSwizzles( int swizzle1 );
	void SetSwizzles( int swizzle1, int swizzle2 );
	void SetSwizzles( int swizzle1, int swizzle2, int swizzle3 );
	void SetSwizzles( int swizzle1, int swizzle2, int swizzle3, int swizzle4 );
	
	/** Generate source code. */
	virtual void GenerateSourceCode( deoglSASTGenerateCode &gencode );
	/*@}*/
};

#endif
