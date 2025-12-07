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

#ifndef _DEOALRTPTROOMESTIMATEFINISH_H_
#define _DEOALRTPTROOMESTIMATEFINISH_H_

#include "deoalRTParallelEnvProbe.h"

#include <dragengine/parallel/deParallelTask.h>

class decPointerList;


/**
 * \brief Finish ray tracing parallel task estimating room parameters.
 */
class deoalRTPTRoomEstimateFinish : public deParallelTask{
public:
	/** \brief Type holding strong reference. */
	typedef deTThreadSafeObjectReference<deoalRTPTRoomEstimateFinish> Ref;
	
	
private:
	deoalRTParallelEnvProbe &pOwner;
	
	decPointerList pTasks;
	float pRange;
	const deoalRayTraceConfig *pProbeConfig;
	
	deoalRTParallelEnvProbe::sRoomParameters *pRoomParameters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create ray trace parallel task. */
	deoalRTPTRoomEstimateFinish( deoalRTParallelEnvProbe &owner );
	
protected:
	/** \brief Clean up ray trace parallel task. */
	virtual ~deoalRTPTRoomEstimateFinish();
	/*@}*/
	
	
	
public:
	/** \name Manegement */
	/*@{*/
	/** \brief Add dependencies. */
	void AddDependencies( const decPointerList &tasks );
	
	/** \brief Set range. */
	void SetRange( float range );
	
	/** \brief Probe configuration. */
	void SetProbeConfig( const deoalRayTraceConfig *probeConfig );
	
	/** \brief Set room parameters to update. */
	void SetRoomParameters( deoalRTParallelEnvProbe::sRoomParameters *roomParameters );
	
	/** \brief Estimate tasks (deoalRTPTRoomEstimateFinish*). */
	inline decPointerList GetEstimateTasks(){ return pTasks; }
	
	
	
	/** \brief Parallel task implementation. */
	virtual void Run();
	
	/** \brief Processing of task Run() finished. */
	virtual void Finished();
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Short task name for debugging. */
	virtual decString GetDebugName() const;
	
	/** \brief Task details for debugging. */
	virtual decString GetDebugDetails() const;
	/*@}*/
	
	
	
private:
	void pRun();
};

#endif
