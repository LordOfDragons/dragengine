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

#include "meDecal.h"
#include "meAttachedDecal.h"
#include "../meWorld.h"
#include "../object/meObject.h"
#include "../meWorldGuiParameters.h"
#include "../../collisions/meCLCollect.h"
#include "../../collisions/meCLHitListEntry.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeTimer.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/decal/deDecal.h>
#include <dragengine/resources/decal/deDecalList.h>
#include <dragengine/resources/decal/deDecalManager.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/skin/deSkinTexture.h>
#include <dragengine/resources/skin/property/deSkinProperty.h>
#include <dragengine/resources/skin/property/deSkinPropertyImage.h>
#include <dragengine/resources/skin/property/deSkinPropertyVideo.h>
#include <dragengine/resources/skin/property/deSkinPropertyConstructed.h>
#include <dragengine/resources/skin/property/deSkinPropertyVisitorIdentify.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeGroup.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkinManager.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableColor.h>
#include <dragengine/resources/video/deVideo.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/systems/modules/physics/deBasePhysicsWorld.h>



// Reattach declas timer
//////////////////////////

class meDecalTimerReattachDecals : public igdeTimer {
private:
	meDecal &pDecal;
	
public:
	meDecalTimerReattachDecals(meDecal &decal) : igdeTimer(*decal.GetEnvironment()), pDecal(decal){
	}
	
	virtual void OnTimeout(){
		pDecal.AttachDecals();
	}
};



// Class meDecal
//////////////////

// Constructor, destructor
////////////////////////////

meDecal::meDecal(igdeEnvironment *environment) :
pEnvironment(environment),

pEngSkin(NULL),

pAttachedDecals(NULL),
pAttachedDecalCount(0),

pWorld(NULL),

pDebugDrawer(NULL),
pDDSDecal(NULL),
pCollider(NULL),

pParentObject(NULL),

pSize(0.5f, 0.5f, 1.0f),
pTexCoordScaling(1.0f, 1.0f),
pTexCoordRotation(0.0f),

pColorTint(1.0f, 1.0f, 1.0f),

pDynamicSkin(NULL),

pID(0),
pVisible(true),

pSelected(false),
pActive(false),

pColliderOwner(this)
{
	if(!environment){
		DETHROW(deeInvalidParam);
	}
	
	pInitShared();
}

meDecal::meDecal(const meDecal &decal) :
pEnvironment(decal.pEnvironment),

pEngSkin(NULL),

pAttachedDecals(NULL),
pAttachedDecalCount(0),

pWorld(NULL),

pDebugDrawer(NULL),
pDDSDecal(NULL),
pCollider(NULL),

pParentObject(NULL),

pSkinPath(decal.pSkinPath),
pTextureName(decal.pTextureName),
pPosition(decal.pPosition),
pRotation(decal.pRotation),
pSize(decal.pSize),
pTexCoordOffset(decal.pTexCoordOffset),
pTexCoordScaling(decal.pTexCoordScaling),
pTexCoordRotation(decal.pTexCoordRotation),

pProperties(decal.pProperties),
pActiveProperty(decal.pActiveProperty),

pColorTint(decal.pColorTint),

pDynamicSkin(NULL),

pID(0),
pVisible(decal.pVisible),

pSelected(false),
pActive(false),

