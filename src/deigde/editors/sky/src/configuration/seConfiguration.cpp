/* 
 * Drag[en]gine IGDE Sky Editor
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

#include "seConfiguration.h"
#include "seConfigurationXML.h"
#include "../gui/seWindowMain.h"

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

#define FILE_EDITOR_CONFIG_SYSTEM	"/config/system/editors/sky/settings.xml"
#define FILE_EDITOR_CONFIG_USER		"/config/user/editors/sky/settings.xml"

#define LOGSOURCE "Sky Editor"



// Class seConfiguration
//////////////////////////

// Constructor, destructor
////////////////////////////

seConfiguration::seConfiguration( seWindowMain &windowMain ) :
pWindowMain( windowMain ),
pPreventSaving( false ){
}

seConfiguration::~seConfiguration(){
	pCleanUp();
}



// Management
///////////////

void seConfiguration::SetPreventSaving( bool preventSaving ){
	pPreventSaving = preventSaving;
}

void seConfiguration::LoadConfiguration(){
	deVirtualFileSystem &vfs = *pWindowMain.GetEnvironment().GetFileSystemGame();
	
	const decPath pathFile( decPath::CreatePathUnix( "/igde/local/skyEditor.xml" ) );
	if( ! vfs.ExistsFile( pathFile ) || vfs.GetFileType( pathFile ) != deVFSContainer::eftRegularFile ){
		return;
	}
	
	decBaseFileReaderReference reader;
	pPreventSaving = true;
	try{
		reader.TakeOver( vfs.OpenFileForReading( pathFile ) );
		seConfigurationXML( pWindowMain.GetLogger(), LOGSOURCE ).ReadFromFile( reader, *this );
		pPreventSaving = false;
		
	}catch( const deException &e ){
		pPreventSaving = false;
		pWindowMain.GetLogger()->LogException( LOGSOURCE, e );
	}
}

void seConfiguration::SaveConfiguration(){
	if( pPreventSaving ){
		return;
	}
	
	deVirtualFileSystem &vfs = *pWindowMain.GetEnvironment().GetFileSystemGame();
	
	const decPath pathFile( decPath::CreatePathUnix( "/igde/local/skyEditor.xml" ) );
	if( ! vfs.CanWriteFile( pathFile ) ){
		return;
	}
	
	decBaseFileWriterReference writer;
	try{
		writer.TakeOver( vfs.OpenFileForWriting( pathFile ) );
		seConfigurationXML( pWindowMain.GetLogger(), LOGSOURCE ).WriteToFile( writer, *this );
		
	}catch( const deException &e ){
		pWindowMain.GetLogger()->LogException( LOGSOURCE, e );
	}
}



// Private Functions
//////////////////////

void seConfiguration::pCleanUp(){
}
