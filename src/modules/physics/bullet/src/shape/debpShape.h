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

#ifndef _DEBPSHAPE_H_
#define _DEBPSHAPE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>

class decShape;
class debpDCollisionVolume;
class debpDCollisionBox;
class dePhysicsBullet;



/**
 * \brief Bullet Shape.
 *
 * Bullet counterpart of an engine shape. The bullet shape contains the original engine
 * shape and the collision volume used for collision testing. The collision volume can
 * be updated using a transformation matrix. A type is held to easily identify the
 * shape without requiring a visitor.
 */
class debpShape : public deObject{
public:
	/** \brief Shape types. */
	enum eShapeTypes{
		estSphere,
		estBox,
		estCylinder,
		estCapsule
	};
	
	
	
private:
	int pType;
	decShape *pShape;
	debpDCollisionVolume *pCollisionVolume;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shape. */
	debpShape( int type, decShape *shape );
	
protected:
	/** \brief Clean up shape. */
	virtual ~debpShape();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Shape type. */
	inline int GetType() const{ return pType; }
	
	/** \brief Engine shape. */
	inline decShape *GetShape() const{ return pShape; }
	
	/** \brief Collision volume. */
	inline debpDCollisionVolume *GetCollisionVolume() const{ return pCollisionVolume; }
	
	/** \brief Update collision volume using a transformation matrix. */
	virtual void UpdateWithMatrix( const decDMatrix &transformation ) = 0;
	
	/** \brief Print out on console debugging information about shape. */
	virtual void PrintDebug( dePhysicsBullet &module ) = 0;
	/*@}*/
	
	
	
protected:
	void SetCollisionVolume( debpDCollisionVolume *collisionVolume );
	static float UniformScale( const decDMatrix &matrix );
};

#endif
