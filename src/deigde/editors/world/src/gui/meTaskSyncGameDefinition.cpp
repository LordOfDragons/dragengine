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

#include "meTaskSyncGameDefinition.h"
#include "meWindowMain.h"
#include "properties/meWindowProperties.h"
#include "../world/meWorld.h"
#include "../world/decal/meDecal.h"
#include "../world/object/meObject.h"
#include "../world/object/texture/meObjectTexture.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/wrapper/igdeWSky.h>
#include <deigde/gui/wrapper/igdeWObject.h>

#include <dragengine/common/exceptions.h>



// Class meTaskSyncGameDefinition
///////////////////////////////////

// Constructor, destructor
////////////////////////////

meTaskSyncGameDefinition::meTaskSyncGameDefinition(meWindowMain &windowMain) :
pWindowMain(windowMain),
pState(esProcessWorld),
pObjectIndex(0),
pDecalIndex(0),
pStepIndex(0),
pStepCount(0),
pBatchSizeObjects(100),
pBatchSizeDecals(100)
{
	SetMessage("World Editor: Synchronize Game Definition");
}

meTaskSyncGameDefinition::~meTaskSyncGameDefinition(){
}



// Management
///////////////

bool meTaskSyncGameDefinition::Step(){
	if(!pWindowMain.GetWorld()){
		return false;
	}
	
	meWorld &world = *pWindowMain.GetWorld();
	
	switch(pState){
	case esProcessWorld:
		pStepCount = 1; // esProcessWorld
		pStepCount += (decMath::max(world.GetObjects().GetCount() - 1, 0)
			/ pBatchSizeObjects) + 1;
		pStepCount += (decMath::max(world.GetDecals().GetCount() - 1, 0)
			/ pBatchSizeDecals) + 1;
		pStepCount += 1; // esProcessWindows
		
		world.GetSky()->OnGameDefinitionChanged();
		
		pState = esProcessObjects;
		pStepIndex++;
		return true;
		
	case esProcessObjects:{
		const meObject::List &objects = world.GetObjects();
		const int objectCount = objects.GetCount();
		const int lastIndex = decMath::min(pObjectIndex + pBatchSizeObjects, objectCount);
		int i;
		
		SetProgress((float)pStepIndex / (float)pStepCount);
		
		decString message;
		message.Format("World Editor: Synchronize Game Definition: Objects (%d/%d)",
			pObjectIndex, objectCount);
		SetMessage(message);
		
		for(; pObjectIndex<lastIndex; pObjectIndex++){
			meObject &object = *objects.GetAt(pObjectIndex);
			object.OnGameDefinitionChanged();
			
			const int textureCount = object.GetTextures().GetCount();
			for(i=0; i<textureCount; i++){
				meObjectTexture &texture = *object.GetTextures().GetAt(i);
				texture.LoadSkin();
			}
			
			const int decalCount = object.GetDecals().GetCount();
			for(i=0; i<decalCount; i++){
				meDecal &decal = *object.GetDecals().GetAt(i);
				decal.OnGameDefinitionChanged();
				decal.LoadSkin();
			}
		}
		
		pStepIndex++;
		if(pObjectIndex == objectCount){
			pState = esProcessDecals;
		}
		}
		return true;
		
	case esProcessDecals:{
		const meDecal::List &decals = world.GetDecals();
		const int decalCount = decals.GetCount();
		const int lastIndex = decMath::min(pDecalIndex + pBatchSizeDecals, decalCount);
		
		SetProgress((float)pStepIndex / (float)pStepCount);
		decString message;
		message.Format("World Editor: Synchronize Game Definition: Decals (%d/%d)",
			pDecalIndex, pDecalIndex);
		SetMessage(message);
		
		for(; pDecalIndex<lastIndex; pDecalIndex++){
			meDecal &decal = *decals.GetAt(pDecalIndex);
			decal.OnGameDefinitionChanged();
			decal.LoadSkin();
		}
		
		pStepIndex++;
		if(pDecalIndex == decalCount){
			pState = esProcessWindows;
		}
		}
		return true;
		
	case esProcessWindows:
		SetProgress((float)pStepIndex / (float)pStepCount);
		SetMessage("World Editor: Synchronize Game Definition");
		
		pWindowMain.GetWindowProperties()->OnGameDefinitionChanged();
		pState = esFinished;
		pStepIndex++;
		return false;
		
	case esFinished:
		return false;
	}
	
	return false;
}
