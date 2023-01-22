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

#ifndef _DEOSLSHADERBINDINGLIST_H_
#define _DEOSLSHADERBINDINGLIST_H_

#include <dragengine/common/string/decString.h>


/**
 * Shader Shader Binding List.
 */
class deoglShaderBindingList{
private:
	/** Bindings. */
	struct sBinding{
		decString name;
		int target;
	};
	
	
	
private:
	sBinding *pBindings;
	int pCount;
	int pSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create binding list. */
	deoglShaderBindingList();
	
	/** Clean up binding list. */
	~deoglShaderBindingList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Number of bindings. */
	inline int GetCount() const{ return pCount; }
	
	/** Named binding is present. */
	bool HasNamed( const char *name );
	
	/** Index of named binding or -1 if absent. */
	int IndexOfNamed( const char *name ) const;
	
	/** Binding name at index. */
	const decString &GetNameAt( int index ) const;
	
	/** Binding target at index. */
	int GetTargetAt( int index ) const;
	
	/** Add binding. */
	void Add( const char *name, int target );
	
	/** Remove all bindings. */
	void RemoveAll();
	/*@}*/
};

#endif
