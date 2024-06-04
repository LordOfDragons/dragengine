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
