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
#ifndef _DEOGLOPTIMIZER_H_
#define _DEOGLOPTIMIZER_H_



/**
 * @brief Optimizer.
 * Optimizers are able to optimize progessively an engine object over time.
 */
class deoglOptimizer{
public:
	enum ePriorities{
		epLowest,
		epLower,
		epNormal,
		epHigher,
		epHighest
	};
	
private:
	int pRemainingTime;
	int pMaxTime;
	int pPriority;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new optimizer. */
	deoglOptimizer();
	/** Cleans up the optimizer. */
	virtual ~deoglOptimizer();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the remaining time for this optimizer in micro-seconds. */
	inline int GetRemainingTime() const{ return pRemainingTime; }
	/** Decreases the remaining time by the given amount of micro-seconds. */
	void DecreaseRemainingTime( int microSeconds );
	/** Resets the remaining Time to the maximal time. */
	void ResetRemainingTime();
	/** Retrieves the maximum time int micro-seconds. */
	inline int GetMaximumTime() const{ return pMaxTime; }
	/** Retrieves the priority. */
	inline int GetPriority() const{ return pPriority; }
	/** Sets the priority. */
	void SetPriority( int priority );
	/**
	 * Runs optimizer until the given time slice in micro-seconds has elapsed.
	 * Returns true if the optimizer requires more time to finish. If false
	 * is returned the optimizer has finished running and will be removed.
	 */
	virtual bool Run( int timeSlice );
	/*@}*/
};

// end of include only once
#endif
