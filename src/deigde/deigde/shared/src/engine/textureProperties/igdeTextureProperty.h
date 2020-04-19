/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDETEXTUREPROPERTY_H_
#define _IGDETEXTUREPROPERTY_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringSet.h>



/**
 * \brief Texture Property.
 * 
 * Stores the definition of a texture property.
 */
class igdeTextureProperty{
public:
	/** \brief Value types. */
	enum ePropertyTypes{
		/** \brief Single value. */
		eptValue,
		
		/** \brief Color value. */
		eptColor,
		
		/** \brief Image value. */
		eptImage
	};
	
	
	
private:
	decString pName;
	decString pDescription;
	ePropertyTypes pType;
	int pComponentCount;
	float pDefaultValue;
	decColor pDefaultColor;
	decStringSet pAffectedModules;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create texture property. */
	igdeTextureProperty( const char *name );
	
	/** \brief Clean up texture property. */
	~igdeTextureProperty();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description. */
	void SetDescription( const char *description );
	
	/** \brief Type. */
	inline ePropertyTypes GetType() const{ return pType; }
	
	/** \brief Set type. */
	void SetType( ePropertyTypes type );
	
	/** \brief Number of components. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** \brief Set number of components. */
	void SetComponentCount( int count );
	
	/** \brief Default value. */
	inline float GetDefaultValue() const{ return pDefaultValue; }
	
	/** \brief Set default value. */
	void SetDefaultValue( float value );
	
	/** \brief Default color. */
	inline const decColor &GetDefaultColor() const{ return pDefaultColor; }
	
	/** \brief Set default color. */
	void SetDefaultColor( const decColor &color );
	
	/** \brief List of affected modules. */
	inline decStringSet &GetAffectedModules(){ return pAffectedModules; }
	inline const decStringSet &GetAffectedModules() const{ return pAffectedModules; }
	/*@}*/
};

#endif
