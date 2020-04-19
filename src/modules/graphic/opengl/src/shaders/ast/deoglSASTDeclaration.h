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

#ifndef _DEOGLSASTDECLARATION_H_
#define _DEOGLSASTDECLARATION_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class deoglSASTNode;



/**
 * @brief Shader Abstract Syntax Tree Declaration.
 */
class deoglSASTDeclaration : public deObject{
public:
	/** Direction qualifier used for function parameter declarations. */
	enum eDirectionQualifiers{
		edqNone,
		edqIn,
		edqOut,
		edqInOut,
		EDQ_COUNT
	};
	
private:
	decString pName;
	int pDataType;
	deoglSASTNode *pNodeInit;
	int pDirectionQualifier;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shader abstract syntax tree declaration. */
	deoglSASTDeclaration( const char *name );
	deoglSASTDeclaration( const char *name, int dataType );
	deoglSASTDeclaration( const char *name, int dataType, deoglSASTNode *nodeInit );
	deoglSASTDeclaration( const char *name, int dataType, int directionQualifitier );
	/** Cleans up the shader abstract syntax tree declaration. */
	virtual ~deoglSASTDeclaration();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Retrieves the data type. */
	inline int GetDataType() const{ return pDataType; }
	/** Sets the data type. */
	void SetDataType( int dataType );
	/** Retrieves the initialization node or NULL if not used. */
	inline deoglSASTNode *GetNodeInit() const{ return pNodeInit; }
	/** Sets the initialization node or NULL if not used. */
	void SetNodeInit( deoglSASTNode *node );
	/** Retrieves the direction qualifier. */
	inline int GetDirectionQualifier() const{ return pDirectionQualifier; }
	/** Sets the direction qualifier. */
	void SetDirectionQualifier( int qualifier );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
