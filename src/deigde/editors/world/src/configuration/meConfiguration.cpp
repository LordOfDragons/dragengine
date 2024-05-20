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

void meConfiguration::SetEnableAuralization( bool enable ){
	if( enable == pEnableAuralization ){
		return;
	}
	
	pEnableAuralization = enable;
	SaveConfiguration();
	
	pWindowMain.ConfigEnableAuralizationChanged();
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
	pEnableAuralization = false;
	
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
