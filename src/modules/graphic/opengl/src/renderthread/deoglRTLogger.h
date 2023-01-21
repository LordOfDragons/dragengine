/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLRRTLOGGING_H_
#define _DEOGLRRTLOGGING_H_

#include <dragengine/logger/deLoggerBuffer.h>

class deException;
class deoglRenderThread;



/**
 * Render thread logging.
 */
class deoglRTLogger{
private:
	deoglRenderThread &pRenderThread;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render thread logger. */
	deoglRTLogger( deoglRenderThread &renderThread );
	
	/** Clean up render thread renderers. */
	~deoglRTLogger();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * Logger.
	 * \details For asynchronous rendering returns the buffered logger. For synchronous
	 *          rendering returns the game engine logger. */
	deLogger &GetLogger();
	
	/** Synchronize logging. */
	void Synchronize();
	
	
	
	/** Outputs an information message on the console. */
	void LogInfo( const char *message );
	
	/** Outputs a formated information message on the console. */
	void LogInfoFormat( const char *message, ... )
		#ifdef __GNUC__
		__attribute__ ((format (printf, 2, 3)))
		#endif
		;
	
	/** Outputs a formated information message on the console. */
	void LogInfoFormatUsing( const char *message, va_list args );
	
	/** Outputs a warning message on the console. */
	void LogWarn( const char *message );
	
	/** Outputs a formated warning message on the console. */
	void LogWarnFormat( const char *message, ... )
		#ifdef __GNUC__
		__attribute__ ((format (printf, 2, 3)))
		#endif
		;
	
	/** Outputs a formated warning message on the console. */
	void LogWarnFormatUsing( const char *message, va_list args );
	
	/** Outputs an error message on the console. */
	void LogError( const char *message );
	
	/** Outputs a formated error message on the console. */
	void LogErrorFormat( const char *message, ... )
		#ifdef __GNUC__
		__attribute__ ((format (printf, 2, 3)))
		#endif
		;
	
	/** Outputs a formated error message on the console. */
	void LogErrorFormatUsing( const char *message, va_list args );
	
	/** Outputs an exception as error message on the console. */
	void LogException( const deException &exception );
	/*@}*/
};

#endif
