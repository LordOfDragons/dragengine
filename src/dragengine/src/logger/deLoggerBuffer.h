/* 
 * Drag[en]gine Game Engine
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

#ifndef _DELOGGERBUFFER_H_
#define _DELOGGERBUFFER_H_

#include "deLogger.h"
#include "../common/string/decString.h"
#include "../threading/deMutex.h"


/**
 * \brief Logger buffer.
 * 
 * Buffers messages to send them at a later time to another logger. Allows for example
 * collecting log messages in parallel tasks and sending them to a main thread logger
 * during synchronization time. The logger buffer is not thread safe. It only provides
 * a simple to use buffer for log messages.
 * 
 * \note Logger buffer is thread safe.
 */
class DE_DLL_EXPORT deLoggerBuffer : public deLogger{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deLoggerBuffer> Ref;
	
	
	
public:
	/** \brief Message types. */
	enum eMessageTypes{
		/** \brief Information. */
		emtInfo,
		
		/** \brief Warning. */
		emtWarn,
		
		/** \brief Error. */
		emtError
	};
	
	/** \brief Logger buffer entry. */
	struct DE_DLL_EXPORT sEntry{
		/** \brief Type. */
		eMessageTypes type;
		
		/** \brief Source. */
		decString source;
		
		/** \brief Message. */
		decString message;
		
		/** \brief Copy constructor. */
		sEntry();
		
		/** \brief Copy constructor. */
		sEntry( const sEntry &entry );
		
		/** \brief Assign entry. */
		sEntry &operator=( const sEntry &entry );
	};
	
	
	
private:
	sEntry *pEntries;
	int pEntrySize;
	int pEntryCount;
	deMutex pMutex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create logger buffer. */
	deLoggerBuffer();
	
protected:
	/**
	 * \brief Clean up logger buffer.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deLoggerBuffer();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Number of entries. */
	int GetEntryCount();
	
	/** \brief Copy of entry at index. */
	sEntry GetEntryAt( int index );
	
	/** \brief Add entry. */
	void AddEntry( eMessageTypes type, const decString &source, const decString &message );
	
	/** \brief Clear buffer. */
	void Clear();
	
	/** \brief Send messages in the buffer to a logger and clear the buffer afterwards. */
	void SendToLogger( deLogger &logger );
	
	
	
	/** \brief Log information message. */
	virtual void LogInfo( const char *source, const char *message );
	
	/** \brief Log warning message. */
	virtual void LogWarn( const char *source, const char *message );
	
	/** \brief Log error message. */
	virtual void LogError( const char *source, const char *message );
	/*@}*/
	
	
	
protected:
	inline deMutex &GetMutex(){ return pMutex; }
	
	
	
private:
	void pClear();
	void pAddEntry( eMessageTypes type, const decString &source, const decString &message );
};

#endif
