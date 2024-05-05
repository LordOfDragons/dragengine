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

#include "seWPEffect.h"
#include "seWPEffectListener.h"
#include "../../synthesizer/seSynthesizer.h"
#include "../../synthesizer/source/seSource.h"

#include <dragengine/common/exceptions.h>
#include <stdio.h>



// Class seWPEffectListener
/////////////////////////////

// Constructor, destructor
////////////////////////////

seWPEffectListener::seWPEffectListener( seWPEffect &panel ) :
pPanel( panel ){
}

seWPEffectListener::~seWPEffectListener(){
}



// Notifications
//////////////////

void seWPEffectListener::ControllerNameChanged( seSynthesizer *synthesizer, seController* ){
	if( synthesizer != pPanel.GetSynthesizer() ){
		return;
	}
	
	pPanel.UpdateControllerList();
}

void seWPEffectListener::ControllerStructureChanged( seSynthesizer *synthesizer ){
	if( synthesizer != pPanel.GetSynthesizer() ){
		return;
	}
	
	pPanel.UpdateControllerList();
}



void seWPEffectListener::LinkNameChanged( seSynthesizer *synthesizer, seLink* ){
	if( synthesizer != pPanel.GetSynthesizer() ){
		return;
	}
	
	pPanel.UpdateLinkList();
}

void seWPEffectListener::LinkStructureChanged( seSynthesizer *synthesizer ){
	if( synthesizer != pPanel.GetSynthesizer() ){
		return;
	}
	
	pPanel.UpdateLinkList();
}



void seWPEffectListener::ActiveSourceChanged( seSynthesizer *synthesizer, seSource* ){
	if( synthesizer != pPanel.GetSynthesizer() ){
		return;
	}
	
	pPanel.UpdateEffectList();
}

void seWPEffectListener::ActiveEffectChanged( seSynthesizer*, seSource *source ){
	if( source != pPanel.GetSource() ){
		return;
	}
	
	pPanel.SelectActiveEffect();
	pPanel.ShowActiveEffectPanel();
	pPanel.UpdateEffect();
}

void seWPEffectListener::EffectChanged( seSynthesizer*, seSource*, seEffect *effect ) {
	if( effect != pPanel.GetEffect() ){
		return;
	}
	
	pPanel.UpdateEffect();
}

void seWPEffectListener::EffectStructureChanged( seSynthesizer*, seSource *source ) {
	if( source != pPanel.GetSource() ){
		return;
	}
	
	pPanel.UpdateEffectList();
}
