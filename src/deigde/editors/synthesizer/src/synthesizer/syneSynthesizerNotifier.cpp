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

#include "syneSynthesizerNotifier.h"



// Class syneSynthesizerNotifier
////////////////////////////////

// Constructor, destructor
////////////////////////////

syneSynthesizerNotifier::syneSynthesizerNotifier(){
}

syneSynthesizerNotifier::~syneSynthesizerNotifier(){
}



// Notifications
//////////////////

void syneSynthesizerNotifier::StateChanged(syneSynthesizer*){
}

void syneSynthesizerNotifier::UndoChanged(syneSynthesizer*){
}

void syneSynthesizerNotifier::SynthesizerChanged(syneSynthesizer*){
}

void syneSynthesizerNotifier::PlaybackChanged(syneSynthesizer*){
}



void syneSynthesizerNotifier::ActiveControllerChanged(syneSynthesizer* , syneController*){
}

void syneSynthesizerNotifier::ControllerChanged(syneSynthesizer* , syneController*){
}

void syneSynthesizerNotifier::ControllerNameChanged(syneSynthesizer *synthesizer, syneController *controller){
}

void syneSynthesizerNotifier::ControllerRangeChanged(syneSynthesizer *synthesizer, syneController *controller){
	ControllerChanged(synthesizer, controller);
}

void syneSynthesizerNotifier::ControllerCurveChanged(syneSynthesizer *synthesizer, syneController *controller){
	ControllerChanged(synthesizer, controller);
}

void syneSynthesizerNotifier::ControllerStructureChanged(syneSynthesizer*){
}



void syneSynthesizerNotifier::ActiveLinkChanged(syneSynthesizer* , syneLink*){
}

void syneSynthesizerNotifier::LinkChanged(syneSynthesizer* , syneLink*){
}

void syneSynthesizerNotifier::LinkNameChanged(syneSynthesizer* , syneLink*){
}

void syneSynthesizerNotifier::LinkStructureChanged(syneSynthesizer*){
}



void syneSynthesizerNotifier::ActiveSourceChanged(syneSynthesizer* , syneSource*){
}

void syneSynthesizerNotifier::SourceChanged(syneSynthesizer* , syneSource*){
}

void syneSynthesizerNotifier::SourceNameChanged(syneSynthesizer* , syneSource*){
}

void syneSynthesizerNotifier::SourceStructureChanged(syneSynthesizer*){
}

void syneSynthesizerNotifier::ActiveEffectChanged(syneSynthesizer* , syneSource*){
}

void syneSynthesizerNotifier::EffectChanged(syneSynthesizer *synthesizer, syneSource *source, syneEffect*){
	SourceChanged(synthesizer, source);
}

void syneSynthesizerNotifier::EffectStructureChanged(syneSynthesizer *synthesizer, syneSource *source){
	SourceChanged(synthesizer, source);
}
