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

#include "igdeGizmo.h"
#include "../../environment/igdeEnvironment.h"
#include "../../engine/igdeEngineController.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelFace.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>



// Class igdeGizmo
////////////////////

// Constructor, destructor
////////////////////////////

igdeGizmo::igdeGizmo(igdeEnvironment &environment) :
pEnvironment(environment),
pTransparency(0.45f),
pHoverTransparency(0.95f),
pHoverColorMultiply(1.0f, 1.0f, 1.0f),
pIsEditing(false),
pIsHovering(false)
{
	deEngine &engine = *environment.GetEngineController()->GetEngine();
	
	pDebugDrawer.TakeOver(engine.GetDebugDrawerManager()->CreateDebugDrawer());
	
	pCollider.TakeOver(engine.GetColliderManager()->CreateColliderRig());
	pCollider->SetUseLocalGravity(true);
	pCollider->SetResponseType(deCollider::ertKinematic);
	
	decLayerMask cfcategory, cffilter;
	
	deColliderAttachment *attachment = nullptr;
	try{
		attachment = new deColliderAttachment(pDebugDrawer);
		attachment->SetAttachType(deColliderAttachment::eatStatic);
		pCollider->AddAttachment(attachment);
		
	}catch(const deException &){
		if(attachment){
			delete attachment;
		}
		throw;
	}
}

igdeGizmo::~igdeGizmo(){
	if(pCollider){
		pEnvironment.SetColliderUserPointer(pCollider, nullptr);
	}
	
	SetWorld(nullptr);
}



// Management
///////////////

void igdeGizmo::SetWorld(deWorld *world){
	if(world == pWorld){
		return;
	}
	
	if(pWorld){
		pWorld->RemoveCollider(pCollider);
		pWorld->RemoveDebugDrawer(pDebugDrawer);
		OnRemoveFromWorld();
	}
	
	pWorld = world;
	
	if(world){
		world->AddDebugDrawer(pDebugDrawer);
		world->AddCollider(pCollider);
		OnAddToWorld();
	}
}



const decColor &igdeGizmo::GetShapeColor(const char *name) const{
	const cShapeColor * const shapeColor = pNamedShapeColor(name);
	DEASSERT_NOTNULL(shapeColor)
	
	return shapeColor->color;
}

void igdeGizmo::SetShapeColor(const char *name, const decColor &color){
	cShapeColor * const shapeColor = pNamedShapeColor(name);
	if(shapeColor){
		if(! shapeColor->color.IsEqualTo(color)){
			shapeColor->color = color;
			pApplyShapeColors();
		}
		
	}else{
		const int ddshapeIndex = pModelTextureNames.IndexOf(name);
		pShapeColors.Add(deObject::Ref::New(new cShapeColor(name, color, ddshapeIndex)));
		if(ddshapeIndex != -1){
			pApplyShapeColors();
		}
	}
}

