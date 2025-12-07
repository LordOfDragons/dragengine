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

#include "projConfiguration.h"
#include "projConfigurationXml.h"
#include "../gui/projWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/module/igdeEditorModule.h>
#include <deigde/gameproject/igdeGameProject.h>

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVFSContainer.h>


// Definitions
////////////////

#define LOGSOURCE "Project Editor"


// Class projConfiguration
////////////////////////////

// Constructor, destructor
////////////////////////////

projConfiguration::projConfiguration(projWindowMain &windowMain) :
pWindowMain(windowMain),
pPreventSaving(false)
{
	pReset();
}

projConfiguration::~projConfiguration(){
}



// Management
///////////////

void projConfiguration::SetPreventSaving(bool preventSaving){
	pPreventSaving = preventSaving;
}

void projConfiguration::SetRemoteAddress(const decString &address){
	pRemoteAddress = address;
}

void projConfiguration::LoadConfiguration(){
	pPreventSaving = true;
	try{
		deVirtualFileSystem &vfs = *pWindowMain.GetEnvironment().GetFileSystemGame();
		
		pReset();
		pWindowMain.GetRecentFiles().RemoveAllFiles();
		
		const decPath pathFile(decPath::CreatePathUnix("/igde/local/projectEditor.xml"));
		if(!vfs.ExistsFile(pathFile) || vfs.GetFileType(pathFile) != deVFSContainer::eftRegularFile){
			pPreventSaving = false;
			return;
		}
		
		projConfigurationXml(pWindowMain.GetLogger(), LOGSOURCE).ReadFromFile(
			decBaseFileReader::Ref::New(vfs.OpenFileForReading(pathFile)), *this);
		pPreventSaving = false;
		
	}catch(const deException &e){
		pPreventSaving = false;
		pWindowMain.GetLogger()->LogException(LOGSOURCE, e);
	}
}

void projConfiguration::SaveConfiguration(){
	if(pPreventSaving){
		return;
	}
	
	deVirtualFileSystem &vfs = *pWindowMain.GetEnvironment().GetFileSystemGame();
	
	const decPath pathFile(decPath::CreatePathUnix("/igde/local/projectEditor.xml"));
	if(!vfs.CanWriteFile(pathFile)){
		return;
	}
	
	try{
		projConfigurationXml(pWindowMain.GetLogger(), LOGSOURCE).WriteToFile(
			decBaseFileWriter::Ref::New(vfs.OpenFileForWriting(pathFile)), *this);
		
	}catch(const deException &e){
		pWindowMain.GetLogger()->LogException(LOGSOURCE, e);
	}
}


// Private Functions
//////////////////////

void projConfiguration::pReset(){
	pRemoteAddress = "localhost";
}
