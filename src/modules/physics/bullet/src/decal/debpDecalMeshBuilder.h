/* 
 * Drag[en]gine Bullet Physics Module
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

// include only once
#ifndef _DEBPDECALMESHBUILDER_H_
#define _DEBPDECALMESHBUILDER_H_

// includes
#include "dragengine/common/math/decMath.h"

// predefintions
class debpDMBConvexVolumeList;
class debpDMBConvexVolumeFace;
class debpDecalMeshBuilderFace;
class debpComponent;
class dePhysicsBullet;
class debpDCollisionBox;
class decConvexVolume;
class deDecal;



/**
 * @brief Bullet Decal Mesh Builder.
 * Helper class to build a decal mesh. The mesh is constructed using
 * a convex volume list. The starting volume is the cube formed by
 * the decal projected along the view direction. The triangles from
 * the underlaying surface are used to cut away invisible parts. In
 * the end a volume remains with the faces pointing towards the decal
 * 
 */
class debpDecalMeshBuilder{
public:
	debpDMBConvexVolumeList *pCVolList;
	decMatrix pDecalMatrix;
	decVector pDecalView;
	decVector pOrigin;
	decVector pProjVector;
	float pDistance;
	debpDCollisionBox *pDecalBox;
	
	decVector *pPoints;
	int pPointCount;
	int pPointSize;
	debpDecalMeshBuilderFace *pFaces;
	int pFaceCount;
	int pFaceSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new decal mesh builder. */
	debpDecalMeshBuilder();
	/** Cleans up the decal mesh builder. */
	~debpDecalMeshBuilder();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Initializes the builder. */
	void Init( deDecal *decal, float distance );
	/** Cut using the given face. */
	void CutTriangle( const decVector &v1, const decVector &v2, const decVector &v3 );
	/** Builds mesh for a component. */
	void BuildMeshForComponent( debpComponent *bpComponent );
	/** Debug. */
	void Debug( dePhysicsBullet &module );
	
	/** Retrieves the number of points. */
	inline int GetPointCount() const{ return pPointCount; }
	/** Retrieves the point at the given index. */
	const decVector &GetPointAt( int index ) const;
	/**
	 * Adds a point and returns the index. If the point already exists only
	 * the index is returned.
	 */
	int AddPoint( const decVector &point );
	/** Removes all points. */
	void RemoveAllPoints();
	
	/** Retrieves the number of faces. */
	inline int GetFaceCount() const{ return pFaceCount; }
	/** Retrieves the face at the given index. */
	debpDecalMeshBuilderFace *GetFaceAt( int index ) const;
	/** Adds a new face. */
	debpDecalMeshBuilderFace *AddFace();
	/** Removes all faces. */
	void RemoveAllFaces();
	/*@}*/
	
private:
	void pVolumeAddFace( decConvexVolume *volume, int p1, int p2, int p3, const decVector &normal, bool decal );
	void pVolumeAddFace( decConvexVolume *volume, int p1, int p2, int p3, int p4, const decVector &normal, bool decal );
	int pIndexOfPoint( const decVector &point ) const;
};

// end of include only once
#endif
