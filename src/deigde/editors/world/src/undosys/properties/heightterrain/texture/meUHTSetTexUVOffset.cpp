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

#include "meUHTSetTexUVOffset.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/terrain/meHeightTerrain.h"
#include "../../../../world/terrain/meHeightTerrainSector.h"
#include "../../../../world/terrain/meHeightTerrainTexture.h"

#include <dragengine/common/exceptions.h>

// Class meUHTSetTexUVOffset
/////////////////////////

// Constructor, destructor
////////////////////////////

meUHTSetTexUVOffset::meUHTSetTexUVOffset(meWorld *world, meHeightTerrainSector *sector, meHeightTerrainTexture *texture, const decVector2 &newOffset){
	if(!world || !sector || !texture) DETHROW(deeInvalidParam);
	
	pWorld = world;
	pSector = sector;
	
	pOldOffset.x = texture->GetProjectionOffsetU();
	pOldOffset.y = texture->GetProjectionOffsetV();
	pNewOffset = newOffset;
	
	SetShortInfo("Set Height Terrain Texture UV Offset");
	
	world->AddReference();
	
	pTexture = texture;
	texture->AddReference();
}

meUHTSetTexUVOffset::~meUHTSetTexUVOffset(){

}

// Management
///////////////

void meUHTSetTexUVOffset::Undo(){
	pTexture->SetProjectionOffsetU(pOldOffset.x);
	pTexture->SetProjectionOffsetV(pOldOffset.y);
}

void meUHTSetTexUVOffset::Redo(){
	pTexture->SetProjectionOffsetU(pNewOffset.x);
	pTexture->SetProjectionOffsetV(pNewOffset.y);
}
