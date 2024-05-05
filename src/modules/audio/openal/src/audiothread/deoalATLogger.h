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

#ifndef _DEOALATLOGGER_H_
#define _DEOALATLOGGER_H_

#include <dragengine/logger/deLoggerBuffer.h>

class deException;
class deoalAudioThread;



/**
 * \brief Audio thread logging.
 */
class deoalATLogger{
private:
	deoalAudioThread &pAudioThread;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render thread logger. */
	deoalATLogger( deoalAudioThread &audioThread );
	
	/** \brief Clean up render thread renderers. */
	~deoalATLogger();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Logger.
	 * \details For asynchronous rendering returns the buffered logger. For synchronous
	 *          rendering returns the game engine logger. */
	deLogger &GetLogger();
	
	/** \brief Synchronize logging. */
	void Synchronize();
	
	
	
	/** \brief Outputs an information message on the console. */
	void LogInfo( const char *message );
	
	/** \brief Outputs a formated information message on the console. */
	void LogInfoFormat( const char *message, ... )
		#ifdef __GNUC__
		__attribute__ ((format (printf, 2, 3)))
		#endif
		;
	
	/** \brief Outputs a formated information message on the console. */
	void LogInfoFormatUsing( const char *message, va_list args );
	
	/** \brief Outputs a warning message on the console. */
	void LogWarn( const char *message );
	
	/** \brief Outputs a formated warning message on the console. */
	void LogWarnFormat( const char *message, ... )
		#ifdef __GNUC__
		__attribute__ ((format (printf, 2, 3)))
		#endif
		;
	
	/** \brief Outputs a formated warning message on the console. */
	void LogWarnFormatUsing( const char *message, va_list args );
	
	/** \brief Outputs an error message on the console. */
	void LogError( const char *message );
	
	/** \brief Outputs a formated error message on the console. */
	void LogErrorFormat( const char *message, ... )
		#ifdef __GNUC__
		__attribute__ ((format (printf, 2, 3)))
		#endif
		;
	
	/** \brief Outputs a formated error message on the console. */
	void LogErrorFormatUsing( const char *message, va_list args );
	
	/** \brief Outputs an exception as error message on the console. */
	void LogException( const deException &exception );
	/*@}*/
};

#endif
