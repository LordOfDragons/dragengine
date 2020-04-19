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

// include only once
#ifndef _DEBPSHAPEGROUPEXTENDS_H_
#define _DEBPSHAPEGROUPEXTENDS_H_

// includes
#include "dragengine/common/shape/decShapeVisitor.h"
#include "dragengine/common/math/decMath.h"

// predefinitions
class debpDCollisionVolume;



/**
 * @brief Bullet Shape group extends Visitor.
 *
 * Calculates the minimal bounding box ( extends ) of a groups of shapes.
 */
class debpShapeGroupExtends : public decShapeVisitor{
private:
	decDVector pMinExtend;
	decDVector pMaxExtend;
	bool pExtendsNotSetYet;
	
	decDMatrix pMatrix;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new visitor. */
	debpShapeGroupExtends();
	/** Cleans up the visitor. */
	virtual ~debpShapeGroupExtends();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the minimum extend. */
	inline const decDVector &GetMinimumExtend() const{ return pMinExtend; }
	/** Retrieves the maximum extend. */
	inline const decDVector &GetMaximumExtend() const{ return pMaxExtend; }
	/** Resets the visitor. */
	void Reset();
	/** Sets the rotation to apply to shapes before calculating the extends. */
	void SetRotation( const decQuaternion &rotation );
	/** Sets the reference point. */
	void SetReferencePoint( const decDVector &referencePoint );
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
	
private:
	void pAddExtends( const decDVector &minExtend, const decDVector &maxExtend );
	void pAddExtendsFrom( debpDCollisionVolume &volume );
};

// end of include only once
#endif
