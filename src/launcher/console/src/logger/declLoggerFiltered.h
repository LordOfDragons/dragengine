/* 
 * Drag[en]gine Console Launcher
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

#ifndef _DECLLOGGERFILTERED_H_
#define _DECLLOGGERFILTERED_H_

#include <dragengine/logger/deLogger.h>



/**
 * @brief Logger Filtered.
 * A logger passing messages to other loggers depending on the message type.
 */
class declLoggerFiltered : public deLogger{
private:
	deLogger *pLoggerInfo;
	deLogger *pLoggerWarn;
	deLogger *pLoggerError;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new logger. */
	declLoggerFiltered();
	/** Cleans up the logger. */
	virtual ~declLoggerFiltered();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the logger to pass information to or NULL to ignore them. */
	inline deLogger *GetLoggerInfo() const{ return pLoggerInfo; }
	/** Sets the logger to pass information to or NULL to ignore them. */
	void SetLoggerInfo( deLogger *logger );
	/** Retrieves the logger to pass warnings to or NULL to ignore them. */
	inline deLogger *GetLoggerWarning() const{ return pLoggerWarn; }
	/** Sets the logger to pass warnings to or NULL to ignore them. */
	void SetLoggerWarning( deLogger *logger );
	/** Retrieves the logger to pass errors to or NULL to ignore them. */
	inline deLogger *GetLoggerError() const{ return pLoggerError; }
	/** Sets the logger to pass errors to or NULL to ignore them. */
	void SetLoggerError( deLogger *logger );
	
	/** Log an information message. */
	virtual void LogInfo( const char *source, const char *message );
	
	/** Log a formated information message. */
	virtual void LogInfoFormat( const char *source, const char *message, ... )
		#ifdef __GNUC__
		__attribute__ ((format (printf, 3, 4)))
		#endif
		;
	
	/** Log a formated information message. */
	virtual void LogInfoFormatUsing( const char *source, const char *message, va_list args );
	
	/** Log a warning message. */
	virtual void LogWarn( const char *source, const char *message );
	
	/** Log a formated warning message. */
	virtual void LogWarnFormat( const char *source, const char *message, ... )
		#ifdef __GNUC__
		__attribute__ ((format (printf, 3, 4)))
		#endif
		;
		
	/** Log a formated warning message. */
	virtual void LogWarnFormatUsing( const char *source, const char *message, va_list args );
	
	/** Log an error message. */
	virtual void LogError( const char *source, const char *message );
	
	/** Log a formated error message. */
	virtual void LogErrorFormat( const char *source, const char *message, ... )
		#ifdef __GNUC__
		__attribute__ ((format (printf, 3, 4)))
		#endif
		;
		
	/** Log a formated error message. */
	virtual void LogErrorFormatUsing( const char *source, const char *message, va_list args );
	
	/** Log an exception as error message. */
	virtual void LogException( const char *source, const deException &exception );
	/*@}*/
};

#endif
