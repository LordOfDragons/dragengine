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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "declConfigXML.h"
#include "declConfiguration.h"
#include "../declLauncher.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>


// Definitions
////////////////

#define FILE_LAUNCHER_CONFIG_SYSTEM		"/config/system/delauncherconsole.xml"
#define FILE_LAUNCHER_CONFIG_USER		"/config/user/delauncherconsole.xml"

#define LOGSOURCE "Launcher"



// Class declConfiguration
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

declConfiguration::declConfiguration(declLauncher &launcher) :
pLauncher(launcher),
pCanSave(false){
}

declConfiguration::~declConfiguration(){
}



// Management
///////////////

void declConfiguration::LoadConfiguration(){
	declConfigXML configXML(pLauncher.GetLogger(), LOGSOURCE);
	deVirtualFileSystem &vfs = *pLauncher.GetVFS();
	deLogger &logger = *pLauncher.GetLogger();
	decPath pathFile;
	
	// read the system wide config file if existing
	pathFile.SetFromUnix(FILE_LAUNCHER_CONFIG_SYSTEM);
	
	if(vfs.ExistsFile(pathFile)){
		if(vfs.GetFileType(pathFile) == deVFSContainer::eftRegularFile){
			logger.LogInfo(LOGSOURCE, "Reading system configuration file");
			configXML.ReadFromFile(decBaseFileReader::Ref::New(vfs.OpenFileForReading(pathFile)), *this);
			
		}else{
			logger.LogError(LOGSOURCE, "System configuration file is not a regular file");
			DETHROW(deeInvalidParam);
		}
		
	}else{
		logger.LogInfo(LOGSOURCE, "System configuration file not found, skipped");
	}
	
	// read the user config file if existing
	pathFile.SetFromUnix(FILE_LAUNCHER_CONFIG_USER);
	
	if(vfs.ExistsFile(pathFile)){
		if(vfs.GetFileType(pathFile) == deVFSContainer::eftRegularFile){
			logger.LogInfo(LOGSOURCE, "Reading user configuration file");
			configXML.ReadFromFile(decBaseFileReader::Ref::New(vfs.OpenFileForReading(pathFile)), *this);
			
		}else{
			logger.LogError(LOGSOURCE, "User configuration file is not a regular file");
			DETHROW(deeInvalidParam);
		}
		
	}else{
		logger.LogInfo(LOGSOURCE, "User configuration file not found, will be created upon exiting");
	}
	
	pCanSave = true;
}

void declConfiguration::SaveConfiguration(){
	if(! pCanSave){
		return;
	}
	
	declConfigXML configXML(pLauncher.GetLogger(), LOGSOURCE);
	deVirtualFileSystem &vfs = *pLauncher.GetVFS();
	deLogger &logger = *pLauncher.GetLogger();
	decPath pathFile;
	
	pathFile.SetFromUnix(FILE_LAUNCHER_CONFIG_USER);
	if(vfs.CanWriteFile(pathFile)){
		logger.LogInfo(LOGSOURCE, "Writing user configuration file");
		
		try{
			configXML.WriteToFile(decBaseFileWriter::Ref::New(vfs.OpenFileForWriting(pathFile)), *this);
			
		}catch(const deException &){
			logger.LogError(LOGSOURCE, "Failed to write user configuration file (file permission problem)");
		}
		
	}else{
		logger.LogError(LOGSOURCE, "Failed to write user configuration file (file writing problem)");
	}
}
