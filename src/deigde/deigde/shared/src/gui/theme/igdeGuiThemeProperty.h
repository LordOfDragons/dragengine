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

#ifndef _IGDEGUITHEMEPROPERTY_H_
#define _IGDEGUITHEMEPROPERTY_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Immutable IGDE UI Theme Property.
 * 
 * Value can be of type integer, floating point, point, color or string.
 */
class DE_DLL_EXPORT igdeGuiThemeProperty : public deObject{
public:
	/** \brief Type. */
	enum eType{
		/** \brief Integer value. */
		etInteger,
		
		/** \brief Floating point value. */
		etFloat,
		
		/** \brief Point value. */
		etPoint,
		
		/** \brief Color value. */
		etColor,
		
		/** \brief String value. */
		etString
	};
	
	
	
private:
	decString pName;
	eType pType;
	int pInteger;
	float pFloat;
	decPoint pPoint;
	decColor pColor;
	decString pString;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create gui theme property. */
	igdeGuiThemeProperty( const char *name, int value );
	igdeGuiThemeProperty( const char *name, float value );
	igdeGuiThemeProperty( const char *name, const decPoint &value );
	igdeGuiThemeProperty( const char *name, const decColor &value );
	igdeGuiThemeProperty( const char *name, const decString &value );
	
	
	
protected:
	/**
	 * \brief Clean up gui theme property.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeGuiThemeProperty();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Type. */
	inline eType GetType() const{ return pType; }
	
	/**
	 * \brief Integer value.
	 * \throws EInvalidParam Type is not etInteger.
	 */
	int GetInt() const;
	
	/**
	 * \brief Floating point value.
	 * \throws EInvalidParam Type is not etFloat.
	 */
	float GetFloat() const;
	
	/**
	 * \brief Point value.
	 * \throws EInvalidParam Type is not etPoint.
	 */
	const decPoint &GetPoint() const;
	
	/**
	 * \brief Color value.
	 * \throws EInvalidParam Type is not etColor.
	 */
	const decColor &GetColor() const;
	
	/**
	 * \brief String value.
	 * \throws EInvalidParam Type is not etString.
	 */
	const decString &GetString() const;
	/*@}*/
};

#endif
