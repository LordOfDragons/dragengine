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
