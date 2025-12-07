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
#ifndef _DEOGLOPTIMIZER_H_
#define _DEOGLOPTIMIZER_H_



/**
 * Optimizer.
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
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new optimizer. */
	deoglOptimizer();
	/** Cleans up the optimizer. */
	virtual ~deoglOptimizer();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the remaining time for this optimizer in micro-seconds. */
	inline int GetRemainingTime() const{return pRemainingTime;}
	/** Decreases the remaining time by the given amount of micro-seconds. */
	void DecreaseRemainingTime(int microSeconds);
	/** Resets the remaining Time to the maximal time. */
	void ResetRemainingTime();
	/** Retrieves the maximum time int micro-seconds. */
	inline int GetMaximumTime() const{return pMaxTime;}
	/** Retrieves the priority. */
	inline int GetPriority() const{return pPriority;}
	/** Sets the priority. */
	void SetPriority(int priority);
	/**
	 * Runs optimizer until the given time slice in micro-seconds has elapsed.
	 * Returns true if the optimizer requires more time to finish. If false
	 * is returned the optimizer has finished running and will be removed.
	 */
	virtual bool Run(int timeSlice);
	/*@}*/
};

// end of include only once
#endif
