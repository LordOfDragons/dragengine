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

#ifndef _DEOGLVCSIMULATOR_H_
#define _DEOGLVCSIMULATOR_H_



/**
 * Vertex Cache Simulator.
 * Simulates a FIFO type cache of fixed size.
 */
class deoglVCSimulator{
private:
	int *pCache;
	int pCacheSize;
	int pCacheHitCount;
	int pCacheMissCount;
	int pFaceCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new vertex cache simulator. */
	deoglVCSimulator();
	/** Cleans up the vertex cache simulator. */
	~deoglVCSimulator();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the cache size. */
	inline int GetCacheSize() const{ return pCacheSize; }
	/** Sets the cache size to simulate. */
	void SetCacheSize( int cacheSize );
	
	/** Resets the simulator. */
	void Reset();
	/** Process a face composed of three vertex indices. */
	void ProcessFace( int vertex1, int vertex2, int vertex3 );
	
	/** Retrieves the number of cache hits. */
	inline int GetCacheHitCount() const{ return pCacheHitCount; }
	/** Retrieves the number of cache misses. */
	inline int GetCacheMissCount() const{ return pCacheMissCount; }
	/** Retrieves the number of processed faces. */
	inline int GetFaceCount() const{ return pFaceCount; }
	/** Calculates the average cache miss ratio. */
	float GetAvgCacheMissRatio() const;
	
	/**
	 * Test vertex against the cache. If the vertex exists the cache is left unchanged. If
	 * the vertex does not exist in the cache the content of the cache is shifted by one
	 * removing the pushed out element and the new vertex is added. If the vertex has been
	 * in the cache the cache hit count is incremented by one. Otherwise the cache miss
	 * count is incremented by one.
	 */
	void TestVertexAgainstCache( int vertex );
	/*@}*/
};

#endif
