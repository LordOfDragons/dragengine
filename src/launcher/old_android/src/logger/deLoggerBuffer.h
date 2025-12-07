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

#ifndef _DELOGGERBUFFER_H_
#define _DELOGGERBUFFER_H_

#include "deLogger.h"
#include "../common/string/decString.h"
#include "../threading/deMutex.h"



/**
 * \brief Logger buffer.
 * 
 * Buffers messages to send them at a later time to another logger. Allows for example
 * collecting log messages in parallel tasks and sending them to a main thread logger
 * during synchronization time. The logger buffer is not thread safe. It only provides
 * a simple to use buffer for log messages.
 * 
 * \note Logger buffer is thread safe.
 */
class deLoggerBuffer : public deLogger{
public:
	/** \brief Message types. */
	enum eMessageTypes{
		/** \brief Information. */
		emtInfo,
		
		/** \brief Warning. */
		emtWarn,
		
		/** \brief Error. */
		emtError
	};
	
	/** \brief Logger buffer entry. */
	struct sEntry{
		/** \brief Type. */
		eMessageTypes type;
		
		/** \brief Source. */
		decString source;
		
		/** \brief Message. */
		decString message;
		
		/** \brief Copy constructor. */
		sEntry();
		
		/** \brief Copy constructor. */
		sEntry(const sEntry &entry);
		
		/** \brief Assign entry. */
		sEntry &operator=(const sEntry &entry);
	};
	
	
	
private:
	sEntry *pEntries;
	int pEntrySize;
	int pEntryCount;
	deMutex pMutex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create logger buffer. */
	deLoggerBuffer();
	
protected:
	/**
	 * \brief Clean up logger buffer.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deLoggerBuffer() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Number of entries. */
	int GetEntryCount();
	
	/** \brief Copy of entry at index. */
	sEntry GetEntryAt(int index);
	
	/** \brief Add entry. */
	void AddEntry(eMessageTypes type, const decString &source, const decString &message);
	
	/** \brief Clear buffer. */
	void Clear();
	
	/** \brief Send messages in the buffer to a logger and clear the buffer afterwards. */
	void SendToLogger(deLogger &logger);
	
	
	
	/** \brief Log information message. */
	void LogInfo(const char *source, const char *message) override;
	
	/** \brief Log warning message. */
	void LogWarn(const char *source, const char *message) override;
	
	/** \brief Log error message. */
	void LogError(const char *source, const char *message) override;
	/*@}*/
	
	
	
protected:
	inline deMutex &GetMutex(){ return pMutex; }
	
	
	
private:
	void pClear();
	void pAddEntry(eMessageTypes type, const decString &source, const decString &message);
};

#endif
