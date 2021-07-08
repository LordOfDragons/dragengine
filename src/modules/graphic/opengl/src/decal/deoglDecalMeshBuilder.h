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

#ifndef _DEOGLDECALMESHBUILDER_H_
#define _DEOGLDECALMESHBUILDER_H_

#include "dragengine/common/math/decMath.h"

class deoglRDecal;
class deoglDMBConvexVolumeList;
class deoglDMBConvexVolumeFace;
class deoglDecalMeshBuilderFace;
class deoglRComponent;
class deoglRComponentLOD;
class deoglRenderThread;
class deoglDCollisionBox;
class decConvexVolume;



/**
 * \brief OpenGL Decal Mesh Builder.
 * 
 * Helper class to build a decal mesh. The mesh is constructed using a convex volume list.
 * The starting volume is the cube formed by the decal projected along the view direction.
 * The triangles from the underlaying surface are used to cut away invisible parts.
 * In the end a volume remains with the faces pointing towards the decal
 */
class deoglDecalMeshBuilder{
public:
	deoglRenderThread &pRenderThread;
	
	deoglDMBConvexVolumeList *pCVolList;
	decMatrix pDecalMatrix;
	decVector pDecalView;
	decVector pOrigin;
	decVector pProjVector;
	decVector pSize;
	float pDistance;
	deoglDCollisionBox *pDecalBox;
	
	decVector *pPoints;
	int pPointCount;
	int pPointSize;
	deoglDecalMeshBuilderFace *pFaces;
	int pFaceCount;
	int pFaceSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create decal mesh builder. */
	deoglDecalMeshBuilder( deoglRenderThread &renderThread );
	
	/** \brief Clean up decal mesh builder. */
	~deoglDecalMeshBuilder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Initializes the builder. */
	void Init( const deoglRDecal &decals, float distance );
	
	/** \brief Cut using the given face. */
	void CutTriangle( const decVector &v1, const decVector &v2, const decVector &v3 );
	
	/** \brief Build mesh for component. */
	void BuildMeshForComponent( const deoglRComponent &oglComponent );
	void BuildMeshForComponent( const deoglRComponentLOD &lod );
	
	/** \brief Debug. */
	void Debug();
	
	
	
	/** \brief Number of points. */
	inline int GetPointCount() const{ return pPointCount; }
	
	/** \brief Point at index. */
	const decVector &GetPointAt( int index ) const;
	
	/**
	 * \brief Add point and return index.
	 * 
	 * If the point already exists only the index is returned.
	 */
	int AddPoint( const decVector &point );
	
	/** \brief Remove all points. */
	void RemoveAllPoints();
	
	
	
	/** \brief Number of faces. */
	inline int GetFaceCount() const{ return pFaceCount; }
	
	/** \brief Face at index. */
	deoglDecalMeshBuilderFace *GetFaceAt( int index ) const;
	
	/** \brief Add face. */
	deoglDecalMeshBuilderFace *AddFace();
	
	/** \brief Remove all faces. */
	void RemoveAllFaces();
	/*@}*/
	
	
	
private:
	void pVolumeAddFace( decConvexVolume *volume, int p1, int p2, int p3,
		const decVector &normal, bool decalFace );
	void pVolumeAddFace( decConvexVolume *volume, int p1, int p2, int p3, int p4,
		const decVector &normal, bool decalFace );
	int pIndexOfPoint( const decVector &point ) const;
};

#endif
