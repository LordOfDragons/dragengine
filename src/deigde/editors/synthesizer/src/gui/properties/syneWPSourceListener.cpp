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

#include <stdlib.h>
#include <string.h>

#include "syneWPSource.h"
#include "syneWPSourceListener.h"
#include "../../synthesizer/syneSynthesizer.h"

#include <dragengine/common/exceptions.h>



// Class syneWPSourceListener
/////////////////////////////

// Constructor, destructor
////////////////////////////

syneWPSourceListener::syneWPSourceListener(syneWPSource &panel) :
pPanel(panel){
}

syneWPSourceListener::~syneWPSourceListener(){
}



// Notifications
//////////////////

void syneWPSourceListener::SynthesizerChanged(syneSynthesizer *synthesizer){
	if(synthesizer != pPanel.GetSynthesizer()){
		return;
	}
	
	pPanel.UpdateSynthesizer();
}




void syneWPSourceListener::ControllerNameChanged(syneSynthesizer *synthesizer, syneController*){
	if(synthesizer != pPanel.GetSynthesizer()){
		return;
	}
	
	pPanel.UpdateControllerList();
}

void syneWPSourceListener::ControllerStructureChanged(syneSynthesizer *synthesizer){
	if(synthesizer != pPanel.GetSynthesizer()){
		return;
	}
	
	pPanel.UpdateControllerList();
}



void syneWPSourceListener::LinkNameChanged(syneSynthesizer *synthesizer, syneLink*){
	if(synthesizer != pPanel.GetSynthesizer()){
		return;
	}
	
	pPanel.UpdateLinkList();
}

void syneWPSourceListener::LinkStructureChanged(syneSynthesizer *synthesizer){
	if(synthesizer != pPanel.GetSynthesizer()){
		return;
	}
	
	pPanel.UpdateLinkList();
}



void syneWPSourceListener::ActiveSourceChanged(syneSynthesizer *synthesizer, syneSource*){
	if(synthesizer != pPanel.GetSynthesizer()){
		return;
	}
	
	pPanel.SelectActiveSource();
	pPanel.ShowActiveSourcePanel();
	pPanel.UpdateSource();
}

void syneWPSourceListener::SourceChanged(syneSynthesizer*, syneSource *source){
	if(source != pPanel.GetSource()){
		return;
	}
	
	pPanel.UpdateSource();
}

void syneWPSourceListener::SourceNameChanged(syneSynthesizer*, syneSource *source){
	if(source != pPanel.GetSource()){
		return;
	}
	
	pPanel.UpdateSourceTree();
	pPanel.UpdateSource();
}

void syneWPSourceListener::SourceStructureChanged(syneSynthesizer *synthesizer){
	if(synthesizer != pPanel.GetSynthesizer()){
		return;
	}
	
	pPanel.UpdateSourceTree();
	pPanel.UpdateSource();
}
