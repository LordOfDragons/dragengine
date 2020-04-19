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

#ifndef _DEOGLMEMORYCONSUMPTIONTEXTURE_H_
#define _DEOGLMEMORYCONSUMPTIONTEXTURE_H_

#include <dragengine/common/math/decMath.h>



/**
 * @brief Memory Consumption Texture.
 * Memory consumption of textures.
 */
class deoglMemoryConsumptionTexture{
private:
	int pCount;
	unsigned int pGPU;
	unsigned int pGPUCompressed;
	unsigned int pGPUUncompressed;
	
	int pColorCount;
	unsigned int pColorGPU;
	unsigned int pColorGPUCompressed;
	unsigned int pColorGPUUncompressed;
	
	int pDepthCount;
	unsigned int pDepthGPU;
	unsigned int pDepthGPUCompressed;
	unsigned int pDepthGPUUncompressed;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new texture memory consumption. */
	deoglMemoryConsumptionTexture();
	/** Cleans up the texture memory consumption. */
	~deoglMemoryConsumptionTexture();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the total texture count. */
	inline int GetCount() const{ return pCount; }
	/** Increments the total texture count by 1. */
	inline void IncrementCount(){ pCount++; }
	/** Increments the total texture count by 1. */
	inline void IncrementCountBy( int count ){ pCount += count; }
	/** Decrements the total texture count by 1. */
	inline void DecrementCount(){ pCount--; }
	/** Decrements the total texture count by 1. */
	inline void DecrementCountBy( int count ){ pCount -= count; }
	/** Retrieves the total texture gpu memory. */
	inline unsigned int GetGPU() const{ return pGPU; }
	/** Increments the total texture gpu memory. */
	inline void IncrementGPU( int bytes ){ pGPU += bytes; }
	/** Decrements the total texture gpu memory. */
	inline void DecrementGPU( int bytes ){ pGPU -= bytes; }
	/** Retrieves the total texture gpu compressed memory. */
	inline unsigned int GetGPUCompressed() const{ return pGPUCompressed; }
	/** Increments the total texture gpu compressed memory. */
	inline void IncrementGPUCompressed( int bytes ){ pGPUCompressed += bytes; }
	/** Decrements the total texture gpu compressed memory. */
	inline void DecrementGPUCompressed( int bytes ){ pGPUCompressed -= bytes; }
	/** Retrieves the total texture gpu uncompressed memory. */
	inline unsigned int GetGPUUncompressed() const{ return pGPUUncompressed; }
	/** Increments the total texture gpu uncompressed memory. */
	inline void IncrementGPUUncompressed( int bytes ){ pGPUUncompressed += bytes; }
	/** Decrements the total texture gpu uncompressed memory. */
	inline void DecrementGPUUncompressed( int bytes ){ pGPUUncompressed -= bytes; }
	
	/** Retrieves the total texture color count. */
	inline int GetColorCount() const{ return pColorCount; }
	/** Increments the total texture color count by 1. */
	inline void IncrementColorCount(){ pColorCount++; }
	/** Decrements the total texture color count by 1. */
	inline void DecrementColorCount(){ pColorCount--; }
	/** Retrieves the total texture color gpu memory. */
	inline unsigned int GetColorGPU() const{ return pColorGPU; }
	/** Increments the total texture color gpu memory. */
	inline void IncrementColorGPU( int bytes ){ pColorGPU += bytes; }
	/** Decrements the total texture color gpu memory. */
	inline void DecrementColorGPU( int bytes ){ pColorGPU -= bytes; }
	/** Retrieves the total texture color gpu compressed memory. */
	inline unsigned int GetColorGPUCompressed() const{ return pColorGPUCompressed; }
	/** Increments the total texture color gpu compressed memory. */
	inline void IncrementColorGPUCompressed( int bytes ){ pColorGPUCompressed += bytes; }
	/** Decrements the total texture color gpu compressed memory. */
	inline void DecrementColorGPUCompressed( int bytes ){ pColorGPUCompressed -= bytes; }
	/** Retrieves the total texture color gpu uncompressed memory. */
	inline unsigned int GetColorGPUUncompressed() const{ return pColorGPUUncompressed; }
	/** Increments the total texture color gpu uncompressed memory. */
	inline void IncrementColorGPUUncompressed( int bytes ){ pColorGPUUncompressed += bytes; }
	/** Decrements the total texture color gpu uncompressed memory. */
	inline void DecrementColorGPUUncompressed( int bytes ){ pColorGPUUncompressed -= bytes; }
	
	/** Retrieves the total texture depth count. */
	inline int GetDepthCount() const{ return pDepthCount; }
	/** Increments the total texture depth count by 1. */
	inline void IncrementDepthCount(){ pDepthCount++; }
	/** Decrements the total texture depth count by 1. */
	inline void DecrementDepthCount(){ pDepthCount--; }
	/** Retrieves the total texture depth gpu memory. */
	inline unsigned int GetDepthGPU() const{ return pDepthGPU; }
	/** Increments the total texture depth gpu memory. */
	inline void IncrementDepthGPU( int bytes ){ pDepthGPU += bytes; }
	/** Decrements the total texture depth gpu memory. */
	inline void DecrementDepthGPU( int bytes ){ pDepthGPU -= bytes; }
	/** Retrieves the total texture depth gpu compressed memory. */
	inline unsigned int GetDepthGPUCompressed() const{ return pDepthGPUCompressed; }
	/** Increments the total texture depth gpu compressed memory. */
	inline void IncrementDepthGPUCompressed( int bytes ){ pDepthGPUCompressed += bytes; }
	/** Decrements the total texture depth gpu compressed memory. */
	inline void DecrementDepthGPUCompressed( int bytes ){ pDepthGPUCompressed -= bytes; }
	/** Retrieves the total texture depth gpu uncompressed memory. */
	inline unsigned int GetDepthGPUUncompressed() const{ return pDepthGPUUncompressed; }
	/** Increments the total texture depth gpu uncompressed memory. */
	inline void IncrementDepthGPUUncompressed( int bytes ){ pDepthGPUUncompressed += bytes; }
	/** Decrements the total texture depth gpu uncompressed memory. */
	inline void DecrementDepthGPUUncompressed( int bytes ){ pDepthGPUUncompressed -= bytes; }
	/*@}*/
};

#endif
