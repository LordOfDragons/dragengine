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

// include only once
#ifndef _DEOGLOPTIMIZERMANAGER_H_
#define _DEOGLOPTIMIZERMANAGER_H_

// predefinitions
class deoglOptimizer;



/**
 * Optimizer Manager.
 * Manages optmimizers. Each optimizer is able to optimize progessively
 * an engine object over time. The Optimizer Manager stores a list of
 * optimizers for different priorities. Every frame the optimizer can
 * be assigned a time slice which is used to run one or more optimizers.
 */
class deoglOptimizerManager{
private:
	deoglOptimizer **pOptimizers;
	int pOptimizerCount, pOptimizerSize;
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
	/** Retrieves the number of optimizers. */
	inline int GetOptimizerCount() const{ return pOptimizerCount; }
	/** Retrieves the optimizer at the given index. */
	deoglOptimizer *GetOptimizerAt( int index ) const;
	/** Determines if the given optimizer exists. */
	bool HasOptimizer( deoglOptimizer *optimizer ) const;
	/** Determines the index of the given optimizer or -1 if not found. */
	int IndexOfOptimizer( deoglOptimizer *optimizer ) const;
	/** Adds an optimizer. */
	void AddOptimizer( deoglOptimizer *optimizer );
	/** Removes the given optimizer. */
	void RemoveOptimizer( deoglOptimizer *optimizer );
	/** Removes all optimizers. */
	void RemoveAllOptimizers();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Runs optimizers until the given time slice in micro-seconds has elapsed. */
	void Run( int timeSlice );
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
