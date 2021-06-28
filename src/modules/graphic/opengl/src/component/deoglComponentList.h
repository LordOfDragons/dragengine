/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLCOMPONENTLIST_H_
#define _DEOGLCOMPONENTLIST_H_

#include <dragengine/common/collection/decPointerList.h>

class deoglRComponent;


/**
 * Component pointer list.
 */
class deoglComponentList{
private:
	decPointerList pComponents;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create list. */
	deoglComponentList();
	
	/** Clean up list. */
	~deoglComponentList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Count of components. */
	int GetCount() const;
	
	/** Index of the component or -1 if absent. */
	int IndexOfComponent( deoglRComponent *component ) const;
	
	/** Component at index. */
	deoglRComponent *GetAt( int index ) const;
	
	/** Add component. */
	void Add( deoglRComponent *component );
	
	/** Remove component from index. */
	void RemoveFrom( int index );
	
	/** Remove all components. */
	void RemoveAll();
	/*@}*/
};

#endif
