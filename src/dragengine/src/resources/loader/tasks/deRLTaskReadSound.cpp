/* 
 * Drag[en]gine Game Engine
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
#include <string.h>

#include "deRLTaskReadSound.h"
#include "../deResourceLoader.h"
#include "../../sound/deSound.h"
#include "../../sound/deSoundManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decBaseFileReader.h"
#include "../../../common/file/decBaseFileReaderReference.h"
#include "../../../common/file/decPath.h"
#include "../../../filesystem/deVirtualFileSystem.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/deAudioSystem.h"
#include "../../../systems/deSynthesizerSystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/sound/deBaseSoundModule.h"
#include "../../../systems/modules/sound/deBaseSoundInfo.h"



// Class deRLTaskReadSound
////////////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskReadSound::deRLTaskReadSound( deEngine &engine, deResourceLoader &resourceLoader,
deVirtualFileSystem *vfs, const char *path, deSound *sound ) :
deResourceLoaderTask( engine, resourceLoader, vfs, path, deResourceLoader::ertSound ),
pSucceeded( false )
{
	LogCreateEnter();
	// if already loaded set finished
	if( sound ){
		pSound = sound;
		SetResource( sound );
		SetState( esSucceeded );
		pSucceeded = true;
		SetFinished();
		return;
	}
	
	pSound.TakeOver( new deSound( engine.GetSoundManager(), vfs, path, 0 ) );
	LogCreateExit();
}

deRLTaskReadSound::~deRLTaskReadSound(){
}



// Management
///////////////

void deRLTaskReadSound::Run(){
	LogRunEnter();
	deBaseSoundModule * const module = ( deBaseSoundModule* )GetEngine().
		GetModuleSystem()->GetModuleAbleToLoad( deModuleSystem::emtSound, GetPath() );
	if( ! module ){
		DETHROW( deeInvalidParam );
	}
	
	const decPath vfsPath( decPath::CreatePathUnix( GetPath() ) );
	
	decBaseFileReaderReference reader;
	reader.TakeOver( GetVFS()->OpenFileForReading( vfsPath ) );
	
	deBaseSoundInfo soundInfo;
	module->InitLoadSound( reader, soundInfo );
	reader = NULL;
	
	pSound->SetModificationTime( GetVFS()->GetFileModificationTime( vfsPath ) );
	pSound->SetAsynchron( true );
	pSound->FinalizeConstruction( soundInfo.GetBytesPerSample(),
		soundInfo.GetSampleRate(), soundInfo.GetSampleCount(),
		soundInfo.GetChannelCount() );
	
	// create peers. modules can request to load the sound data if small enough
	GetEngine().GetAudioSystem()->LoadSound( pSound );
	GetEngine().GetSynthesizerSystem()->LoadSound( pSound );
	
	pSucceeded = true;
	LogRunExit();
}

void deRLTaskReadSound::Finished(){
	LogFinishedEnter();
	if( ! pSucceeded ){
		SetState( esFailed );
		pSound = NULL;
		LogFinishedExit();
		GetResourceLoader().FinishTask( this );
		return;
	}
	
	deSoundManager &soundManager = *GetEngine().GetSoundManager();
	deSound * const checkSound = soundManager.GetSoundWith( GetPath() );
	
	if( checkSound ){
		SetResource( checkSound );
		
	}else{
		pSound->SetAsynchron( false );
		soundManager.AddLoadedSound( pSound );
		SetResource( pSound );
	}
	
	SetState( esSucceeded );
	LogFinishedExit();
	GetResourceLoader().FinishTask( this );
}



// Debugging
//////////////

decString deRLTaskReadSound::GetDebugName() const{
	return deResourceLoaderTask::GetDebugName() + "-Sound-Read";
}
