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
#ifndef _MEUSETOBJECTCLASS_H_
#define _MEUSETOBJECTCLASS_H_

// includes
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

// predefinitions
class meObject;
class meObjectList;



/**
 * Undo action for setting object class.
 */
class meUSetObjectClass : public igdeUndo{
private:
	struct sObject{
		meObject *object;
		decString oldcname;
		decString newcname;
		decVector oldsize;
		decVector oldscaling;
	};
	
private:
	sObject *pObjects;
	int pObjectCount;
	
public:
	// constructor, destructor
	meUSetObjectClass( meObject *object, const char *newcname );
	meUSetObjectClass( meObjectList &objects, const char *newcname );
	
protected:
	~meUSetObjectClass();
	
public:
	// management
	void Undo();
	void Redo();
	
private:
	void pCleanUp();
};

// end of include only once
#endif
