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

#ifndef _DEOGLVOLUMESHAPE_H_
#define _DEOGLVOLUMESHAPE_H_

#include <dragengine/common/shape/decShapeVisitor.h>
#include <dragengine/common/math/decMath.h>

class decShape;
class deoglShape;
class deoglRenderThread;



/**
 * Retrieves render shapes from dragengine shapes visitor.
 */
class deoglVolumeShape : public decShapeVisitor{
private:
	deoglRenderThread &pRenderThread;
	deoglShape *pShape;
	decMatrix pMatrix1;
	decMatrix pMatrix2;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new visitor. */
	deoglVolumeShape( deoglRenderThread &renderThread );
	/** Cleans up the visitor. */
	~deoglVolumeShape();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the shape. */
	inline deoglShape *GetShape() const{ return pShape; }
	/** Retrieves the first matrix. */
	inline const decMatrix &GetMatrix1() const{ return pMatrix1; }
	/** Retrieves the second matrix. */
	inline const decMatrix &GetMatrix2() const{ return pMatrix2; }
	
	/** Resets the visitor. */
	void Reset();
	/*@}*/
	
	/** \name Visiting */
	/*@{*/
	/** Visit shape. */
	virtual void VisitShape( decShape &shape );
	
	/** Visit sphere shape. */
	virtual void VisitShapeSphere( decShapeSphere &sphere );
	
	/** Visit box shape. */
	virtual void VisitShapeBox( decShapeBox &box );
	
	/** Visit cylinder shape. */
	virtual void VisitShapeCylinder( decShapeCylinder &cylinder );
	
	/** Visit capsule shape. */
	virtual void VisitShapeCapsule( decShapeCapsule &capsule );
	
	/** Visit hull shape. */
	virtual void VisitShapeHull( decShapeHull &hull );
	/*@}*/
};

#endif
