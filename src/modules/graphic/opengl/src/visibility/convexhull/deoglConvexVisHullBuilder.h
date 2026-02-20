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

#ifndef _DEOGLCONVEXVISHULLBUILDER_H_
#define _DEOGLCONVEXVISHULLBUILDER_H_

#include <dragengine/common/math/decMath.h>

class decConvexVolume;
class decConvexVolumeList;
class deoglTriangleSorter;
class deoglROcclusionMesh;


/**
 * Convex Visibility Hull Builder.
 */
class deoglConvexVisHullBuilder{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new convex visibility hull builder. */
	deoglConvexVisHullBuilder();
	/** Cleans up the convex visibility hull builder. */
	~deoglConvexVisHullBuilder();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Builds a sphere volume. */
	void BuildSphere(decConvexVolumeList &volumeList, const decVector &position, float radius) const;
	/** Builds a cone volume. */
	void BuildCone(decConvexVolumeList &volumeList, const decMatrix &matrix, float distance, float angle, int resolution) const;
	/** Builds a frustum volume. */
	void BuildFrustum(decConvexVolumeList &volumeList, const decMatrix &matrix, float farDistance, float angleX, float angleY) const;
	
	/** Retrieves the minimum and maximum extend of the given volumes. */
	void GetVolumeExtends(const decConvexVolumeList &volumeList, decVector &minExtend, decVector &maxExtend) const;
	/**
	 * Retrieves the minimum and maximum extend of the given transformed volumes. The minExtend and maxExtend
	 * are kept untouched if the volume list contains no vertices.
	 */
	void GetTransformedVolumeExtends(const decConvexVolumeList &volumeList, const decDMatrix &matrix, decDVector &minExtend, decDVector &maxExtend) const;
	
	/**
	 * Crops a volume list using a triangles list. The triangles are extruded radially from the origin
	 * blocked by obstacles. The distance indicates how far triangles are extruded and should be at
	 * least the radius of the sphere enclosing the hull boundary box.
	 */
	void CropByTriangles(decConvexVolumeList &volumeList, const deoglTriangleSorter &triangles,
	const decVector &origin, float distance) const;
	/**
	 * Crops the volume list using an occlusion mesh. The faces are extruded radially from the origin
	 * blocked by obstacles. The distance indicates how far faces are extruded and should be at least
	 * the radius of the sphere enclosing the volumes boundary box.
	 */
	void CropByOcclusionMesh(decConvexVolumeList &volumeList, const deoglROcclusionMesh &occlusionMesh,
	const decMatrix &matrix, const decVector &origin, float distance) const;
	/**
	 * Crops the volume list using a bounding box. The faces of the bounding box are used as clip planes
	 * cropping anything outside the bounding box. The matrix is used to transform the bounding box into
	 * the volume coordinate frame before cropping.
	 */
	void CropByBoundingBox(decConvexVolumeList &volumeList, const decDMatrix &matrix,
	const decDVector &minExtend, const decDVector &maxExtend) const;
	/*@}*/
	
private:
	void pAddTriangle(decConvexVolume *volume, int p1, int p2, int p3) const;
	void pAddTriangle(decConvexVolume *volume, int p1, int p2, int p3, const decVector &normal) const;
	void pAddQuad(decConvexVolume *volume, int p1, int p2, int p3, int p4) const;
	void pAddQuad(decConvexVolume *volume, int p1, int p2, int p3, int p4, const decVector &normal) const;
};

#endif
