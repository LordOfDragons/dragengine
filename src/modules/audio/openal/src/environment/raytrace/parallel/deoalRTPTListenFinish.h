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

#ifndef _DEOALRTPTLISTENFINISH_H_
#define _DEOALRTPTLISTENFINISH_H_

#include "deoalRTParallelEnvProbe.h"

#include <dragengine/parallel/deParallelTask.h>

class decPointerList;


/**
 * \brief Finish listen parallel task estimating room parameters.
 */
class deoalRTPTListenFinish : public deParallelTask{
private:
	deoalRTParallelEnvProbe &pOwner;
	
	decPointerList pTasks;
	const deoalEnvProbe *pSourceProbe;
	const deoalEnvProbe *pListenProbe;
	decDVector pPosition;
	
	deoalImpulseResponse pDecayCurve;
	
	deoalEnvProbeListener *pListener;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create finish listen parallel task. */
	deoalRTPTListenFinish( deoalRTParallelEnvProbe &owner );
	
protected:
	/** \brief Clean up finish listen parallel task. */
	virtual ~deoalRTPTListenFinish();
	/*@}*/
	
	
	
public:
	/** \name Manegement */
	/*@{*/
	/** \brief Add dependencies. */
	void AddDependencies( const decPointerList &tasks );
	
	/** \brief Set probes. */
	void SetProbes( const deoalEnvProbe *source, const deoalEnvProbe *listen );
	
	/** \brief Set listen position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Set listener to update. */
	void SetListener( deoalEnvProbeListener *listener );
	
	/** \brief Listen tasks (deoalRTPTListen*). */
	inline decPointerList GetListenTasks(){ return pTasks; }
	
	
	
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
