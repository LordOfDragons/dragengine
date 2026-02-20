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

#ifndef _DECLLOGGERFILTERED_H_
#define _DECLLOGGERFILTERED_H_

#include <dragengine/logger/deLogger.h>



/**
 * A logger passing messages to other loggers depending on the message type.
 */
class declLoggerFiltered : public deLogger{
public:
	/** Type holding strong reference. */
	using Ref = deTObjectReference<declLoggerFiltered>;
	
	
private:
	deLogger::Ref pLoggerInfo;
	deLogger::Ref pLoggerWarn;
	deLogger::Ref pLoggerError;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create logger. */
	declLoggerFiltered();
	
protected:
	/** Clean up logger. */
	~declLoggerFiltered() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Logger to pass information to or NULL to ignore them. */
	inline const deLogger::Ref &GetLoggerInfo() const{ return pLoggerInfo; }
	
	/** Set logger to pass information to or NULL to ignore them. */
	void SetLoggerInfo(deLogger *logger);
	
	/** Logger to pass warnings to or NULL to ignore them. */
	inline const deLogger::Ref &GetLoggerWarning() const{ return pLoggerWarn; }
	
	/** Set logger to pass warnings to or NULL to ignore them. */
	void SetLoggerWarning(deLogger *logger);
	
	/** Logger to pass errors to or NULL to ignore them. */
	inline const deLogger::Ref &GetLoggerError() const{ return pLoggerError; }
	
	/** Set logger to pass errors to or NULL to ignore them. */
	void SetLoggerError(deLogger *logger);
	
	
	
	/** Log an information message. */
	void LogInfo(const char *source, const char *message) override;
	
	/** Log a formated information message. */
	void LogInfoFormat(const char *source, const char *message, ...) override
		#ifdef __GNUC__
		__attribute__ ((format (printf, 3, 4)))
		#endif
		;
	
	/** Log a formated information message. */
	void LogInfoFormatUsing(const char *source, const char *message, va_list args) override;
	
	/** Log a warning message. */
	void LogWarn(const char *source, const char *message) override;
	
	/** Log a formated warning message. */
	void LogWarnFormat(const char *source, const char *message, ...) override
		#ifdef __GNUC__
		__attribute__ ((format (printf, 3, 4)))
		#endif
		;
		
	/** Log a formated warning message. */
	void LogWarnFormatUsing(const char *source, const char *message, va_list args) override;
	
	/** Log an error message. */
	void LogError(const char *source, const char *message) override;
	
	/** Log a formated error message. */
	void LogErrorFormat(const char *source, const char *message, ...) override
		#ifdef __GNUC__
		__attribute__ ((format (printf, 3, 4)))
		#endif
		;
		
	/** Log a formated error message. */
	void LogErrorFormatUsing(const char *source, const char *message, va_list args) override;
	
	/** Log an exception as error message. */
	void LogException(const char *source, const deException &exception) override;
	/*@}*/
};

#endif
