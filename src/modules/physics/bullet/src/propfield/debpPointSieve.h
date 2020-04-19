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
#ifndef _DEBPPOINTSIEVE_H_
#define _DEBPPOINTSIEVE_H_

// includes

// predefinitions
class debpPointSieveBucket;



/**
 * @brief Point Sieve.
 *
 * A simple sieve to sort points into a regular grid.
 */
class debpPointSieve{
private:
	float pSieveSizeX;
	float pSieveSizeZ;
	float pSieveLeft;
	float pSieveTop;
	float pSieveDivX;
	float pSieveDivZ;
	
	debpPointSieveBucket *pBuckets;
	int pBucketCountX;
	int pBucketCountZ;
	int pBucketCount;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new sieve. */
	debpPointSieve( int bucketCountX, int bucketCountZ, float sieveSizeX, float sieveSizeZ );
	/** Cleans up the sieve. */
	~debpPointSieve();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the sieve size in x direction. */
	inline float GetSieveSizeX() const{ return pSieveSizeX; }
	/** Retrieves the sieve size in z direction. */
	inline float GetSieveSizeZ() const{ return pSieveSizeZ; }
	
	/** Retrieves the number of buckets in the x direction. */
	inline int GetBucketCountX() const{ return pBucketCountX; }
	/** Retrieves the number of buckets in the z direction. */
	inline int GetBucketCountZ() const{ return pBucketCountZ; }
	/** Retrieves the number of buckets. */
	inline int GetBucketCount() const{ return pBucketCount; }
	/** Retrieves the bucket at the given index. */
	debpPointSieveBucket &GetBucketAt( int index ) const;
	/** Retrieves the bucket at the given index. */
	debpPointSieveBucket &GetBucketWith( int x, int z ) const;
	/** Retrieves the bucket containing the given position. */
	debpPointSieveBucket &GetBucketContaining( float x, float z ) const;
	
	/** Drops a point into the sieve. */
	void DropPoint( float x, float z, int index );
	
	/** Clears the sieve. */
	void Clear();
	/*@}*/
};

// end of include only once
#endif