pColliderOwner(this)
{
	pInitShared();
	
	try{
		pEngSkin = decal.pEngSkin;
		if(pEngSkin){
			pEngSkin->AddReference();
		}
		// pUpdateSkin(); // not needed as there are no decals yet
		
		pRepositionShapes();
		pUpdateShapes();
		// pAttachDecals(); // not needed as we have no parent world yet
		
		UpdateDynamicSkin();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meDecal::~meDecal(){
	pCleanUp();
}



// Management
///////////////

void meDecal::SetWorld(meWorld *world){
	DetachDecals();
	
	if(pWorld){
		pWorld->GetEngineWorld()->RemoveCollider(pCollider);
		pWorld->GetEngineWorld()->RemoveDebugDrawer(pDebugDrawer);
	}
	
	pWorld = world;
	
	if(world){
		world->GetEngineWorld()->AddDebugDrawer(pDebugDrawer);
		world->GetEngineWorld()->AddCollider(pCollider);
	}
	
	ShowStateChanged();
	InvalidateDecals();
}

void meDecal::SetParentObject(meObject *object){
	if(object == pParentObject){
		return;
	}
	
	pParentObject = object;
	pRepositionShapes();
}



void meDecal::SetSkinPath(const char *path){
	if(pSkinPath == path){
		return;
	}
	
	pSkinPath = path;
	
	if(pWorld){
		pWorld->SetChanged(true);
		pWorld->NotifyDecalChanged(this);
	}
	
	LoadSkin();
}

void meDecal::LoadSkin(){
	pLoadSkin();
	pUpdateSkin();
}

decVector meDecal::GetDefaultSize(float baseSize) const{
	if(!pEngSkin){
		return decVector(baseSize, baseSize, baseSize);
	}
	
	if(pEngSkin->GetTextureCount() == 0){
		return decVector(baseSize, baseSize, baseSize);
	}
	
	const deSkinTexture &texture = *pEngSkin->GetTextureAt(0);
	const int count = texture.GetPropertyCount();
	deSkinPropertyVisitorIdentify identify;
	decPoint size;
	int i;
	
	for(i=0; i<count; i++){
		texture.GetPropertyAt(i)->Visit(identify);
		
		if(identify.IsImage()){
			const deImage * const image = identify.CastToImage().GetImage();
			size.Set(image->GetWidth(), image->GetHeight());
			
		}else if(identify.IsVideo()){
			const deVideo * const video = identify.CastToVideo().GetVideo();
			size.Set(video->GetWidth(), video->GetHeight());
			
		}else if(identify.IsConstructed()){
			const decPoint3 &contentSize = identify.CastToConstructed().GetContent().GetSize();
			size.Set(contentSize.x, contentSize.y);
			
		}else{
			continue;
		}
		
		if(size.x > size.y){
			return decVector(baseSize, baseSize * (float)size.y / (float)size.x, baseSize);
			
		}else{
			return decVector(baseSize * (float)size.x / (float)size.y, baseSize, baseSize);
		}
	}
	
	return decVector(baseSize, baseSize, baseSize);
}

void meDecal::SetTextureName(const char *texture){
	if(pTextureName == texture){
		return;
	}
	
	pTextureName = texture;
	
	if(pWorld){
		pWorld->SetChanged(true);
		pWorld->NotifyDecalChanged(this);
	}
	
	pUpdateSkin();
}

void meDecal::SetPosition(const decDVector &position){
	if(pPosition.IsEqualTo(position)){
		return;
	}
	
	pPosition = position;
	
	if(pWorld){
		pWorld->SetChanged(true);
		pWorld->NotifyDecalGeometryChanged(this);
	}
	
	pRepositionShapes();
	pUpdateShapes();
	InvalidateDecals();
}

void meDecal::SetRotation(const decVector &rotation){
	if(pRotation.IsEqualTo(rotation)){
		return;
	}
	
	pRotation = rotation;
	
	if(pWorld){
		pWorld->SetChanged(true);
		pWorld->NotifyDecalGeometryChanged(this);
	}
	
	pRepositionShapes();
	pUpdateShapes();
	InvalidateDecals();
}

void meDecal::SetSize(const decVector &size){
	const decVector csize(decVector(0.001f, 0.001f, 0.001f).Largest(size));
	if(pSize.IsEqualTo(csize)){
		return;
	}
	
	pSize = size;
	
	if(pWorld){
		pWorld->SetChanged(true);
		pWorld->NotifyDecalGeometryChanged(this);
	}
	
	pUpdateShapes();
	InvalidateDecals();
}

void meDecal::SetTexCoordOffset(const decVector2 &offset){
	if(pTexCoordOffset.IsEqualTo(offset)){
		return;
	}
	
	pTexCoordOffset = offset;
	pUpdateTransform();
	
	if(pWorld){
		pWorld->SetChanged(true);
		pWorld->NotifyDecalChanged(this);
	}
}

void meDecal::SetTexCoordScaling(const decVector2 &scaling){
	if(pTexCoordScaling.IsEqualTo(scaling)){
		return;
	}
	
	pTexCoordScaling = scaling;
	pUpdateTransform();
	
	if(pWorld){
		pWorld->SetChanged(true);
		pWorld->NotifyDecalChanged(this);
	}
}

void meDecal::SetTexCoordRotation(float rotation){
	if(fabsf(pTexCoordRotation - rotation) < 1e-5f){
		return;
	}
	
	pTexCoordRotation = rotation;
	pUpdateTransform();
	
	if(pWorld){
		pWorld->SetChanged(true);
		pWorld->NotifyDecalChanged(this);
	}
}

void meDecal::SetColorTint(const decColor &color){
	if(color.IsEqualTo(pColorTint)){
		return;
	}
	
	pColorTint = color;
	UpdateDynamicSkin();
	
	if(pWorld){
		pWorld->SetChanged(true);
		pWorld->NotifyDecalChanged(this);
	}
}

void meDecal::UpdateDynamicSkin(){
	const bool hasTint = !decColor(1.0f, 1.0f, 1.0f).IsEqualTo(pColorTint);
	bool requiresDynamicSkin = false;
	
	if(hasTint){
		requiresDynamicSkin = true;
	}
	
	if(requiresDynamicSkin){
		if(!pDynamicSkin){
			deEngine &engine = *pEnvironment->GetEngineController()->GetEngine();
			pDynamicSkin = engine.GetDynamicSkinManager()->CreateDynamicSkin();
		}
		
		pDynamicSkin->RemoveAllRenderables();
		
		if(hasTint){
			deDSRenderableColor *renderable = new deDSRenderableColor("tint");
			renderable->SetColor(pColorTint);
			pDynamicSkin->AddRenderable(renderable);
		}
		
	}else{
		if(pDynamicSkin){
			pDynamicSkin->FreeReference();
			pDynamicSkin = NULL;
		}
	}
	
	pUpdateSkin();
}



void meDecal::SetID(int id){
	pID = id;
}

void meDecal::SetVisible(bool visible){
	if(visible == pVisible){
		return;
	}
	
	//int a;
	
	pVisible = visible;
	/*
	// hiding the decal is bad as the user needs to know what the decal is. hence keep it visible.
	// maybe later on a visual cue can be used of some sort
	for(a=0; a<pAttachedDecalCount; a++){
		pAttachedDecals[a]->GetEngineDecal()->SetVisible(visible);
	}
	*/
	
	if(pWorld){
		pWorld->SetChanged(true);
		pWorld->NotifyDecalChanged(this);
	}
}



void meDecal::SetSelected(bool selected){
	if(selected == pSelected){
		return;
	}
	
	pSelected = selected;
	pUpdateDDSColors();
}

void meDecal::SetActive(bool active){
	if(active == pActive){
		return;
	}
	
	pActive = active;
	pUpdateDDSColors();
}



void meDecal::NotifyParentChanged(){
	pRepositionShapes();
	pUpdateShapes();
	InvalidateDecals();
}



void meDecal::ShowStateChanged(){
	if(pWorld){
		const meWorldGuiParameters &guiParams = pWorld->GetGuiParameters();
		const meWorldGuiParameters::eElementModes elementMode = guiParams.GetElementMode();
		const bool modeDecal = (elementMode == meWorldGuiParameters::eemDecal);
		
		pDDSDecal->SetVisible(modeDecal);
	}
}

void meDecal::InvalidateDecals(){
	DetachDecals();
	pTimerReattachDecals->Start(250, false);
}

void meDecal::OnGameDefinitionChanged(){
	InvalidateDecals();
}




// Properties
///////////////

void meDecal::SetProperty(const char *key, const char *value){
	bool activeChanged = false;
	
	pProperties.SetAt(key, value);
	
	if(pActiveProperty.IsEmpty()){
		pActiveProperty = key;
		activeChanged = true;
	}
	
	//UpdateProperties();
	
	NotifyPropertiesChanged();
	if(activeChanged){
		NotifyActivePropertyChanged();
	}
}

void meDecal::SetProperties(const decStringDictionary &properties){
	pProperties = properties;
	
	if(pProperties.GetCount() == 0){
		pActiveProperty = "";
		
	}else{
		pActiveProperty = pProperties.GetAt(pProperties.GetKeys().GetAt(0));
	}
	
	//UpdateProperties();
	
	NotifyPropertiesChanged();
	NotifyActivePropertyChanged();
}

void meDecal::RemoveProperty(const char *key){
	if(!pProperties.Has(key)){
		return;
	}
	
	bool activeChanged = false;
	
	pProperties.Remove(key);
	
	if(pActiveProperty == key){
		if(pProperties.GetCount() == 0){
			pActiveProperty = "";
			
		}else{
			pActiveProperty = pProperties.GetAt(pProperties.GetKeys().GetAt(0));
		}
		activeChanged = true;
	}
	
	//UpdateProperties();
	
	NotifyPropertiesChanged();
	if(activeChanged){
		NotifyActivePropertyChanged();
	}
}

void meDecal::RemoveAllProperties(){
	if(pProperties.GetCount() == 0){
		return;
	}
	
	pProperties.RemoveAll();
	pActiveProperty = "";
	
	//UpdateProperties();
	
	NotifyPropertiesChanged();
	NotifyActivePropertyChanged();
}

void meDecal::SetActiveProperty(const char *property){
	if(pActiveProperty == property){
		return;
	}
	
	pActiveProperty = property;
	
	NotifyActivePropertyChanged();
}



void meDecal::NotifyPropertiesChanged(){
	if(!pWorld){
		return;
	}
	
	pWorld->SetChanged(true);
	
	meWorld * const world = pWorld;
	if(world){
		world->NotifyDecalPropertiesChanged(this);
	}
}

void meDecal::NotifyActivePropertyChanged(){
	if(!pWorld){
		return;
	}
	
	pWorld->SetChanged(true);
	
	meWorld * const world = pWorld;
	if(world){
		world->NotifyDecalActivePropertyChanged(this);
	}
}



// Private Functions
//////////////////////

void meDecal::pInitShared(){
	deEngine &engine = *pEnvironment->GetEngineController()->GetEngine();
	
	try{
		pTimerReattachDecals.TakeOver(new meDecalTimerReattachDecals(*this));
		
		pCollider = engine.GetColliderManager()->CreateColliderVolume();
		pCollider->SetEnabled(true);
		pCollider->SetResponseType(deCollider::ertKinematic);
		pCollider->SetUseLocalGravity(true);
		
		decLayerMask collisionCategory;
		collisionCategory.SetBit(meWorld::eclmDecals);
		
		decLayerMask collisionFilter;
		collisionFilter.SetBit(meWorld::eclmEditing);
		
		pCollider->SetCollisionFilter(decCollisionFilter(collisionCategory, collisionFilter));
		
		pEnvironment->SetColliderUserPointer(pCollider, &pColliderOwner);
		
		// create debug drawer and shapes
		pDebugDrawer = engine.GetDebugDrawerManager()->CreateDebugDrawer();
		pDebugDrawer->SetXRay(true);
		
		pDDSDecal = new igdeWDebugDrawerShape;
		pDDSDecal->SetVisible(true);
		pDDSDecal->SetFillColor(decColor(0.0f, 0.0f, 0.0f, 0.0f));
		pDDSDecal->SetParentDebugDrawer(pDebugDrawer);
		
		pUpdateDDSColors();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

void meDecal::pCleanUp(){
	SetWorld(NULL);
	SetParentObject(NULL);
	
	if(pCollider){
		pEnvironment->SetColliderUserPointer(pCollider, NULL);
		pCollider->FreeReference();
	}
	
	DetachDecals();
	
	if(pDynamicSkin){
		pDynamicSkin->FreeReference();
	}
	if(pEngSkin){
		pEngSkin->FreeReference();
	}
	
	if(pDDSDecal){
		delete pDDSDecal;
	}
	if(pDebugDrawer){
		pDebugDrawer->FreeReference();
	}
}



void meDecal::DetachDecals(){
	if(!pAttachedDecals){
		return;
	}
	
	while(pAttachedDecalCount > 0){
		pAttachedDecalCount--;
		delete pAttachedDecals[pAttachedDecalCount];
	}
	
	delete [] pAttachedDecals;
	pAttachedDecals = NULL;
}

void meDecal::AttachDecals(){
	DetachDecals();
	
	meWorld *world = NULL;
	if(pWorld){
		world = pWorld;
	}
	
	if(world){
		meCLCollect collect(*world);
		
		decDMatrix matrix = decDMatrix::CreateRT(pRotation * DEG2RAD, pPosition);
		const decVector halfSize(decVector(0.001f, 0.001f, 0.001f).Largest(decVector(pSize * 0.5f)));
		
		decLayerMask collisionCategory;
		collisionCategory.SetBit(meWorld::eclmEditing);
		
		decLayerMask collisionFilter;
		collisionFilter.SetBit(meWorld::eclmObjects);
		collisionFilter.SetBit(meWorld::eclmHeightTerrains);
		
		collect.SetTestObjects(true);
		
		world->CollisionTestBox(matrix * decDVector(0.0, 0.0, -halfSize.z), matrix.ToQuaternion(),
			halfSize, &collect, decCollisionFilter(collisionCategory, collisionFilter));
		
		const meCLHitList &hitlist = collect.GetCollectedElements();
		int e, entryCount = hitlist.GetEntryCount();
		
		if(entryCount > 0){
			pAttachedDecals = new meAttachedDecal*[entryCount];
			pAttachedDecalCount = 0;
			
			for(e=0; e<entryCount; e++){
				meObject * const object = hitlist.GetEntryAt(e)->GetObject();
				if(!object){
					continue;
				}
				
				pAttachedDecals[pAttachedDecalCount] = new meAttachedDecal(
					pEnvironment->GetEngineController()->GetEngine(), this);
				meAttachedDecal &attachedDecal = *pAttachedDecals[pAttachedDecalCount++];
				attachedDecal.SetParentObject(object);
				attachedDecal.AttachToParent();
			}
		}
	}
	
	pUpdateSkin();
	pUpdateTransform();
}



void meDecal::pRepositionShapes(){
	decQuaternion orientation;
	decDVector position;
	
	if(pParentObject){
		const decDMatrix matrix = decDMatrix::CreateRT(decDVector(pRotation * DEG2RAD), pPosition)
			* decDMatrix::CreateSRT( decDVector( pParentObject->GetScaling() ),
				decDVector(pParentObject->GetRotation()), pParentObject->GetPosition());
		
		position = matrix.GetPosition();
		orientation = matrix.ToQuaternion();
		
	}else{
		position = pPosition;
		orientation = decMatrix::CreateRotation(pRotation * DEG2RAD).ToQuaternion();
	}
	
	pDebugDrawer->SetPosition(position);
	pDebugDrawer->SetOrientation(orientation);
	
	pCollider->SetPosition(position);
	pCollider->SetOrientation(orientation);
}

void meDecal::pUpdateDDSColors(){
	if(pActive){
		pDDSDecal->SetEdgeColor(decColor(1.0f, 0.5f, 0.0f, 1.0));
		
	}else if(pSelected){
		pDDSDecal->SetEdgeColor(decColor(1.0f, 0.0f, 0.0f, 1.0));
		
	}else{
		pDDSDecal->SetEdgeColor(decColor(0.5f, 0.5f, 0.5f, 1.0f));
	}
}

void meDecal::pUpdateShapes(){
	decShapeBox *box = NULL;
	
	// size has to be kept above a minimum to avoid problems and to keep the decal selectable by the user
	const decVector size(decVector(0.01f, 0.01f, 0.01f).Largest(decVector(pSize * 0.5f)));
	decVector position = decVector(0.0f, 0.0f, -size.z);
	
	// update debug drawer shape
	pDDSDecal->RemoveAllShapes();
	pDDSDecal->AddBoxShape(size, position, decQuaternion());
	
	// update collider shape
	decShapeList shapeList;
	
	if(pCollider){
		try{
			box = new decShapeBox(size, position);
			shapeList.Add(box);
			box = NULL;
			
		}catch(const deException &){
			if(box) delete box;
			throw;
		}
	}
	
	pCollider->SetShapes(shapeList);
}

void meDecal::pUpdateTransform(){
	decVector2 transformScale(pTexCoordScaling);
	decVector2 transformOffset(pTexCoordOffset);
	float transformRotation = pTexCoordRotation;
	int a;
	
	if(transformScale.x == 0.0f){
		transformScale.x = 1.0f;
	}
	if(transformScale.y == 0.0f){
		transformScale.y = 1.0f;
	}
	
	const decTexMatrix2 transformMatrix(
		decTexMatrix2::CreateScale(transformScale.x, transformScale.y) *
		decTexMatrix2::CreateRotation(transformRotation * DEG2RAD) *
		decTexMatrix2::CreateTranslation(transformOffset.x, transformOffset.y));
	
	for(a=0; a<pAttachedDecalCount; a++){
		pAttachedDecals[a]->GetEngineDecal()->SetTransform(transformMatrix);
	}
}

/*
void meDecal::pUpdateShapes(){
	if(pDDVolume){
		decVector view, up, right, size, pos;
		decShapeBox *shapeBox = NULL;
		decMatrix matrix;
		
		matrix.SetSRT(decVector(pSize.x * 0.5f, pSize.y * 0.5f, 0.01f),
			pRotation * DEG2RAD, pPosition);
		
		if(pParentObject){
			matrix *= decMatrix::CreateSRT(pParentObject->GetScaling(),
				pParentObject->GetRotation(), pParentObject->GetPosition().ToVector());
		}
		
		view = matrix.TransformView();
		up = matrix.TransformUp();
		right = matrix.TransformRight();
		pos = matrix.GetPosition();
		
		size.x = right.Length();
		size.y = up.Length();
		size.z = view.Length();
		
		matrix.SetVU(view, up);
		
		pDDVolume->RemoveAllShapes();
		
		try{
			shapeBox = new decShapeBox(size, pos, matrix.ToQuaternion());
			
			pDDVolume->AddShape(shapeBox);
			
		}catch(const deException &){
			if(shapeBox) delete shapeBox;
		}
	}
}
*/



void meDecal::pLoadSkin(){
	deEngine &engine = *pEnvironment->GetEngineController()->GetEngine();
	deSkinManager *skinMgr = engine.GetSkinManager();
	deSkin *skin = NULL;
	
	if(!pSkinPath.IsEmpty()){
		try{
			skin = skinMgr->LoadSkin(pSkinPath, "/");
			
		}catch(const deException &){
			skin = pEnvironment->GetStockSkin(igdeEnvironment::essError);
			skin->AddReference();
		}
	}
	
	if(pEngSkin){
		pEngSkin->FreeReference();
		pEngSkin = NULL;
	}
	
	pEngSkin = skin;
}

void meDecal::pUpdateSkin(){
	int i;
	
	for(i=0; i<pAttachedDecalCount; i++){
		deDecal &engDecal = *pAttachedDecals[i]->GetEngineDecal();
		
		engDecal.SetSkin(pEngSkin);
		
		if(pEngSkin){
			const int textureIndex = pEngSkin->IndexOfTextureNamed(pTextureName);
			
			if(textureIndex == -1){
				engDecal.SetTexture(0);
				
			}else{
				engDecal.SetTexture(textureIndex);
			}
			
		}else{
			engDecal.SetTexture(0);
		}
		
		engDecal.SetDynamicSkin(pDynamicSkin);
	}
}
