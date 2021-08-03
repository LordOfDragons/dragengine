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

#ifndef _DEOGLMEMORYCONSUMPTIONGPU_H_
#define _DEOGLMEMORYCONSUMPTIONGPU_H_

#include <dragengine/common/string/decString.h>


/**
 * GPU memory consumption.
 */
class deoglMemoryConsumptionGPU{
private:
	const decString pName;
	int pCount;
	unsigned long long pConsumption;
	deoglMemoryConsumptionGPU *pGroup;
	deoglMemoryConsumptionGPU *pGroup2;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create memory consumption. */
	deoglMemoryConsumptionGPU( const char *name );
	
	/** Create memory consumption updating also group memory consumption. */
	deoglMemoryConsumptionGPU( const char *name, deoglMemoryConsumptionGPU &group );
	
	/** Create memory consumption updating also group memory consumption. */
	deoglMemoryConsumptionGPU( const char *name, deoglMemoryConsumptionGPU &group,
		deoglMemoryConsumptionGPU &group2 );
	
	/** Clean up vbo memory consumption. */
	~deoglMemoryConsumptionGPU();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	
	
	/** Count. */
	inline int GetCount() const{ return pCount; }
	
	/** Increment count by 1. */
	void IncrementCount();
	
	/** Decrement count by 1. */
	void DecrementCount();
	
	
	
	/** Consumption in bytes. */
	inline unsigned long long GetConsumption() const{ return pConsumption; }
	
	/** Consumption in KB. */
	inline unsigned long long GetConsumptionKB() const{ return pConsumption / 1024ull; }
	
	/** Consumption in MB. */
	inline unsigned int GetConsumptionMB() const{ return pConsumption / 1024000ull; }
	
	/** Consumption in GB. */
	inline unsigned int GetConsumptionGB() const{ return pConsumption / 1024000000ull; }
	
	/** Consumption in KB as double. */
	inline double GetConsumptionKBDouble() const{ return ( double )pConsumption / 1024e0; }
	
	/** Consumption in MB as double. */
	inline double GetConsumptionMBDouble() const{ return ( double )pConsumption / 1024e3; }
	
	/** Consumption in GB as double. */
	inline double GetConsumptionGBDouble() const{ return ( double )pConsumption / 1024e6; }
	
	/** Increment consumption. */
	void IncrementConsumption( unsigned long long bytes );
	
	/** Decrement consumption. */
	void DecrementConsumption( unsigned long long bytes );
	/*@}*/
};

#endif
