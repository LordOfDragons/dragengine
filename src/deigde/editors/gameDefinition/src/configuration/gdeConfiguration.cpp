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

#include "gdeConfiguration.h"
#include "gdeConfigurationXML.h"
#include "../gdEditor.h"
#include "../gui/gdeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gameproject/igdeGameProject.h>

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decBaseFileWriterReference.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>



// Definitions
////////////////

#define FILE_EDITOR_CONFIG_SYSTEM	"/config/system/editors/gameDefinition/settings.xml"
#define FILE_EDITOR_CONFIG_USER		"/config/user/editors/gameDefinition/settings.xml"



// Class gdeConfiguration
//////////////////////////

// Constructor, destructor
////////////////////////////

#define COLOR_ALPHA				0.1f
#define COLOR_ALPHA_ACTIVE		0.2f

gdeConfiguration::gdeConfiguration( gdeWindowMain &windowMain ) :
pWindowMain( windowMain ),
pPreventSaving( false )
{
	pReset();
}

gdeConfiguration::~gdeConfiguration(){
	pCleanUp();
}



// Management
///////////////

void gdeConfiguration::SetPreventSaving( bool preventSaving ){
	pPreventSaving = preventSaving;
}

void gdeConfiguration::LoadConfiguration(){
	pPreventSaving = true;
	try{
		deVirtualFileSystem &vfs = *pWindowMain.GetEnvironment().GetFileSystemGame();
		
		pReset();
		pWindowMain.GetRecentFiles().RemoveAllFiles();
		
		const decPath pathFile( decPath::CreatePathUnix( "/igde/local/gameDefinitionEditor.xml" ) );
		if( ! vfs.ExistsFile( pathFile ) || vfs.GetFileType( pathFile ) != deVFSContainer::eftRegularFile ){
			pPreventSaving = false;
			return;
		}
		
		decBaseFileReaderReference reader;
		reader.TakeOver( vfs.OpenFileForReading( pathFile ) );
		gdeConfigurationXML( pWindowMain.GetLogger(), LOGSOURCE ).ReadFromFile( reader, *this );
		pPreventSaving = false;
		
	}catch( const deException &e ){
		pPreventSaving = false;
		pWindowMain.GetLogger()->LogException( LOGSOURCE, e );
	}
}

void gdeConfiguration::SaveConfiguration(){
	if( pPreventSaving ){
		return;
	}
	
	deVirtualFileSystem &vfs = *pWindowMain.GetEnvironment().GetFileSystemGame();
	
	const decPath pathFile( decPath::CreatePathUnix( "/igde/local/gameDefinitionEditor.xml" ) );
	if( ! vfs.CanWriteFile( pathFile ) ){
		return;
	}
	
	decBaseFileWriterReference writer;
	try{
		writer.TakeOver( vfs.OpenFileForWriting( pathFile ) );
		gdeConfigurationXML( pWindowMain.GetLogger(), LOGSOURCE ).WriteToFile( writer, *this );
		
	}catch( const deException &e ){
		pWindowMain.GetLogger()->LogException( LOGSOURCE, e );
	}
}



// Private Functions
//////////////////////

void gdeConfiguration::pCleanUp(){
}

void gdeConfiguration::pReset(){
	pColorBoundingBox.Set( 0.75f, 0.25f, 0.25f, COLOR_ALPHA );
	
	pColorCamera.Set( 0.0f, 0.0f, 1.0f, COLOR_ALPHA );
	pColorCameraActive.Set( 0.0f, 0.5f, 1.0f, COLOR_ALPHA_ACTIVE );
	
	pColorEnvMapProbe.Set( 0.5f, 0.5f, 0.5f, COLOR_ALPHA );
	pColorEnvMapProbeReflection.Set( 0.65f, 0.35f, 0.35f, COLOR_ALPHA );
	pColorEnvMapProbeInfluence.Set( 0.35f, 0.65f, 0.35f, COLOR_ALPHA );
	pColorEnvMapProbeMask.Set( 0.35f, 0.35f, 0.65f, COLOR_ALPHA );
	pColorEnvMapProbeActive.Set( 0.7f, 0.7f, 0.7f, COLOR_ALPHA_ACTIVE );
	pColorEnvMapProbeActiveReflection.Set( 0.85f, 0.55f, 0.55f, COLOR_ALPHA_ACTIVE );
	pColorEnvMapProbeActiveInfluence.Set( 0.55f, 0.85f, 0.55f, COLOR_ALPHA_ACTIVE );
	pColorEnvMapProbeActiveMask.Set( 0.55f, 0.55f, 0.85f, COLOR_ALPHA_ACTIVE );
	
	pColorLight.Set( 1.0f, 1.0f, 0.0f, COLOR_ALPHA );
	pColorLightActive.Set( 1.0f, 1.0f, 0.25f, COLOR_ALPHA_ACTIVE );
	
	pColorNavigationSpace.Set( 0.0f, 0.25f, 1.0f, COLOR_ALPHA );
	pColorNavigationSpaceActive.Set( 0.0f, 0.5f, 1.0f, COLOR_ALPHA_ACTIVE );
	
	pColorNavigationBlocker.Set( 0.0f, 0.5f, 0.0f, COLOR_ALPHA );
	pColorNavigationBlockerActive.Set( 0.0f, 0.5f, 0.25f, COLOR_ALPHA_ACTIVE );
	
	pColorParticleEmitter.Set( 0.5f, 0.0f, 0.25f, COLOR_ALPHA );
	pColorParticleEmitterActive.Set( 1.0f, 0.0f, 0.5f, COLOR_ALPHA_ACTIVE );
	
	pColorForceField.Set( 0.5f, 0.25f, 0.25f, COLOR_ALPHA );
	pColorForceFieldActive.Set( 1.0f, 0.25f, 0.5f, COLOR_ALPHA_ACTIVE );
	
	pColorSnapPoint.Set( 0.0f, 0.5f, 0.0f, COLOR_ALPHA );
	pColorSnapPointActive.Set( 0.0f, 0.75f, 0.0f, COLOR_ALPHA_ACTIVE );
	
	pColorSpeaker.Set( 0.5f, 0.0f, 0.0f, COLOR_ALPHA );
	pColorSpeakerActive.Set( 0.75f, 0.0f, 0.0f, COLOR_ALPHA_ACTIVE );
}
