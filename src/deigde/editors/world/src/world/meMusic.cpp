/* 
 * Drag[en]gine IGDE World Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "meMusic.h"
#include "meWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/resources/sound/deSpeakerManager.h>
#include <dragengine/resources/sound/deSoundManager.h>

#include <deigde/environment/igdeEnvironment.h>


// Definitions
////////////////

#define LOGSOURCE "World Editor"


// Class meMusic
//////////////////

// Constructor, destructor
////////////////////////////

meMusic::meMusic( meWorld &world ) :
pWorld( world ),
pSpeaker( world.GetEngine()->GetSpeakerManager()->CreateSpeaker() )
{
	world.GetMicrophone()->AddSpeaker( pSpeaker );
}

meMusic::~meMusic(){
	pCleanUp();
}



// Management
///////////////

void meMusic::SetPath( const char *path ){
	if( pPath == path ){
		return;
	}
	
	pSpeaker->SetSound( nullptr );
	
	pPath = path;
	
	if( ! pPath.IsEmpty() ){
		try{
			pSpeaker->SetSound( pWorld.GetEngine()->GetSoundManager()->LoadSound( path, "/", false ) );
			
		}catch( const deException &e ){
			pWorld.GetEnvironment()->GetLogger()->LogException( LOGSOURCE, e );
		}
	}
	
	pWorld.NotifyMusicChanged();
}

float meMusic::GetVolume() const{
	return pSpeaker->GetVolume();
}

void meMusic::SetVolume( float volume ){
	if( fabsf( volume - pSpeaker->GetVolume() ) < 0.001f ){
		return;
	}
	
	pSpeaker->SetVolume( volume );
	pWorld.NotifyMusicChanged();
}

void meMusic::Play(){
	pSpeaker->Play();
}

void meMusic::Pause(){
	pSpeaker->Pause();
}

void meMusic::Stop(){
	pSpeaker->Stop();
}


// Private Functions
//////////////////////

void meMusic::pCleanUp(){
	if( pSpeaker ){
		pWorld.GetMicrophone()->RemoveSpeaker( pSpeaker );
	}
}
