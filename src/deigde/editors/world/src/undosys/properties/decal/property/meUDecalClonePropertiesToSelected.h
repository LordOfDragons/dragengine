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

#ifndef _MEUDECALCLONEPROPERTIESTOSELECTED_H_
#define _MEUDECALCLONEPROPERTIESTOSELECTED_H_

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/string/decStringDictionary.h>

#include <deigde/undo/igdeUndo.h>

class meDecal;
class meDecalList;
class meUndoDataDecalProperty;



/**
 * \brief Undo action decal clone properties to all selected decals.
 */
class meUDecalClonePropertiesToSelected : public igdeUndo{
private:
	decObjectList pList;
	decStringDictionary pPropertyList;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo decal. */
	meUDecalClonePropertiesToSelected( const meDecalList &list, const decStringDictionary &propertyList );
	
protected:
	/** \brief Clean up undo decal. */
	virtual ~meUDecalClonePropertiesToSelected();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief List of decals to manipulate. */
	inline decObjectList &GetList(){ return pList; }
	inline const decObjectList &GetList() const{ return pList; }
	
	/** \brief List of properties to apply. */
	inline decStringDictionary &GetPropertyList(){ return pPropertyList; }
	inline const decStringDictionary &GetPropertyList() const{ return pPropertyList; }
	
	/** \brief Undo action. */
	virtual void Undo();
	
	/** \brief Redo action. */
	virtual void Redo();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
