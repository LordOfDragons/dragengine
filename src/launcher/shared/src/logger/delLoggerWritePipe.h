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

#ifndef _DELLOGGERWRITEPIPE_H_
#define _DELLOGGERWRITEPIPE_H_

#include <dragengine/dragengine_configuration.h>

#ifdef OS_W32
#include <dragengine/app/include_windows.h>
#endif

#include <dragengine/logger/deLogger.h>



/**
 * \brief Logger writing logs to pipe.
 */
class DE_DLL_EXPORT delLoggerWritePipe : public deLogger{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<delLoggerWritePipe> Ref;
	
	
	
private:
#ifdef OS_W32
	HANDLE pPipe;
#else
	int pPipe;
#endif
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create logger. */
#ifdef OS_W32
	delLoggerWritePipe( HANDLE pipe );
#else
	delLoggerWritePipe( int pipe );
#endif
	
protected:
	/** \brief Clean up logger. */
	virtual ~delLoggerWritePipe();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Log an information message. */
	virtual void LogInfo( const char *source, const char *message );
	
	/** \brief Log a warning message. */
	virtual void LogWarn( const char *source, const char *message );
	
	/** \brief Log an error message. */
	virtual void LogError( const char *source, const char *message );
	/*@}*/
	
	
	
protected:
	void LogToPipe( const char *source, const char *message, int type );
	void WriteToPipe( const void *data, int length );
};

#endif
