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
 * OpenGL Decal Mesh Builder.
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
	/** Create decal mesh builder. */
	deoglDecalMeshBuilder( deoglRenderThread &renderThread );
	
	/** Clean up decal mesh builder. */
	~deoglDecalMeshBuilder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Initializes the builder. */
	void Init( const deoglRDecal &decals, float distance );
	
	/** Cut using the given face. */
	void CutTriangle( const decVector &v1, const decVector &v2, const decVector &v3 );
	
	/** Build mesh for component. */
	void BuildMeshForComponent( const deoglRComponent &oglComponent );
	void BuildMeshForComponent( const deoglRComponentLOD &lod );
	
	/** Debug. */
	void Debug();
	
	
	
	/** Number of points. */
	inline int GetPointCount() const{ return pPointCount; }
	
	/** Point at index. */
	const decVector &GetPointAt( int index ) const;
	
	/**
	 * Add point and return index.
	 * 
	 * If the point already exists only the index is returned.
	 */
	int AddPoint( const decVector &point );
	
	/** Remove all points. */
	void RemoveAllPoints();
	
	
	
	/** Number of faces. */
	inline int GetFaceCount() const{ return pFaceCount; }
	
	/** Face at index. */
	deoglDecalMeshBuilderFace *GetFaceAt( int index ) const;
	
	/** Add face. */
	deoglDecalMeshBuilderFace *AddFace();
	
	/** Remove all faces. */
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
