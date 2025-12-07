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

#include "meCLSelect.h"
#include "meCLHitListEntry.h"
#include "../world/meWorld.h"
#include "../world/meColliderOwner.h"
#include "../world/decal/meDecal.h"
#include "../world/decal/meDecalSelection.h"
#include "../world/object/meObject.h"
#include "../world/object/meObjectSelection.h"
#include "../world/navspace/meNavigationSpace.h"
#include "../world/navspace/meNavigationSpaceSelection.h"
#include "../world/objectshape/meObjectShape.h"
#include "../world/terrain/meHeightTerrain.h"
#include "../world/terrain/meHeightTerrainSector.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainSector.h>



// Class meCLSelect
/////////////////////

// Constructor, destructor
////////////////////////////

meCLSelect::meCLSelect(meWorld &world) :
pWorld(world),
pSingleSelect(true),
pRectSelect(false),

pCanHitObjects(true),
pCanHitObjectShapes(false),
pCanHitDecals(false),
pCanHitNavSpaces(false),
pCanHitHTNavPoints(false),

pCanSelectObjects(true),
pCanSelectObjectShapes(false),
pCanSelectDecals(false),
pCanSelectNavSpaces(false),
pCanSelectHTNavPoints(false){
}

meCLSelect::~meCLSelect(){
}



// Management
///////////////

void meCLSelect::SetRay(const decDVector &origin, const decVector &direction){
	pRayOrigin = origin;
	pRayDirection = direction;
}



void meCLSelect::SetSingleSelect(bool singleSelect){
	pSingleSelect = singleSelect;
}

void meCLSelect::SetRectSelect(bool rectSelect){
	pRectSelect = rectSelect;
}



void meCLSelect::SetCanHitObjects(bool canHit){
	pCanHitObjects = canHit;
}

void meCLSelect::SetCanHitObjectShapes(bool canHit){
	pCanHitObjectShapes = canHit;
}

void meCLSelect::SetCanHitDecals(bool canHit){
	pCanHitDecals = canHit;
}

void meCLSelect::SetCanHitNavSpaces(bool canHit){
	pCanHitNavSpaces = canHit;
}

void meCLSelect::SetCanHitHTNavPoints(bool canHit){
	pCanHitHTNavPoints = canHit;
}

void meCLSelect::SetCanHitAll(bool canHit){
	pCanHitObjects = canHit;
	pCanHitObjectShapes = canHit;
	pCanHitDecals = canHit;
	pCanHitNavSpaces = canHit;
	pCanHitHTNavPoints = canHit;
}



void meCLSelect::SetCanSelectObjects(bool canSelect){
	pCanSelectObjects = canSelect;
}

void meCLSelect::SetCanSelectObjectShapes(bool canSelect){
	pCanSelectObjectShapes = canSelect;
}

void meCLSelect::SetCanSelectDecals(bool canSelect){
	pCanSelectDecals = canSelect;
}

void meCLSelect::SetCanSelectNavSpaces(bool canSelect){
	pCanSelectNavSpaces = canSelect;
}

void meCLSelect::SetCanSelectHTNavPoints(bool canSelect){
	pCanSelectHTNavPoints = canSelect;
}

void meCLSelect::SetCanSelectAll(bool canSelect){
	pCanSelectObjects = canSelect;
	pCanSelectObjectShapes = canSelect;
	pCanSelectDecals = canSelect;
	pCanSelectNavSpaces = canSelect;
	pCanSelectHTNavPoints = canSelect;
}



void meCLSelect::Reset(){
	pRectSelOldObjectList.RemoveAll();
	pRectSelOldObjectShapeList.RemoveAll();
	pHitList.RemoveAllEntries();
}

void meCLSelect::StoreRectSelect(){
	if(pCanHitObjects){
		pRectSelOldObjectList = pWorld.GetSelectionObject().GetSelected();
	}
	if(pCanHitObjectShapes){
		pRectSelOldObjectShapeList = pWorld.GetSelectionObjectShape().GetSelected();
	}
}

void meCLSelect::Prepare(){
	pHitList.RemoveAllEntries();
}

