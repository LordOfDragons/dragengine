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

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "gdeVAOBillboard.h"
#include "gdeViewActiveObject.h"
#include "../gdeWindowMain.h"
#include "../../configuration/gdeConfiguration.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/objectClass/gdeObjectClass.h"
#include "../../gamedef/objectClass/billboard/gdeOCBillboard.h"
#include "../../gdEditor.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/billboard/deBillboard.h>
#include <dragengine/resources/billboard/deBillboardManager.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/world/deWorld.h>



// Class gdeVAOBillboard
//////////////////////////

// Constructor, destructor
////////////////////////////

gdeVAOBillboard::gdeVAOBillboard(gdeViewActiveObject &view, const gdeObjectClass &objectClass,
	const decString &propertyPrefix, gdeOCBillboard::Ref ocbillboard) :
gdeVAOSubObject(view, objectClass, propertyPrefix),
pOCBillboard(ocbillboard)
{
	if(!ocbillboard){
		DETHROW(deeInvalidParam);
	}
	pCreateBillboard();
	pCreateCollider();
	pAttachBillboard();
}

gdeVAOBillboard::~gdeVAOBillboard(){
	pCleanUp();
}



// Management
///////////////

void gdeVAOBillboard::RebuildResources(){
	const deBillboard::Ref oldBillboard(pBillboard);
	
	pReleaseResources();
	
	pCreateBillboard();
	pCreateCollider();
	pAttachBillboard();
}

void gdeVAOBillboard::SelectedObjectChanged(){
}

void gdeVAOBillboard::GetExtends(decVector &minExtend, decVector &maxExtend) const{
	minExtend.SetZero();
	maxExtend.SetZero();
	if(!pBillboard){
		return;
	}
	
	const decVector position(pOCBillboard->GetPosition());
	const decVector2 &size = pOCBillboard->GetSize();
	const float width = size.x * 0.5f;
	const float height = size.y * 0.5f;
	
	if(width > height){
		minExtend.x = position.x - width;
		minExtend.y = position.y - width;
		minExtend.z = position.z - width;
		maxExtend.x = position.x + width;
		maxExtend.y = position.y + width;
		maxExtend.z = position.z + width;
		
	}else{
		minExtend.x = position.x - height;
		minExtend.y = position.y - height;
		minExtend.z = position.z - height;
		maxExtend.x = position.x + height;
		maxExtend.y = position.y + height;
		maxExtend.z = position.z + height;
	}
}



// Private functions
//////////////////////

void gdeVAOBillboard::pCleanUp(){
	pReleaseResources();
}

void gdeVAOBillboard::pCreateBillboard(){
	// load the new resources. if the resource is already in use it is not loaded again just the
	// reference count increased by one. loading of individual resources is allowed to fail. in
	// this case the exception is logged and the resource simply not loaded or replaced with a
	// placeholder. this behavior can be changed later on
	deVirtualFileSystem * const vfs = pView.GetGameDefinition()->GetPreviewVFS();
	igdeEnvironment &environment = pView.GetWindowMain().GetEnvironment();
	const deEngine &engine = *pView.GetGameDefinition()->GetEngine();
	deSkin::Ref skin;
	decString path;
	
	path = PropertyString(pOCBillboard->GetPropertyName(gdeOCBillboard::epSkin), pOCBillboard->GetSkinPath());
	if(!path.IsEmpty()){
		try{
			skin.TakeOver(engine.GetSkinManager()->LoadSkin(vfs, path, "/"));
			
		}catch(const deException &){
			skin = environment.GetStockSkin(igdeEnvironment::essError);
		}
	}
	
	// create billboard if skin is present
	if(skin){
		pBillboard.TakeOver(engine.GetBillboardManager()->CreateBillboard());
		pBillboard->SetSkin(skin);
		pBillboard->SetAxis(PropertyVector(pOCBillboard->GetPropertyName(gdeOCBillboard::epAxis), pOCBillboard->GetAxis()));
		pBillboard->SetSize(pOCBillboard->GetSize());
		pBillboard->SetOffset(PropertyVector2(pOCBillboard->GetPropertyName(gdeOCBillboard::epOffset), pOCBillboard->GetOffset()));
		pBillboard->SetLocked(PropertyBool(pOCBillboard->GetPropertyName(gdeOCBillboard::epLocked), pOCBillboard->GetLocked()));
		pBillboard->SetSpherical(PropertyBool(pOCBillboard->GetPropertyName(gdeOCBillboard::epSpherical), pOCBillboard->GetSpherical()));
		pBillboard->SetSizeFixedToScreen(pOCBillboard->GetSizeFixedToScreen());
		pView.GetGameDefinition()->GetWorld()->AddBillboard(pBillboard);
	}
}

void gdeVAOBillboard::pCreateCollider(){
	decShapeList shapeList;
	if(pBillboard){
		shapeList.Add(new decShapeBox(decVector(
			pOCBillboard->GetSize().x * 0.5f, pOCBillboard->GetSize().y * 0.5f, 0.02f)));
		
	}else{
		shapeList.Add(new decShapeBox(decVector(0.1f, 0.1f, 0.1f)));
	}
	
	pCollider.TakeOver(pView.GetGameDefinition()->GetEngine()->GetColliderManager()->CreateColliderVolume());
	pCollider->SetEnabled(true);
	pCollider->SetResponseType(deCollider::ertKinematic);
	pCollider->SetUseLocalGravity(true);
	((deColliderVolume&)(deCollider&)pCollider).SetShapes(shapeList);
	
	decLayerMask collisionMask;
	collisionMask.SetBit(0);
	pCollider->SetCollisionFilter(decCollisionFilter(collisionMask));
	
	pView.GetGameDefinition()->GetWorld()->AddCollider(pCollider);
}

void gdeVAOBillboard::pAttachBillboard(){
	if(!pBillboard || !pCollider){
		return;
	}
	
	deColliderAttachment *attachment = NULL;
	
	try{
		attachment = new deColliderAttachment(pCollider);
		attachment->SetAttachType(deColliderAttachment::eatStatic);
		pCollider->AddAttachment(attachment);
		attachment = NULL;
		
	}catch(const deException &){
		if(attachment){
			delete attachment;
		}
		throw;
	}
}

void gdeVAOBillboard::pReleaseResources(){
	deWorld &world = *pView.GetGameDefinition()->GetWorld();
	
	if(pCollider){
		pCollider->RemoveAllAttachments(); // because otherwise cyclic loop with component
		world.RemoveCollider(pCollider);
		pCollider = NULL;
	}
	
	if(pBillboard){
		world.RemoveBillboard(pBillboard);
		pBillboard = NULL;
	}
}
