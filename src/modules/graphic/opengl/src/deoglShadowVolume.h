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
