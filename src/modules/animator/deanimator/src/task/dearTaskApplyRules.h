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

#ifndef _DEARTASKAPPLYRULES_H_
#define _DEARTASKAPPLYRULES_H_

#include <dragengine/parallel/deParallelTask.h>

class dearAnimatorInstance;



/**
 * Task applying rules to an animator instance state.
 */
class dearTaskApplyRules : public deParallelTask{
private:
	dearAnimatorInstance &pInstance;
	bool pDropped;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create task. */
	dearTaskApplyRules( dearAnimatorInstance &instance );
	
	/** Clean up task. */
	virtual ~dearTaskApplyRules();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Animator instance owning this task. */
	inline dearAnimatorInstance &GetInstance() const{ return pInstance; }
	
	/** Instance dropped task. */
	void Drop();
	
	/** Parallel task implementation. */
	virtual void Run();
	
	/**
	 * Synchronous processing of task Run() finished.
	 * \details Called after the task has finished and has been collected by
	 *          deParallelProcessing::Update(). This call runs synchronously in the
	 *          main engine thread. You have to check using IsCancelled() if the
	 *          task has been cancelled or finished successfully. Finished() will
	 *          be called in all circumstances to allow proper cleaning up.
	 */
	virtual void Finished();
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** Short task name for debugging. */
	virtual decString GetDebugName() const;
	
	/** Task details for debugging. */
	virtual decString GetDebugDetails() const;
	/*@}*/
};

#endif
