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

#include "seWPSource.h"
#include "seWPSourceListener.h"
#include "../../synthesizer/seSynthesizer.h"

#include <dragengine/common/exceptions.h>



// Class seWPSourceListener
/////////////////////////////

// Constructor, destructor
////////////////////////////

seWPSourceListener::seWPSourceListener( seWPSource &panel ) :
pPanel( panel ){
}

seWPSourceListener::~seWPSourceListener(){
}



// Notifications
//////////////////

void seWPSourceListener::SynthesizerChanged( seSynthesizer *synthesizer ){
	if( synthesizer != pPanel.GetSynthesizer() ){
		return;
	}
	
	pPanel.UpdateSynthesizer();
}




void seWPSourceListener::ControllerNameChanged( seSynthesizer *synthesizer, seController* ){
	if( synthesizer != pPanel.GetSynthesizer() ){
		return;
	}
	
	pPanel.UpdateControllerList();
}

void seWPSourceListener::ControllerStructureChanged( seSynthesizer *synthesizer ){
	if( synthesizer != pPanel.GetSynthesizer() ){
		return;
	}
	
	pPanel.UpdateControllerList();
}



void seWPSourceListener::LinkNameChanged( seSynthesizer *synthesizer, seLink* ){
	if( synthesizer != pPanel.GetSynthesizer() ){
		return;
	}
	
	pPanel.UpdateLinkList();
}

void seWPSourceListener::LinkStructureChanged( seSynthesizer *synthesizer ){
	if( synthesizer != pPanel.GetSynthesizer() ){
		return;
	}
	
	pPanel.UpdateLinkList();
}



void seWPSourceListener::ActiveSourceChanged( seSynthesizer *synthesizer, seSource* ){
	if( synthesizer != pPanel.GetSynthesizer() ){
		return;
	}
	
	pPanel.SelectActiveSource();
	pPanel.ShowActiveSourcePanel();
	pPanel.UpdateSource();
}

void seWPSourceListener::SourceChanged( seSynthesizer*, seSource *source ){
	if( source != pPanel.GetSource() ){
		return;
	}
	
	pPanel.UpdateSource();
}

void seWPSourceListener::SourceNameChanged( seSynthesizer*, seSource *source ){
	if( source != pPanel.GetSource() ){
		return;
	}
	
	pPanel.UpdateSourceTree();
	pPanel.UpdateSource();
}

void seWPSourceListener::SourceStructureChanged( seSynthesizer *synthesizer ){
	if( synthesizer != pPanel.GetSynthesizer() ){
		return;
	}
	
	pPanel.UpdateSourceTree();
	pPanel.UpdateSource();
}
