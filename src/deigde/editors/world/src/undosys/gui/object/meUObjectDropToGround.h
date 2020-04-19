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

// include only once
#ifndef _MEUOBJECTDROPTOGROUND_H_
#define _MEUOBJECTDROPTOGROUND_H_

// includes
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

// predefinitions
class meWorld;
class meObject;
class meObjectList;



/**
 * \brief Drop objects to ground undo action.
 *
 * Undo action dropping a list of objects individually onto the ground.
 */
class meUObjectDropToGround : public igdeUndo{
private:
	struct sObject{
		meObject *object;
		decDVector oldpos;
		decVector oldrot;
	};
	
private:
	meWorld *pWorld;
	
	sObject *pObjects;
	int pObjectCount;
	
	bool pDropOnObjects;
	bool pAlign;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meUObjectDropToGround( meWorld *world, const meObjectList &objects );
	/** \brief Clean up object. */
	virtual ~meUObjectDropToGround();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Sets if the objects can be dropped on other objects. */
	void SetDropOnObjects( bool dropOnObjects );
	/** Sets if the objects are aligned with the ground or just dropped down. */
	void SetAlign( bool align );
	
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
