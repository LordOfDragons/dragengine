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

#ifndef _DEOALRTPTLISTENFINISH_H_
#define _DEOALRTPTLISTENFINISH_H_

#include "deoalRTParallelEnvProbe.h"

#include <dragengine/parallel/deParallelTask.h>

class decPointerList;


/**
 * \brief Finish listen parallel task estimating room parameters.
 */
class deoalRTPTListenFinish : public deParallelTask{
public:
	/** \brief Type holding strong reference. */
	typedef deTThreadSafeObjectReference<deoalRTPTListenFinish> Ref;
	
	
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
