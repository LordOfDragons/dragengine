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

#ifndef _DEOGLVCOPTIMIZER_H_
#define _DEOGLVCOPTIMIZER_H_

class deoglVCOFace;
class deoglVCOVertex;
class deoglRenderThread;



/**
 * Vertex Cache Optimizer.
 * 
 * Based on http://home.comcast.net/~tom_forsyth/papers/fast_vert_cache_opt.html .
 */
class deoglVCOptimizer{
public:
	struct sVertex{
		bool consumed;
		int cachePosition;
		float score;
		int firstFace;
		int faceCount;
		int reorderedIndex;
	};
	
	struct sFace{
		bool consumed;
		float score;
		int vertices[ 3 ];
	};
	
	struct sGroup{
		int firstFace;
		int faceCount;
	};
	
private:
	float pCacheDecayPower;
	float pLastFaceScore;
	float pUsageBoostScale;
	float pUsageBoostPower;
	
	int *pCache;
	int pCacheSize;
	float pInvCacheFactor;
	
	sGroup *pGroups;
	sFace *pFaces;
	sVertex *pVertices;
	int *pVertexFaces;
	int *pReorderedVertices;
	int *pReorderedFaces;
	int pGroupCount;
	int pFaceCount;
	int pVertexCount;
	int pReorderedVertexCount;
	int pReorderedFaceCount;
	int pNextFaceCheck;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new vertex cache optimizer. */
	deoglVCOptimizer();
	/** Cleans up the vertex cache optimizer. */
	~deoglVCOptimizer();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of vertices. */
	inline int GetVertexCount() const{ return pVertexCount; }
	/** Retrieves the vertices. */
	inline const sVertex *GetVertices() const{ return pVertices; }
	/** Retrieves the indices of the reordered vertices. */
	inline const int *GetReorderedVertices() const{ return pReorderedVertices; }
	/** Sets the number of vertices. This resets the vertices array. */
	void SetVertexCount( int vertexCount );
	
	/** Retrieves the number of faces. */
	inline int GetFaceCount() const{ return pFaceCount; }
	/** Retrieves the indices of the reordered faces. */
	inline const int *GetReorderedFaces() const{ return pReorderedFaces; }
	/** Retrieves the faces. */
	inline const sFace *GetFaces() const{ return pFaces; }
	/**
	 * Sets the number of faces. After this call the faces in the array are undefined. No initialization
	 * is done for speed reason. Make sure you set all faces using SetFaceAt.
	 */
	void SetFaceCount( int faceCount );
	/** Sets a face. */
	void SetFaceAt( int face, int vertex1, int vertex2, int vertex3 );
	
	/** Retrieves the number of groups. */
	inline int GetGroupCount() const{ return pGroupCount; }
	/**
	 * Sets the group count. After this call the groups in the array are undefined. No initialization
	 * is done for speed reason. Make sure you set all groups using SetGroupAt.
	 */
	void SetGroupCount( int groupCount );
	/** Sets a group. */
	void SetGroupAt( int group, int firstFace, int faceCount );
	
	/** Optimize for good vertex cache usage. */
	void Optimize();
	
	/** Optimize the face order. */
	void OptimizeFaceOrder();
	/** Optimize the vertex order. */
	void OptimizeVertexOrder();
	
	/** Calculate vertex score. */
	void CalculateVertexScore( int vertex );
	/** Calculate face score. */
	void CalculateFaceScore( int face );
	/** Add vertex to the top of the cache. If the vertex is in the cache already it is moved. */
	void AddVertexToCache( int vertex );
	/** Update scores for all vertices in the cache as well as all faces using vertices in the cache. */
	void UpdateCacheScores();
	/** Find the face using a vertex in the cache with the highest score. */
	int FindBestScoreFaceInCache();
	/** Find the face with the highest score using a full exhaustive search. */
	int FindBestScoreFace( int firstFace, int faceLimit );
	/** Find the next face that is not consumed. */
	int FindNextFace( int faceLimit );
	
	/** Debug print. */
	void DebugPrint( deoglRenderThread &renderThread );
	/*@}*/
};

#endif
