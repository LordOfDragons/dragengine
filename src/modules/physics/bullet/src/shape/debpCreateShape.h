/* 
 * Drag[en]gine Bullet Physics Module
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

#ifndef _DEBPCREATESHAPE_H_
#define _DEBPCREATESHAPE_H_

#include <dragengine/deObjectReference.h>
#include <dragengine/common/shape/decShapeVisitor.h>

class debpShape;



/**
 * \brief Create Bullet Shape Visitor.
 *
 * Visitor for creating a bullet shape for engine shapes. The created
 * shape is not held by the visitor.
 */
class debpCreateShape : public decShapeVisitor{
private:
	deObjectReference pShape;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	debpCreateShape();
	
	/** \brief Clean up visitor. */
	~debpCreateShape();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Created shape or NULL if something went wrong. */
	inline debpShape *GetCreatedShape() const{ return ( debpShape* )( deObject* )pShape; }
	
	/** \brief Set created shape or NULL. */
	void SetCreatedShape( debpShape *shape );
	
	/** \brief Reset visitor setting shape to NULL. */
	void Reset();
	/*@}*/
	
	
	
	/** \name Visiting */
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

#endif
