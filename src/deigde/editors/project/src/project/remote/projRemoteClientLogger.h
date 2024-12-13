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

#ifndef _PROJREMOTECLIENTLOGGER_H_
#define _PROJREMOTECLIENTLOGGER_H_

#include <denetwork/denLogger.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/filesystem/deVFSContainer.h>


/**
 * \brief Remote client logger.
 */
class projRemoteClientLogger : public denLogger{
private:
	const decString pLogSource;
	deVFSContainer::Ref pContainer;
	deLogger::Ref pLogger;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create remote logger. */
	projRemoteClientLogger(const char *logSource, const char *pathLogFile);
	
	/** \brief Clean up remote server. */
	~projRemoteClientLogger() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Logging. */
	void Log(LogSeverity severity, const std::string &message) override;
	/*@}*/
};

#endif
