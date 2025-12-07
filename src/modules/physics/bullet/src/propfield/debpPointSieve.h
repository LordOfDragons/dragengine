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
	debpPointSieve(int bucketCountX, int bucketCountZ, float sieveSizeX, float sieveSizeZ);
	/** Cleans up the sieve. */
	~debpPointSieve();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the sieve size in x direction. */
	inline float GetSieveSizeX() const{return pSieveSizeX;}
	/** Retrieves the sieve size in z direction. */
	inline float GetSieveSizeZ() const{return pSieveSizeZ;}
	
	/** Retrieves the number of buckets in the x direction. */
	inline int GetBucketCountX() const{return pBucketCountX;}
	/** Retrieves the number of buckets in the z direction. */
	inline int GetBucketCountZ() const{return pBucketCountZ;}
	/** Retrieves the number of buckets. */
	inline int GetBucketCount() const{return pBucketCount;}
	/** Retrieves the bucket at the given index. */
	debpPointSieveBucket &GetBucketAt(int index) const;
	/** Retrieves the bucket at the given index. */
	debpPointSieveBucket &GetBucketWith(int x, int z) const;
	/** Retrieves the bucket containing the given position. */
	debpPointSieveBucket &GetBucketContaining(float x, float z) const;
	
	/** Drops a point into the sieve. */
	void DropPoint(float x, float z, int index);
	
	/** Clears the sieve. */
	void Clear();
	/*@}*/
};

// end of include only once
#endif
