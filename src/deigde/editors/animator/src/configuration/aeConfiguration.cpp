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

#include "aeConfiguration.h"
#include "aeConfigurationXML.h"
#include "../gui/aeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gameproject/igdeGameProject.h>

#include <dragengine/logger/deLogger.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decBaseFileWriterReference.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define FILE_EDITOR_CONFIG_SYSTEM	"/config/system/editors/animator/settings.xml"
#define FILE_EDITOR_CONFIG_USER		"/config/user/editors/animator/settings.xml"

#define LOGSOURCE "Animator Editor"



// Class aeConfiguration
//////////////////////////

// Constructor, destructor
////////////////////////////

aeConfiguration::aeConfiguration( aeWindowMain &windowMain ) :
pWindowMain( windowMain ),
pPreventSaving( false )
{
	pReset();
}

aeConfiguration::~aeConfiguration(){
	pCleanUp();
}



// Management
///////////////

void aeConfiguration::SetLocoKeyForward( deInputEvent::eKeyCodes key ){
	pLocoKeyForward = key;
}

void aeConfiguration::SetLocoKeyBackwards( deInputEvent::eKeyCodes key ){
	pLocoKeyBackwards = key;
}

void aeConfiguration::SetLocoKeyLeft( deInputEvent::eKeyCodes key ){
	pLocoKeyLeft = key;
}

void aeConfiguration::SetLocoKeyRight( deInputEvent::eKeyCodes key ){
	pLocoKeyRight = key;
}

void aeConfiguration::SetLocoKeyCrouch( deInputEvent::eKeyCodes key ){
	pLocoKeyCrouch = key;
}

void aeConfiguration::SetLocoKeyRun( deInputEvent::eKeyCodes key ){
	pLocoKeyRun = key;
}



void aeConfiguration::SetPreventSaving( bool preventSaving ){
	pPreventSaving = preventSaving;
}



void aeConfiguration::LoadConfiguration(){
	pPreventSaving = true;
	try{
		deVirtualFileSystem &vfs = *pWindowMain.GetEnvironment().GetFileSystemGame();
		
		pReset();
		pWindowMain.GetRecentFiles().RemoveAllFiles();
		
		const decPath pathFile( decPath::CreatePathUnix( "/igde/local/animatorEditor.xml" ) );
		if( ! vfs.ExistsFile( pathFile ) || vfs.GetFileType( pathFile ) != deVFSContainer::eftRegularFile ){
			pPreventSaving = false;
			return;
		}
		
		decBaseFileReaderReference reader;
		reader.TakeOver( vfs.OpenFileForReading( pathFile ) );
		aeConfigurationXML( pWindowMain.GetLogger(), LOGSOURCE ).ReadFromFile( reader, *this );
		pPreventSaving = false;
		
	}catch( const deException &e ){
		pPreventSaving = false;
		pWindowMain.GetLogger()->LogException( LOGSOURCE, e );
	}
}

void aeConfiguration::SaveConfiguration(){
	if( pPreventSaving ){
		return;
	}
	
	deVirtualFileSystem &vfs = *pWindowMain.GetEnvironment().GetFileSystemGame();
	
	const decPath pathFile( decPath::CreatePathUnix( "/igde/local/animatorEditor.xml" ) );
	if( ! vfs.CanWriteFile( pathFile ) ){
		return;
	}
	
	decBaseFileWriterReference writer;
	try{
		writer.TakeOver( vfs.OpenFileForWriting( pathFile ) );
		aeConfigurationXML( pWindowMain.GetLogger(), LOGSOURCE ).WriteToFile( writer, *this );
		
	}catch( const deException &e ){
		pWindowMain.GetLogger()->LogException( LOGSOURCE, e );
	}
}



// Private Functions
//////////////////////

void aeConfiguration::pCleanUp(){
}

void aeConfiguration::pReset(){
	pLocoKeyForward = deInputEvent::ekcW;
	pLocoKeyBackwards = deInputEvent::ekcS;
	pLocoKeyLeft = deInputEvent::ekcA;
	pLocoKeyRight = deInputEvent::ekcD;
	pLocoKeyCrouch = deInputEvent::ekcQ;
	pLocoKeyRun = deInputEvent::ekcE;
}
