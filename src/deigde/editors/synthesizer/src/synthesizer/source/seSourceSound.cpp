/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "seSourceSound.h"
#include "../seSynthesizer.h"

#include <dragengine/common/file/decPath.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/resources/sound/deSound.h>
#include <dragengine/resources/sound/deSoundManager.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSource.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceSound.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceVisitorIdentify.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <deigde/environment/igdeEnvironment.h>



// Class seSourceSound
////////////////////////

// Constructor, destructor
////////////////////////////

seSourceSound::seSourceSound( deEngine *engine ) :
seSource( deSynthesizerSourceVisitorIdentify::estSound ),
pEngine( engine ),
pSound( NULL ),
pMinSpeed( 1.0f ),
pMaxSpeed( 1.0f ),
pLooping( false ){
}

seSourceSound::seSourceSound( const seSourceSound &copy ) : 
seSource( copy ),
pEngine( copy.pEngine ),
pPathSound( copy.pPathSound ),
pSound( copy.pSound ),
pMinSpeed( copy.pMinSpeed ),
pMaxSpeed( copy.pMaxSpeed ),
pLooping( copy.pLooping ),
pTargetSpeed( copy.pTargetSpeed ),
pTargetPlay( copy.pTargetPlay )
{
	if( pSound ){
		pSound->AddReference();
	}
}

seSourceSound::~seSourceSound(){
	if( pSound ){
		pSound->FreeReference();
	}
}



// Management
///////////////

void seSourceSound::SetPathSound( const char *path ){
	if( pPathSound == path ){
		return;
	}
	
	pPathSound = path;
	UpdateSound();
	NotifySourceChanged();
}

void seSourceSound::UpdateSound(){
	if( pSound ){
		pSound->FreeReference();
		pSound = NULL;
	}
	
	if( ! pPathSound.IsEmpty() && GetSynthesizer() ){
		const decString &basePath = GetSynthesizer()->GetDirectoryPath();
		
		try{
			pSound = pEngine->GetSoundManager()->LoadSound( pPathSound, basePath, false );
			
		}catch( const deException & ){
			GetSynthesizer()->GetEnvironment()->GetLogger()->LogInfoFormat( "Synthesizer Editor",
				"Failed to load sound '%s' (base directory '%s')", pPathSound.GetString(), basePath.GetString() );
		}
	}
	
	if( GetEngineSource() ){
		( ( deSynthesizerSourceSound* )GetEngineSource() )->SetSound( pSound );
	}
}

void seSourceSound::SetMinSpeed( float speed ){
	if( fabsf( speed - pMinSpeed ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pMinSpeed = speed;
	
	if( GetEngineSource() ){
		( ( deSynthesizerSourceSound* )GetEngineSource() )->SetMinSpeed( speed );
		NotifySourceChanged();
	}
}

void seSourceSound::SetMaxSpeed( float speed ){
	if( fabsf( speed - pMaxSpeed ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pMaxSpeed = speed;
	
	if( GetEngineSource() ){
		( ( deSynthesizerSourceSound* )GetEngineSource() )->SetMaxSpeed( speed );
		NotifySourceChanged();
	}
}

void seSourceSound::SetLooping( bool looping ){
	if( looping == pLooping ){
		return;
	}
	
	pLooping = looping;
	
	if( GetEngineSource() ){
		( ( deSynthesizerSourceSound* )GetEngineSource() )->SetLooping( looping );
		NotifySourceChanged();
	}
}



void seSourceSound::UpdateTargets(){
	deSynthesizerSourceSound *source = ( deSynthesizerSourceSound* )GetEngineSource();
	
	seSource::UpdateTargets();
	
	if( source ){
		pTargetSpeed.UpdateEngineTarget( GetSynthesizer(), source->GetTargetSpeed() );
		pTargetPlay.UpdateEngineTarget( GetSynthesizer(), source->GetTargetPlay() );
	}
}

int seSourceSound::CountLinkUsage( seLink *link ) const{
	int usageCount = seSource::CountLinkUsage( link );
	
	if( pTargetSpeed.HasLink( link ) ){
		usageCount++;
	}
	if( pTargetPlay.HasLink( link ) ){
		usageCount++;
	}
	
	return usageCount;
}

void seSourceSound::RemoveLinkFromTargets( seLink *link ){
	seSource::RemoveLinkFromTargets( link );
	
	if( pTargetSpeed.HasLink( link ) ){
		pTargetSpeed.RemoveLink( link );
	}
	if( pTargetPlay.HasLink( link ) ){
		pTargetPlay.RemoveLink( link );
	}
	
	UpdateTargets();
}

void seSourceSound::RemoveLinksFromAllTargets(){
	seSource::RemoveLinksFromAllTargets();
	
	pTargetSpeed.RemoveAllLinks();
	pTargetPlay.RemoveAllLinks();
	
	UpdateTargets();
}



deSynthesizerSource *seSourceSound::CreateEngineSource(){
	deSynthesizerSourceSound *engSource = NULL;
	
	try{
		// create source
		engSource = new deSynthesizerSourceSound;
		
		// init source
		InitEngineSource( engSource );
		
		engSource->SetSound( pSound );
		engSource->SetMinSpeed( pMinSpeed );
		engSource->SetMaxSpeed( pMaxSpeed );
		engSource->SetLooping( pLooping );
		
		pTargetSpeed.UpdateEngineTarget( GetSynthesizer(), engSource->GetTargetSpeed() );
		pTargetPlay.UpdateEngineTarget( GetSynthesizer(), engSource->GetTargetPlay() );
		
	}catch( const deException & ){
		if( engSource ){
			engSource->FreeReference();
		}
		throw;
	}
	
	// finished
	return engSource;
}



seSource *seSourceSound::CreateCopy() const{
	return new seSourceSound( *this );
}



void seSourceSound::ListLinks( seLinkList &list ){
	seSource::ListLinks( list );
	pTargetSpeed.AddLinksToList( list );
	pTargetPlay.AddLinksToList( list );
}



void seSourceSound::SynthesizerChanged(){
	seSource::SynthesizerChanged();
	UpdateSound();
	NotifyEngineOnlySourceChanged();
}

void seSourceSound::SynthesizerDirectoryChanged(){
	seSource::SynthesizerDirectoryChanged();
	UpdateSound();
	NotifyEngineOnlySourceChanged();
}



// Operators
//////////////

seSourceSound &seSourceSound::operator=( const seSourceSound &copy ){
	SetPathSound( copy.pPathSound );
	SetMinSpeed( copy.pMinSpeed );
	SetMaxSpeed( copy.pMaxSpeed );
	SetLooping( copy.pLooping );
	pTargetSpeed = copy.pTargetSpeed;
	pTargetPlay = copy.pTargetPlay;
	seSource::operator=( copy );
	return *this;
}
