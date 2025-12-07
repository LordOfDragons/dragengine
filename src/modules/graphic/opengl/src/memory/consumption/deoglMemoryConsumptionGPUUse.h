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
	deoglMemoryConsumptionGPUUse(deoglMemoryConsumptionGPU &tracker);
	
	/** Clean up vbo memory consumption. */
	~deoglMemoryConsumptionGPUUse();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Get memory consumption. */
	inline unsigned long long GetConsumption() const{return pConsumption;}
	
	/** Consumption in KB. */
	inline unsigned long long GetConsumptionKB() const{return pConsumption / 1024ull;}
	
	/** Consumption in MB. */
	inline unsigned int GetConsumptionMB() const{return (unsigned int)(pConsumption / 1024000ull);}
	
	/** Consumption in GB. */
	inline unsigned int GetConsumptionGB() const{return (unsigned int)(pConsumption / 1024000000ull);}
	
	/** Consumption in KB as double. */
	inline double GetConsumptionKBDouble() const{return (double)pConsumption / 1024e0;}
	
	/** Consumption in MB as double. */
	inline double GetConsumptionMBDouble() const{return (double)pConsumption / 1024e3;}
	
	/** Consumption in GB as double. */
	inline double GetConsumptionGBDouble() const{return (double)pConsumption / 1024e6;}
	
	/** Set memory consumption. */
	void SetConsumption(unsigned long long consumption);
	
	/** Increment memory consumption. */
	void IncrementConsumption(unsigned long long consumption);
	
	/** Decrement memory consumption. */
	void DecrementConsumption(unsigned long long consumption);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Get memory consumption. */
	inline operator unsigned long long() const{return pConsumption;}
	
	/** Set memory consumption. */
	deoglMemoryConsumptionGPUUse &operator=(unsigned long long consumption);
	
	/** Increment memory consumption. */
	deoglMemoryConsumptionGPUUse &operator+=(unsigned long long consumption);
	
	/** Decrement memory consumption. */
	deoglMemoryConsumptionGPUUse &operator-=(unsigned long long consumption);
	/*@}*/
};

#endif
