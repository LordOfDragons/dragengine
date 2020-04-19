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

#ifndef _DEOGLSASTDECLARATIONLIST_H_
#define _DEOGLSASTDECLARATIONLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class deoglSASTDeclaration;



/**
 * @brief Shader Abstract Syntax Tree Declaration List.
 */
class deoglSASTDeclarationList{
private:
	decObjectOrderedSet pDeclarations;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shader abstraxt syntax tree declaration list. */
	deoglSASTDeclarationList();
	/** Cleans up the shader abstraxt syntax tree declaration list. */
	~deoglSASTDeclarationList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of declarations. */
	int GetCount() const;
	/** Retrieves the index of the declaration or -1 if not in the list. */
	int IndexOf( deoglSASTDeclaration *declaration ) const;
	/** Retrieves the index of the named declaration or -1 if not in the list. */
	int IndexOfNamed( const char *name ) const;
	/** Determines if the declaration exists. */
	bool Has( deoglSASTDeclaration *declaration ) const;
	/** Determines if the named declaration exists. */
	bool HasNamed( const char *name ) const;
	/** Retrieves the declaration at the given index. */
	deoglSASTDeclaration *GetAt( int index ) const;
	/** Retrieves the named declaration or NULL if not found. */
	deoglSASTDeclaration *GetNamed( const char *name ) const;
	/** Adds a declaration. */
	void Add( deoglSASTDeclaration *declaration );
	/** Removes a declaration. */
	void Remove( deoglSASTDeclaration *declaration );
	/** Removes all declarations. */
	void RemoveAll();
	/*@}*/
};

#endif
