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
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeGuiThemeProperty> Ref;
	
	
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
	igdeGuiThemeProperty(const char *name, int value);
	igdeGuiThemeProperty(const char *name, float value);
	igdeGuiThemeProperty(const char *name, const decPoint &value);
	igdeGuiThemeProperty(const char *name, const decColor &value);
	igdeGuiThemeProperty(const char *name, const decString &value);
	
	
	
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
	inline const decString &GetName() const{return pName;}
	
	/** \brief Type. */
	inline eType GetType() const{return pType;}
	
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
