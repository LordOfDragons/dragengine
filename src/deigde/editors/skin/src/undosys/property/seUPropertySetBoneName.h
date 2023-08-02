/* 
 * Drag[en]gine IGDE Skin Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _SEUPROPERTYSETBONENAME_H_
#define _SEUPROPERTYSETBONENAME_H_

#include "../../skin/property/seProperty.h"

#include <deigde/undo/igdeUndo.h>


/**
 * \brief Undo Action Property Set Bone Name.
 */
class seUPropertySetBoneName : public igdeUndo{
private:
	const seProperty::Ref pProperty;
	const decString pOldName;
	const decString pNewName;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	seUPropertySetBoneName( seProperty *property, const char *newName );
	
protected:
	/** \brief Clean up undo. */
	virtual ~seUPropertySetBoneName() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo action. */
	virtual void Undo() override;
	
	/** \brief Redo action. */
	virtual void Redo() override;
	/*@}*/
};

#endif
