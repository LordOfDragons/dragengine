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

#ifndef _DEOGLSGUNODE_H_
#define _DEOGLSGUNODE_H_

#include <dragengine/deObject.h>

class deoglSGUGenerateAST;



/**
 * @brief Shader Generator Unit Tree Node.
 */
class deoglSGUNode : public deObject{
public:
	/** Node types. */
	enum eNodeTypes{
		entFragSkinTexture,
		ENT_COUNT
	};
	
private:
	int pNodeType;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shader generator unit tree node. */
	deoglSGUNode( int nodeType );
	/** Cleans up the shader generator unit tree node. */
	virtual ~deoglSGUNode();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the node type. */
	inline int GetNodeType() const{ return pNodeType; }
	
	/** Generate shader abstract syntax tree. */
	virtual void GenerateAST( deoglSGUGenerateAST &genast ) = 0;
	/*@}*/
};

#endif