void meCLSelect::RunAction(int selectIndex){
	const int entryCount = pHitList.GetEntryCount();
	meObjectShapeList rselListObjectShapes;
	meObjectList rselListObjects;
	int i;
	
	if(pRectSelect){
		rselListObjects = pRectSelOldObjectList;
		rselListObjectShapes = pRectSelOldObjectShapeList;
		
	}else{
		pHitList.SortByDistance();
	}
	
	for(i=selectIndex; i<entryCount; i++){
		const meCLHitListEntry &entry = *pHitList.GetEntryAt(i);
		meObject * const object = entry.GetObject();
		meObjectShape * const objectShape = entry.GetObjectShape();
		meDecal * const decal = entry.GetDecal();
		meNavigationSpace * const navspace = entry.GetNavigationSpace();
		const int htnavPoint = entry.GetHTNavSpacePoint();
		
		if(pCanSelectObjects && object){
			meObjectSelection &selection = pWorld.GetSelectionObject();
			
			if(pRectSelect){
				rselListObjects.AddIfAbsent(object);
				
				if(!object->GetSelected()){
					selection.Add(object);
				}
				
			}else{
				if(pSingleSelect){
					selection.Reset();
					selection.Add(object);
					selection.SetActive(object);
					
				}else{
					if(object->GetSelected()){
						if(selection.GetActive() == object){
							selection.ActivateNext();
						}
						selection.Remove(object);
						
					}else{
						selection.Add(object);
						selection.SetActive(object);
					}
				}
			}
			
			if(!pRectSelect){
				pWorld.NotifyObjectSelectionChanged();
				break;
			}
			
		}else if(pCanSelectObjectShapes && objectShape){
			meObjectShapeSelection &selection = pWorld.GetSelectionObjectShape();
			
			if(pRectSelect){
				rselListObjectShapes.AddIfAbsent(objectShape);
				
				if(!objectShape->GetSelected()){
					selection.Add(objectShape);
				}
				
			}else{
				if(pSingleSelect){
					selection.Reset();
					selection.Add(objectShape);
					selection.SetActive(objectShape);
					
				}else{
					if(objectShape->GetSelected()){
						if(selection.GetActive() == objectShape){
							selection.ActivateNext();
						}
						selection.Remove(objectShape);
						
					}else{
						selection.Add(objectShape);
						selection.SetActive(objectShape);
					}
				}
			}
			
			if(!pRectSelect){
				pWorld.NotifyObjectShapeSelectionChanged();
				break;
			}
			
		}else if(pCanSelectDecals && decal){
			meDecalSelection &selection = pWorld.GetSelectionDecal();
			
			if(pSingleSelect){
				selection.Reset();
				selection.Add(decal);
				selection.SetActive(decal);
				
			}else{
				if(decal->GetSelected()){
					if(selection.GetActive() == decal){
						selection.SetActive(NULL);
					}
					selection.Remove(decal);
					
				}else{
					selection.Add(decal);
					selection.SetActive(decal);
				}
			}
			
			pWorld.NotifyDecalSelectionChanged();
			break;
			
		}else if(pCanSelectNavSpaces && navspace){
			meNavigationSpaceSelection &selection = pWorld.GetSelectionNavigationSpace();
			
			if(pSingleSelect){
				selection.Reset();
				selection.Add(navspace);
				selection.SetActive(navspace);
				
			}else{
				if(navspace->GetSelected()){
					if(selection.GetActive() == navspace){
						selection.SetActive(NULL);
					}
					selection.Remove(navspace);
					
				}else{
					selection.Add(navspace);
					selection.SetActive(navspace);
				}
			}
			
			pWorld.NotifyNavSpaceSelectionChanged();
			break;
			
		}else if(pCanSelectHTNavPoints && htnavPoint != -1){
			meHeightTerrain * const heightTerrain = pWorld.GetHeightTerrain();
			if(!heightTerrain){
				break;
			}
			
			meHeightTerrainSector * const sector = heightTerrain->GetActiveSector();
			if(!sector){
				break;
			}
			
			decIntList selection(sector->GetSelectedNavPoints());
			
			if(pSingleSelect){
				selection.RemoveAll();
				selection.Add(htnavPoint);
				
			}else{
				const int index = selection.IndexOf(htnavPoint);
				
				if(index == -1){
					selection.Add(htnavPoint);
					
				}else{
					selection.RemoveFrom(index);
				}
			}
			
			sector->SetSelectedNavPoints(selection);
			pWorld.NotifyHTNavSpaceSelectedPointsChanged();
			break;
		}
	}
	
	if(pRectSelect){
		if(pCanSelectObjects){
			meObjectSelection &selection = pWorld.GetSelectionObject();
			const meObjectList &listSelected = selection.GetSelected();
			meObjectList listDeselect;
			int count;
			
			count = listSelected.GetCount();
			for(i=0; i<count; i++){
				if(!rselListObjects.Has(listSelected.GetAt(i))){
					listDeselect.Add(listSelected.GetAt(i));
				}
			}
			
			count = listDeselect.GetCount();
			for(i=0; i<count; i++){
				selection.Remove(listDeselect.GetAt(i));
			}
			
			if(!selection.HasActive() || !selection.GetActive()->GetSelected()){
				selection.ActivateNext();
			}
			
			pWorld.NotifyObjectSelectionChanged();
			
		}else if(pCanSelectObjectShapes){
			meObjectShapeSelection &selection = pWorld.GetSelectionObjectShape();
			const meObjectShapeList &listSelected = selection.GetSelected();
			meObjectShapeList listDeselect;
			int count;
			
			count = listSelected.GetCount();
			for(i=0; i<count; i++){
				if(!rselListObjectShapes.Has(listSelected.GetAt(i))){
					listDeselect.Add(listSelected.GetAt(i));
				}
			}
			
			count = listDeselect.GetCount();
			for(i=0; i<count; i++){
				selection.Remove(listDeselect.GetAt(i));
			}
			
			if(!selection.HasActive() || !selection.GetActive()->GetSelected()){
				selection.ActivateNext();
			}
			
			pWorld.NotifyObjectShapeSelectionChanged();
		}
	}
	
	pHitList.RemoveAllEntries();
}



