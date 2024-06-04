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

#ifndef _DELENGINEINSTANCEREADLOG_H_
#define _DELENGINEINSTANCEREADLOG_H_

#include <dragengine/logger/deLogger.h>
#include <dragengine/threading/deThread.h>

class delEngineInstance;


/**
 * \brief Game Engine Instance Read Log.
 * 
 * Thread reading logs from an engine instance log pipe sending them to a logger.
 */
class DE_DLL_EXPORT delEngineInstanceReadLog : public deThread{
private:
	delEngineInstance &pEngineInstance;
	deLogger::Ref pLogger;
	
	char *pBufferSource;
	int pBufferSourceLen;
	char *pBufferMessage;
	int pBufferMessageLen;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create engine instance read log. */
	delEngineInstanceReadLog( delEngineInstance &engineInstance, deLogger *logger );
	
	/** \brief Clean up engine instance. */
	virtual ~delEngineInstanceReadLog();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Run thread function. */
	virtual void Run();
	
	/**
	 * \brief Read data from log pipe.
	 * \returns true if new data is available or false if the pipe broke.
	 */
	bool ReadFromPipe( void *data, int length );
	
	/** \brief Prepare source buffer to be at least requiredLength long excluding the 0-termination character. */
	void PrepareBufferSource( int requiredLength );
	
	/** \brief Prepare the message buffer to be at least requiredLength long excluding the 0-termination character. */
	void PrepareBufferMessage( int requiredLength );
	/*@}*/
};

#endif
