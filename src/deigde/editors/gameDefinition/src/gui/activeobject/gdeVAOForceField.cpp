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

#include "gdeVAOForceField.h"
#include "gdeViewActiveObject.h"
#include "../gdeWindowMain.h"
#include "../../gdEditor.h"
#include "../../configuration/gdeConfiguration.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/objectClass/forceField/gdeOCForceField.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWCoordSysArrows.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/forcefield/deForceField.h>
#include <dragengine/resources/forcefield/deForceFieldManager.h>
#include <dragengine/resources/world/deWorld.h>



// Class gdeVAOForceField
///////////////////////////

// Constructor, destructor
////////////////////////////

gdeVAOForceField::gdeVAOForceField(gdeViewActiveObject &view, const gdeObjectClass &objectClass,
	const decString &propertyPrefix, gdeOCForceField *ocfield) :
gdeVAOSubObject(view, objectClass, propertyPrefix),
pOCForceField(ocfield),
pDDSCenter(NULL),
pDDSCoordSystem(NULL)
{
	if(!ocfield){
		DETHROW(deeInvalidParam);
	}
	
	
	try{
		pCreateDebugDrawer();
		pCreateForceField();
		pUpdateDDShapes();
		pUpdateDDShapeColor();
		
		AttachResources();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

gdeVAOForceField::~gdeVAOForceField(){
	pCleanUp();
}



// Management
///////////////

void gdeVAOForceField::RebuildResources(){
	DetachResources();
	pReleaseResources();
	
	pCreateForceField();
	AttachResources();
	
	pUpdateDDShapes();
	pUpdateDDShapeColor();
}

void gdeVAOForceField::AttachResources(){
	if(!pForceField){
		return;
	}
	
	deCollider * const attachCollider = pView.GetAttachComponentCollider();
	if(!attachCollider){
		return;
	}
	
	const decVector &position = pOCForceField->GetPosition();
	const decQuaternion orientation(decQuaternion::CreateFromEuler(
		pOCForceField->GetRotation() * DEG2RAD));
	
	deColliderAttachment *attachment = NULL;
	try{
		// attach particleEmitter
		attachment = new deColliderAttachment(pForceField);
		attachment->SetPosition(position);
		attachment->SetOrientation(orientation);
		attachment->SetAttachType(deColliderAttachment::eatStatic);
		
		attachCollider->AddAttachment(attachment);
		attachment = NULL;
		
		// attach debug drawer
		attachment = new deColliderAttachment(pDebugDrawer);
		attachment->SetPosition(position);
		attachment->SetOrientation(orientation);
		attachment->SetAttachType(deColliderAttachment::eatStatic);
		
		attachCollider->AddAttachment(attachment);
		attachment = NULL;
		
	}catch(const deException &){
		if(attachment){
			delete attachment;
		}
		throw;
	}
}

void gdeVAOForceField::DetachResources(){
	if(!pForceField){
		return;
	}
	
	deCollider * const attachCollider = pView.GetAttachComponentCollider();
	if(!attachCollider){
		return;
	}
	
	deColliderAttachment *attachment = NULL;
	attachment = attachCollider->GetAttachmentWith(pForceField);
	if(attachment){
		attachCollider->RemoveAttachment(attachment);
	}
	
	attachment = attachCollider->GetAttachmentWith(pDebugDrawer);
	if(attachment){
		attachCollider->RemoveAttachment(attachment);
	}
}

void gdeVAOForceField::SelectedObjectChanged(){
	pUpdateDDShapeColor();
}



// Private functions
//////////////////////

void gdeVAOForceField::pCleanUp(){
	DetachResources();
	pReleaseResources();
	
	if(pDDSCoordSystem){
		delete pDDSCoordSystem;
	}
	if(pDDSCenter){
		delete pDDSCenter;
	}
	if(pDebugDrawer){
		pView.GetGameDefinition()->GetWorld()->RemoveDebugDrawer(pDebugDrawer);
		pDebugDrawer = NULL;
	}
	
}



void gdeVAOForceField::pCreateDebugDrawer(){
	const deEngine &engine = *pView.GetGameDefinition()->GetEngine();
	
	// create debug drawer
	pDebugDrawer.TakeOver(engine.GetDebugDrawerManager()->CreateDebugDrawer());
	pDebugDrawer->SetXRay(true);
	pView.GetGameDefinition()->GetWorld()->AddDebugDrawer(pDebugDrawer);
	
	// create center shape
	pDDSCenter = new igdeWDebugDrawerShape;
	pDDSCenter->AddSphereShape(0.05f, decVector());
	pDDSCenter->SetParentDebugDrawer(pDebugDrawer);
	
	// create coordinate system shape
	pDDSCoordSystem = new igdeWCoordSysArrows;
	pDDSCoordSystem->SetArrowLength(0.2f);
	pDDSCoordSystem->SetArrowSize(0.01f);
	pDDSCoordSystem->SetParentDebugDrawer(pDebugDrawer);
}

void gdeVAOForceField::pCreateForceField(){
	const deEngine &engine = *pView.GetGameDefinition()->GetEngine();
	pForceField.TakeOver(engine.GetForceFieldManager()->CreateForceField());
	
	pForceField->SetInfluenceArea(pOCForceField->GetInfluenceArea());
	pForceField->SetRadius(pOCForceField->GetRadius());
	pForceField->SetExponent(pOCForceField->GetExponent());
	pForceField->SetFieldType(pOCForceField->GetFieldType());
	pForceField->SetApplicationType(pOCForceField->GetApplicationType());
	pForceField->SetDirection(pOCForceField->GetDirection());
	pForceField->SetForce(pOCForceField->GetForce());
	pForceField->SetFluctuationDirection(pOCForceField->GetFluctuationDirection() * DEG2RAD);
	pForceField->SetFluctuationForce(pOCForceField->GetFluctuationForce());
	pForceField->SetShape(pOCForceField->GetShape());
	pForceField->SetEnabled(pOCForceField->GetEnabled());
	
	decLayerMask collisionMask;
	collisionMask.SetBit(0);
	const decCollisionFilter collisionFilter(collisionMask);
	pForceField->SetCollisionFilter(collisionFilter);
	
	pView.GetGameDefinition()->GetWorld()->AddForceField(pForceField);
}

void gdeVAOForceField::pUpdateDDShapes(){
}

void gdeVAOForceField::pUpdateDDShapeColor(){
	const gdeConfiguration &config = pView.GetWindowMain().GetConfiguration();
	
	if(pView.GetGameDefinition()->GetSelectedObjectType() == gdeGameDefinition::eotOCForceField
	&& pView.GetGameDefinition()->GetActiveOCForceField() == pOCForceField){
		pDDSCenter->SetEdgeColor(decColor(config.GetColorForceFieldActive(), 1.0f));
		pDDSCenter->SetFillColor(config.GetColorForceFieldActive());
		
	}else{
		pDDSCenter->SetEdgeColor(decColor(config.GetColorForceField(), 0.25f));
		pDDSCenter->SetFillColor(config.GetColorForceField());
	}
}



void gdeVAOForceField::pReleaseResources(){
	deWorld &world = *pView.GetGameDefinition()->GetWorld();
	
	if(pForceField){
		world.RemoveForceField(pForceField);
		pForceField = NULL;
	}
}
