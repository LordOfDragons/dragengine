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



/**
 * \brief Shader Shader Binding List.
 */
class deoglShaderBindingList{
private:
	/** \brief Bindings. */
	struct sBinding{
		char *name;
		int target;
	};
	
	
	
private:
	sBinding *pBindings;
	int pCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create binding list. */
	deoglShaderBindingList();
	
	/** \brief Clean up binding list. */
	~deoglShaderBindingList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of bindings. */
	inline int GetCount() const{ return pCount; }
	
	/** \brief Named binding is present. */
	bool HasNamed( const char *name );
	
	/** \brief Index of named binding or -1 if absent. */
	int IndexOfNamed( const char *name ) const;
	
	/** \brief Binding name at index. */
	const char *GetNameAt( int index ) const;
	
	/** \brief Binding target at index. */
	int GetTargetAt( int index ) const;
	
	/** \brief Add binding. */
	void Add( const char *name, int target );
	
	/** \brief Remove all bindings. */
	void RemoveAll();
	/*@}*/
};

#endif
