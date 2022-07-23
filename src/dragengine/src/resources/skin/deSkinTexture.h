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

#ifndef _DESKINTEXTURE_H_
#define _DESKINTEXTURE_H_

#include "../../common/math/decMath.h"
#include "../../common/string/decString.h"

class deSkinProperty;
class deSkinVisitor;


/**
 * \brief Skin Texture.
 *
 * Stores information about a skin texture. Each texture contains properties
 * defining the material properties for rendering. Furthermore audio related
 * properties are stored as well as physics properties.
 */
class DE_DLL_EXPORT deSkinTexture{
private:
	const decString pName;
	
	deSkinProperty **pProperties;
	int pPropertyCount;
	int pPropertySize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new skin texture with the given name. */
	deSkinTexture( const char *name );
	
	/** \brief Clean up skin texture. */
	~deSkinTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name of the texture. */
	inline const decString &GetName() const{ return pName; }
	/*@}*/
	
	
	
	/** \name Properties */
	/*@{*/
	/** \brief Count of properties. */
	inline int GetPropertyCount() const{ return pPropertyCount; }
	
	/** \brief Property at the given index. */
	deSkinProperty *GetPropertyAt( int index ) const;
	
	/** \brief Property with the given type or NULL if not found. */
	deSkinProperty *GetPropertyWithType( const char *type ) const;
	
	/** \brief Index of the property or -1 if not found. */
	int IndexOfProperty( deSkinProperty *property ) const;
	
	/** \brief Determines if a property exists. */
	bool HasProperty( deSkinProperty *property ) const;
	
	/** \brief Determines if a property with the given type exists. */
	bool HasPropertyWithType( const char *type ) const;
	
	/** \brief Adds a property. */
	void AddProperty( deSkinProperty *property );
	/*@}*/
};

#endif
