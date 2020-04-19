/* 
 * Drag[en]gine IGDE World Editor
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "meWindowVegetation.h"
#include "meWindowVegetationListener.h"
#include "../../world/meWorld.h"
#include "../../world/terrain/meHeightTerrain.h"
#include "../../world/terrain/meHeightTerrainSector.h"

#include <dragengine/common/exceptions.h>



// Class meWindowVegetationListener
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

meWindowVegetationListener::meWindowVegetationListener( meWindowVegetation &window ) :
pWindow( window ){
}

meWindowVegetationListener::~meWindowVegetationListener(){
}



// Notifications
//////////////////

void meWindowVegetationListener::ActiveSectorChanged( meWorld *world ){
	meHeightTerrainSector * const sector = world->GetHeightTerrain()->GetActiveSector();
	pWindow.SetSector( sector ? sector->GetCoordinates() : decPoint() );
}



void meWindowVegetationListener::HTVLayerCountChanged( meWorld* ){
}

void meWindowVegetationListener::HTActiveVLayerChanged( meWorld *world ){
	pWindow.SetVLayer( world->GetHeightTerrain()->GetActiveVLayer() );
}

void meWindowVegetationListener::HTVLayerChanged( meWorld*, meHTVegetationLayer *vlayer ){
	if( pWindow.GetVLayer() != vlayer ){
		return;
	}
	pWindow.UpdateNodesFromVLayer();
}



void meWindowVegetationListener::HTVLVariationCountChanged( meWorld*, meHTVegetationLayer* ){
}

void meWindowVegetationListener::HTVLActiveVariationChanged( meWorld*, meHTVegetationLayer* ){
}

void meWindowVegetationListener::HTVLVariationChanged( meWorld*, meHTVegetationLayer*, meHTVVariation* ){
}



void meWindowVegetationListener::HTVLRuleCountChanged( meWorld*, meHTVegetationLayer *vlayer ){
	if( pWindow.GetVLayer() != vlayer ){
		return;
	}
	pWindow.UpdateNodesFromVLayer();
}

void meWindowVegetationListener::HTVLActiveRuleChanged( meWorld*, meHTVegetationLayer *vlayer ){
	if( pWindow.GetVLayer() != vlayer ){
		return;
	}
	
	pWindow.SelectNodeOfActiveRule();
}

void meWindowVegetationListener::HTVLRuleChanged( meWorld*,
meHTVegetationLayer *vlayer, meHTVRule *rule ){
	if( pWindow.GetVLayer() != vlayer ){
		return;
	}
	
	pWindow.UpdateNodeWithRule( rule );
}

void meWindowVegetationListener::HTVLRuleMoved( meWorld*,
meHTVegetationLayer *vlayer, meHTVRule *rule ){
	if( pWindow.GetVLayer() != vlayer ){
		return;
	}
	
	pWindow.NodeWithRuleMoved( rule );
}



void meWindowVegetationListener::HTVLLinkCountChanged( meWorld*,
meHTVegetationLayer *vlayer ){
	if( pWindow.GetVLayer() != vlayer ){
		return;
	}
	
	pWindow.UpdateLinksFromVLayer();
}
