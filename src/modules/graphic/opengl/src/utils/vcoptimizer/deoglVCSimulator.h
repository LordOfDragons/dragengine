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

#ifndef _DEOGLVCSIMULATOR_H_
#define _DEOGLVCSIMULATOR_H_



/**
 * @brief Vertex Cache Simulator.
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
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new vertex cache simulator. */
	deoglVCSimulator();
	/** Cleans up the vertex cache simulator. */
	~deoglVCSimulator();
	/*@}*/
	
	/** @name Management */
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
