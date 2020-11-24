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

#ifndef _MEUROTATEOBJECT_H_
#define _MEUROTATEOBJECT_H_

#include "../meBaseUndoRotate.h"
#include <dragengine/common/collection/decObjectOrderedSet.h>

class meObjectList;
class meWorld;



/**
 * Undo action for rotating a set of objects.
 */
class meURotateObject : public meBaseUndoRotate{
private:
	meWorld *pWorld;
	decObjectOrderedSet pObjects;
	
public:
	// constructor, destructor
	meURotateObject( meWorld *world, const meObjectList &objects );
	virtual ~meURotateObject();
	
	// undo and redo operations
	virtual void Undo();
	virtual void Redo();
	virtual void ProgressiveRedo();
	
private:
	void pCleanUp();
};

// end of include only once
#endif
