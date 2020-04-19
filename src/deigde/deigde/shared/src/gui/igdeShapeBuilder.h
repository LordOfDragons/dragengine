/* 
 * Drag[en]gine IGDE
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
class igdeShapeBuilder{
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
