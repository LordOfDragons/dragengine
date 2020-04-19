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

#ifndef _MEUNDODATAOBJTEXPROPERTY_H_
#define _MEUNDODATAOBJTEXPROPERTY_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decStringDictionary.h>

class meObjectTexture;



/**
 * \brief Undo data object texture properties.
 */
class meUndoDataObjTexProperty : public deObject{
private:
	meObjectTexture *pTexture;
	decString pOldValue;
	bool pPropertyExists;
	decStringDictionary pOldProperties;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo data. */
	meUndoDataObjTexProperty( meObjectTexture *texture );
	
protected:
	/** \brief Clean up undo data. */
	virtual ~meUndoDataObjTexProperty();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Texture. */
	inline meObjectTexture *GetTexture() const{ return pTexture; }
	
	/** \brief Old property value. */
	inline const decString &GetOldValue() const{ return pOldValue; }
	
	/** \brief Set old property value. */
	void SetOldValue( const char *value );
	
	/** \brief Property exists before redo operation. */
	inline bool GetPropertyExists() const{ return pPropertyExists; }
	
	/** \brief Set if property exists before redo operation. */
	void SetPropertyExists( bool exists );
	
	/** \brief Old properties. */
	inline decStringDictionary &GetOldProperties(){ return pOldProperties; }
	inline const decStringDictionary &GetOldProperties() const{ return pOldProperties; }
	/*@}*/
};

#endif
