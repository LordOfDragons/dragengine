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

meWPHeightTerrainListener::meWPHeightTerrainListener(meWPHeightTerrain &panel) :
pPanel(panel){
}

meWPHeightTerrainListener::~meWPHeightTerrainListener(){
}



// Notifications
//////////////////

void meWPHeightTerrainListener::HeightPaintChanged(meWorld*){
	pPanel.UpdateHeightPaint();
}

void meWPHeightTerrainListener::MaskPaintChanged(meWorld*){
	pPanel.UpdateMaskPaint();
}

void meWPHeightTerrainListener::VisibilityPaintChanged(meWorld*){
	pPanel.UpdateVisibilityPaint();
}



void meWPHeightTerrainListener::ActiveSectorChanged(meWorld*){
	pPanel.SetTexture(nullptr);
	
	pPanel.UpdateSector();
	
	if(pPanel.GetSector()){
		pPanel.SetTexture(pPanel.GetSector()->GetActiveTexture());
	}
}



void meWPHeightTerrainListener::HTStateChanged(meWorld*){
	pPanel.UpdateHeightTerrain();
}

void meWPHeightTerrainListener::HTChanged(meWorld*){
	pPanel.UpdateHeightTerrain();
}

void meWPHeightTerrainListener::HTSChanged(meWorld*, meHeightTerrainSector *sector){
	if(sector != pPanel.GetSector()){
		return;
	}
	pPanel.UpdateSector();
}

void meWPHeightTerrainListener::HTSHeightChanged(meWorld*, meHeightTerrainSector *sector){
	if(sector != pPanel.GetSector()){
		return;
	}
	pPanel.UpdateSector();
}

void meWPHeightTerrainListener::HTSVisibilityChanged(meWorld*, meHeightTerrainSector *sector){
	if(sector != pPanel.GetSector()){
		return;
	}
	pPanel.UpdateSector();
}



void meWPHeightTerrainListener::HTSTextureCountChanged(meWorld*, meHeightTerrainSector *sector){
	if(sector != pPanel.GetSector()){
		return;
	}
	pPanel.UpdateTextureList();
}

void meWPHeightTerrainListener::HTSActiveTextureChanged(meWorld*, meHeightTerrainSector *sector){
	if(sector != pPanel.GetSector()){
		return;
	}
	pPanel.SetTexture(sector->GetActiveTexture());
}

void meWPHeightTerrainListener::HTSTextureChanged(meWorld*, meHeightTerrainSector *sector,
meHeightTerrainTexture *texture){
	if(sector != pPanel.GetSector() || texture != pPanel.GetTexture()){
		return;
	}
	pPanel.UpdateTexture();
}

void meWPHeightTerrainListener::HTSTextureMaskChanged(meWorld*, meHeightTerrainSector *sector,
meHeightTerrainTexture *texture){
	if(sector != pPanel.GetSector() || texture != pPanel.GetTexture()){
		return;
	}
	pPanel.UpdateTexture();
}



void meWPHeightTerrainListener::HTNavSpaceCountChanged(meWorld *world){
	if(world != pPanel.GetWorld()){
		return;
	}
	
	pPanel.UpdateNavSpaceList();
}

void meWPHeightTerrainListener::HTActiveNavSpaceChanged(meWorld *world) {
	if(world != pPanel.GetWorld()){
		return;
	}
	
	pPanel.SelectActiveNavSpace();
}

void meWPHeightTerrainListener::HTNavSpaceChanged(meWorld *world, meHeightTerrainNavSpace *navspace){
	if(world != pPanel.GetWorld() || !navspace->GetActive()){
		return;
	}
	
	pPanel.UpdateNavSpace();
}

void meWPHeightTerrainListener::HTNavSpaceTypeCountChanged(meWorld *world, meHeightTerrainNavSpace *navspace){
	if(world != pPanel.GetWorld() || !navspace->GetActive()){
		return;
	}
	
	pPanel.UpdateNavSpaceTypeList();
}

void meWPHeightTerrainListener::HTNavSpaceActiveTypeChanged(meWorld *world, meHeightTerrainNavSpace *navspace){
	if(world != pPanel.GetWorld() || !navspace->GetActive()){
		return;
	}
	
	pPanel.SelectActiveNavSpaceType();
}

void meWPHeightTerrainListener::HTNavSpaceTypeChanged(meWorld *world,
meHeightTerrainNavSpace *navspace, meHeightTerrainNavSpaceType *type){
	if(world != pPanel.GetWorld() || !navspace->GetActive() || navspace->GetActiveType() != type){
		return;
	}
	
	// name could change so update list too. not a problem here since the data is small
	pPanel.UpdateNavSpaceTypeList();
}

void meWPHeightTerrainListener::HTNavSpaceFacesChanged(meWorld *world, meHeightTerrainNavSpace *navspace){
	if(world != pPanel.GetWorld() || !navspace->GetActive()){
		return;
	}
	
// 	pPanel.UpdateSelectedPoints();
}

void meWPHeightTerrainListener::HTNavSpaceSelectedPointsChanged(meWorld *world){
	if(world != pPanel.GetWorld()){
		return;
	}
	
	pPanel.UpdateSelectedPoints();
}



void meWPHeightTerrainListener::HTVLayerCountChanged(meWorld*){
	pPanel.UpdateVLayerList();
}

void meWPHeightTerrainListener::HTActiveVLayerChanged(meWorld *world){
	pPanel.SetVLayer(world->GetHeightTerrain()->GetActiveVLayer());
}

void meWPHeightTerrainListener::HTVLayerChanged(meWorld*, meHTVegetationLayer *vlayer){
	if(vlayer != pPanel.GetVLayer()){
		return;
	}
	pPanel.UpdateVLayer();
}



void meWPHeightTerrainListener::HTVLVariationCountChanged(meWorld*, meHTVegetationLayer *vlayer){
	if(vlayer != pPanel.GetVLayer()){
		return;
	}
	pPanel.UpdateVVariationList();
}

void meWPHeightTerrainListener::HTVLActiveVariationChanged(meWorld*, meHTVegetationLayer *vlayer){
	if(vlayer != pPanel.GetVLayer()){
		return;
	}
	pPanel.SetVVariation(vlayer->GetActiveVariation());
}

void meWPHeightTerrainListener::HTVLVariationChanged(meWorld*, meHTVegetationLayer *vlayer,
meHTVVariation *variation){
	if(vlayer != pPanel.GetVLayer() || variation != pPanel.GetVVariation()){
		return;
	}
	pPanel.UpdateVVariation();
}
