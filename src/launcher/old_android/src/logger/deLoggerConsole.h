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

#ifndef _DELOGGERCONSOLE_H_
#define _DELOGGERCONSOLE_H_

#include "deLogger.h"
#include "../threading/deMutex.h"



/**
 * \brief Logs to the standard output stream (aka the console in most cases).
 * 
 * \note Logger console is thread safe.
 */
class deLoggerConsole : public deLogger{
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
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deLoggerConsole();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Log information message. */
	virtual void LogInfo(const char *source, const char *message);
	
	/** \brief Log warning message. */
	virtual void LogWarn(const char *source, const char *message);
	
	/** \brief Log error message. */
	virtual void LogError(const char *source, const char *message);
	/*@}*/
	
	
	
protected:
	inline deMutex &GetMutex(){ return pMutex; }
};

#endif
