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

#ifndef _DEOGLSASTFUNCTIONLIST_H_
#define _DEOGLSASTFUNCTIONLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class deoglSASTFunction;



/**
 * @brief Shader Abstract Syntax Tree Function List.
 */
class deoglSASTFunctionList{
private:
	decObjectOrderedSet pFunctions;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shader abstract syntax tree function list. */
	deoglSASTFunctionList();
	/** Cleans up the shader abstract syntax tree function list. */
	~deoglSASTFunctionList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of functions. */
	int GetCount() const;
	/** Retrieves the index of the function or -1 if not in the list. */
	int IndexOf( deoglSASTFunction *function ) const;
	/** Retrieves the index of the named function or -1 if not in the list. */
	int IndexOfNamed( const char *name ) const;
	/** Determines if the function exists. */
	bool Has( deoglSASTFunction *function ) const;
	/** Determines if the named function exists. */
	bool HasNamed( const char *name ) const;
	/** Retrieves the function at the given index. */
	deoglSASTFunction *GetAt( int index ) const;
	/** Retrieves the named function or NULL if not found. */
	deoglSASTFunction *GetNamed( const char *name ) const;
	/** Adds a function. */
	void Add( deoglSASTFunction *function );
	/** Removes a function. */
	void Remove( deoglSASTFunction *function );
	/** Removes all functions. */
	void RemoveAll();
	/*@}*/
};

#endif
