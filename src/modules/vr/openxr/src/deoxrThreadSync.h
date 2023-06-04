/* 
 * Drag[en]gine OpenXR
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOXRTHREADSYNC_H_
#define _DEOXRTHREADSYNC_H_

#include "deoxrBasics.h"

#include <dragengine/threading/deThread.h>
#include <dragengine/threading/deSemaphore.h>
#include <dragengine/threading/deMutex.h>

class deVROpenXR;


/**
 * Synchronization thread.
 */
class deoxrThreadSync : public deThread{
private:
	enum eCommand{
		ecNone,
		ecExit,
		ecWaitFrame
	};
	
	deVROpenXR &pOxr;
	
	deMutex pMutex;
	
	deSemaphore pSemaphoreNextCommand;
	eCommand pNextCommand;
	
	deSemaphore pSemaphoreWaitFrameFinished;
	bool pWaitFrameRunning;
	bool pWaitFrameFailed;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create synchronization thread. */
	deoxrThreadSync( deVROpenXR &oxr );
	
	/** Clean up synchronization thread. */
	virtual ~deoxrThreadSync();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Start wait frame. */
	void StartWaitFrame();
	
	/** Wait for wait frame to finish. */
	void WaitWaitFrameFinished();
	
	
	
	/** Exit thread. */
	void ExitThread();
	
	
	
	/**
	 * \brief Run function of the thread.
	 * 
	 * Overwrite to implement what the threa is supposed to be doing. Once this function
	 * ends the thread is stopped.
	 */
	virtual void Run();
	/*@}*/
	
	
	
private:
	void pWaitFrame();
};

#endif
