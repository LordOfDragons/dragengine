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

#ifndef _MEUADDOBJECTDECAL_H_
#define _MEUADDOBJECTDECAL_H_

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

class meWorld;
class meDecal;
class meObject;



/**
 * Adds a new decal to the an object.
 */
class meUAddObjectDecal : public igdeUndo{
private:
	meWorld *pWorld;
	meDecal *pDecal;
	meObject *pParentObject;
	
public:
	// constructor, destructor
	meUAddObjectDecal( meWorld *world, meObject *parentObject );
	~meUAddObjectDecal();
	
	// Management
	/** Retrieves the decal. */
	inline meDecal *GetDecal() const{ return pDecal; }
	/** Retrieves the parent object or NULL. */
	inline meObject *GetParentObject() const{ return pParentObject; }
	
	// undo and redo operations
	virtual void Undo();
	virtual void Redo();
	
	// internal
	void UpdateInfos();
	
private:
	void pCleanUp();
};

// end of include only once
#endif
