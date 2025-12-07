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

#include <stdlib.h>

#include "meCLAddDecal.h"
#include "meCLHitListEntry.h"
#include "../worldedit.h"
#include "../gui/meWindowMain.h"
#include "../world/meColliderOwner.h"
#include "../world/meWorld.h"
#include "../world/meWorldGuiParameters.h"
#include "../world/decal/meDecal.h"
#include "../world/decal/meDecalSelection.h"
#include "../world/object/meObject.h"
#include "../undosys/gui/decal/meUAddDecal.h"
#include "../undosys/gui/decal/meUAddObjectDecal.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/skin/igdeGDSkin.h>
#include <deigde/gamedefinition/skin/igdeGDSkinManager.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/decal/deDecal.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define LOGSOURCE "World Editor"



// Class meCLAddDecal
///////////////////////

// Constructor, destructor
////////////////////////////

meCLAddDecal::meCLAddDecal(meWindowMain *windowMain, meWorld *world){
	if(!windowMain || !world) DETHROW(deeInvalidParam);
	
	pWndMain = windowMain;
	pWorld = world;
	
	pAlignWithNormal = true;
}

meCLAddDecal::~meCLAddDecal(){
}



// Management
///////////////

void meCLAddDecal::SetRay(const decDVector &rayOrigin, const decVector &rayDirection){
	pRayOrigin = rayOrigin;
	pRayDirection = rayDirection;
}

void meCLAddDecal::SetAlignWithNormal(bool alignWithNormal){
	pAlignWithNormal = alignWithNormal;
}



void meCLAddDecal::Reset(){
	if(pUndo){
		pUndo->Undo();
	}
	
	pHitList.RemoveAllEntries();
}

void meCLAddDecal::RunAction(){
	pHitList.SortByDistance();
	
	if(pHitList.GetEntryCount() == 0){
		pUndo = NULL;
		return;
	}
	
	meCLHitListEntry * const entry = pHitList.GetEntryAt(0);
	meObject *object = entry->GetObject();
	decVector normal;
	
	if(pAlignWithNormal){
		normal = entry->GetNormal().Normalized();
		
	}else{
		normal = -pRayDirection.Normalized();
	}
	
	decDVector hitPoint(pRayOrigin + pRayDirection * entry->GetDistance());
	//pWndMain->GetLogger()->LogErrorFormat( LOGSOURCE, "debug: %g,%g,%g | %g,%g,%g\n", pRayOrigin.x, pRayOrigin.y, pRayOrigin.z, pRayDirection.x, pRayDirection.y, pRayDirection.z );
	//pWndMain->GetLogger()->LogErrorFormat( LOGSOURCE, "debug: %g | %g,%g,%g | %g,%g,%g\n", entry->GetDistance(), hitPoint.x, hitPoint.y, hitPoint.z, normal.x, normal.y, normal.z );
	
	if(!object){
		pUndo = NULL;
		return;
	}
	
	const meWorldGuiParameters &guiparams = pWorld->GetGuiParameters();
	const decVector rotation(pGetRotationForNormal(normal));
	const decString &browseSkin = guiparams.GetBrowseSkin();
	decVector decalSize;
	
	if(pUndo){
		decalSize = ((meUAddDecal&)(igdeUndo&)pUndo).GetDecal()->GetSize();
		
	}else{
		meDecal::Ref decal(meDecal::Ref::NewWith(pWorld->GetEnvironment()));
		
		decal->SetSkinPath(browseSkin);
		decal->SetSize(decal->GetDefaultSize(0.5f));
		decalSize = decal->GetSize();
		
		pUndo.TakeOver(new meUAddDecal(pWorld, decal));
	}
	
	meDecal &decal = *((meUAddDecal&)(igdeUndo&)pUndo).GetDecal();
	decal.SetPosition(hitPoint + decDVector(normal * (decalSize.z * 0.5f)));
	decal.SetRotation(rotation);
	pUndo->Redo();
}

void meCLAddDecal::Finish(){
	if(!pUndo){
		return;
	}
	
	pWorld->GetUndoSystem()->Add(pUndo, false);
	pUndo = NULL;
}

void meCLAddDecal::Cancel(){
	pUndo = NULL;
}



// Notifications
//////////////////

void meCLAddDecal::CollisionResponse(deCollider *owner, deCollisionInfo *info){
	if(info->IsCollider()){
		const meColliderOwner * const colliderOwner = meColliderOwner::GetColliderOwner(
			*pWorld->GetEnvironment(), info->GetCollider() );
		if(!colliderOwner){
			return;
		}
		
		if(colliderOwner->GetObject()){
			const meCLHitListEntry::Ref entry(meCLHitListEntry::Ref::NewWith());
			entry->SetObject(colliderOwner->GetObject());
			entry->SetDistance(info->GetDistance());
			entry->SetNormal(info->GetNormal());
			pHitList.AddEntry(entry);
		}
		
	}else if(info->IsHTSector()){
	}
}

bool meCLAddDecal::CanHitCollider(deCollider *owner, deCollider *collider){
	return true;
}

void meCLAddDecal::ColliderChanged(deCollider *owner){
}



// Private Functions
//////////////////////

decVector meCLAddDecal::pGetRotationForNormal(const decVector &normal) const{
	decMatrix matrix;
	
	if(fabs(normal.y) > 0.707106){
		matrix.SetVU(normal, decVector(0.0f, 0.0f, 1.0f));
		
	}else{
		matrix.SetVU(normal, decVector(0.0f, 1.0f, 0.0f));
	}
	
	return matrix.GetEulerAngles() / DEG2RAD;
}
