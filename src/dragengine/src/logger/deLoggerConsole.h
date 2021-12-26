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

#ifndef _DELOGGERCONSOLE_H_
#define _DELOGGERCONSOLE_H_

#include "deLogger.h"
#include "../threading/deMutex.h"


/**
 * \brief Logs to the standard output stream (aka the console in most cases).
 * 
 * \note Logger console is thread safe.
 */
class DE_DLL_EXPORT deLoggerConsole : public deLogger{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deLoggerConsole> Ref;
	
	
	
private:
	deMutex pMutex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create console logger. */
	deLoggerConsole();
	
protected:
	/**
	 * \brief Clean up console logger.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deLoggerConsole();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Log information message. */
	virtual void LogInfo( const char *source, const char *message );
	
	/** \brief Log warning message. */
	virtual void LogWarn( const char *source, const char *message );
	
	/** \brief Log error message. */
	virtual void LogError( const char *source, const char *message );
	/*@}*/
	
	
	
protected:
	inline deMutex &GetMutex(){ return pMutex; }
	void LogPrefix( const char *source, const char *message, const char *prefix );
};

#endif
