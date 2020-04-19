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

#include "meWPHeightTerrain.h"
#include "meWPHeightTerrainListener.h"
#include "../../world/meWorld.h"
#include "../../world/terrain/meHeightTerrain.h"
#include "../../world/terrain/meHeightTerrainSector.h"
#include "../../world/terrain/meHeightTerrainTexture.h"
#include "../../world/terrain/meHeightTerrainNavSpace.h"
#include "../../world/heightterrain/meHTVegetationLayer.h"
#include "../../world/heightterrain/meHTVVariation.h"

#include <dragengine/common/exceptions.h>



// Class meWPHeightTerrainListener
////////////////////////////////////

// Constructor, destructor
////////////////////////////

meWPHeightTerrainListener::meWPHeightTerrainListener( meWPHeightTerrain &panel ) :
pPanel( panel ){
}

meWPHeightTerrainListener::~meWPHeightTerrainListener(){
}



// Notifications
//////////////////

void meWPHeightTerrainListener::HeightPaintChanged( meWorld* ){
	pPanel.UpdateHeightPaint();
}

void meWPHeightTerrainListener::MaskPaintChanged( meWorld* ){
	pPanel.UpdateMaskPaint();
}

void meWPHeightTerrainListener::VisibilityPaintChanged( meWorld* ){
	pPanel.UpdateVisibilityPaint();
}



void meWPHeightTerrainListener::ActiveSectorChanged( meWorld* ){
	pPanel.SetTexture( NULL );
	
	pPanel.UpdateSector();
	
	if( pPanel.GetSector() ){
		pPanel.SetTexture( pPanel.GetSector()->GetActiveTexture() );
	}
}



void meWPHeightTerrainListener::HTStateChanged( meWorld* ){
	pPanel.UpdateHeightPaint();
}

void meWPHeightTerrainListener::HTChanged( meWorld* ){
	pPanel.UpdateHeightPaint();
}

void meWPHeightTerrainListener::HTSChanged( meWorld*, meHeightTerrainSector *sector ){
	if( sector != pPanel.GetSector() ){
		return;
	}
	pPanel.UpdateSector();
}

void meWPHeightTerrainListener::HTSHeightChanged( meWorld*, meHeightTerrainSector *sector ){
	if( sector != pPanel.GetSector() ){
		return;
	}
	pPanel.UpdateSector();
}

void meWPHeightTerrainListener::HTSVisibilityChanged( meWorld*, meHeightTerrainSector *sector ){
	if( sector != pPanel.GetSector() ){
		return;
	}
	pPanel.UpdateSector();
}



void meWPHeightTerrainListener::HTSTextureCountChanged( meWorld*, meHeightTerrainSector *sector ){
	if( sector != pPanel.GetSector() ){
		return;
	}
	pPanel.UpdateTextureList();
}

void meWPHeightTerrainListener::HTSActiveTextureChanged( meWorld*, meHeightTerrainSector *sector ){
	if( sector != pPanel.GetSector() ){
		return;
	}
	pPanel.SetTexture( sector->GetActiveTexture() );
}

void meWPHeightTerrainListener::HTSTextureChanged( meWorld*, meHeightTerrainSector *sector,
meHeightTerrainTexture *texture ){
	if( sector != pPanel.GetSector() || texture != pPanel.GetTexture() ){
		return;
	}
	pPanel.UpdateTexture();
}

void meWPHeightTerrainListener::HTSTextureMaskChanged( meWorld*, meHeightTerrainSector *sector,
meHeightTerrainTexture *texture ){
	if( sector != pPanel.GetSector() || texture != pPanel.GetTexture() ){
		return;
	}
	pPanel.UpdateTexture();
}



void meWPHeightTerrainListener::HTNavSpaceCountChanged( meWorld *world ){
	if( world != pPanel.GetWorld() ){
		return;
	}
	
	pPanel.UpdateNavSpaceList();
}

void meWPHeightTerrainListener::HTActiveNavSpaceChanged( meWorld *world ) {
	if( world != pPanel.GetWorld() ){
		return;
	}
	
	pPanel.SelectActiveNavSpace();
}

void meWPHeightTerrainListener::HTNavSpaceChanged( meWorld *world, meHeightTerrainNavSpace *navspace ){
	if( world != pPanel.GetWorld() || ! navspace->GetActive() ){
		return;
	}
	
	pPanel.UpdateNavSpace();
}

void meWPHeightTerrainListener::HTNavSpaceTypeCountChanged( meWorld *world, meHeightTerrainNavSpace *navspace ){
	if( world != pPanel.GetWorld() || ! navspace->GetActive() ){
		return;
	}
	
	pPanel.UpdateNavSpaceTypeList();
}

void meWPHeightTerrainListener::HTNavSpaceActiveTypeChanged( meWorld *world, meHeightTerrainNavSpace *navspace ){
	if( world != pPanel.GetWorld() || ! navspace->GetActive() ){
		return;
	}
	
	pPanel.SelectActiveNavSpaceType();
}

void meWPHeightTerrainListener::HTNavSpaceTypeChanged( meWorld *world,
meHeightTerrainNavSpace *navspace, meHeightTerrainNavSpaceType *type ){
	if( world != pPanel.GetWorld() || ! navspace->GetActive() || navspace->GetActiveType() != type ){
		return;
	}
	
	// name could change so update list too. not a problem here since the data is small
	pPanel.UpdateNavSpaceTypeList();
}

void meWPHeightTerrainListener::HTNavSpaceFacesChanged( meWorld *world, meHeightTerrainNavSpace *navspace ){
	if( world != pPanel.GetWorld() || ! navspace->GetActive() ){
		return;
	}
	
// 	pPanel.UpdateSelectedPoints();
}

void meWPHeightTerrainListener::HTNavSpaceSelectedPointsChanged( meWorld *world ){
	if( world != pPanel.GetWorld() ){
		return;
	}
	
	pPanel.UpdateSelectedPoints();
}



void meWPHeightTerrainListener::HTVLayerCountChanged( meWorld* ){
	pPanel.UpdateVLayerList();
}

void meWPHeightTerrainListener::HTActiveVLayerChanged( meWorld *world ){
	pPanel.SetVLayer( world->GetHeightTerrain()->GetActiveVLayer() );
}

void meWPHeightTerrainListener::HTVLayerChanged( meWorld*, meHTVegetationLayer *vlayer ){
	if( vlayer != pPanel.GetVLayer() ){
		return;
	}
	pPanel.UpdateVLayer();
}



void meWPHeightTerrainListener::HTVLVariationCountChanged( meWorld*, meHTVegetationLayer *vlayer ){
	if( vlayer != pPanel.GetVLayer() ){
		return;
	}
	pPanel.UpdateVVariationList();
}

void meWPHeightTerrainListener::HTVLActiveVariationChanged( meWorld*, meHTVegetationLayer *vlayer ){
	if( vlayer != pPanel.GetVLayer() ){
		return;
	}
	pPanel.SetVVariation( vlayer->GetActiveVariation() );
}

void meWPHeightTerrainListener::HTVLVariationChanged( meWorld*, meHTVegetationLayer *vlayer,
meHTVVariation *variation ){
	if( vlayer != pPanel.GetVLayer() || variation != pPanel.GetVVariation() ){
		return;
	}
	pPanel.UpdateVVariation();
}
