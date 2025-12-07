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

#include "seWPSource.h"
#include "seWPSourceListener.h"
#include "../../synthesizer/seSynthesizer.h"

#include <dragengine/common/exceptions.h>



// Class seWPSourceListener
/////////////////////////////

// Constructor, destructor
////////////////////////////

seWPSourceListener::seWPSourceListener(seWPSource &panel) :
pPanel(panel){
}

seWPSourceListener::~seWPSourceListener(){
}



// Notifications
//////////////////

void seWPSourceListener::SynthesizerChanged(seSynthesizer *synthesizer){
	if(synthesizer != pPanel.GetSynthesizer()){
		return;
	}
	
	pPanel.UpdateSynthesizer();
}




void seWPSourceListener::ControllerNameChanged(seSynthesizer *synthesizer, seController*){
	if(synthesizer != pPanel.GetSynthesizer()){
		return;
	}
	
	pPanel.UpdateControllerList();
}

void seWPSourceListener::ControllerStructureChanged(seSynthesizer *synthesizer){
	if(synthesizer != pPanel.GetSynthesizer()){
		return;
	}
	
	pPanel.UpdateControllerList();
}



void seWPSourceListener::LinkNameChanged(seSynthesizer *synthesizer, seLink*){
	if(synthesizer != pPanel.GetSynthesizer()){
		return;
	}
	
	pPanel.UpdateLinkList();
}

void seWPSourceListener::LinkStructureChanged(seSynthesizer *synthesizer){
	if(synthesizer != pPanel.GetSynthesizer()){
		return;
	}
	
	pPanel.UpdateLinkList();
}



void seWPSourceListener::ActiveSourceChanged(seSynthesizer *synthesizer, seSource*){
	if(synthesizer != pPanel.GetSynthesizer()){
		return;
	}
	
	pPanel.SelectActiveSource();
	pPanel.ShowActiveSourcePanel();
	pPanel.UpdateSource();
}

void seWPSourceListener::SourceChanged(seSynthesizer*, seSource *source){
	if(source != pPanel.GetSource()){
		return;
	}
	
	pPanel.UpdateSource();
}

void seWPSourceListener::SourceNameChanged(seSynthesizer*, seSource *source){
	if(source != pPanel.GetSource()){
		return;
	}
	
	pPanel.UpdateSourceTree();
	pPanel.UpdateSource();
}

void seWPSourceListener::SourceStructureChanged(seSynthesizer *synthesizer){
	if(synthesizer != pPanel.GetSynthesizer()){
		return;
	}
	
	pPanel.UpdateSourceTree();
	pPanel.UpdateSource();
}
