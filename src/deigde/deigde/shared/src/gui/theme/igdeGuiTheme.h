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

#ifndef _IGDEGUITHEME_H_
#define _IGDEGUITHEME_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectDictionary.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class igdeGuiThemeProperty;



/**
 * \brief IGDE UI Theme.
 * 
 * Stores parameters to alter the visual aspects of UI elements. Values can be of type
 * integer, floating point, point, color or string.
 */
class DE_DLL_EXPORT igdeGuiTheme : public deObject{
private:
	decString pName;
	decObjectDictionary pProperties;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create gui theme. */
	igdeGuiTheme( const char *name );
	
	/** \brief Create copy of gui theme. */
	igdeGuiTheme( const char *name, const igdeGuiTheme &guitheme );
	
	
	
protected:
	/**
	 * \brief Clean up gui theme.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeGuiTheme();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	
	
	/** \brief Number of properties. */
	int GetPropertyCount() const;
	
	/** \brief Property or NULL if absent. */
	igdeGuiThemeProperty *GetProperty( const char *name ) const;
	
	/** \brief Property is present. */
	bool HasProperty( const char *name ) const;
	
	/** \brief Set property. */
	void SetProperty( igdeGuiThemeProperty *property );
	
	/** \brief Remove property. */
	void RemoveProperty( const char *name );
	
	/** \brief Remove property if present. */
	void RemovePropertyIfPresent( const char *name );
	
	/** \brief Remove all properties. */
	void RemoveAllProperties();
	
	/** \brief List of property names. */
	decStringList GetPropertyNames() const;
	
	
	
	/**
	 * \brief Named integer property or default value if not found.
	 * \throws EInvalidParam Type is not etInteger.
	 */
	int GetIntProperty( const char *name, int defaultValue ) const;
	
	/**
	 * \brief Named floating point value or default value if not found.
	 * \throws EInvalidParam Type is not etFloat.
	 */
	float GetFloatProperty( const char *name, float defaultValue ) const;
	
	/**
	 * \brief Named point value or default value if not found.
	 * \throws EInvalidParam Type is not etPoint.
	 */
	const decPoint &GetPointProperty( const char *name, const decPoint &defaultValue ) const;
	
	/**
	 * \brief Named color value or default value if not found.
	 * \throws EInvalidParam Type is not etColor.
	 */
	const decColor &GetColorProperty( const char *name, const decColor &defaultValue ) const;
	
	/**
	 * \brief Named string value or default value if not found.
	 * \throws EInvalidParam Type is not etString.
	 */
	const decString &GetStringProperty( const char *name, const decString &defaultValue ) const;
	
	
	
	/** \brief Set integer property. */
	void SetIntProperty( const char *name, int value );
	
	/** \brief Set nfloating point property. */
	void SetFloatProperty( const char *name, float value );
	
	/** \brief Set point property. */
	void SetPointProperty( const char *name, const decPoint &value );
	
	/** \brief Set color property. */
	void SetColorProperty( const char *name, const decColor &value );
	
	/** \brief Set string property. */
	void SetStringProperty( const char *name, const decString &value );
	
	
	
	/** \brief Assign gui theme properties. */
	igdeGuiTheme &operator=( const igdeGuiTheme &guitheme );
	/*@}*/
};

#endif
