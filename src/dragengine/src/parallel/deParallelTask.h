/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEPARALLELTASK_H_
#define _DEPARALLELTASK_H_

#include "../common/collection/decThreadSafeObjectOrderedSet.h"
#include "../common/string/decString.h"
#include "../threading/deThreadSafeObject.h"

class deLogger;
class deBaseModule;


/**
 * \brief Parallel processing task.
 * 
 * Subclass to implement task to run. Tasks have to finished as quick as possible to
 * allow for best throughput. Tasks can be assigned as dependency of other tasks. A task
 * will not run unless all dependency tasks are marked finished. There are two kinds of
 * parallel tasks differeing in when they are marked finished.
 * 
 * Purely asynchronous tasks chained with each other are marked finished after their Run()
 * methods exist. This allows a chain of tasks to run one after the other without waiting
 * for the application to do another frame loop. Their Finished() methods are called during
 * the next time the a frame loop is started. Tasks of this kind do require only the
 * calculated result of the previous task Run() method not Finished(). In general these
 * kind of task chains have to complete during the same frame loop.
 * \note If the task is cancelled before Run() is called the task is marked finished at
 * the time Cancel() is called.
 * 
 * Partially asynchronous tasks chained with each other require their Finished() method to
 * be called during the next frame loop for subsequent tasks to function properly. These
 * tasks are mark finished after their Finished() method exists. Task chains of this kind
 * can not run through fully in one frame loop. Although spreading across multiple frame
 * loops they do not block parallel processing threads. they are optimal for complex tasks
 * split into small chunks of work not required to be calculated in a single frame loop.
 * \note If the task is cancelled before Run() is called the task is marked finished after
 * Finished() is called.
 * 
 * Use SetFinishAfterRun() to set the appropriate behavior.
 */
class deParallelTask : public deThreadSafeObject{
private:
	deBaseModule *pOwner;
	
	bool pCancel;
	bool pFinished;
	bool pMarkFinishedAfterRun;
	bool pEmptyRun;
	bool pLowPriority;
	
	decThreadSafeObjectOrderedSet pDependsOn;
	decThreadSafeObjectOrderedSet pDependedOnBy;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create task.
	 * \param[in] owner Module owning the task or NULL if global.
	 */
	deParallelTask( deBaseModule *owner );
	
protected:
	/** \brief Clean up task. */
	virtual ~deParallelTask();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Module owning the task or NULL if global. */
	inline deBaseModule *GetOwner() const{ return pOwner; }
	
	/**
	 * \brief Mark task finished after Run() method leaves.
	 * 
	 * Default value is true.
	 * 
	 * \retval true Task is marked finished after Run() method exits.
	 * \retval false Task is marked finished after Finished() method exits.
	 */
	inline bool GetMarkFinishedAfterRun() const{ return pMarkFinishedAfterRun; }
	
	/**
	 * \brief Set if task is marked finished after Run() method leaves.
	 * \param finishAfterRun If true task is marked finished after Run() method exits.
	 * If false Task is marked finished after Finished() method exits.
	 */
	void SetMarkFinishedAfterRun( bool markFinishedAfterRun );
	
	/** \brief Task has empty run implementation. */
	inline bool GetEmptyRun() const{ return pEmptyRun; }
	
	/**
	 * \brief Set if task has empty run implementation.
	 * 
	 * This is an optimization. If the run implementation is empty the task will be finished
	 * without running the empty run method using a free thread.
	 */
	void SetEmptyRun( bool emptyRun );
	
	/** \brief Task has lower priority than other tasks. */
	inline bool GetLowPriority() const{ return pLowPriority; }
	
	/** \brief Set if task has lower priority than other tasks. */
	void SetLowPriority( bool lowPriority );
	
	/** \brief Task has been cancelled. */
	inline bool IsCancelled() const{ return pCancel; }
	
	/**
	 * \brief Cancel task.
	 * 
	 * Call from the main thread to cancel a task. A cancelled task does not
	 * call Finished() or Run() if still pending.
	 */
	void Cancel();
	
	/** \brief Task is finished. */
	inline bool GetFinished() const{ return pFinished; }
	
	/**
	 * \brief Set task finished.
	 * 
	 * Call from Run() or Finished() to allow tasks dependeing on this task to start running. 
	 */
	void SetFinished();
	
	/** \brief Number of tasks this task depends on. */
	int GetDependsOnCount() const;
	
	/** \brief Depend on task at index. */
	deParallelTask *GetDependsOnAt( int index ) const;
	
	/** \brief Task depends on another task. */
	bool DoesDependOn( deParallelTask *task ) const;
	
	/**
	 * \brief Add task this task depends on.
	 * \throws deeInvalidParam \em task is NULL.
	 * \throws deeInvalidParam \em task is this task.
	 * \throws deeInvalidParam \em task has been already added.
	 */
	void AddDependsOn( deParallelTask *task );
	
	/**
	 * \brief Remove task this task depends on.
	 * \throws deeInvalidParam \em task is NULL.
	 * \throws deeInvalidParam \em task is this task.
	 * \throws deeInvalidParam \em task has not been added.
	 */
	void RemoveDependsOn( deParallelTask *task );
	
	/** \brief Remove all tasks this task depends on. */
	void RemoveAllDependsOn();
	
	/**
	 * \brief List of tasks depending on this task.
	 * 
	 * Used by deParallelProcessing only.
	 */
	inline decThreadSafeObjectOrderedSet &GetDependedOnBy(){ return pDependedOnBy; }
	inline const decThreadSafeObjectOrderedSet &GetDependedOnBy() const{ return pDependedOnBy; }
	
	/**
	 * \brief Remove from all tasks depending on this task.
	 * 
	 * Used by deParallelProcessing only.
	 */
	void RemoveFromAllDependedOnTasks();
	
	/**
	 * \brief Task can run.
	 * \returns true if task does not depend on other tasks or all other tasks finished.
	 */
	bool CanRun() const;
	
	/**
	 * \brief Reset task.
	 * 
	 * Used only by deParallelProcessing.
	 */
	void Reset();
	/*@}*/
	
	
	
	/** \name Subclass Responsibility */
	/*@{*/
	/**
	 * \brief Parallel task implementation.
	 * 
	 * \warning Do not manipulate the reference count or depends-on of the tasks or any
	 * task depending on it.
	 */
	virtual void Run() = 0;
	
	/**
	 * \brief Processing of task Run() finished.
	 * 
	 * Called after the task has finished and has been collected by deParallelProcessing::Update().
	 * This call runs synchronously in the main engine thread. You have to check using
	 * IsCancelled() if the task has been cancelled or finished successfully. Finished() will
	 * be called in all circumstances to allow proper cleaning up.
	 * 
	 * \note It is allowed to start new tasks from inside Finished().
	 * \note When called the 'depends on' and 'depended on by' list will be empty.
	 * \warning Do not re-add this tasks from inside Finished().
	 * \warning Do as little work as possible here to not stall too much.
	 */
	virtual void Finished() = 0;
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Short task name for debugging. */
	virtual decString GetDebugName() const;
	
	/** \brief Task details for debugging. */
	virtual decString GetDebugDetails() const;
	
// 	void VerifyDependsOn();
	/*@}*/
};

#endif
