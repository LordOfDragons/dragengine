/* 
 * Drag[en]gine IGDE World Editor
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _MEUOBJECTCOPYSCALE_H_
#define _MEUOBJECTCOPYSCALE_H_

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/math/decMath.h>

class meWorld;


/**
 * Undo action for setting copy object scale.
 */
class meUObjectCopyScale : public igdeUndo{
private:
	decObjectOrderedSet pObjects;
	decVector pNewSize;
	bool pCopyX;
	bool pCopyY;
	bool pCopyZ;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo object. */
	meUObjectCopyScale( meWorld *world, bool copyX, bool copyY, bool copyZ );
	
protected:
	/** \brief Clean up undo object. */
	virtual ~meUObjectCopyScale();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
