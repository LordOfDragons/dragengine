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

#ifndef _IGDEGDCLASSINHERIT_H_
#define _IGDEGDCLASSINHERIT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

#include "igdeGDClassReference.h"

class igdeGDClassManager;



/**
 * \brief Inherit from another game definition class.
 */
class DE_DLL_EXPORT igdeGDClassInherit : public deObject{
private:
	decString pName;
	igdeGDClassReference pClass;
	decString pPropertyPrefix;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create inherit object. */
	igdeGDClassInherit( const char *name );
	
	/** \brief Create copy of inherit object. */
	igdeGDClassInherit( const igdeGDClassInherit &inherit );
	
	
	
protected:
	/** \brief Clean up link. */
	virtual ~igdeGDClassInherit();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name of class to inherit if present. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Class if resolved. */
	inline igdeGDClass *GetClass() const{ return pClass; }
	
	/** \brief Resolv class if possible. */
	void ResolveClass( const igdeGDClassManager &classManager );
	
	/** \brief Prefix to add to properties defined in inherited class. */
	inline const decString &GetPropertyPrefix() const{ return pPropertyPrefix; }
	
	/** \brief Set prefix to add to properties defined in inherited class. */
	void SetPropertyPrefix( const char *prefix );
	/*@}*/
};

#endif
