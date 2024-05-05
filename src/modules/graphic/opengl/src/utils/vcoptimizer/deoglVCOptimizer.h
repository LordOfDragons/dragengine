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
