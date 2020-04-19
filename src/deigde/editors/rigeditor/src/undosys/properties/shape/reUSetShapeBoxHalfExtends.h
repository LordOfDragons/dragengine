/* 
 * Drag[en]gine IGDE Rig Editor
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

#ifndef _REUSETSHAPEBOXHALFEXTENDS_H_
#define _REUSETSHAPEBOXHALFEXTENDS_H_

#include <deigde/undo/igdeUndo.h>
#include "dragengine/common/math/decMath.h"

class reRigShapeBox;



/**
 * \brief Undo Set Shape HalfExtends.
 */
class reUSetShapeBoxHalfExtends : public igdeUndo{
private:
	reRigShapeBox *pShape;
	
	decVector pOldHalfExtends;
	decVector pNewHalfExtends;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	reUSetShapeBoxHalfExtends( reRigShapeBox *shape, const decVector &halfExtends );
	
protected:
	/** \brief Clean up undo. */
	virtual ~reUSetShapeBoxHalfExtends();
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
