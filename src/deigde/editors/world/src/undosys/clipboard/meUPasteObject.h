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

#ifndef _MEUPASTEOBJECT_H_
#define _MEUPASTEOBJECT_H_

#include "../../world/object/meObjectList.h"

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decIntList.h>

class meClipboardDataObject;
class meWorld;



/**
 * \brief Undo action for objects pasted from the clipboard.
 * 
 * Creates a copy of each object in the clipboard and then inserts or removes this copy
 * from the current map.
 */
class meUPasteObject : public igdeUndo{
private:
	meWorld *pWorld;
	meObjectList pObjects;
	decIntList pAttachedToIndexList;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo object. */
	meUPasteObject( meWorld *world, meClipboardDataObject *clip );
	
	/** \brief Clean up undo object. */
	virtual ~meUPasteObject();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Undo action. */
	virtual void Undo();
	
	/** \brief Redo action. */
	virtual void Redo();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
