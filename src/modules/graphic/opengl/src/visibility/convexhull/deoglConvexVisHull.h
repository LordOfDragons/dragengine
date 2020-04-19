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

#ifndef _DEOGLCONVEXVISHULL_H_
#define _DEOGLCONVEXVISHULL_H_

#include <dragengine/common/math/decMath.h>

#include "../../deoglBasics.h"


class deoglRenderThread;
class decConvexVolumeList;



/**
 * @brief Convex Visibility Hull.
 * Convex hull enclosing a visible area as seen from a point in space. The hull is guaranteed to be not
 * smaller than the visible concave area seen from a point in space. Most of the time the hull is though
 * larger especially if a tight fitting hull would be concave. The hull can be constructed from a convex
 * volume list. The hull provides the minimum and maximum extend relative to the reference point without
 * any rotation relative to the parent world. The hull supports also to be rendered using a VAO.
 */
class deoglConvexVisHull{
public:
	deoglRenderThread &pRenderThread;
	
	decVector pMinExtend;
	decVector pMaxExtend;
	
	GLuint pVAO;
	
	oglVector3 *pPoints;
	int pPointCount;
	
	GLuint pVBO;
	int pVBOPointCount;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new convex visibility hull. */
	deoglConvexVisHull( deoglRenderThread &renderThread );
	/** Cleans up the convex visibility hull. */
	~deoglConvexVisHull();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the minimum extend. */
	inline const decVector &GetMinimumExtend() const{ return pMinExtend; }
	/** Retrieves the maximum extend. */
	inline const decVector &GetMaximumExtend() const{ return pMaxExtend; }
	/** Sets the extends. */
	void SetExtends( const decVector &minExtend, const decVector &maxExtend );
	/** Calculate axis aligned bounding box for a specific transformation. */
	void CalcBoundingBox( decDVector &boxMinExtend, decDVector &boxMaxExtend, const decDMatrix &matrix ) const;
	
	/** Retrieves the points. */
	inline oglVector3 *GetPoints() const{ return pPoints; }
	/** Retrieves the number of points. */
	inline int GetPointCount() const{ return pPointCount; }
	/** Sets the number of points. */
	void SetPointCount( int count );
	
	/** Retrieves the VAO. */
	inline GLuint GetVAO() const{ return pVAO; }
	/** Updates the VBO from the points. */
	void UpdateVBO();
	
	/** Creates the hull using a convex volume list. Creates the VBO and calculates the extends. */
	void CreateFrom( const decConvexVolumeList &list );
	/*@}*/
};

#endif
