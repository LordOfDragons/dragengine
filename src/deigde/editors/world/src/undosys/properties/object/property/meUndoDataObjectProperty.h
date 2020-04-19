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

#ifndef _MEUNDODATAOBJECTPROPERTY_H_
#define _MEUNDODATAOBJECTPROPERTY_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringDictionary.h>

class meObject;



/**
 * \brief Object Property Undo Data.
 */
class meUndoDataObjectProperty : public deObject{
private:
	meObject *pObject;
	decString pOldValue;
	bool pPropertyExists;
	decStringDictionary pOldProperties;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new undo data object. */
	meUndoDataObjectProperty( meObject *object );
	
protected:
	/** \brief Clean up undo data object. */
	virtual ~meUndoDataObjectProperty();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** Retrieves the object pointer. */
	inline meObject *GetObject() const{ return pObject; }
	/** Retrieves the old property value. */
	inline const decString &GetOldValue() const{ return pOldValue; }
	/** Sets the old property value. */
	void SetOldValue( const char *value );
	/** Determines if the property exists already before the redo operation. */
	inline bool GetPropertyExists() const{ return pPropertyExists; }
	/** Sets if the property exists already before the redo operation. */
	void SetPropertyExists( bool exists );
	/** Retrieves the old property list. */
	inline decStringDictionary &GetOldProperties(){ return pOldProperties; }
	inline const decStringDictionary &GetOldProperties() const{ return pOldProperties; }
	/*@}*/
};

#endif
