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

#ifndef _IGDESHAPEBUILDER_H_
#define _IGDESHAPEBUILDER_H_

#include "dragengine/common/math/decMath.h"

// predefinitions
class decConvexVolumeList;
class decConvexVolume;
class deDebugDrawerShape;
class igdeWDebugDrawerShape;



/**
 * \brief Builds convex volumes.
 * Helper class to build various convex volumes.
 */
class DE_DLL_EXPORT igdeShapeBuilder{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create convex volume builder. */
	igdeShapeBuilder();
	
	/** \brief Clean up convex volume builder. */
	~igdeShapeBuilder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create sphere volume. */
	void CreateSphere( decConvexVolumeList &list, const decVector &center, float radius ) const;
	
	/** \brief Create frustum volume. */
	void CreateFrustum( decConvexVolumeList &list, const decVector &position,
		const decQuaternion &orientation, float sizeX, float sizeY, float angleX,
		float angleY, float distance ) const;
	
	/** \brief Creates an arrow volume. */
	void CreateArrow( decConvexVolumeList &list, const decVector &from,
		const decVector &to, float headRadius, float headLength, float tailRadius ) const;
	
	/** \brief Creates an arrow head volume. */
	void CreateArrowHead( decConvexVolumeList &list, const decVector &from,
		const decVector &to, float headRadius ) const;
	
	/** \brief Converts a convex volume list into a debug drawer volume. */
	void Convert( decConvexVolumeList &cvList, deDebugDrawerShape &ddVolume ) const;
	
	/** \brief Converts a convex volume list into a debug drawer volume. */
	void Convert( decConvexVolumeList &cvList, igdeWDebugDrawerShape &ddShape ) const;
	
	/** \brief Create sphere volume. */
	void CreateSphere( deDebugDrawerShape &ddVolume, const decVector &center, float radius ) const;
	
	/** \brief Create frustum volume. */
	void CreateFrustum( deDebugDrawerShape &ddVolume, const decVector &position,
		const decQuaternion &orientation, float sizeX, float sizeY, float angleX,
		float angleY, float distance ) const;
	
	/** \brief Creates an arrow volume. */
	void CreateArrow( deDebugDrawerShape &ddVolume, const decVector &from,
		const decVector &to, float headRadius, float headLength, float tailRadius ) const;
	
	/** \brief Creates an arrow head volume. */
	void CreateArrowHead( deDebugDrawerShape &ddVolume, const decVector &from,
		const decVector &to, float headRadius ) const;
	
	/** \brief Create sphere shape. */
	void CreateSphere( igdeWDebugDrawerShape &ddShape, const decVector &center,
		float radius ) const;
	
	/** \brief Create box shape. */
	void CreateBox( igdeWDebugDrawerShape &ddShape, const decVector &center,
		const decVector &halfExtends ) const;
	
	/** \brief Create box shape. */
	void CreateBoxFromExtends( igdeWDebugDrawerShape &ddShape, const decVector &lower,
		const decVector &upper ) const;
	
	/** \brief Create frustum shape. */
	void CreateFrustum( igdeWDebugDrawerShape &ddShape, const decVector &position,
		const decQuaternion &orientation, float sizeX, float sizeY, float angleX,
		float angleY, float distance ) const;
	
	/** \brief Creates an arrow shape. */
	void CreateArrow( igdeWDebugDrawerShape &ddShape, const decVector &from,
		const decVector &to, float headRadius, float headLength, float tailRadius ) const;
	
	/** \brief Creates an arrow head shape. */
	void CreateArrowHead( igdeWDebugDrawerShape &ddShape, const decVector &from,
		const decVector &to, float headRadius ) const;
	
	/** \brief Create quadrilateral shape. */
	void CreateQuad( igdeWDebugDrawerShape &ddShape, const decVector &p1,
		const decVector &p2, const decVector &p3, const decVector &p4,
		const decVector &normal ) const;
	
	/** \brief Adds a triangular face to a shape. */
	void AddTriToShape( igdeWDebugDrawerShape &ddShape, const decVector &p1,
		const decVector &p2, const decVector &p3 ) const;
	
	/** \brief Adds a triangular face to a shape. */
	void AddTriToShape( igdeWDebugDrawerShape &ddShape, const decVector &p1,
		const decVector &p2, const decVector &p3, const decVector &normal ) const;
	
	/** \brief Adds a quadrilateral face to a shape. */
	void AddQuadToShape( igdeWDebugDrawerShape &ddShape, const decVector &p1,
		const decVector &p2, const decVector &p3, const decVector &p4 ) const;
	
	/** \brief Adds a quadrilateral face to a shape. */
	void AddQuadToShape( igdeWDebugDrawerShape &ddShape, const decVector &p1,
		const decVector &p2, const decVector &p3, const decVector &p4,
		const decVector &normal ) const;
	/*@}*/
	
	
	
private:
	void pVolumeAddFace( decConvexVolume *volume, int p1, int p2, int p3, const decVector &normal ) const;
	void pVolumeAddFace( decConvexVolume *volume, int p1, int p2, int p3, int p4, const decVector &normal ) const;
	void pVolumeAddFace( decConvexVolume *volume, int p1, int p2, int p3 ) const;
};

#endif
