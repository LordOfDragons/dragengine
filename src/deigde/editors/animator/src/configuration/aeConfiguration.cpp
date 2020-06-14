/* 
 * Drag[en]gine IGDE Animator Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
