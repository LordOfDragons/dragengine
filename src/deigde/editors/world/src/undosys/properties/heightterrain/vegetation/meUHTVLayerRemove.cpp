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

#include "meUHTVLayerRemove.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/terrain/meHeightTerrain.h"
#include "../../../../world/heightterrain/meHTVegetationLayer.h"

#include <dragengine/common/exceptions.h>



// Class meUHTVLayerRemove
//////////////////////////////

// Constructor, destructor
////////////////////////////

meUHTVLayerRemove::meUHTVLayerRemove(meWorld *world, meHeightTerrain *heightTerrain, meHTVegetationLayer *vlayer){
	if(!world || !heightTerrain || !vlayer) DETHROW(deeInvalidParam);
	
	pWorld = world;
	pHeightTerrain = heightTerrain;
	
	pIndex = heightTerrain->IndexOfVLayer(vlayer);
	if(pIndex == -1) DETHROW(deeInvalidParam);
	
	SetShortInfo("Remove Height Terrain Vegetation Layer");
	
	world->AddReference();
	
	pVLayer = vlayer;
	vlayer->AddReference();
}

meUHTVLayerRemove::~meUHTVLayerRemove(){
}



// Management
///////////////

void meUHTVLayerRemove::Undo(){
	pHeightTerrain->InsertVLayer(pIndex, pVLayer);
}

void meUHTVLayerRemove::Redo(){
	pHeightTerrain->RemoveVLayer(pVLayer);
}
