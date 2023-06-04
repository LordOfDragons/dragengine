/* 
 * Drag[en]gine IGDE
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
#include <sys/types.h>

#include "igdeConfigurationLocal.h"
#include "igdeConfigurationLocalXML.h"
#include "../gui/igdeWindowMain.h"
#include "../module/igdeEditorModuleDefinition.h"
#include "../module/igdeEditorModuleManager.h"

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

#define LOGSOURCE "IGDE"



// Class igdeConfigurationLocal
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeConfigurationLocal::igdeConfigurationLocal( igdeWindowMain &windowMain ) :
pWindowMain( windowMain ),
pRecentEditorFiles( windowMain.GetEnvironment(), true, 20 ),
pPreventSaving( false ){
}

igdeConfigurationLocal::~igdeConfigurationLocal(){
}



// Management
///////////////

void igdeConfigurationLocal::SetPreventSaving( bool preventSaving ){
	pPreventSaving = preventSaving;
}

void igdeConfigurationLocal::LoadConfiguration(){
	pPreventSaving = true;
	try{
		deVirtualFileSystem &vfs = *pWindowMain.GetEnvironment().GetFileSystemGame();
		
		pReset();
		pRecentEditorFiles.RemoveAllFiles();
		pWindowMain.GetModuleManager().ResetRecentUsedPosition();
		pWindowMain.GetModuleManager().ActivateProjectManager();
		
		const decPath pathFile( decPath::CreatePathUnix( "/igde/local/igde.xml" ) );
		if( ! vfs.ExistsFile( pathFile ) || vfs.GetFileType( pathFile ) != deVFSContainer::eftRegularFile ){
			pPreventSaving = false;
			return;
		}
		
		decBaseFileReaderReference reader;
		reader.TakeOver( vfs.OpenFileForReading( pathFile ) );
		igdeConfigurationLocalXML( pWindowMain.GetLogger(), LOGSOURCE ).ReadFromFile( reader, *this );
		pPreventSaving = false;
		
	}catch( const deException &e ){
		pPreventSaving = false;
		pWindowMain.GetLogger()->LogException( LOGSOURCE, e );
	}
}

void igdeConfigurationLocal::SaveConfiguration(){
	if( pPreventSaving ){
		return;
	}
	
	deVirtualFileSystem &vfs = *pWindowMain.GetEnvironment().GetFileSystemGame();
	
	const decPath pathFile( decPath::CreatePathUnix( "/igde/local/igde.xml" ) );
	if( ! vfs.CanWriteFile( pathFile ) ){
		return;
	}
	
	decBaseFileWriterReference writer;
	try{
		writer.TakeOver( vfs.OpenFileForWriting( pathFile ) );
		igdeConfigurationLocalXML( pWindowMain.GetLogger(), LOGSOURCE ).WriteToFile( writer, *this );
		
	}catch( const deException &e ){
		pWindowMain.GetLogger()->LogException( LOGSOURCE, e );
	}
}



// Private Functions
//////////////////////

void igdeConfigurationLocal::pReset(){
}
