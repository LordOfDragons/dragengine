/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DEBPSHAPETOVOLUME_H_
#define _DEBPSHAPETOVOLUME_H_

#include <dragengine/common/shape/decShapeVisitor.h>
#include <dragengine/common/math/decMath.h>

class debpDCollisionVolume;
class debpDCollisionSphere;
class debpDCollisionBox;
class debpDCollisionCylinder;
class debpDCollisionCapsule;



/**
 * \brief Bullet Shape to Collision Volume Visitor.
 * 
 * Transforms a shape using a matrix to a collision volume. This is a rather hacky class and
 * serves more as a filler until a better solution is found. Only very few shapes are handled
 * by this class.
 */
class debpShapeToVolume : public decShapeVisitor{
private:
	debpDCollisionSphere *pSphere;
	debpDCollisionBox *pBox;
	debpDCollisionCylinder *pCylinder;
	debpDCollisionCapsule *pCapsule;
	debpDCollisionVolume *pVolume;
	decDMatrix pMatrix;
	
	
	
public:
	/** \name Constructor, Destructor */
	/*@{*/
	/** \brief Create visitor. */
	debpShapeToVolume();
	
	/** \brief Clean up visitor. */
	virtual ~debpShapeToVolume();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	inline const decDMatrix &GetMatrix() const{ return pMatrix; }
	inline debpDCollisionVolume *GetVolume() const{ return pVolume; }
	void SetMatrix( const decDMatrix &matrix );
	
	debpDCollisionVolume *GetVolumeFor( decShape *shape );
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
	void pCleanUp();
};

#endif
