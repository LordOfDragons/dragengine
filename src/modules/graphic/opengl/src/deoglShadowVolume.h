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

#if 0

// include only once
#ifndef _DEOGLSHADOWVOLUME_H_
#define _DEOGLSHADOWVOLUME_H_

// includes
#include "dragengine/common/math/decMath.h"

// predefinitions
class deoglMeshData;
class deoglRenderThread;


// opengl shadow volume class
class deoglShadowVolume{
public:
	struct sFace{
		int vertex[4];
		bool isOpen;
		bool isLit;
	};
	struct sEdge{
		int vertex[2];
		int face[2];
		bool isSilhouette;
	};
private:
	deoglRenderThread &pRenderThread;
	sFace *pFaces;
	int pFaceCount, pFaceSize;
	sEdge *pEdges;
	int pEdgeCount, pEdgeSize;
	deoglMeshData *pPointSource;
	bool pHasTris, pHasQuads;
public:
	// constructor, destructor
	deoglShadowVolume( deoglRenderThread &renderThread );
	~deoglShadowVolume();
	// management
	void Clear();
	void FindOpenFaces();
	void FindPointSilhouette(const decVector &lightPos, deoglMeshData *mesh);
	void FindDirectionalSilhouette(const decVector &lightDir, deoglMeshData *mesh);
	void RenderShadows(deoglMeshData *mesh, bool renderCaps);
	// faces
	inline int GetFaceCount() const{ return pFaceCount; }
	const sFace &GetFace(int index) const;
	void AddFace(int vertex1, int vertex2, int vertex3, int vertex4);
	int GetOpenFaceCount() const;
	void CloseVolume();
	// edges
	inline int GetEdgeCount() const{ return pEdgeCount; }
	const sEdge &GetEdge(int index) const;
private:
	void pAddFace(int vertex1, int vertex2, int vertex3, int vertex4);
	void pAddEdge(int face, int vertex1, int vertex2);
	int pFindEdge(int vertex1, int vertex2);
	void pFindEdgeSilhouette();
	void pRenderShadowPass(deoglMeshData *mesh, bool renderCaps);
};

// end of include only once
#endif

#endif
