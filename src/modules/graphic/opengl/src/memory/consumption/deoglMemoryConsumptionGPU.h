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
	deoglMemoryConsumptionGPU(const char *name);
	
	/** Create memory consumption updating also group memory consumption. */
	deoglMemoryConsumptionGPU(const char *name, deoglMemoryConsumptionGPU &group);
	
	/** Create memory consumption updating also group memory consumption. */
	deoglMemoryConsumptionGPU(const char *name, deoglMemoryConsumptionGPU &group,
		deoglMemoryConsumptionGPU &group2);
	
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
	inline unsigned int GetConsumptionMB() const{ return (unsigned int)(pConsumption / 1024000ull); }
	
	/** Consumption in GB. */
	inline unsigned int GetConsumptionGB() const{ return (unsigned int)(pConsumption / 1024000000ull); }
	
	/** Consumption in KB as double. */
	inline double GetConsumptionKBDouble() const{ return (double)pConsumption / 1024e0; }
	
	/** Consumption in MB as double. */
	inline double GetConsumptionMBDouble() const{ return (double)pConsumption / 1024e3; }
	
	/** Consumption in GB as double. */
	inline double GetConsumptionGBDouble() const{ return (double)pConsumption / 1024e6; }
	
	/** Increment consumption. */
	void IncrementConsumption(unsigned long long bytes);
	
	/** Decrement consumption. */
	void DecrementConsumption(unsigned long long bytes);
	/*@}*/
};

#endif
