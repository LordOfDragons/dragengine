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
	void SetMatrix( const decMatrix &matrix );
	
	
	
	/** Edge color. */
	inline const decColor &GetEdgeColor() const{ return pEdgeColor; }
	
	/** Set edge color. */
	void SetEdgeColor( const decColor &color );
	
	/** Fill color. */
	inline const decColor &GetFillColor() const{ return pFillColor; }
	
	/** Set fill color. */
	void SetFillColor( const decColor &color );
	
	
	
	/** Shape list. */
	inline const decShapeList &GetShapeList() const{ return pShapeList; }
	
	/** Set shape list. */
	void SetShapeList( const decShapeList &shapes );
	
	
	
	/** First point for rendering fill triangles. */
	inline int GetFillFirstPoint() const{ return pFillFirstPoint; }
	
	/** Set first point for rendering fill triangles. */
	void SetFillFirstPoint( int firstPoint );
	
	/** Point count for rendering fill triangles. */
	inline int GetFillPointCount() const{ return pFillPointCount; }
	
	/** Set point count for rendering fill triangles. */
	void SetFillPointCount( int pointCount );
	
	/** First point for rendering lines. */
	inline int GetLineFirstPoint() const{ return pLineFirstPoint; }
	
	/** Set first point for rendering lines. */
	void SetLineFirstPoint( int firstPoint );
	
	/** Point count for rendering lines. */
	inline int GetLinePointCount() const{ return pLinePointCount; }
	
	/** Set point count for rendering lines. */
	void SetLinePointCount( int pointCount );
	
	
	
	/** Calculate the number of points required and stores them. */
	int CalcRequiredPoints( const deDebugDrawerShape &ddshape, int firstPoint );
	
	/** Write points to VBO data. */
	void WriteVBOData( const deDebugDrawerShape &ddshape, oglVector3 *vboData );
	/*@}*/
	
	
	
private:
	void pPrepareHulls();
};

#endif
