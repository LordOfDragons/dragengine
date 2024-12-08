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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "projRemoteClientLogger.h"
#include "../projProject.h"

#include <dragengine/logger/deLoggerFile.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>


// Class projRemoteClientLogger
/////////////////////////////////

// Constructor, destructor
////////////////////////////

projRemoteClientLogger::projRemoteClientLogger(const char *logSource, const char *pathLogFile) :
pLogSource(logSource)
{
	decPath diskPath(decPath::CreatePathNative(pathLogFile));
	decPath filePath;
	filePath.AddComponent(diskPath.GetLastComponent());
	diskPath.RemoveLastComponent();
	
	pContainer.TakeOver(new deVFSDiskDirectory(diskPath));
	pLogger.TakeOver(new deLoggerFile(decBaseFileWriter::Ref::New(
		pContainer->OpenFileForWriting(filePath))));
}

projRemoteClientLogger::~projRemoteClientLogger(){
}

// Management
///////////////

void projRemoteClientLogger::Log(LogSeverity severity, const std::string &message){
	switch(severity){
	case LogSeverity::error:
		pLogger->LogError(pLogSource, message.c_str());
		break;
		
	case LogSeverity::warning:
		pLogger->LogWarn(pLogSource, message.c_str());
		break;
		
	case LogSeverity::info:
	case LogSeverity::debug:
		pLogger->LogInfo(pLogSource, message.c_str());
		break;
	}
}
