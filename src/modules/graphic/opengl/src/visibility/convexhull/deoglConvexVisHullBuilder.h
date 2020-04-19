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

#ifndef _DEOGLCONVEXVISHULLBUILDER_H_
#define _DEOGLCONVEXVISHULLBUILDER_H_

#include <dragengine/common/math/decMath.h>

class decConvexVolume;
class decConvexVolumeList;
class deoglTriangleSorter;
class deoglROcclusionMesh;


/**
 * @brief Convex Visibility Hull Builder.
 */
class deoglConvexVisHullBuilder{
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new convex visibility hull builder. */
	deoglConvexVisHullBuilder();
	/** Cleans up the convex visibility hull builder. */
	~deoglConvexVisHullBuilder();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Builds a sphere volume. */
	void BuildSphere( decConvexVolumeList &volumeList, const decVector &position, float radius ) const;
	/** Builds a cone volume. */
	void BuildCone( decConvexVolumeList &volumeList, const decMatrix &matrix, float distance, float angle, int resolution ) const;
	/** Builds a frustum volume. */
	void BuildFrustum( decConvexVolumeList &volumeList, const decMatrix &matrix, float farDistance, float angleX, float angleY ) const;
	
	/** Retrieves the minimum and maximum extend of the given volumes. */
	void GetVolumeExtends( const decConvexVolumeList &volumeList, decVector &minExtend, decVector &maxExtend ) const;
	/**
	 * Retrieves the minimum and maximum extend of the given transformed volumes. The minExtend and maxExtend
	 * are kept untouched if the volume list contains no vertices.
	 */
	void GetTransformedVolumeExtends( const decConvexVolumeList &volumeList, const decDMatrix &matrix, decDVector &minExtend, decDVector &maxExtend ) const;
	
	/**
	 * Crops a volume list using a triangles list. The triangles are extruded radially from the origin
	 * blocked by obstacles. The distance indicates how far triangles are extruded and should be at
	 * least the radius of the sphere enclosing the hull boundary box.
	 */
	void CropByTriangles( decConvexVolumeList &volumeList, const deoglTriangleSorter &triangles,
	const decVector &origin, float distance ) const;
	/**
	 * Crops the volume list using an occlusion mesh. The faces are extruded radially from the origin
	 * blocked by obstacles. The distance indicates how far faces are extruded and should be at least
	 * the radius of the sphere enclosing the volumes boundary box.
	 */
	void CropByOcclusionMesh( decConvexVolumeList &volumeList, const deoglROcclusionMesh &occlusionMesh,
	const decMatrix &matrix, const decVector &origin, float distance ) const;
	/**
	 * Crops the volume list using a bounding box. The faces of the bounding box are used as clip planes
	 * cropping anything outside the bounding box. The matrix is used to transform the bounding box into
	 * the volume coordinate frame before cropping.
	 */
	void CropByBoundingBox( decConvexVolumeList &volumeList, const decDMatrix &matrix,
	const decDVector &minExtend, const decDVector &maxExtend ) const;
	/*@}*/
	
private:
	void pAddTriangle( decConvexVolume *volume, int p1, int p2, int p3 ) const;
	void pAddTriangle( decConvexVolume *volume, int p1, int p2, int p3, const decVector &normal ) const;
	void pAddQuad( decConvexVolume *volume, int p1, int p2, int p3, int p4 ) const;
	void pAddQuad( decConvexVolume *volume, int p1, int p2, int p3, int p4, const decVector &normal ) const;
};

#endif
