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