// Notifications
//////////////////

void meCLSelect::CollisionResponse(deCollider *owner, deCollisionInfo *info){
	if(info->IsCollider()){
		const meColliderOwner * const colliderOwner = meColliderOwner::GetColliderOwner(
			*pWorld.GetEnvironment(), info->GetCollider() );
		if(!colliderOwner){
			return;
		}
		
		if(colliderOwner->GetObject()){
			if(!pCanHitObjects){
				return;
			}
			
			const meCLHitListEntry::Ref entry(meCLHitListEntry::Ref::NewWith());
			entry->SetObject(colliderOwner->GetObject());
			entry->SetDistance(info->GetDistance());
			pHitList.AddEntry(entry);
			
		}else if(colliderOwner->GetDecal()){
			if(!pCanHitDecals){
				return;
			}
			
			const meCLHitListEntry::Ref entry(meCLHitListEntry::Ref::NewWith());
			entry->SetDecal(colliderOwner->GetDecal());
			entry->SetDistance(info->GetDistance());
			pHitList.AddEntry(entry);
			
		}else if(colliderOwner->GetNavigationSpace()){
			if(!pCanHitNavSpaces){
				return;
			}
			
			const meCLHitListEntry::Ref entry(meCLHitListEntry::Ref::NewWith());
			entry->SetNavigationSpace(colliderOwner->GetNavigationSpace());
			entry->SetDistance(info->GetDistance());
			pHitList.AddEntry(entry);
			
		}else if(colliderOwner->GetShape()){
			if(!pCanHitObjectShapes){
				return;
			}
			
			const meCLHitListEntry::Ref entry(meCLHitListEntry::Ref::NewWith());
			entry->SetObjectShape(colliderOwner->GetShape());
			entry->SetDistance(info->GetDistance());
			pHitList.AddEntry(entry);
		}
		
	}else if(info->IsHTSector()){
		deHeightTerrainSector * const engSector = info->GetHTSector();
		meHeightTerrainSector *sector = NULL;
		if(engSector){
			sector = pWorld.GetHeightTerrain()->GetSectorWith(engSector->GetSector());
		}
		
		if(sector){
			if(pCanHitHTNavPoints){
				const int resolution = pWorld.GetHeightTerrain()->GetSectorResolution();
				const decPoint comax(resolution - 1, resolution - 1);
				const decDVector hitPoint(pRayOrigin + pRayDirection * (double)info->GetDistance());
				const decVector2 coordinates(sector->GetGridPointAt(hitPoint));
				const decPoint closest(decPoint(coordinates + decVector2(0.5f, 0.5f)).Clamped(decPoint(), comax));
				
				const meCLHitListEntry::Ref entry(meCLHitListEntry::Ref::NewWith());
				entry->SetHTNavSpacePoint(resolution * closest.y + closest.x);
				entry->SetDistance(info->GetDistance());
				pHitList.AddEntry(entry);
			}
			
			return;
		}
	}
}

bool meCLSelect::CanHitCollider(deCollider *owner, deCollider *collider){
	return true;
}

void meCLSelect::ColliderChanged(deCollider *owner){
}
