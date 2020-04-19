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

#include "deSynthesizer.h"
#include "deSynthesizerManager.h"
#include "deSynthesizerLink.h"
#include "deSynthesizerController.h"
#include "source/deSynthesizerSource.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/synthesizer/deBaseSynthesizerSynthesizer.h"



// Class deSynthesizer
////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizer::deSynthesizer( deSynthesizerManager *manager ) :
deResource( manager ),
pChannelCount( 1 ),
pSampleRate( 44100 ),
pBytesPerSample( 2 ),
pSampleCount( pSampleRate * 10 ),
pPeerSynthesizer( NULL ){
}

deSynthesizer::~deSynthesizer(){
	if( pPeerSynthesizer ){
		delete pPeerSynthesizer;
		pPeerSynthesizer = NULL;
	}
	
	RemoveAllSources();
	RemoveAllLinks();
	RemoveAllControllers();
}



// Management
///////////////

void deSynthesizer::SetChannelCount( int channelCount ){
	if( channelCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( channelCount == pChannelCount ){
		return;
	}
	
	pChannelCount = channelCount;
	
	if( pPeerSynthesizer ){
		pPeerSynthesizer->ParametersChanged();
	}
}

void deSynthesizer::SetSampleRate( int sampleRate ){
	if( sampleRate < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( sampleRate == pSampleRate ){
		return;
	}
	
	pSampleRate = sampleRate;
	
	if( pPeerSynthesizer ){
		pPeerSynthesizer->ParametersChanged();
	}
}

void deSynthesizer::SetBytesPerSample( int bytesPerSample ){
	if( bytesPerSample < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( bytesPerSample == pBytesPerSample ){
		return;
	}
	
	pBytesPerSample = bytesPerSample;
	
	if( pPeerSynthesizer ){
		pPeerSynthesizer->ParametersChanged();
	}
}

void deSynthesizer::SetSampleCount( int sampleCount ){
	if( sampleCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( sampleCount == pSampleCount ){
		return;
	}
	
	pSampleCount = sampleCount;
	
	if( pPeerSynthesizer ){
		pPeerSynthesizer->ParametersChanged();
	}
}



// Controllers
////////////////

int deSynthesizer::GetControllerCount() const{
	return pControllers.GetCount();
}

deSynthesizerController *deSynthesizer::GetControllerAt( int index ) const{
	return ( deSynthesizerController* )pControllers.GetAt( index );
}

int deSynthesizer::IndexOfController( deSynthesizerController *controller ) const{
	return pControllers.IndexOf( controller );
}

int deSynthesizer::IndexOfControllerNamed( const char *name ) const{
	const int count = pControllers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( deSynthesizerController* )pControllers.GetAt( i ) )->GetName() == name ){
			return i;
		}
	}
	
	return -1;
}

bool deSynthesizer::HasController( deSynthesizerController *controller ) const{
	return pControllers.Has( controller );
}

void deSynthesizer::AddController( deSynthesizerController *controller ){
	if( ! controller ){
		DETHROW( deeInvalidParam );
	}
	
	pControllers.Add( controller );
	
	if( pPeerSynthesizer ){
		pPeerSynthesizer->ControllersChanged();
	}
}

void deSynthesizer::RemoveController( deSynthesizerController *controller ){
	pControllers.Remove( controller );
	
	if( pPeerSynthesizer ){
		pPeerSynthesizer->ControllersChanged();
	}
}

void deSynthesizer::RemoveAllControllers(){
	pControllers.RemoveAll();
	
	if( pPeerSynthesizer ){
		pPeerSynthesizer->ControllersChanged();
	}
}

void deSynthesizer::NotifyControllersChanged(){
	if( pPeerSynthesizer ){
		pPeerSynthesizer->ControllersChanged();
	}
}



// Links
//////////

int deSynthesizer::GetLinkCount() const{
	return pLinks.GetCount();
}

deSynthesizerLink *deSynthesizer::GetLinkAt( int index ) const{
	return ( deSynthesizerLink* )pLinks.GetAt( index );
}

int deSynthesizer::IndexOfLink( deSynthesizerLink *link ) const{
	return pLinks.IndexOf( link );
}

bool deSynthesizer::HasLink( deSynthesizerLink *link ) const{
	return pLinks.Has( link );
}

void deSynthesizer::AddLink( deSynthesizerLink *link ){
	if( ! link ){
		DETHROW( deeInvalidParam );
	}
	
	pLinks.Add( link );
	
	if( pPeerSynthesizer ){
		pPeerSynthesizer->LinksChanged();
	}
}

void deSynthesizer::RemoveLink( deSynthesizerLink *link ){
	pLinks.Remove( link );
	
	if( pPeerSynthesizer ){
		pPeerSynthesizer->LinksChanged();
	}
}

void deSynthesizer::RemoveAllLinks(){
	pLinks.RemoveAll();
	
	if( pPeerSynthesizer ){
		pPeerSynthesizer->LinksChanged();
	}
}

void deSynthesizer::NotifyLinksChanged(){
	if( pPeerSynthesizer ){
		pPeerSynthesizer->LinksChanged();
	}
}



// Sources
////////////

int deSynthesizer::GetSourceCount() const{
	return pSources.GetCount();
}

deSynthesizerSource *deSynthesizer::GetSourceAt( int index ) const{
	return ( deSynthesizerSource* )pSources.GetAt( index );
}

int deSynthesizer::IndexOfSource( deSynthesizerSource *source ) const{
	return pSources.IndexOf( source );
}

bool deSynthesizer::HasSource( deSynthesizerSource *source ) const{
	return pSources.Has( source );
}

void deSynthesizer::AddSource( deSynthesizerSource *source ){
	if( ! source ){
		DETHROW( deeInvalidParam );
	}
	
	pSources.Add( source );
	
	if( pPeerSynthesizer ){
		pPeerSynthesizer->SourcesChanged();
	}
}

void deSynthesizer::RemoveSource( deSynthesizerSource *source ){
	pSources.Remove( source );
	
	if( pPeerSynthesizer ){
		pPeerSynthesizer->SourcesChanged();
	}
}

void deSynthesizer::RemoveAllSources(){
	pSources.RemoveAll();
	
	if( pPeerSynthesizer ){
		pPeerSynthesizer->SourcesChanged();
	}
}

void deSynthesizer::NotifySourcesChanged(){
	if( pPeerSynthesizer ){
		pPeerSynthesizer->SourcesChanged();
	}
}



// System Peers
/////////////////

void deSynthesizer::SetPeerSynthesizer( deBaseSynthesizerSynthesizer *peer ){
	if( peer == pPeerSynthesizer ){
		return;
	}
	
	if( pPeerSynthesizer ){
		delete pPeerSynthesizer;
	}
	pPeerSynthesizer = peer;
}
