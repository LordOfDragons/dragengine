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

#ifndef _DEOGLSGUGENERATEAST_H_
#define _DEOGLSGUGENERATEAST_H_

class deoglShaderGenerator;
class deoglSGeneratorUnit;
class deoglShaderAST;
class deoglSASTNode;
class deoglSGUNode;



/**
 * @brief Shader Generatur Unit Generate Abstract Syntax Tree.
 */
class deoglSGUGenerateAST{
private:
	deoglShaderGenerator *pGenerator;
	deoglSGeneratorUnit *pUnit;
	deoglShaderAST *pAST;
	deoglSASTNode *pParentASTNode;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shader generator unit generate abstract syntax tree. */
	deoglSGUGenerateAST( deoglShaderGenerator *generator, deoglSGeneratorUnit *unit, deoglShaderAST *ast );
	/** Cleans up the shader generator unit generate abstract syntax tree. */
	virtual ~deoglSGUGenerateAST();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the generator. */
	inline deoglShaderGenerator *GetGenerator() const{ return pGenerator; }
	/** Retrieves the generator unit. */
	inline deoglSGeneratorUnit *GetUnit() const{ return pUnit; }
	/** Retrieves the abstract syntax tree. */
	inline deoglShaderAST *GetAST() const{ return pAST; }
	/** Retrieves the parent abstract syntax tree node or NULL if there is none. */
	inline deoglSASTNode *GetParentASTNode() const{ return pParentASTNode; }
	/** Sets the parent abstract syntax tree node or NULL if there is none. */
	void SetParentASTNode( deoglSASTNode *node );
	/*@}*/
};

#endif
