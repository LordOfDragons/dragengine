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
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decBaseFileWriter.h>
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
		
		decBaseFileReader::Ref reader;
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
	
	decBaseFileWriter::Ref writer;
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
