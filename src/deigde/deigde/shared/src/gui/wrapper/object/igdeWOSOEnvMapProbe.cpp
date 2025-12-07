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

#include "igdeWOSOEnvMapProbe.h"
#include "igdeWOSOVisitor.h"
#include "../igdeWObject.h"
#include "../../../codec/igdeCodecPropertyString.h"
#include "../../../engine/igdeEngineController.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../gamedefinition/igdeGameDefinition.h"
#include "../../../gamedefinition/class/igdeGDClass.h"
#include "../../../gamedefinition/class/envmapprobe/igdeGDCEnvMapProbe.h"
#include "../../../gameproject/igdeGameProject.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/probe/deEnvMapProbe.h>
#include <dragengine/resources/probe/deEnvMapProbeManager.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/world/deWorld.h>



// Class igdeWOSOEnvMapProbe
////////////////////////

// Constructor, destructor
////////////////////////////

igdeWOSOEnvMapProbe::igdeWOSOEnvMapProbe(igdeWObject &wrapper,
	const igdeGDCEnvMapProbe &gdEnvMapProbe, const decString &prefix) :
igdeWOSubObject(wrapper, prefix),
pGDEnvMapProbe(gdEnvMapProbe),
pAddedToWorld(false),
pAttachment(NULL)
{
	wrapper.SubObjectFinishedLoading(*this, true);
}

igdeWOSOEnvMapProbe::~igdeWOSOEnvMapProbe(){
	pDestroyEnvMapProbe();
}



// Management
///////////////

void igdeWOSOEnvMapProbe::UpdateParameters(){
	GetWrapper().SubObjectFinishedLoading(*this, true);
}

void igdeWOSOEnvMapProbe::UpdateLayerMasks(){
	if(pEnvMapProbe){
		pEnvMapProbe->SetLayerMask(LayerMaskFromInt(GetWrapper().GetRenderEnvMapMask()));
	}
}

void igdeWOSOEnvMapProbe::OnAllSubObjectsFinishedLoading(){
	pUpdateEnvMapProbe();
}

void igdeWOSOEnvMapProbe::Visit(igdeWOSOVisitor &visitor){
	visitor.VisitEnvMapProbe(*this);
}



// Private Functions
//////////////////////

void igdeWOSOEnvMapProbe::pUpdateEnvMapProbe(){
	if(!pEnvMapProbe){
		pEnvMapProbe.TakeOver(GetEngine().GetEnvMapProbeManager()->CreateEnvMapProbe());
		
		pEnvMapProbe->SetScaling(pGDEnvMapProbe.GetScaling());
		
		UpdateLayerMasks();
	}
	
	igdeCodecPropertyString codec;
	decShapeList shapeList;
	decString value;
	
	pEnvMapProbe->SetInfluencePriority(GetIntProperty(
		pGDEnvMapProbe.GetPropertyName(igdeGDCEnvMapProbe::epInfluencePriority),
		pGDEnvMapProbe.GetInfluencePriority()));
	pEnvMapProbe->SetInfluenceBorderSize(GetFloatProperty(
		pGDEnvMapProbe.GetPropertyName(igdeGDCEnvMapProbe::epInfluenceBorderSize),
		pGDEnvMapProbe.GetInfluenceBorderSize()));
	
	// influence area property
	if(GetPropertyValue(pGDEnvMapProbe.GetPropertyName(igdeGDCEnvMapProbe::epInfluenceArea), value)){
		codec.DecodeShapeList(value, pEnvMapProbe->GetShapeListInfluence());
		
	}else{
		pEnvMapProbe->GetShapeListInfluence() = pGDEnvMapProbe.GetShapeListInfluence();
	}
	pEnvMapProbe->NotifyShapeListInfluenceChanged();
	
	// reflection shape
	if(GetPropertyValue(pGDEnvMapProbe.GetPropertyName(igdeGDCEnvMapProbe::epReflectionShape), value)){
		codec.DecodeShapeList(value, shapeList);
		
		if(shapeList.GetCount() == 0){
			pEnvMapProbe->SetShapeReflection(NULL);
			
		}else{
			pEnvMapProbe->SetShapeReflection(shapeList.GetAt(0)->Copy());
		}
		
	}else{
		if(pGDEnvMapProbe.GetShapeReflection()){
			pEnvMapProbe->SetShapeReflection(pGDEnvMapProbe.GetShapeReflection()->Copy());
			
		}else{
			pEnvMapProbe->SetShapeReflection(NULL);
		}
	}
	
	// reflection mask property
	if(GetPropertyValue(pGDEnvMapProbe.GetPropertyName(igdeGDCEnvMapProbe::epReflectionMask), value)){
		codec.DecodeShapeList(value, pEnvMapProbe->GetShapeListReflectionMask());
		
	}else{
		pEnvMapProbe->GetShapeListReflectionMask() = pGDEnvMapProbe.GetShapeListReflectionMask();
	}
	pEnvMapProbe->NotifyShapeReflectionChanged();
	
	if(!pAddedToWorld){
		GetWrapper().GetWorld()->AddEnvMapProbe(pEnvMapProbe);
		pAddedToWorld = true;
	}
	if(pAddedToWorld && !pAttachedToCollider){
		AttachToCollider();
	}
}

void igdeWOSOEnvMapProbe::pDestroyEnvMapProbe(){
	if(!pEnvMapProbe){
		return;
	}
	
	DetachFromCollider();
	
	if(pAddedToWorld){
		GetWrapper().GetWorld()->RemoveEnvMapProbe(pEnvMapProbe);
	}
	
	pEnvMapProbe = NULL;
	pAddedToWorld = false;
}

void igdeWOSOEnvMapProbe::AttachToCollider(){
	DetachFromCollider();
	
	if(!pEnvMapProbe){
		return;
	}
	
	deColliderComponent * const colliderComponent = GetAttachableColliderComponent();
	deColliderVolume * const colliderFallback = GetWrapper().GetColliderFallback();
	deColliderAttachment *attachment = NULL;
	
	try{
		attachment = new deColliderAttachment(pEnvMapProbe);
		attachment->SetAttachType(deColliderAttachment::eatStatic);
		attachment->SetPosition(GetVectorProperty(
			pGDEnvMapProbe.GetPropertyName(igdeGDCEnvMapProbe::epAttachPosition),
			pGDEnvMapProbe.GetPosition()));
		attachment->SetOrientation(GetRotationProperty(
			pGDEnvMapProbe.GetPropertyName(igdeGDCEnvMapProbe::epAttachRotation),
			pGDEnvMapProbe.GetOrientation()));
		
		if(colliderComponent){
			colliderComponent->AddAttachment(attachment);
			pAttachedToCollider = colliderComponent;
			
		}else{
			colliderFallback->AddAttachment(attachment);
			pAttachedToCollider = colliderFallback;
		}
		
		pAttachment = attachment;
		
	}catch(const deException &){
		if(attachment){
			delete attachment;
		}
		throw;
	}
}

void igdeWOSOEnvMapProbe::DetachFromCollider(){
	if(!pAttachedToCollider){
		return;
	}
	
	pAttachedToCollider->RemoveAttachment(pAttachment);
	pAttachment = NULL;
	pAttachedToCollider = NULL;
}
