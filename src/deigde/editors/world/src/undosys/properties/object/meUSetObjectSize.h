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
#ifndef _MEUNDOSETOBJECTSIZE_H_
#define _MEUNDOSETOBJECTSIZE_H_

// includes
#include <deigde/undo/igdeUndo.h>
#include "dragengine/common/math/decMath.h"

// predefinitions
class meObject;



/**
 * Undo action for setting object size.
 */
class meUSetObjectSize : public igdeUndo{
private:
	meObject *pObject;
	
	decVector pOldSize;
	decVector pNewSize;
	
public:
	// constructor, destructor
	meUSetObjectSize( meObject *object, const decVector &newSize );
	
protected:
	~meUSetObjectSize();
	
public:
	// management
	void Undo();
	void Redo();
};

// end of include only once
#endif
