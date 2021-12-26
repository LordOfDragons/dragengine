/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEPROPFIELDGROUND_H_
#define _DEPROPFIELDGROUND_H_

#include "../../dragengine_export.h"


class deComponent;
class deHeightTerrain;


/**
 * \brief Prop Field Ground.
 *
 * Defines the ground for instances of a prop field to be projected
 * upon. This list contains components as well as the
 * height terrain making up the ground. This list is used by the
 * physics module to project prop field instances onto the ground.
 * This list is designed to be used for a short time to align
 * instances added to a prop field and is afterwards released.
 * components are held by the prop field ground. Since
 * this object is designed to live only for a short time no
 * reference counting is used.
 */
class DE_DLL_EXPORT dePropFieldGround{
private:
	deHeightTerrain *pHeightTerrain;
	
	deComponent **pComponents;
	int pComponentCount;
	int pComponentSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new prop field ground. */
	dePropFieldGround();
	
	/** \brief Clean up prop field ground. */
	~dePropFieldGround();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Clears the ground of the height terrain and all terrains and components. */
	void Clear();
	
	/** \brief Height terrain or NULL if not set. */
	inline deHeightTerrain *GetHeightTerrain() const{ return pHeightTerrain; }
	
	/** \brief Set height terrain or NULL if not set. */
	void SetHeightTerrain( deHeightTerrain *heightTerrain );
	/*@}*/
	
	
	
	/** \name Components */
	/*@{*/
	/** \brief Number of components. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** \brief Component at the given index. */
	deComponent *GetComponentAt( int index ) const;
	
	/** \brief Index of the given component or -1 if not found. */
	int IndexOfComponent( deComponent *component ) const;
	
	/** \brief Given component exists. */
	bool HasComponent( deComponent *component ) const;
	
	/** \brief Adds a component. */
	void AddComponent( deComponent *component );
	
	/** \brief Removes a component. */
	void RemoveComponent( deComponent *component );
	
	/** \brief Removes all components. */
	void RemoveAllComponents();
	/*@}*/
};

#endif
