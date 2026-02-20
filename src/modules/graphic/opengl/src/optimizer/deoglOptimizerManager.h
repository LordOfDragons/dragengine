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
#ifndef _DEOGLOPTIMIZERMANAGER_H_
#define _DEOGLOPTIMIZERMANAGER_H_

#include "deoglOptimizer.h"

#include <dragengine/common/collection/decTOrderedSet.h>



/**
 * Optimizer Manager.
 * Manages optmimizers. Each optimizer is able to optimize progessively
 * an engine object over time. The Optimizer Manager stores a list of
 * optimizers for different priorities. Every frame the optimizer can
 * be assigned a time slice which is used to run one or more optimizers.
 */
class deoglOptimizerManager{
private:
	decTObjectOrderedSet<deoglOptimizer> pOptimizers;
	int pNextOptimizer;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new optimizer manager. */
	deoglOptimizerManager();
	/** Cleans up the optimizer manager. */
	~deoglOptimizerManager();
	/*@}*/
	
	/** \name Optimizer Management */
	/*@{*/
	/** Optimizers. */
	inline const decTObjectOrderedSet<deoglOptimizer>& Optimizers() const{ return pOptimizers; }
	
	/** Adds an optimizer. */
	void AddOptimizer(deoglOptimizer *optimizer);

	/** Removes the given optimizer. */
	void RemoveOptimizer(deoglOptimizer *optimizer);

	/** Removes all optimizers. */
	void RemoveAllOptimizers();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Runs optimizers until the given time slice in micro-seconds has elapsed. */
	void Run(int timeSlice);
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
