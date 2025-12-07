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
class DE_DLL_EXPORT igdeTextureProperty{
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
	igdeTextureProperty(const char *name);
	
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
	void SetDescription(const char *description);
	
	/** \brief Type. */
	inline ePropertyTypes GetType() const{ return pType; }
	
	/** \brief Set type. */
	void SetType(ePropertyTypes type);
	
	/** \brief Number of components. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** \brief Set number of components. */
	void SetComponentCount(int count);
	
	/** \brief Default value. */
	inline float GetDefaultValue() const{ return pDefaultValue; }
	
	/** \brief Set default value. */
	void SetDefaultValue(float value);
	
	/** \brief Default color. */
	inline const decColor &GetDefaultColor() const{ return pDefaultColor; }
	
	/** \brief Set default color. */
	void SetDefaultColor(const decColor &color);
	
	/** \brief List of affected modules. */
	inline decStringSet &GetAffectedModules(){ return pAffectedModules; }
	inline const decStringSet &GetAffectedModules() const{ return pAffectedModules; }
	/*@}*/
};

#endif
