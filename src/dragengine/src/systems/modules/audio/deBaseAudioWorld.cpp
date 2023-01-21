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

#include "deBaseAudioWorld.h"



// Class deBaseAudioWorld
///////////////////////////

deBaseAudioWorld::deBaseAudioWorld(){
}

deBaseAudioWorld::~deBaseAudioWorld(){
}



// Notifications
//////////////////

void deBaseAudioWorld::Update( float ){
}

void deBaseAudioWorld::SizeChanged(){
}

void deBaseAudioWorld::AudioChanged(){
}



void deBaseAudioWorld::SpeakerAdded( deSpeaker* ){
}

void deBaseAudioWorld::SpeakerRemoved( deSpeaker* ){
}

void deBaseAudioWorld::AllSpeakersRemoved(){
}



void deBaseAudioWorld::MicrophoneAdded( deMicrophone* ){
}

void deBaseAudioWorld::MicrophoneRemoved( deMicrophone* ){
}

void deBaseAudioWorld::AllMicrophonesRemoved(){
}



void deBaseAudioWorld::ComponentAdded( deComponent* ){
}

void deBaseAudioWorld::ComponentRemoved( deComponent* ){
}

void deBaseAudioWorld::AllComponentsRemoved(){
}



void deBaseAudioWorld::SoundLevelMeterAdded( deSoundLevelMeter* ){
}

void deBaseAudioWorld::SoundLevelMeterRemoved( deSoundLevelMeter* ){
}

void deBaseAudioWorld::AllSoundLevelMetersRemoved(){
}
