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

#ifndef _DELOGGERFILE_H_
#define _DELOGGERFILE_H_

#include "deLogger.h"
#include "../threading/deMutex.h"

#include "../common/file/decBaseFileWriter.h"


/**
 * \brief Logs to file.
 * 
 * Adds a prefix depending on the log message type. The file is given by a file writer.
 * The file writer object is owned by the file logger. It is therefore deleted when
 * the file logger is freed.
 * 
 * \note Logger console is thread safe. To avoid torn logs the entire text line is
 * formated in memory and send as one write call then fflush is called.
 * Be careful with the use of the file writer outside the logger. deObject
 * reference counting is not thread safe.
 */
class DE_DLL_EXPORT deLoggerFile : public deLogger{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deLoggerFile> Ref;
	
	
private:
	decBaseFileWriter::Ref pWriter;
	deMutex pMutex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file logger. */
	deLoggerFile(decBaseFileWriter *writer);
	
protected:
	/**
	 * \brief Clean up file logger.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deLoggerFile() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief File writer. */
	inline const decBaseFileWriter::Ref &GetWriter() const{ return pWriter; }
	
	
	
	/** \brief Log information message. */
	void LogInfo(const char *source, const char *message) override;
	
	/** \brief Log warning message. */
	void LogWarn(const char *source, const char *message) override;
	
	/** \brief Log error message. */
	void LogError(const char *source, const char *message) override;
	/*@}*/
	
	
	
protected:
	inline deMutex &GetMutex(){ return pMutex; }
	void LogPrefix(const char *source, const char *message, const char *prefix);
};

#endif
