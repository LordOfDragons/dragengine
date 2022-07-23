/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DELENGINEINSTANCEREADLOG_H_
#define _DELENGINEINSTANCEREADLOG_H_

#include <dragengine/logger/deLogger.h>
#include <dragengine/threading/deThread.h>

class delEngineInstance;


/**
 * \brief Game Engine Instance Read Log.
 * 
 * Thread reading logs from an engine instance log pipe sending them to a logger.
 */
class delEngineInstanceReadLog : public deThread{
private:
	delEngineInstance &pEngineInstance;
	deLogger::Ref pLogger;
	
	char *pBufferSource;
	int pBufferSourceLen;
	char *pBufferMessage;
	int pBufferMessageLen;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create engine instance read log. */
	delEngineInstanceReadLog( delEngineInstance &engineInstance, deLogger *logger );
	
	/** \brief Clean up engine instance. */
	virtual ~delEngineInstanceReadLog();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Run thread function. */
	virtual void Run();
	
	/**
	 * \brief Read data from log pipe.
	 * \returns true if new data is available or false if the pipe broke.
	 */
	bool ReadFromPipe( void *data, int length );
	
	/** \brief Prepare source buffer to be at least requiredLength long excluding the 0-termination character. */
	void PrepareBufferSource( int requiredLength );
	
	/** \brief Prepare the message buffer to be at least requiredLength long excluding the 0-termination character. */
	void PrepareBufferMessage( int requiredLength );
	/*@}*/
};

#endif
