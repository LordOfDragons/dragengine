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

#ifndef _DEOGLMEMORYCONSUMPTIONVBO_H_
#define _DEOGLMEMORYCONSUMPTIONVBO_H_

#include <dragengine/common/math/decMath.h>



/**
 * @brief Memory Consumption VBO.
 * Memory consumption of vbos.
 */
class deoglMemoryConsumptionVBO{
private:
	int pCount;
	unsigned int pGPU;
	
	int pSharedCount;
	unsigned int pSharedGPU;
	
	int pIBOCount;
	unsigned int pIBOGPU;
	
	int pTBOCount;
	unsigned int pTBOGPU;
	
	int pUBOCount;
	unsigned int pUBOGPU;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new vbo memory consumption. */
	deoglMemoryConsumptionVBO();
	/** Cleans up the vbo memory consumption. */
	~deoglMemoryConsumptionVBO();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the vbo count. */
	inline int GetCount() const{ return pCount; }
	/** Increments the vbo count by 1. */
	inline void IncrementCount(){ pCount++; }
	/** Decrements the vbo count by 1. */
	inline void DecrementCount(){ pCount--; }
	/** Retrieves the vbo gpu memory. */
	inline unsigned int GetGPU() const{ return pGPU; }
	/** Increments the vbo gpu memory. */
	inline void IncrementGPU( int bytes ){ pGPU += bytes; }
	/** Decrements the vbo gpu memory. */
	inline void DecrementGPU( int bytes ){ pGPU -= bytes; }
	
	/** Retrieves the shared vbo count. */
	inline int GetSharedCount() const{ return pSharedCount; }
	/** Increments the shared vbo count by 1. */
	inline void IncrementSharedCount(){ pSharedCount++; }
	/** Decrements the shared vbo count by 1. */
	inline void DecrementSharedCount(){ pSharedCount--; }
	/** Retrieves the shared vbo gpu memory. */
	inline unsigned int GetSharedGPU() const{ return pSharedGPU; }
	/** Increments the shared vbo gpu memory. */
	inline void IncrementSharedGPU( int bytes ){ pSharedGPU += bytes; }
	/** Decrements the shared vbo gpu memory. */
	inline void DecrementSharedGPU( int bytes ){ pSharedGPU -= bytes; }
	
	/** Retrieves the ibo vbo count. */
	inline int GetIBOCount() const{ return pIBOCount; }
	/** Increments the ibo vbo count by 1. */
	inline void IncrementIBOCount(){ pIBOCount++; }
	/** Decrements the ibo vbo count by 1. */
	inline void DecrementIBOCount(){ pIBOCount--; }
	/** Retrieves the ibo vbo gpu memory. */
	inline unsigned int GetIBOGPU() const{ return pIBOGPU; }
	/** Increments the ibo vbo gpu memory. */
	inline void IncrementIBOGPU( int bytes ){ pIBOGPU += bytes; }
	/** Decrements the ibo vbo gpu memory. */
	inline void DecrementIBOGPU( int bytes ){ pIBOGPU -= bytes; }
	
	/** Retrieves the tbo vbo count. */
	inline int GetTBOCount() const{ return pTBOCount; }
	/** Increments the tbo vbo count by 1. */
	inline void IncrementTBOCount(){ pTBOCount++; }
	/** Decrements the tbo vbo count by 1. */
	inline void DecrementTBOCount(){ pTBOCount--; }
	/** Retrieves the tbo vbo gpu memory. */
	inline unsigned int GetTBOGPU() const{ return pTBOGPU; }
	/** Increments the tbo vbo gpu memory. */
	inline void IncrementTBOGPU( int bytes ){ pTBOGPU += bytes; }
	/** Decrements the tbo vbo gpu memory. */
	inline void DecrementTBOGPU( int bytes ){ pTBOGPU -= bytes; }
	
	/** Retrieves the ubo vbo count. */
	inline int GetUBOCount() const{ return pUBOCount; }
	/** Increments the ubo vbo count by 1. */
	inline void IncrementUBOCount(){ pUBOCount++; }
	/** Decrements the ubo vbo count by 1. */
	inline void DecrementUBOCount(){ pUBOCount--; }
	/** Retrieves the ubo vbo gpu memory. */
	inline unsigned int GetUBOGPU() const{ return pUBOGPU; }
	/** Increments the ubo vbo gpu memory. */
	inline void IncrementUBOGPU( int bytes ){ pUBOGPU += bytes; }
	/** Decrements the ubo vbo gpu memory. */
	inline void DecrementUBOGPU( int bytes ){ pUBOGPU -= bytes; }
	/*@}*/
};

#endif
