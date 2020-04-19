/* 
 * Drag[en]gine GUI Launcher
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

#ifndef _DEGLENGINEINSTANCEREADLOG_H_
#define _DEGLENGINEINSTANCEREADLOG_H_

#include <dragengine/threading/deThread.h>

class deglEngineInstance;
class deLogger;



/**
 * @brief Game Engine Instance Read Log.
 * Thread reading logs from an engine instance log pipe sending them to
 * a logger.
 */
class deglEngineInstanceReadLog : public deThread{
private:
	deglEngineInstance *pEngineInstance;
	
	char *pBufferSource;
	int pBufferSourceLen;
	char *pBufferMessage;
	int pBufferMessageLen;
	
	deLogger *pLogger;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new engine instance read log. */
	deglEngineInstanceReadLog( deglEngineInstance *engineInstance, deLogger *logger );
	/** Cleans up the engine instance. */
	virtual ~deglEngineInstanceReadLog();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/**
	 * Run function of the thread. Overwrite to implement what the
	 * threa is supposed to be doing. Once this function ends the
	 * thread is stopped.
	 */
	virtual void Run();
	
	/** Read data from log pipe. Returns true if new data is available or false if the pipe broke. */
	bool ReadFromPipe( void *data, int length );
	
	/** Prepare the source buffer to be at least requiredLength long excluding the 0-termination character. */
	void PrepareBufferSource( int requiredLength );
	/** Prepare the message buffer to be at least requiredLength long excluding the 0-termination character. */
	void PrepareBufferMessage( int requiredLength );
	/*@}*/
};

#endif // _DEGLGAME_H_
