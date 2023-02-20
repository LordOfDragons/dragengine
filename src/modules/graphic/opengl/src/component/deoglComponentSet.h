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

#ifndef _DEOGLCOMPONENTSET_H_
#define _DEOGLCOMPONENTSET_H_

class deoglRComponent;



/**
 * Component set.
 * 
 * Set of pointers to components. The ordering of the components in the list is
 * not defined and can change when components are removed. This allows to
 * remove elements with constant time requirements.
 */
class deoglComponentSet{
private:
	deoglRComponent **pComponents;
	int pComponentCount;
	int pComponentSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create component list. */
	deoglComponentSet();
	
	/** Clean up component list. */
	~deoglComponentSet();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Number of components. */
	inline int GetCount() const{ return pComponentCount; }
	
	/** Index of the component or -1 if not in the list. */
	int IndexOfComponent( deoglRComponent *component ) const;
	
	/** Component exists. */
	bool Has( deoglRComponent *component ) const;
	
	/** Component at the given index. */
	deoglRComponent *GetAt( int index ) const;
	
	/** Add component. */
	void Add( deoglRComponent *component );
	
	/** Add component if missing in the list. Returns true if added. */
	bool AddIfMissing( deoglRComponent *component );
	
	/** Remove component. */
	void Remove( deoglRComponent *component );
	
	/** Remove component if existing in the list. Returns true if removed. */
	bool RemoveIfExisting( deoglRComponent *component );
	
	/** Remove component from the given index. */
	void RemoveFrom( int index );
	
	/** Remove all components. */
	void RemoveAll();
	
	/** Mark all components. */
	void MarkAll( bool mark );
	
	/** Remove marked components. */
	void RemoveAllMarked( bool mark );
	/*@}*/
	
private:
	void pAddComponent( deoglRComponent *component );
};

#endif
