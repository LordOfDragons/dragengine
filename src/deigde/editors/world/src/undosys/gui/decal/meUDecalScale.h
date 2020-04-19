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
#ifndef _MEUDECALSCALE_H_
#define _MEUDECALSCALE_H_

// includes
#include "../meBaseUndoScale.h"

// predefinitions
class meUndoDataDecal;
class meWorld;



/**
 * \brief Scale Decals Undo Action.
 *
 * Undo Action for interactively scaling decals.
 */
class meUDecalScale : public meBaseUndoScale{
private:
	meWorld *pWorld;
	
	meUndoDataDecal **pDecals;
	int pDecalCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new undo. */
	meUDecalScale( meWorld *world );
	/** \brief Clean up undo. */
	virtual ~meUDecalScale();
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
