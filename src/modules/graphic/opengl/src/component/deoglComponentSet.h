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
 * \brief Component set.
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
	/** \brief Create component list. */
	deoglComponentSet();
	
	/** \brief Clean up component list. */
	~deoglComponentSet();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of components. */
	inline int GetCount() const{ return pComponentCount; }
	
	/** \brief Index of the component or -1 if not in the list. */
	int IndexOfComponent( deoglRComponent *component ) const;
	
	/** \brief Component exists. */
	bool Has( deoglRComponent *component ) const;
	
	/** \brief Component at the given index. */
	deoglRComponent *GetAt( int index ) const;
	
	/** \brief Add component. */
	void Add( deoglRComponent *component );
	
	/** \brief Add component if missing in the list. Returns true if added. */
	bool AddIfMissing( deoglRComponent *component );
	
	/** \brief Remove component. */
	void Remove( deoglRComponent *component );
	
	/** \brief Remove component if existing in the list. Returns true if removed. */
	bool RemoveIfExisting( deoglRComponent *component );
	
	/** \brief Remove component from the given index. */
	void RemoveFrom( int index );
	
	/** \brief Remove all components. */
	void RemoveAll();
	
	/** \brief Mark all components. */
	void MarkAll( bool mark );
	
	/** \brief Remove marked components. */
	void RemoveAllMarked( bool mark );
	/*@}*/
	
private:
	void pAddComponent( deoglRComponent *component );
};

#endif
