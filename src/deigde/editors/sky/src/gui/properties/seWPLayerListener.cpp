/* 
 * Drag[en]gine IGDE Sky Editor
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

#include "seWPLayer.h"
#include "seWPLayerListener.h"
#include "../../sky/seSky.h"
#include "../../sky/body/seBody.h"
#include "../../sky/layer/seLayer.h"
#include "../../sky/controller/seControllerTarget.h"

#include <dragengine/common/exceptions.h>



// Class seWPLayerListener
////////////////////////////

// Constructor, destructor
////////////////////////////

seWPLayerListener::seWPLayerListener( seWPLayer &panel ) :
pPanel( panel ){
}

seWPLayerListener::~seWPLayerListener(){
}



// Management
///////////////

void seWPLayerListener::LinkStructureChanged( seSky* ){
	pPanel.UpdateLinkList();
}

void seWPLayerListener::LinkNameChanged( seSky*, seLink* ){
	pPanel.UpdateLinkList();
}



void seWPLayerListener::LayerStructureChanged( seSky* ){
	pPanel.UpdateLayerList();
}

void seWPLayerListener::LayerChanged( seSky*, seLayer *layer ){
	if( layer->GetActive() ){
		pPanel.UpdateLayer();
	}
}

void seWPLayerListener::LayerNameChanged( seSky*, seLayer* ){
	pPanel.UpdateLayerList();
}

void seWPLayerListener::ActiveLayerChanged( seSky* ){
	pPanel.SelectActiveLayer();
}



void seWPLayerListener::BodyStructureChanged( seSky*, seLayer *layer ){
	if( layer->GetActive() ){
		pPanel.UpdateBodyList();
	}
}

void seWPLayerListener::BodyChanged( seSky*, seLayer *layer, seBody *body ){
	if( layer->GetActive() && body->GetActive() ){
		pPanel.UpdateBody();
	}
}

void seWPLayerListener::ActiveBodyChanged( seSky*, seLayer *layer ){
	if( layer->GetActive() ){
		pPanel.SelectActiveBody();
	}
}



void seWPLayerListener::TargetChanged( seSky*, seLayer* layer, deSkyLayer::eTargets ){
	if( layer->GetActive() ){
		pPanel.UpdateTarget();
	}
}

void seWPLayerListener::ActiveTargetChanged( seSky*, seLayer *layer ){
	if( layer->GetActive() ){
		pPanel.SelectActiveTarget();
	}
}
