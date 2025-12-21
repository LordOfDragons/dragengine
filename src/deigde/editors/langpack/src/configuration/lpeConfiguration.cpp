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

#include "lpeConfiguration.h"
#include "lpeConfigurationXML.h"
#include "../gui/lpeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gameproject/igdeGameProject.h>

#include <dragengine/logger/deLogger.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define FILE_EDITOR_CONFIG_SYSTEM	"/config/system/editors/langpack/settings.xml"
#define FILE_EDITOR_CONFIG_USER		"/config/user/editors/langpack/settings.xml"

#define LOGSOURCE "Language Pack Editor"



// Class lpeConfiguration
//////////////////////////

// Constructor, destructor
////////////////////////////

lpeConfiguration::lpeConfiguration(lpeWindowMain &windowMain) :
pWindowMain(windowMain),
pPreventSaving(false)
{
	pReset();
}

lpeConfiguration::~lpeConfiguration(){
	pCleanUp();
}



// Management
///////////////

void lpeConfiguration::SetPreventSaving(bool preventSaving){
	pPreventSaving = preventSaving;
}

void lpeConfiguration::LoadConfiguration(){
	pPreventSaving = true;
	try{
		deVirtualFileSystem &vfs = *pWindowMain.GetEnvironment().GetFileSystemGame();
		
		pReset();
		pWindowMain.GetRecentFiles().RemoveAllFiles();
		pWindowMain.GetRecentFilesRefLangPack().RemoveAllFiles();
		
		const decPath pathFile(decPath::CreatePathUnix("/igde/local/languagePackEditor.xml"));
		if(!vfs.ExistsFile(pathFile) || vfs.GetFileType(pathFile) != deVFSContainer::eftRegularFile){
			pPreventSaving = false;
			return;
		}
		
		lpeConfigurationXML(pWindowMain.GetLogger(), LOGSOURCE).ReadFromFile(
			vfs.OpenFileForReading(pathFile), *this);
		pPreventSaving = false;
		
	}catch(const deException &e){
		pPreventSaving = false;
		pWindowMain.GetLogger()->LogException(LOGSOURCE, e);
	}
}

void lpeConfiguration::SaveConfiguration(){
	if(pPreventSaving){
		return;
	}
	
	deVirtualFileSystem &vfs = *pWindowMain.GetEnvironment().GetFileSystemGame();
	
	const decPath pathFile(decPath::CreatePathUnix("/igde/local/languagePackEditor.xml"));
	if(!vfs.CanWriteFile(pathFile)){
		return;
	}
	
	decBaseFileWriter::Ref writer;
	try{
		writer = vfs.OpenFileForWriting(pathFile);
		lpeConfigurationXML(pWindowMain.GetLogger(), LOGSOURCE).WriteToFile(writer, *this);
		
	}catch(const deException &e){
		pWindowMain.GetLogger()->LogException(LOGSOURCE, e);
	}
}



// Private Functions
//////////////////////

void lpeConfiguration::pCleanUp(){
}

void lpeConfiguration::pReset(){
}
