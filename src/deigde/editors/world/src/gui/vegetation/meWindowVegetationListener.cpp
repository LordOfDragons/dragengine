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
