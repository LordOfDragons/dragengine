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

#ifndef _DELOGGER_H_
#define _DELOGGER_H_

#include <stdarg.h>
#include "../deObject.h"

class deException;



/**
 * \brief Base logger class for all loggers.
 * 
 * To help sub-classes the formatted version provide a default implementation.
 * forwarding a formatted text to the basic logger functions. In general this
 * is the best solution. Sub classes can though overwrite these functions if
 * they want to handle the formatting themselves.
 * 
 * \note Loggers are thread safe. See subclass for potential restrictions.
 */
class deLogger : public deObject{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates logger. */
	deLogger();
	
	
	
protected:
	/**
	 * \brief Clean up logger.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deLogger();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Log information message. */
	virtual void LogInfo( const char *source, const char *message );
	
	/**
	 * \brief Log formatted information message.
	 * 
	 * The default implementation builds a \ref decString with the formatted
	 * string handling it over to \ref LogInfo.
	 */
	virtual void LogInfoFormat( const char *source, const char *message, ... );
	
	/**
	 * \brief Log formatted information message.
	 * 
	 * The default implementation builds a \ref decString with the formatted
	 * string handling it over to \ref LogInfo.
	 */
	virtual void LogInfoFormatUsing( const char *source, const char *message, va_list args );
	
	
	/** \brief Log warning message. */
	virtual void LogWarn( const char *source, const char *message );
	
	/**
	 * \brief Log formated warning message.
	 * 
	 * The default implementation builds a \ref decString with the formatted
	 * string handling it over to \ref LogWarn.
	 */
	virtual void LogWarnFormat( const char *source, const char *message, ... );
	
	/**
	 * \brief Log formated warning message.
	 * 
	 * The default implementation builds a \ref decString with the formatted
	 * string handling it over to \ref LogWarn.
	 */
	virtual void LogWarnFormatUsing( const char *source, const char *message, va_list args );
	
	/** \brief Log error message. */
	virtual void LogError( const char *source, const char *message );
	
	/**
	 * \brief Log formated error message.
	 * 
	 * The default implementation builds a \ref decString with the formatted
	 * string handling it over to \ref LogError.
	 */
	virtual void LogErrorFormat( const char *source, const char *message, ... );
	
	/**
	 * \brief Log formated error message.
	 * 
	 * The default implementation builds a \ref decString with the formatted
	 * string handling it over to \ref LogError.
	 */
	virtual void LogErrorFormatUsing( const char *source, const char *message, va_list args );
	
	/**
	 * \brief Log exception error message.
	 * 
	 * The default implementation builds a \ref decString with the formatted
	 * string handling it over to \ref LogError. The string is formatted as
	 * "exception(file:line): message (description)" if description is not an
	 * empty string or "exception(file:line): message" otherwise.
	 */
	virtual void LogException( const char *source, const deException &exception );
	/*@}*/
};

#endif
