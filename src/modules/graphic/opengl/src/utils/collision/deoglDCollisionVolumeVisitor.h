/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLDCOLLISIONVOLUMEVISITOR_H_
#define _DEOGLDCOLLISIONVOLUMEVISITOR_H_

#include <dragengine/common/math/decMath.h>

class deoglDCollisionSphere;
class deoglDCollisionCylinder;
class deoglDCollisionCapsule;
class deoglDCollisionBox;
class deoglDCollisionTriangle;
class deoglDCollisionFrustum;



/**
 * Visitor for collision volumes.
 *
 * This class is provided mainly for easily identifying collision volumes
 * and carrying out actions depending on their type. The collision volume
 * system is not mandatory for module programmers hence this class to
 * allow them to easily use their own system. The default implementation
 * of all functions is to do nothing at all.
 */
class deoglDCollisionVolumeVisitor{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new collision volume visitor. */
	deoglDCollisionVolumeVisitor();
	/** Cleans up the collision volume visitor. */
	virtual ~deoglDCollisionVolumeVisitor();
	/*@}*/
	
	/** \name Visiting */
	/*@{*/
	/** Visits a collision sphere. */
	virtual void VisitSphere( deoglDCollisionSphere *sphere );
	/** Visits a collision cylinder. */
	virtual void VisitCylinder( deoglDCollisionCylinder *cylinder );
	/** Visits a collision capsule. */
	virtual void VisitCapsule( deoglDCollisionCapsule *capsule );
	/** Visits a collision box. */
	virtual void VisitBox( deoglDCollisionBox *box );
	/** Visits a collision triangle. */
	virtual void VisitTriangle( deoglDCollisionTriangle *triangle );
	/** Visits a collision frustum. */
	virtual void VisitFrustum( deoglDCollisionFrustum *frustum );
	/*@}*/
	
};


// end of include only once
#endif
