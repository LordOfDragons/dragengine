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

// include only once
#ifndef _DEBPDECALMESHBUILDER_H_
#define _DEBPDECALMESHBUILDER_H_

// includes
#include <dragengine/common/math/decMath.h>

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
	void Init(deDecal *decal, float distance);
	/** Cut using the given face. */
	void CutTriangle(const decVector &v1, const decVector &v2, const decVector &v3);
	/** Builds mesh for a component. */
	void BuildMeshForComponent(debpComponent *bpComponent);
	/** Debug. */
	void Debug(dePhysicsBullet &module);
	
	/** Retrieves the number of points. */
	inline int GetPointCount() const{ return pPointCount; }
	/** Retrieves the point at the given index. */
	const decVector &GetPointAt(int index) const;
	/**
	 * Adds a point and returns the index. If the point already exists only
	 * the index is returned.
	 */
	int AddPoint(const decVector &point);
	/** Removes all points. */
	void RemoveAllPoints();
	
	/** Retrieves the number of faces. */
	inline int GetFaceCount() const{ return pFaceCount; }
	/** Retrieves the face at the given index. */
	debpDecalMeshBuilderFace *GetFaceAt(int index) const;
	/** Adds a new face. */
	debpDecalMeshBuilderFace *AddFace();
	/** Removes all faces. */
	void RemoveAllFaces();
	/*@}*/
	
private:
	void pVolumeAddFace(decConvexVolume *volume, int p1, int p2, int p3, const decVector &normal, bool decal);
	void pVolumeAddFace(decConvexVolume *volume, int p1, int p2, int p3, int p4, const decVector &normal, bool decal);
	int pIndexOfPoint(const decVector &point) const;
};

// end of include only once
#endif
