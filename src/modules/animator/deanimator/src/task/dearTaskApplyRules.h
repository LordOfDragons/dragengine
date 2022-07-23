/* 
 * Drag[en]gine Animator Module
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
