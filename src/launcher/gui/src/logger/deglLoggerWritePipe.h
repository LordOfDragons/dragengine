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

#ifndef _DEGLLOGGERWRITEPIPE_H_
#define _DEGLLOGGERWRITEPIPE_H_

#ifdef OS_W32
#include <dragengine/app/include_windows.h>
#endif

#include <dragengine/logger/deLogger.h>



/**
 * @brief Logger Write Pipe.
 * Logger writing the logs to a pipe.
 */
class deglLoggerWritePipe : public deLogger{
private:
#ifdef OS_W32
	HANDLE pPipe;
#else
	int pPipe;
#endif
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new logger. */
#ifdef OS_W32
	deglLoggerWritePipe( HANDLE pipe );
#else
	deglLoggerWritePipe( int pipe );
#endif
	/** Cleans up the logger. */
	virtual ~deglLoggerWritePipe();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Log an information message. */
	virtual void LogInfo( const char *source, const char *message );
	/** Log a warning message. */
	virtual void LogWarn( const char *source, const char *message );
	/** Log an error message. */
	virtual void LogError( const char *source, const char *message );
	/*@}*/
	
private:
	void pLogToPipe( const char *source, const char *message, int type );
	void pWriteToPipe( const void *data, int length );
};

#endif
