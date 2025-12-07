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

#include "meCLCollect.h"
#include "meCLHitListEntry.h"
#include "../filter/meFilterObjects.h"
#include "../world/meWorld.h"
#include "../world/meColliderOwner.h"
#include "../world/decal/meDecal.h"
#include "../world/object/meObject.h"
#include "../world/terrain/meHeightTerrain.h"
#include "../world/terrain/meHeightTerrainSector.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainSector.h>


// Class meCLCollect
//////////////////////

// Constructor, destructor
////////////////////////////

meCLCollect::meCLCollect(meWorld &world) :
pWorld(world),
pTestHeightTerrain(false),
pTestObjects(false),
pTestDecals(false),
pTestSnapPoints(false),
pFilterObjects(nullptr),
pIgnoreDecal(nullptr){
}

meCLCollect::~meCLCollect(){
}


// Management
///////////////

void meCLCollect::SetTestHeightTerrain(bool test){
	pTestHeightTerrain = test;
}

void meCLCollect::SetTestObjects(bool test){
	pTestObjects = test;
}

void meCLCollect::SetTestDecals(bool test){
	pTestDecals = test;
}

void meCLCollect::SetTestSnapPoints(bool test){
	pTestSnapPoints = test;
}

void meCLCollect::SetFilterObjects(const meFilterObjects *filter){
	pFilterObjects = filter;
}

void meCLCollect::SetIgnoreDecal(meDecal *decal){
	pIgnoreDecal = decal;
}


void meCLCollect::Reset(){
	pElements.RemoveAllEntries();
}


// Notifications
//////////////////

void meCLCollect::CollisionResponse(deCollider *owner, deCollisionInfo *info){
	if(info->IsHTSector()){
		if(!pTestHeightTerrain){
			return;
		}
		
		meHeightTerrainSector * const htsector = pWorld.GetHeightTerrain()->
			GetSectorWith(info->GetHTSector()->GetSector());
		
		if(htsector->GetTextureCount() == 0){
			return;
		}
		
		const meCLHitListEntry::Ref entry(meCLHitListEntry::Ref::NewWith());
		entry->SetHTSector(htsector);
		entry->SetDistance(info->GetDistance());
		entry->SetNormal(info->GetNormal());
		pElements.AddEntry(entry);
		
	}else if(info->IsCollider()){
		const meColliderOwner * const colliderOwner = meColliderOwner::GetColliderOwner(
			*pWorld.GetEnvironment(), info->GetCollider());
		if(!colliderOwner){
			return;
		}
		
		if(colliderOwner->GetObject()){
			if(!pTestObjects){
				return;
			}
			if(pIgnoreObjects.Has(colliderOwner->GetObject())){
				return;
			}
			if(pFilterObjects && !pFilterObjects->AcceptObject(colliderOwner->GetObject())){
				return;
			}
			
			const meCLHitListEntry::Ref entry(meCLHitListEntry::Ref::NewWith());
			entry->SetObject(colliderOwner->GetObject());
			entry->SetDistance(info->GetDistance());
			entry->SetNormal(info->GetNormal());
			pElements.AddEntry(entry);
			
		}else if(colliderOwner->GetDecal()){
			if(!pTestDecals){
				return;
			}
			if(colliderOwner->GetDecal() == pIgnoreDecal){
				return;
			}
			
			const meCLHitListEntry::Ref entry(meCLHitListEntry::Ref::NewWith());
			entry->SetDecal(colliderOwner->GetDecal());
			entry->SetDistance(info->GetDistance());
			entry->SetNormal(info->GetNormal());
			pElements.AddEntry(entry);
			
		}else if(colliderOwner->GetSnapPoint()){
			if(!pTestSnapPoints){
				return;
			}
			
			const meCLHitListEntry::Ref entry(meCLHitListEntry::Ref::NewWith());
			entry->SetSnapPoint(colliderOwner->GetSnapPoint());
			entry->SetDistance(info->GetDistance());
			entry->SetNormal(info->GetNormal());
			pElements.AddEntry(entry);
		}
	}
}

bool meCLCollect::CanHitCollider(deCollider *owner, deCollider *collider){
	return true;
}

void meCLCollect::ColliderChanged(deCollider *owner){
}
