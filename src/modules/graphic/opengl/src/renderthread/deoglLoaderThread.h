/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLRLOADERTHREAD_H_
#define _DEOGLRLOADERTHREAD_H_

#include "../deoglBasics.h"

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/threading/deMutex.h>
#include <dragengine/threading/deSemaphore.h>
#include <dragengine/threading/deThread.h>

class deoglRenderThread;
class deoglLoaderThreadTask;


/**
 * Loader thread. Handles loading and creating object objects outside of the render thread.
 */
class deoglLoaderThread : public deThread{
private:
	deoglRenderThread &pRenderThread;
	
	deSemaphore pSemaphore;
	deMutex pMutex;
	bool pShutdown;
	bool pContextEnabled;
	
	decPointerList pTasks;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create loader thread. */
	deoglLoaderThread( deoglRenderThread &renderThread );
	
	/** Clean up loader thread. */
	virtual ~deoglLoaderThread();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Run render thread. */
	virtual void Run();
	
	/** Loader is enabled. */
	bool IsEnabled();
	
	/** Enable context. */
	void EnableContext( bool enable );
	
	/** Add task to process. Returns true if added or false if loader is disabled. */
	bool AddTask( deoglLoaderThreadTask *task );
	
	/**
	 * Add task and wait for it to be processed then returns.
	 * Returns true if process or false if loader is disabled.
	 */
	bool AwaitTask( deoglLoaderThreadTask *task );
	/*@}*/
	
	
	
private:
	void pInit();
	void pCleanUp();
};

#endif
