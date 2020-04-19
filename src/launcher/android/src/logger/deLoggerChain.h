/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DELOGGERCHAIN_H_
#define _DELOGGERCHAIN_H_

#include "deLogger.h"
#include "../common/collection/decObjectOrderedSet.h"
#include "../threading/deMutex.h"



/**
 * \brief Logs messages to a list of other loggers.
 * 
 * Useful to log a message in more than one place for example on the console and in a file.
 * 
 * \note Logger chain is thread safe for logging. It is not thread safe to change the logger
 *       layout while the logger is in active use due to deObject reference counting not
 *       being thread safe.
 */
class deLoggerChain : public deLogger{
private:
	decObjectOrderedSet pLoggerList;
	deMutex pMutex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create chain logger. */
	deLoggerChain();
	
protected:
	/**
	 * \brief Clean up chain logger.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deLoggerChain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Number of loggers in the chain. */
	int GetLoggerCount();
	
	/** \brief Logger at position. */
	deLogger *GetLoggerAt( int index );
	
	/** \brief Add logger to chain. */
	void AddLogger( deLogger *logger );
	
	/** \brief Remove logger from chain. */
	void RemoveLogger( deLogger *logger );
	
	/** \brief Remove all logger from chain. */
	void RemoveAllLoggers();
	
	
	
	/** \brief Log information message. */
	virtual void LogInfo( const char *source, const char *message );
	
	/** \brief Log warning message. */
	virtual void LogWarn( const char *source, const char *message );
	
	/** \brief Log error message. */
	virtual void LogError( const char *source, const char *message );
	/*@}*/
	
	
	
protected:
	inline deMutex &GetMutex(){ return pMutex; }
};

#endif
