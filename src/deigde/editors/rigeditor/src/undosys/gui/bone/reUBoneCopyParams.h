#if 0
/* 
 * Drag[en]gine Rig Editor
 *
 * Copyright (C) 2019, Plüss Roland ( roland@rptd.ch )
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
#ifndef _REUBONECOPYPARAMS_H_
#define _REUBONECOPYPARAMS_H_

// includes
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

#include "../../../rig/constraint/reRigConstraint.h"

// predefinitions
class reRigShapeList;



/**
 * \brief Undo Copy Bone Parameters.
 *
 * Undo action to copy bone parameters. This includes the physics
 * parameters, shapes and constraints but not the geometry
 * information like the position or rotation since these usually
 * have to stay the same.
 */
class reUBoneCopy : public igdeUndo{
public:
	typedef deTObjectReference<reUBoneCopy> Ref;
	
	
private:
	struct sBone{
		
		reRigShapeList *shapes;
		reRigConstraint::List *constraints;
	};
	
private:
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new CLASS. */
	reUBoneCopy();
	/** \brief Clean up the CLASS. */
	~reUBoneCopy();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/*@}*/
	
private:
};

// end of include only once
#endif

#endif
