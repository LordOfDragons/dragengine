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

#ifndef _DELLOGGERWRITEPIPE_H_
#define _DELLOGGERWRITEPIPE_H_

#include <dragengine/dragengine_configuration.h>

#ifdef OS_W32
#include <dragengine/app/include_windows.h>
#endif

#include <dragengine/logger/deLogger.h>



/**
 * \brief Logger writing logs to pipe.
 */
class delLoggerWritePipe : public deLogger{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<delLoggerWritePipe> Ref;
	
	
	
private:
#ifdef OS_W32
	HANDLE pPipe;
#else
	int pPipe;
#endif
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create logger. */
#ifdef OS_W32
	delLoggerWritePipe( HANDLE pipe );
#else
	delLoggerWritePipe( int pipe );
#endif
	
protected:
	/** \brief Clean up logger. */
	virtual ~delLoggerWritePipe();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Log an information message. */
	virtual void LogInfo( const char *source, const char *message );
	
	/** \brief Log a warning message. */
	virtual void LogWarn( const char *source, const char *message );
	
	/** \brief Log an error message. */
	virtual void LogError( const char *source, const char *message );
	/*@}*/
	
	
	
protected:
	void LogToPipe( const char *source, const char *message, int type );
	void WriteToPipe( const void *data, int length );
};

#endif
