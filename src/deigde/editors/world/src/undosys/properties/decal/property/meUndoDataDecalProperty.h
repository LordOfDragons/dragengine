/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEUNDODATADECALPROPERTY_H_
#define _MEUNDODATADECALPROPERTY_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringDictionary.h>

class meDecal;



/**
 * \brief Decal property undo data.
 */
class meUndoDataDecalProperty : public deObject{
private:
	meDecal *pDecal;
	decString pOldValue;
	bool pPropertyExists;
	decStringDictionary pOldProperties;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo data object. */
	meUndoDataDecalProperty( meDecal *decal );
	
protected:
	/** \brief Clean up undo data object. */
	virtual ~meUndoDataDecalProperty();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Decal pointer. */
	inline meDecal *GetDecal() const{ return pDecal; }
	
	/** \brief Old property value. */
	inline const decString &GetOldValue() const{ return pOldValue; }
	
	/** \brief Set old property value. */
	void SetOldValue( const char *value );
	
	/** \brief Property has been present before the redo operation. */
	inline bool GetPropertyExists() const{ return pPropertyExists; }
	
	/** \brief Set if property has been present before the redo operation. */
	void SetPropertyExists( bool exists );
	
	/** \brief Old property list. */
	inline decStringDictionary &GetOldProperties(){ return pOldProperties; }
	inline const decStringDictionary &GetOldProperties() const{ return pOldProperties; }
	/*@}*/
};

#endif
