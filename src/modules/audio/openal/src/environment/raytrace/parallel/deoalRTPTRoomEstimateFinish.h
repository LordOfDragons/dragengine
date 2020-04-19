/* 
 * Drag[en]gine OpenAL Audio Module
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

#ifndef _DEOALRTPTROOMESTIMATEFINISH_H_
#define _DEOALRTPTROOMESTIMATEFINISH_H_

#include "deoalRTParallelEnvProbe.h"

#include <dragengine/parallel/deParallelTask.h>

class decPointerList;


/**
 * \brief Finish ray tracing parallel task estimating room parameters.
 */
class deoalRTPTRoomEstimateFinish : public deParallelTask{
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
