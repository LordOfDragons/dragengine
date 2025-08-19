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

#include <emscripten.h>

#include "dewlLoggerJS.h"

#include <dragengine/common/exceptions.h>


// Class dewlLoggerJS
///////////////////////

pthread_mutex_t dewlLoggerJS::pMutex = PTHREAD_MUTEX_INITIALIZER;
std::vector<dewlLoggerJS::sLogEntry> dewlLoggerJS::pLogEntries;


// Constructor, destructor
////////////////////////////

dewlLoggerJS::dewlLoggerJS(){
}

dewlLoggerJS::~dewlLoggerJS(){
}


// Management
///////////////

void dewlLoggerJS::LogInfo(const char *source, const char *message){
	DEASSERT_NOTNULL(source)
	DEASSERT_NOTNULL(message)
	
	AddLogEntry(eSeverity::info, source, message);
}

void dewlLoggerJS::LogWarn(const char *source, const char *message){
	DEASSERT_NOTNULL(source)
	DEASSERT_NOTNULL(message)
	
	AddLogEntry(eSeverity::warning, source, message);
}

void dewlLoggerJS::LogError(const char *source, const char *message){
	DEASSERT_NOTNULL(source)
	DEASSERT_NOTNULL(message)
	
	AddLogEntry(eSeverity::error, source, message);
}

void dewlLoggerJS::LogException(const char *source, const deException &exception){
	DEASSERT_NOTNULL(source)
	
	AddLogEntry(eSeverity::error, source, exception.FormatOutput().Join("\n"));
}

void dewlLoggerJS::AddLogEntry(eSeverity severity, const char *source, const char *message){
	if(pthread_mutex_lock(&pMutex) == 0){
		pLogEntries.push_back({severity, source, message});
		pthread_mutex_unlock(&pMutex);
	}
}

void dewlLoggerJS::AddLogEntryFormat(eSeverity severity, const char *source, const char * format, ...){
	DEASSERT_NOTNULL(format)
	
	decString message;
	va_list list;
	
	va_start(list, format);
	message.FormatUsing(format, list);
	va_end(list);
	
	AddLogEntry(severity, source, message);
}

void dewlLoggerJS::MainThreadUpdate(){
	static std::vector<sLogEntry> logEntries;
	
	if(pthread_mutex_trylock(&pMutex) == 0){
		logEntries = pLogEntries;
		pLogEntries.clear();
		pthread_mutex_unlock(&pMutex);
		
	}else{
		logEntries.clear();
		return;
	}
	
	for(auto logEntry : logEntries){
		const int severity = (int)logEntry.severity;
		const char * const source = logEntry.source.c_str();
		const char * const message = logEntry.message.c_str();
		
		EM_ASM({
			this.dispatchEvent(new CustomEvent('delauncher:log', {
				detail: {
					severity: $0,
					source: UTF8ToString($1),
					message: UTF8ToString($2)
				}
			}))
		}, severity, source, message);
	}
}
