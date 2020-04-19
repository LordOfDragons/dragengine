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

#include "seSynthesizerNotifier.h"



// Class seSynthesizerNotifier
////////////////////////////////

// Constructor, destructor
////////////////////////////

seSynthesizerNotifier::seSynthesizerNotifier(){
}

seSynthesizerNotifier::~seSynthesizerNotifier(){
}



// Notifications
//////////////////

void seSynthesizerNotifier::StateChanged( seSynthesizer* ){
}

void seSynthesizerNotifier::UndoChanged( seSynthesizer* ){
}

void seSynthesizerNotifier::SynthesizerChanged( seSynthesizer* ){
}

void seSynthesizerNotifier::PlaybackChanged( seSynthesizer* ){
}



void seSynthesizerNotifier::ActiveControllerChanged( seSynthesizer* , seController* ){
}

void seSynthesizerNotifier::ControllerChanged( seSynthesizer* , seController* ){
}

void seSynthesizerNotifier::ControllerNameChanged( seSynthesizer *synthesizer, seController *controller ){
}

void seSynthesizerNotifier::ControllerRangeChanged( seSynthesizer *synthesizer, seController *controller ){
	ControllerChanged( synthesizer, controller );
}

void seSynthesizerNotifier::ControllerCurveChanged( seSynthesizer *synthesizer, seController *controller ){
	ControllerChanged( synthesizer, controller );
}

void seSynthesizerNotifier::ControllerStructureChanged( seSynthesizer* ){
}



void seSynthesizerNotifier::ActiveLinkChanged( seSynthesizer* , seLink* ){
}

void seSynthesizerNotifier::LinkChanged( seSynthesizer* , seLink* ){
}

void seSynthesizerNotifier::LinkNameChanged( seSynthesizer* , seLink* ){
}

void seSynthesizerNotifier::LinkStructureChanged( seSynthesizer* ){
}



void seSynthesizerNotifier::ActiveSourceChanged( seSynthesizer* , seSource* ){
}

void seSynthesizerNotifier::SourceChanged( seSynthesizer* , seSource* ){
}

void seSynthesizerNotifier::SourceNameChanged( seSynthesizer* , seSource* ){
}

void seSynthesizerNotifier::SourceStructureChanged( seSynthesizer* ){
}

void seSynthesizerNotifier::ActiveEffectChanged( seSynthesizer* , seSource* ){
}

void seSynthesizerNotifier::EffectChanged( seSynthesizer *synthesizer, seSource *source, seEffect* ){
	SourceChanged( synthesizer, source );
}

void seSynthesizerNotifier::EffectStructureChanged( seSynthesizer *synthesizer, seSource *source ){
	SourceChanged( synthesizer, source );
}
