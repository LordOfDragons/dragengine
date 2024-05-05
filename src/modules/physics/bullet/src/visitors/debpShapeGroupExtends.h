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
