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
	deoxrThreadSync(deVROpenXR &oxr);
	
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