void igdeGizmo::SetTransparency(float transparency){
	transparency = decMath::clamp(transparency, 0.0f, 1.0f);
	if(fabsf(transparency - pTransparency) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pTransparency = transparency;
	
	if(! pIsHovering){
		pApplyShapeColors();
	}
}

void igdeGizmo::SetHoverTransparency(float transparency){
	transparency = decMath::clamp(transparency, 0.0f, 1.0f);
	if(fabsf(transparency - pHoverTransparency) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pHoverTransparency = transparency;
	
	if(pIsHovering){
		pApplyShapeColors();
	}
}

void igdeGizmo::SetHoverColorMultiply(const decColor &multiply){
	if(multiply.IsEqualTo(pHoverColorMultiply)){
		return;
	}
	
	pHoverColorMultiply = multiply;
	
	if(pIsHovering){
		pApplyShapeColors();
	}
}

const decCollisionFilter &igdeGizmo::GetCollisionFilter() const{
	return pCollider->GetCollisionFilter();
}

void igdeGizmo::SetCollisionFilter(const decCollisionFilter &filter){
	pCollider->SetCollisionFilter(filter);
}



void igdeGizmo::SetColliderUserPointer(void *userPointer){
	pEnvironment.SetColliderUserPointer(pCollider, userPointer);
}



bool igdeGizmo::GetVisible() const{
	return pDebugDrawer->GetVisible();
}

void igdeGizmo::SetVisible(bool visible){
	pCollider->SetEnabled(visible);
	pDebugDrawer->SetVisible(visible);
}

const decDVector &igdeGizmo::GetPosition() const{
	return pCollider->GetPosition();
}

void igdeGizmo::SetPosition(const decDVector &position){
	pCollider->SetPosition(position);
}

const decQuaternion &igdeGizmo::GetOrientation() const{
	return pCollider->GetOrientation();
}

void igdeGizmo::SetOrientation(const decQuaternion &orientation){
	pCollider->SetOrientation(orientation);
}

const decVector &igdeGizmo::GetScale() const{
	return pCollider->GetScale();
}

void igdeGizmo::SetScale(const decVector &scale){
	pCollider->SetScale(scale);
}

void igdeGizmo::SetGeometry(const decDVector &position, const decQuaternion &orientation){
	pCollider->SetGeometry(position, orientation);
}

void igdeGizmo::SetGeometry(const decDMatrix &matrix){
	pCollider->SetGeometry(matrix.GetPosition(), matrix.Normalized().ToQuaternion());
}

decDMatrix igdeGizmo::GetMatrix() const{
	return decDMatrix::CreateWorld(pCollider->GetPosition(), pCollider->GetOrientation());
}



void igdeGizmo::SetShapeFromModel(const deModel &model){
	const int shapeColorCount = pShapeColors.GetCount();
	int i;
	for(i=0; i<shapeColorCount; i++){
		((cShapeColor*)pShapeColors.GetAt(i))->ddshapeIndex = -1;
	}
	
	pModelTextureNames.RemoveAll();
	pDebugDrawer->RemoveAllShapes();
	
	const deModelLOD &lod = *model.GetLODAt(0);
	const deModelVertex * const vertices = lod.GetVertices();
	const deModelFace * const faces = lod.GetFaces();
	const int textureCount = model.GetTextureCount();
	deDebugDrawerShapeFace *ddsface = nullptr;
	const int faceCount = lod.GetFaceCount();
	deDebugDrawerShape *ddshape = nullptr;
	
	try{
		for(i=0; i<textureCount; i++){
			const decString &name = model.GetTextureAt(i)->GetName();
			
			ddshape = new deDebugDrawerShape;
			ddshape->SetEdgeColor(decColor(0.0f, 0.0f, 0.0f, 0.0f));
			pDebugDrawer->AddShape(ddshape);
			ddshape = nullptr;
			
			cShapeColor * const shapeColor = pNamedShapeColor(name);
			if(shapeColor){
				shapeColor->ddshapeIndex = i;
			}
			
			pModelTextureNames.Add(name);
		}
		
	}catch(const deException &){
		if(ddshape){
			delete ddshape;
		}
		pDebugDrawer->NotifyShapeLayoutChanged();
		throw;
	}
	
	try{
		for(i=0; i<faceCount; i++){
			const deModelFace &face = faces[i];
			ddsface = new deDebugDrawerShapeFace;
			ddsface->AddVertex(vertices[face.GetVertex1()].GetPosition());
			ddsface->AddVertex(vertices[face.GetVertex2()].GetPosition());
			ddsface->AddVertex(vertices[face.GetVertex3()].GetPosition());
			ddsface->CalculateNormal();
			pDebugDrawer->GetShapeAt(face.GetTexture())->AddFace(ddsface);
			ddsface = nullptr;
		}
		
	}catch(const deException &){
		if(ddsface){
			delete ddsface;
		}
		pDebugDrawer->NotifyShapeLayoutChanged();
		throw;
	}
	
	pDebugDrawer->NotifyShapeLayoutChanged();
}

deRig *igdeGizmo::GetRig() const{
	return pCollider->GetRig();
}

void igdeGizmo::SetRig(deRig *rig){
	pCollider->SetRig(rig);
}



// Interaction
////////////////

void igdeGizmo::StartHovering(const decDVector &rayOrigin, const decDVector &rayDirection,
double distance, int bone, int shape, int modifiers){
	DEASSERT_FALSE(pIsHovering)
	
	pHoverShapeName = pCollisionShapeName(bone, shape);
	pIsHovering = true;
	pApplyShapeColors();
	
	OnStartHovering(rayOrigin, rayDirection, rayOrigin + rayDirection * distance, modifiers);
}

void igdeGizmo::UpdateHovering(const decDVector &rayOrigin, const decDVector &rayDirection,
double distance, int bone , int shape, int modifiers){
	DEASSERT_TRUE(pIsHovering)
	
	pHoverShapeName = pCollisionShapeName(bone, shape);
	pApplyShapeColors();
	
	OnUpdateHovering(rayOrigin, rayDirection, rayOrigin + rayDirection * distance, modifiers);
}

void igdeGizmo::StopHovering(){
	DEASSERT_TRUE(pIsHovering)
	
	pHoverShapeName.Empty();
	pIsHovering = false;
	pApplyShapeColors();
	
	OnStopHovering();
}



bool igdeGizmo::StartEditing(const decDVector &rayOrigin, const decDVector &rayDirection,
const decDMatrix &viewMatrix, double distance, int bone, int shape, int modifiers){
	DEASSERT_FALSE(pIsEditing)
	if(bone == -1 && shape == -1){
		return false;
	}
	
	if(OnStartEditing(rayOrigin, rayDirection, viewMatrix,
	rayOrigin + rayDirection * distance, pCollisionShapeName(bone, shape), modifiers)){
		pIsEditing = true;
		return true;
		
	}else{
		return false;
	}
}

void igdeGizmo::UpdateEditing(const decDVector &rayOrigin, const decDVector &rayDirection,
const decDMatrix &viewMatrix, int modifiers){
	DEASSERT_TRUE(pIsEditing)
	OnUpdateEditing(rayOrigin, rayDirection, viewMatrix, modifiers);
}

void igdeGizmo::MouseWheeledEditing(const decDVector &rayOrigin, const decDVector &rayDirection,
const decDMatrix &viewMatrix, const decPoint &change, int modifiers){
	DEASSERT_TRUE(pIsEditing)
	OnMouseWheeledEditing(rayOrigin, rayDirection, viewMatrix, change, modifiers);
}

void igdeGizmo::StopEditing(bool cancel){
	DEASSERT_TRUE(pIsEditing)
	OnStopEditing(cancel);
	pIsEditing = false;
}



void igdeGizmo::OnFrameUpdate(float){
}



// Protected Functions
////////////////////////

void igdeGizmo::OnStartHovering(const decDVector &, const decDVector &, const decDVector &, int) {
}

void igdeGizmo::OnUpdateHovering(const decDVector &, const decDVector &, const decDVector &, int) {
}

void igdeGizmo::OnStopHovering(){
}

void igdeGizmo::OnUpdateEditing(const decDVector &, const decDVector &, const decDMatrix &, int){
}

void igdeGizmo::OnMouseWheeledEditing(const decDVector &, const decDVector &,
const decDMatrix &, const decPoint &, int){
}

void igdeGizmo::OnStopEditing(bool){
}

void igdeGizmo::OnAddToWorld(){
}

void igdeGizmo::OnRemoveFromWorld(){
}



// Private Functions
//////////////////////

void igdeGizmo::pApplyShapeColors(){
	const int count = pShapeColors.GetCount();
	decColor color;
	int i;
	
	for(i=0; i<count; i++){
		const cShapeColor &shapeColor = *(cShapeColor*)pShapeColors.GetAt(i);
		if(shapeColor.ddshapeIndex == -1){
			continue;
		}
		
		if(pIsHovering && shapeColor.name == pHoverShapeName){
			color = decColor(shapeColor.color * pHoverColorMultiply, pHoverTransparency);
			
		}else{
			color = decColor(shapeColor.color, pTransparency);
		}
		
		deDebugDrawerShape &shape = *pDebugDrawer->GetShapeAt(shapeColor.ddshapeIndex);
		shape.SetEdgeColor(decColor(color, 0.0f));
		shape.SetFillColor(color);
	}
	
	pDebugDrawer->NotifyShapeColorChanged();
}

igdeGizmo::cShapeColor *igdeGizmo::pNamedShapeColor(const char *name) const{
	const int count = pShapeColors.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		cShapeColor *shapeColor = (cShapeColor*)pShapeColors.GetAt(i);
		if(shapeColor->name == name){
			return shapeColor;
		}
	}
	return nullptr;
}

const decString &igdeGizmo::pCollisionShapeName(int bone, int shape) const{
	if(pCollider->GetRig()){
		if(bone != -1){
			return pCollider->GetRig()->GetBoneAt(bone).GetName();
			
		}else if(shape != -1){
			return pCollider->GetRig()->GetShapeProperties().GetAt(shape);
		}
	}
	
	static const decString emptyName;
	return emptyName;
}
