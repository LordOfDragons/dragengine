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
#ifndef _MEDECALROTATE_H_
#define _MEDECALROTATE_H_

// includes
#include "../meBaseUndoRotate.h"

// predefinitions
class meUndoDataDecal;
class meWorld;



/**
 * \brief Rotate Decals Undo Action.
 *
 * Undo Action for interactively rotating decals.
 */
class meUDecalRotate : public meBaseUndoRotate{
private:
	meWorld *pWorld;
	
	meUndoDataDecal **pDecals;
	int pDecalCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new undo. */
	meUDecalRotate( meWorld *world );
	/** \brief Clean up undo. */
	virtual ~meUDecalRotate();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	virtual void Undo();
	virtual void Redo();
	virtual void ProgressiveRedo();
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
