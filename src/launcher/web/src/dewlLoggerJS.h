/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEWLLOGGERJS_H_
#define _DEWLLOGGERJS_H_

#include <pthread.h>
#include <string>
#include <vector>
#include <dragengine/logger/deLogger.h>


/**
 * Logger passing messages to other javascript.
 */
class dewlLoggerJS : public deLogger{
public:
	typedef deTObjectReference<dewlLoggerJS> Ref;
	
	enum class eSeverity{
		info,
		warning,
		error
	};
	
	struct sLogEntry{
		eSeverity severity;
		std::string source;
		std::string message;
	};
	
	
private:
	static pthread_mutex_t pMutex;
	static std::vector<sLogEntry> pLogEntries;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create logger. */
	dewlLoggerJS();
	
protected:
	/** Clean up logger. */
	~dewlLoggerJS() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Log an information message. */
	void LogInfo(const char *source, const char *message) override;
	
	/** Log a warning message. */
	void LogWarn(const char *source, const char *message) override;
	
	/** Log an error message. */
	void LogError(const char *source, const char *message) override;
	
	/** Log an exception as error message. */
	void LogException(const char *source, const deException &exception) override;
	
	/** Add log entry. */
	static void AddLogEntry(eSeverity severity, const char *source, const char *message);
	
	/** Add log entry. */
	static void AddLogEntryFormat(eSeverity severity, const char *source, const char *format, ...);
	
	/** Main thread update. */
	static void MainThreadUpdate();
	/*@}*/
};

#endif
