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
