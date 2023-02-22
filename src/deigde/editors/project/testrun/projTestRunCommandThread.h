/* 
 * DEIGDE Project
 *
 * Copyright (C) 2018, Plüss Roland ( roland@rptd.ch )
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is projributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _PROJTESTRUNCOMMANDTHREAD_H_
#define _PROJTESTRUNCOMMANDTHREAD_H_

#include <dragengine/dragengine_configuration.h>

#ifdef OS_W32
#include <dragengine/app/include_windows.h>
#else
#include <unistd.h>
#endif

#include <dragengine/common/string/decString.h>
#include <dragengine/threading/deThread.h>

class projTestRunProcess;


/**
 * \brief Command thread.
 * 
 * Blocks on the read-pipe waiting for commands to be send. Commands are send to the
 * process to be processed. This thread manages onyl the reading of the command and
 * writing back the result.
 */
class projTestRunCommandThread : public deThread{
private:
	projTestRunProcess &pProcess;
	bool pAbort;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create command thread. */
	projTestRunCommandThread( projTestRunProcess &process );
	
	/** \brief Cleans up the run game thread. */
	virtual ~projTestRunCommandThread();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Run thread */
	virtual void Run();
	
	/** \brief Abort thread if running. */
	void Abort();
	
	
	
	/**
	 * \brief Read command from pipe.
	 * 
	 * blocks thread until a command is read. Handles interrupts.
	 */
	int ReadCommandFromPipe();
	/*@}*/
};

#endif
