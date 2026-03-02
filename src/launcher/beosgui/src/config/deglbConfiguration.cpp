/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deglbConfigXML.h"
#include "deglbConfiguration.h"
#include "../deglbLauncher.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>


// Definitions
////////////////

#define FILE_LAUNCHER_CONFIG_SYSTEM		"/config/system/delaunchergui.xml"
#define FILE_LAUNCHER_CONFIG_USER		"/config/user/delaunchergui.xml"


// Class deglbConfiguration
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deglbConfiguration::deglbConfiguration(deglbLauncher &launcher) :
pLauncher(launcher),
pCanSave(false),
pClrValidBack({87, 217, 87, 255}),
pClrValidText({0, 0, 0, 255}),
pClrProblemBack({255, 128, 128, 255}),
pClrProblemText({0, 0, 0, 255}){
}

deglbConfiguration::~deglbConfiguration() = default;



// Management
///////////////

void deglbConfiguration::LoadConfiguration(){
	const decString &logSource = pLauncher.GetLogSource();
	deglbConfigXML configXML(pLauncher.GetLogger(), logSource);
	deVirtualFileSystem &vfs = pLauncher.GetVFS();
	deLogger &logger = pLauncher.GetLogger();
	decPath pathFile;
	
	pathFile.SetFromUnix(FILE_LAUNCHER_CONFIG_SYSTEM);
	
	if(vfs.ExistsFile(pathFile)){
		if(vfs.GetFileType(pathFile) == deVFSContainer::eftRegularFile){
			logger.LogInfo(logSource, "Reading system configuration file");
			configXML.ReadFromFile(vfs.OpenFileForReading(pathFile), *this);
			
		}else{
			logger.LogError(logSource, "System configuration file is not a regular file");
			DETHROW_INFO(deeInvalidParam, "System configuration file is not a regular file");
		}
		
	}else{
		logger.LogInfo(logSource, "System configuration file not found, skipped");
	}
	
	pathFile.SetFromUnix(FILE_LAUNCHER_CONFIG_USER);
	
	if(vfs.ExistsFile(pathFile)){
		if(vfs.GetFileType(pathFile) == deVFSContainer::eftRegularFile){
			logger.LogInfo(logSource, "Reading user configuration file");
			configXML.ReadFromFile(vfs.OpenFileForReading(pathFile), *this);
			
		}else{
			logger.LogError(logSource, "User configuration file is not a regular file");
			DETHROW_INFO(deeInvalidParam, "User configuration file is not a regular file");
		}
		
	}else{
		logger.LogInfo(logSource, "User configuration file not found, will be created upon exiting");
	}
	
	pCanSave = true;
}

void deglbConfiguration::SaveConfiguration(){
	if(!pCanSave){
		return;
	}
	
	const decString &logSource = pLauncher.GetLogSource();
	deLogger &logger = pLauncher.GetLogger();
	
	deglbConfigXML configXML(pLauncher.GetLogger(), logSource);
	deVirtualFileSystem &vfs = pLauncher.GetVFS();
	decPath pathFile;
	
	pathFile.SetFromUnix(FILE_LAUNCHER_CONFIG_USER);
	
	if(vfs.CanWriteFile(pathFile)){
		logger.LogInfo(logSource, "Writing user configuration file");
		
		try{
			configXML.WriteToFile(vfs.OpenFileForWriting(pathFile), *this);
			
		}catch(const deException &){
			logger.LogError(logSource,
				"Failed to write user configuration file (file permission problem)");
		}
		
	}else{
		logger.LogError(logSource,
			"Failed to write user configuration file (file writing problem)");
	}
}
