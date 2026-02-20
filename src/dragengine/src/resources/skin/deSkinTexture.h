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

#ifndef _DESKINTEXTURE_H_
#define _DESKINTEXTURE_H_

#include "property/deSkinProperty.h"
#include "../../common/math/decMath.h"
#include "../../common/string/decString.h"
#include "../../common/collection/decTUniqueList.h"

class deSkinVisitor;


/**
 * \brief Skin Texture.
 *
 * Stores information about a skin texture. Each texture contains properties
 * defining the material properties for rendering. Furthermore audio related
 * properties are stored as well as physics properties.
 */
class DE_DLL_EXPORT deSkinTexture{
public:
	/** \brief Reference type. */
	using Ref = deTUniqueReference<deSkinTexture>;
	
	/** \brief List type. */
	using List = decTUniqueList<deSkinTexture>;
	
private:
	const decString pName;
	
	deSkinProperty::List pProperties;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new skin texture with the given name. */
	deSkinTexture(const char *name);
	
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
	/** \brief Properties. */
	inline const deSkinProperty::List &GetProperties() const{ return pProperties; }
	
	/** \brief Count of properties. */
	inline int GetPropertyCount() const{ return pProperties.GetCount(); }
	
	/** \brief Property at the given index. */
	const deSkinProperty::Ref &GetPropertyAt(int index) const{ return pProperties.GetAt(index); }
	
	/** \brief Property with the given type or NULL if not found. */
	deSkinProperty *GetPropertyWithType(const char *type) const;
	
	/** \brief Index of the property or -1 if not found. */
	int IndexOfProperty(deSkinProperty *property) const;
	
	/** \brief Determines if a property exists. */
	bool HasProperty(deSkinProperty *property) const;
	
	/** \brief Determines if a property with the given type exists. */
	bool HasPropertyWithType(const char *type) const;
	
	/** \brief Adds a property. */
	void AddProperty(deSkinProperty::Ref &&property);
	/*@}*/
};

#endif
