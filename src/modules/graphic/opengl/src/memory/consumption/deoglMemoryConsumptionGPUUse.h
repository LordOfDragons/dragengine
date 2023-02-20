/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLMEMORYCONSUMPTIONGPUUSE_H_
#define _DEOGLMEMORYCONSUMPTIONGPUUSE_H_

class deoglMemoryConsumptionGPU;


/**
 * GPU memory consumption usage. Automatically updates deoglMemoryConsumptionGPU.
 */
class deoglMemoryConsumptionGPUUse{
private:
	deoglMemoryConsumptionGPU &pTracker;
	unsigned long long pConsumption;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create memory consumption usage. */
	deoglMemoryConsumptionGPUUse( deoglMemoryConsumptionGPU &tracker );
	
	/** Clean up vbo memory consumption. */
	~deoglMemoryConsumptionGPUUse();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Get memory consumption. */
	inline unsigned long long GetConsumption() const{ return pConsumption; }
	
	/** Consumption in KB. */
	inline unsigned long long GetConsumptionKB() const{ return pConsumption / 1024ull; }
	
	/** Consumption in MB. */
	inline unsigned int GetConsumptionMB() const{ return ( unsigned int )( pConsumption / 1024000ull ); }
	
	/** Consumption in GB. */
	inline unsigned int GetConsumptionGB() const{ return ( unsigned int )( pConsumption / 1024000000ull ); }
	
	/** Consumption in KB as double. */
	inline double GetConsumptionKBDouble() const{ return ( double )pConsumption / 1024e0; }
	
	/** Consumption in MB as double. */
	inline double GetConsumptionMBDouble() const{ return ( double )pConsumption / 1024e3; }
	
	/** Consumption in GB as double. */
	inline double GetConsumptionGBDouble() const{ return ( double )pConsumption / 1024e6; }
	
	/** Set memory consumption. */
	void SetConsumption( unsigned long long consumption );
	
	/** Increment memory consumption. */
	void IncrementConsumption( unsigned long long consumption );
	
	/** Decrement memory consumption. */
	void DecrementConsumption( unsigned long long consumption );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Get memory consumption. */
	inline operator unsigned long long() const{ return pConsumption; }
	
	/** Set memory consumption. */
	deoglMemoryConsumptionGPUUse &operator=( unsigned long long consumption );
	
	/** Increment memory consumption. */
	deoglMemoryConsumptionGPUUse &operator+=( unsigned long long consumption );
	
	/** Decrement memory consumption. */
	deoglMemoryConsumptionGPUUse &operator-=( unsigned long long consumption );
	/*@}*/
};

#endif
