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
