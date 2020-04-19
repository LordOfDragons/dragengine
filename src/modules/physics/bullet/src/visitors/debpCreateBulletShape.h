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

#ifndef _DEBPCREATEBULLETSHAPE_H_
#define _DEBPCREATEBULLETSHAPE_H_

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/shape/decShapeVisitor.h>
#include <dragengine/common/math/decMath.h>

class debpBulletCompoundShape;
class debpBulletShape;
class btTransform;
class btVector3;



/**
 * \brief Create bullet shape from engine shapes.
 * 
 * Creates a bullet shape visiting an engine shape. The visitor
 * creates a minimal shape if possible. The created bullet shape
 * is not held by the visitor. The caller is responsible to free
 * the object also in case of exceptions.
 */
class debpCreateBulletShape : public decShapeVisitor{
private:
	debpBulletShape *pBulletShape;
	debpBulletCompoundShape *pBulletCompoundShape;
	float pCcdThreshold;
	float pCcdRadius;
	decVector pOffset;
	decVector pScale;
	bool pNoMargin;
	bool pHasScale;
	int pShapeIndex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	debpCreateBulletShape();
	
	/** \brief Clean up visitor. */
	virtual ~debpCreateBulletShape();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief CCD threshold. */
	inline float GetCcdThreshold() const{ return pCcdThreshold; }
	
	/** \brief CCD radius. */
	inline float GetCcdRadius() const { return pCcdRadius; }
	
	/** \brief Set offset. */
	void SetOffset( const decVector &offset );
	
	/** \brief Set scale. */
	void SetScale( const decVector &scale );
	
	/** \brief No margin is set on created shapes. */
	inline bool GetNoMargin() const{ return pNoMargin; }
	
	/** \brief Set if no margin is set on created shapes. */
	void SetNoMargin( bool noMargin );
	
	/** \brief Shape or \em NULL if not created. */
	debpBulletShape *GetBulletShape() const;
	
	/** \brief Reset visitor. */
	void Reset();
	
	/** \brief Set shape index. */
	void SetShapeIndex( int index );
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
	
	
	
private:
	void pCreateCompoundShape();
	void pAddCollisionShape( debpBulletShape *collisionShape );
	void pAddTransformedCollisionShape( debpBulletShape *collisionShape, const btTransform &transform );
};

#endif
