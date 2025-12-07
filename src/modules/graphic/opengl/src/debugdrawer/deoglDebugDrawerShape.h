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

#ifndef _DEOGLDEBUGDRAWERSHAPE_H_
#define _DEOGLDEBUGDRAWERSHAPE_H_

#include "../deoglBasics.h"

#include <dragengine/common/collection/decIntList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/shape/decShapeList.h>

class deDebugDrawerShape;



/**
 * Debug drawer peer shape.
 */
class deoglDebugDrawerShape{
private:
	decMatrix pMatrix;
	
	decColor pEdgeColor;
	decColor pFillColor;
	
	decShapeList pShapeList;
	
	int pFillFirstPoint;
	int pFillPointCount;
	int pLineFirstPoint;
	int pLinePointCount;
	
	decVector *pHullPoints;
	int pHullPointCount;
	decIntList pHullIndices;
	bool pDirtyHulls;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create debug drawer shape. */
	deoglDebugDrawerShape();
	
	/** Clean up debug drawer shape. */
	~deoglDebugDrawerShape();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Matrix. */
	inline const decMatrix &GetMatrix() const{ return pMatrix; }
	
	/** Set matrix. */
	void SetMatrix(const decMatrix &matrix);
	
	
	
	/** Edge color. */
	inline const decColor &GetEdgeColor() const{ return pEdgeColor; }
	
	/** Set edge color. */
	void SetEdgeColor(const decColor &color);
	
	/** Fill color. */
	inline const decColor &GetFillColor() const{ return pFillColor; }
	
	/** Set fill color. */
	void SetFillColor(const decColor &color);
	
	
	
	/** Shape list. */
	inline const decShapeList &GetShapeList() const{ return pShapeList; }
	
	/** Set shape list. */
	void SetShapeList(const decShapeList &shapes);
	
	
	
	/** First point for rendering fill triangles. */
	inline int GetFillFirstPoint() const{ return pFillFirstPoint; }
	
	/** Set first point for rendering fill triangles. */
	void SetFillFirstPoint(int firstPoint);
	
	/** Point count for rendering fill triangles. */
	inline int GetFillPointCount() const{ return pFillPointCount; }
	
	/** Set point count for rendering fill triangles. */
	void SetFillPointCount(int pointCount);
	
	/** First point for rendering lines. */
	inline int GetLineFirstPoint() const{ return pLineFirstPoint; }
	
	/** Set first point for rendering lines. */
	void SetLineFirstPoint(int firstPoint);
	
	/** Point count for rendering lines. */
	inline int GetLinePointCount() const{ return pLinePointCount; }
	
	/** Set point count for rendering lines. */
	void SetLinePointCount(int pointCount);
	
	
	
	/** Calculate the number of points required and stores them. */
	int CalcRequiredPoints(const deDebugDrawerShape &ddshape, int firstPoint);
	
	/** Write points to VBO data. */
	void WriteVBOData(const deDebugDrawerShape &ddshape, oglVector3 *vboData);
	/*@}*/
	
	
	
private:
	void pPrepareHulls();
};

#endif
