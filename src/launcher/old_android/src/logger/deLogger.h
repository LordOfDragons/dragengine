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
