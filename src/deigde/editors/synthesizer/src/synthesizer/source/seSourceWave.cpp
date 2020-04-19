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

#include "seSourceWave.h"
#include "../seSynthesizer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSource.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceWave.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceVisitorIdentify.h>



// Class seSourceWave
////////////////////////////////////

// Constructor, destructor
////////////////////////////

seSourceWave::seSourceWave() :
seSource( deSynthesizerSourceVisitorIdentify::estWave ),
pWaveType( deSynthesizerSourceWave::ewtSine ),
pMinFrequency( 440.0f ),
pMaxFrequency( 440.0f ){
}

seSourceWave::seSourceWave( const seSourceWave &copy ) :
seSource( copy ),
pWaveType( copy.pWaveType ),
pMinFrequency( copy.pMinFrequency ),
pMaxFrequency( copy.pMaxFrequency ),
pTargetFrequency( copy.pTargetFrequency ){
}

seSourceWave::~seSourceWave(){
}



// Management
///////////////

void seSourceWave::SetWaveType( deSynthesizerSourceWave::eWaveType type ){
	if( type < deSynthesizerSourceWave::ewtSine || type > deSynthesizerSourceWave::ewtTriangle ){
		DETHROW( deeInvalidParam );
	}
	
	if( type == pWaveType ){
		return;
	}
	
	pWaveType = type;
	
	if( GetEngineSource() ){
		( ( deSynthesizerSourceWave* )GetEngineSource() )->SetType( type );
		NotifySourceChanged();
	}
}

void seSourceWave::SetMinFrequency( float frequency ){
	frequency = decMath::max( frequency, 0.0f );
	if( fabsf( frequency - pMinFrequency ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pMinFrequency = frequency;
	
	if( GetEngineSource() ){
		( ( deSynthesizerSourceWave* )GetEngineSource() )->SetMinFrequency( frequency );
		NotifySourceChanged();
	}
}

void seSourceWave::SetMaxFrequency( float frequency ){
	frequency = decMath::max( frequency, 0.0f );
	if( fabsf( frequency - pMaxFrequency ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pMaxFrequency = frequency;
	
	if( GetEngineSource() ){
		( ( deSynthesizerSourceWave* )GetEngineSource() )->SetMaxFrequency( frequency );
		NotifySourceChanged();
	}
}



void seSourceWave::UpdateTargets(){
	seSource::UpdateTargets();
	
	deSynthesizerSourceWave * const source = ( deSynthesizerSourceWave* )GetEngineSource();
	if( source ){
		pTargetFrequency.UpdateEngineTarget( GetSynthesizer(), source->GetTargetFrequency() );
	}
}

int seSourceWave::CountLinkUsage( seLink *link ) const{
	int usageCount = seSource::CountLinkUsage( link );
	
	if( pTargetFrequency.HasLink( link ) ){
		usageCount++;
	}
	
	return usageCount;
}

void seSourceWave::RemoveLinkFromTargets( seLink *link ){
	seSource::RemoveLinkFromTargets( link );
	
	if( pTargetFrequency.HasLink( link ) ){
		pTargetFrequency.RemoveLink( link );
	}
	
	UpdateTargets();
}

void seSourceWave::RemoveLinksFromAllTargets(){
	seSource::RemoveLinksFromAllTargets();
	
	pTargetFrequency.RemoveAllLinks();
	
	UpdateTargets();
}



deSynthesizerSource *seSourceWave::CreateEngineSource(){
	deSynthesizerSourceWave *engSource = NULL;
	
	try{
		// create source
		engSource = new deSynthesizerSourceWave;
		
		// init source
		InitEngineSource( engSource );
		
		engSource->SetType( pWaveType );
		engSource->SetMinFrequency( pMinFrequency );
		engSource->SetMaxFrequency( pMaxFrequency );
		
		pTargetFrequency.UpdateEngineTarget( GetSynthesizer(), engSource->GetTargetFrequency() );
		
	}catch( const deException & ){
		if( engSource ){
			engSource->FreeReference();
		}
		throw;
	}
	
	// finished
	return engSource;
}



seSource *seSourceWave::CreateCopy() const{
	return new seSourceWave( *this );
}

void seSourceWave::ListLinks( seLinkList &list ){
	seSource::ListLinks( list );
	pTargetFrequency.AddLinksToList( list );
}



// Operators
//////////////

seSourceWave &seSourceWave::operator=( const seSourceWave &copy ){
	SetWaveType( copy.pWaveType );
	SetMinFrequency( copy.pMinFrequency );
	SetMaxFrequency( copy.pMaxFrequency );
	pTargetFrequency = copy.pTargetFrequency;
	seSource::operator=( copy );
	return *this;
}
