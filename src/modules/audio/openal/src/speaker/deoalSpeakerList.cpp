/* 
 * Drag[en]gine OpenAL Audio Module
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

#include "deoalASpeaker.h"
#include "deoalSpeakerList.h"
#include "../microphone/deoalAMicrophone.h"
#include "../world/deoalAWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/resources/sound/deSpeaker.h>
#include <dragengine/resources/world/deWorld.h>



// Class deoalSpeakerList
///////////////////////////

// Constructor, destructor
////////////////////////////

deoalSpeakerList::deoalSpeakerList(){
}

deoalSpeakerList::~deoalSpeakerList(){
}



// Management
///////////////

int deoalSpeakerList::GetCount() const{
	return pSpeakers.GetCount();
}

deoalASpeaker *deoalSpeakerList::GetAt( int index ) const{
	return ( deoalASpeaker* )pSpeakers.GetAt( index );
}

bool deoalSpeakerList::Has( deoalASpeaker *speaker ) const{
	return pSpeakers.Has( speaker );
}

int deoalSpeakerList::IndexOf( deoalASpeaker *speaker ) const{
	return pSpeakers.IndexOf( speaker );
}

void deoalSpeakerList::Add( deoalASpeaker *speaker ){
	pSpeakers.Add( speaker );
}

void deoalSpeakerList::Remove( deoalASpeaker *speaker ){
	pSpeakers.RemoveFrom( pSpeakers.IndexOf( speaker ) );
}

void deoalSpeakerList::RemoveIfExisting( deoalASpeaker *speaker ){
	const int index = pSpeakers.IndexOf( speaker );
	if( index != -1 ){
		pSpeakers.RemoveFrom( index );
	}
}

void deoalSpeakerList::RemoveAll(){
	pSpeakers.RemoveAll();
}

void deoalSpeakerList::FlagAll( bool flag ){
	const int count = pSpeakers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoalASpeaker* )pSpeakers.GetAt( i ) )->SetFlag( flag );
	}
}

void deoalSpeakerList::EnableAll( bool enabled ){
	// WARNING Called during synchronization time from main thread.
	
	const int count = pSpeakers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoalASpeaker* )pSpeakers.GetAt( i ) )->SetEnabled( enabled );
	}
}

void deoalSpeakerList::FlagToEnabledAll(){
	// WARNING Called during synchronization time from main thread.
	
	const int count = pSpeakers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoalASpeaker &speaker = *( ( deoalASpeaker* )pSpeakers.GetAt( i ) );
		speaker.SetEnabled( speaker.GetFlag() );
	}
}

void deoalSpeakerList::UpdateAll(){
	const int count = pSpeakers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoalASpeaker* )pSpeakers.GetAt( i ) )->PrepareProcessAudio();
	}
}

void deoalSpeakerList::UpdateEffectsAll(){
	const int count = pSpeakers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoalASpeaker* )pSpeakers.GetAt( i ) )->UpdateEffects();
	}
}



// Operators
//////////////

deoalSpeakerList &deoalSpeakerList::operator=( const deoalSpeakerList &list ){
	pSpeakers = list.pSpeakers;
	return *this;
}

deoalSpeakerList &deoalSpeakerList::operator+=( const deoalSpeakerList &list ){
	pSpeakers += list.pSpeakers;
	return *this;
}

deoalSpeakerList deoalSpeakerList::operator+( const deoalSpeakerList &list ) const{
	deoalSpeakerList combined;
	combined.pSpeakers = pSpeakers + list.pSpeakers;
	return combined;
}

bool deoalSpeakerList::operator==( const deoalSpeakerList &list ) const{
	const int count = pSpeakers.GetCount();
	if( count != list.pSpeakers.GetCount() ){
		return false;
	}
	
	int i;
	for( i=0; i<count; i++ ){
		if( ! list.pSpeakers.Has( pSpeakers.GetAt( i ) ) ){
			return false;
		}
	}
	
	return true;
}

bool deoalSpeakerList::operator!=( const deoalSpeakerList &list ) const{
	return ! operator==( list );
}
