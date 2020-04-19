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

// include only once
#ifndef _RECREATERIGSHAPE_H_
#define _RECREATERIGSHAPE_H_

// includes
#include "dragengine/common/shape/decShapeVisitor.h"
#include "dragengine/common/math/decMath.h"

// predefintions
class reRigShape;
class deEngine;



/**
 * @brief Create Rig Visitor.
 * Visitor to create an editor shape from an engine shape.
 */
class reCreateRigShape : public decShapeVisitor{
private:
	deEngine *pEngine;
	reRigShape *pRigShape;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new visitor. */
	reCreateRigShape( deEngine *engine );
	/** Cleans up the visitor. */
	virtual ~reCreateRigShape();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Resets the visitor. */
	void Reset();
	/** Retrieves the editor shape. */
	inline reRigShape *GetRigShape() const{ return pRigShape; }
	/*@}*/
	
	/** @name Visiting */
	/*@{*/
	/** \brief Visit shape. */
	virtual void VisitShape( decShape &shape );
	
	/** \brief Visit sphere shape. */
	virtual void VisitShapeSphere( decShapeSphere &sphere );
	
	/** \brief Visit box shape. */
	virtual void VisitShapeBox( decShapeBox &box );
	
	/** \brief Visit cylinder shape. */
	virtual void VisitShapeCylinder( decShapeCylinder &cylinder );
	
	/** \brief Visit capsule shape. */
	virtual void VisitShapeCapsule( decShapeCapsule &capsule );
	
	/** \brief Visit hull shape. */
	virtual void VisitShapeHull( decShapeHull &hull );
	/*@}*/
};

// end of include only once
#endif
