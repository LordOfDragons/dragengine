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
