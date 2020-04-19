/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEOCINHERIT_H_
#define _GDEOCINHERIT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Inherit from another game definition class.
 */
class gdeOCInherit : public deObject{
private:
	decString pName;
	decString pPropertyPrefix;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create inherit object. */
	gdeOCInherit( const char *name );
	
	/** \brief Create copy of inherit object. */
	gdeOCInherit( const gdeOCInherit &inherit );
	
protected:
	/** \brief Cleans up the link. */
	virtual ~gdeOCInherit();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name of class to inherit if present. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name of class to inherit if present. */
	void SetName( const char *name );
	
	/** \brief Prefix to add to properties defined in inherited class. */
	inline const decString &GetPropertyPrefix() const{ return pPropertyPrefix; }
	
	/** \brief Set prefix to add to properties defined in inherited class. */
	void SetPropertyPrefix( const char *prefix );
	/*@}*/
};

#endif
