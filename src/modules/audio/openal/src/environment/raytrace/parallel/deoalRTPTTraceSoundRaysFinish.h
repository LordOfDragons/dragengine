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

#ifndef _DEOALRTPTTRACESOUNDRAYSFINISH_H_
#define _DEOALRTPTTRACESOUNDRAYSFINISH_H_

#include "deoalRTParallelEnvProbe.h"

#include <dragengine/parallel/deParallelTask.h>

class deoalRayTraceConfig;
class deoalSoundRayList;

class decPointerList;


/**
 * \brief Ray tracing parallel task pre-calculating environment probe information.
 */
class deoalRTPTTraceSoundRaysFinish : public deParallelTask{
private:
	deoalRTParallelEnvProbe &pOwner;
	
	decPointerList pTasks;
	float pRange;
	const deoalRayTraceConfig *pProbeConfig;
	
	deoalSoundRayList *pSoundRayList;
	deoalRTParallelEnvProbe::sRoomParameters *pRoomParameters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create ray trace finish parallel task. */
	deoalRTPTTraceSoundRaysFinish( deoalRTParallelEnvProbe &owner );
	
protected:
	/** \brief Clean up ray trace finish parallel task. */
	virtual ~deoalRTPTTraceSoundRaysFinish();
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
	
	/** \brief Set sound ray list to update. */
	void SetSoundRayList( deoalSoundRayList *soundRayList );
	
	/** \brief Set room parameters to update. */
	void SetRoomParameters( deoalRTParallelEnvProbe::sRoomParameters *roomParameters );
	
	/** \brief Trace tasks (deoalRTPTTraceSoundRays*). */
	inline decPointerList GetTraceTasks(){ return pTasks; }
	
	
	
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
