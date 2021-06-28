/* 
 * Drag[en]gine IGDE World Editor
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

#include "meConfiguration.h"
#include "meConfigurationXML.h"
#include "../gui/meWindowMain.h"

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

#define FILE_EDITOR_CONFIG_SYSTEM	"/config/system/editors/world/settings.xml"
#define FILE_EDITOR_CONFIG_USER		"/config/user/editors/world/settings.xml"

#define LOGSOURCE "World Editor"



// Class meConfiguration
//////////////////////////

// Constructor, destructor
////////////////////////////

meConfiguration::meConfiguration( meWindowMain &windowMain ) :
pWindowMain( windowMain ),
pPreventSaving( false )
{
	pReset();
}

meConfiguration::~meConfiguration(){
	pCleanUp();
}



// Management
///////////////

void meConfiguration::SetMoveStep( float step ){
	step = decMath::max( step, 1e-5f );
	if( fabsf( step - pMoveStep ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pMoveStep = step;
	SaveConfiguration();
}

void meConfiguration::SetMoveSnap( bool snap ){
	if( snap == pMoveSnap ){
		return;
	}
	
	pMoveSnap = snap;
	SaveConfiguration();
}

void meConfiguration::SetRotateStep( float step ){
	step = decMath::max( step, 1e-5f );
	if( fabsf( step - pRotStep ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pRotStep = step;
	SaveConfiguration();
}

void meConfiguration::SetRotateSnap( bool snap ){
	if( snap == pRotSnap ){
		return;
	}
	
	pRotSnap = snap;
	SaveConfiguration();
}

void meConfiguration::SetScaleStep( float step ){
	step = decMath::max( step, 1e-5f );
	if( fabsf( step - pScaleStep ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pScaleStep = step;
	SaveConfiguration();
}

void meConfiguration::SetScaleSnap( bool snap ){
	if( snap == pScaleSnap ){
		return;
	}
	
	pScaleSnap = snap;
	SaveConfiguration();
}

void meConfiguration::SetAutoUpdate( bool autoUpdate ){
	if( autoUpdate == pAutoUpdate ){
		return;
	}
	
	pAutoUpdate = autoUpdate;
	SaveConfiguration();
}

void meConfiguration::SetSensitivity( float sensitivity ){
	sensitivity = decMath::max( sensitivity, 0.001f );
	if( fabsf( sensitivity - pSensitivity ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pSensitivity = sensitivity;
	SaveConfiguration();
}



void meConfiguration::SetEnableGI( bool enable ){
	if( enable == pEnableGI ){
		return;
	}
	
	pEnableGI = enable;
	SaveConfiguration();
	
	pWindowMain.ConfigEnableGIChanged();
}



deInputEvent::eKeyCodes meConfiguration::GetHotKeyAt( int hotkey ) const{
	if( hotkey < 0 || hotkey >= EHK_COUNT ){
		DETHROW( deeInvalidParam );
	}
	return pHotKeys[ hotkey ];
}

void meConfiguration::SetHotKeyAt( int hotkey, deInputEvent::eKeyCodes key ){
	if( hotkey < 0 || hotkey >= EHK_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	if( key == pHotKeys[ hotkey ] ){
		return;
	}
	
	pHotKeys[ hotkey ] = key;
	SaveConfiguration();
}



void meConfiguration::SetPreventSaving( bool preventSaving ){
	pPreventSaving = preventSaving;
}

void meConfiguration::LoadConfiguration(){
	pPreventSaving = true;
	try{
		deVirtualFileSystem &vfs = *pWindowMain.GetEnvironment().GetFileSystemGame();
		
		pReset();
		pWindowMain.GetRecentFiles().RemoveAllFiles();
		
		const decPath pathFile( decPath::CreatePathUnix( "/igde/local/worldEditor.xml" ) );
		if( ! vfs.ExistsFile( pathFile ) || vfs.GetFileType( pathFile ) != deVFSContainer::eftRegularFile ){
			pPreventSaving = false;
			return;
		}
		
		decBaseFileReaderReference reader;
		reader.TakeOver( vfs.OpenFileForReading( pathFile ) );
		meConfigurationXML( pWindowMain.GetLogger(), LOGSOURCE ).ReadFromFile( reader, *this );
		pPreventSaving = false;
		
	}catch( const deException &e ){
		pPreventSaving = false;
		pWindowMain.GetLogger()->LogException( LOGSOURCE, e );
	}
}

void meConfiguration::SaveConfiguration(){
	if( pPreventSaving ){
		return;
	}
	
	deVirtualFileSystem &vfs = *pWindowMain.GetEnvironment().GetFileSystemGame();
	
	const decPath pathFile( decPath::CreatePathUnix( "/igde/local/worldEditor.xml" ) );
	if( ! vfs.CanWriteFile( pathFile ) ){
		return;
	}
	
	decBaseFileWriterReference writer;
	try{
		writer.TakeOver( vfs.OpenFileForWriting( pathFile ) );
		meConfigurationXML( pWindowMain.GetLogger(), LOGSOURCE ).WriteToFile( writer, *this );
		
	}catch( const deException &e ){
		pWindowMain.GetLogger()->LogException( LOGSOURCE, e );
	}
}



// Private Functions
//////////////////////

void meConfiguration::pCleanUp(){
}

void meConfiguration::pReset(){
	pMoveStep = 0.05f;
	pMoveSnap = true;
	
	pRotStep = 15.0f;
	pRotSnap = true;
	
	pScaleStep = 0.25f;
	pScaleSnap = true;
	
	pAutoUpdate = false;
	pSensitivity = 1.0f;
	
	pEnableGI = false;
	
	pHotKeys[ ehkSelectWorkMode ] = deInputEvent::ekcW;
	pHotKeys[ ehkSelectElementMode ] = deInputEvent::ekcE;
	
	pHotKeys[ ehkWorldMove ] = deInputEvent::ekcG;
	pHotKeys[ ehkWorldRotate ] = deInputEvent::ekcR;
	pHotKeys[ ehkWorldScale ] = deInputEvent::ekcS;
	
	pHotKeys[ ehkEditLockX ] = deInputEvent::ekcX;
	pHotKeys[ ehkEditLockY ] = deInputEvent::ekcY;
	pHotKeys[ ehkEditLockZ ] = deInputEvent::ekcZ;
	pHotKeys[ ehkEditLocalCFrame ] = deInputEvent::ekcL;
	pHotKeys[ ehkEditSnapToSnapPoints ] = deInputEvent::ekcP;
}
