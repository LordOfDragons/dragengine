/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeGuiTheme> Ref;
	
	
	
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
