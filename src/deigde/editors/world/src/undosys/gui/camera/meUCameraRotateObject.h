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

#ifndef _MEUCAMERAROTATEOBJECT_H_
#define _MEUCAMERAROTATEOBJECT_H_

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

class meObject;



/**
 * Undo action for camera rotating an object.
 */
class meUCameraRotateObject : public igdeUndo{
private:
	meObject *pObject;
	decVector pOldRotation;
	decVector pNewRotation;
	
public:
	// constructor, destructor
	meUCameraRotateObject( meObject *object );
	virtual ~meUCameraRotateObject();
	
	void SetNewRotation( const decVector &rotation );
	bool HasChanged() const;
	
	// undo and redo operations
	virtual void Undo();
	virtual void Redo();
};

#endif
