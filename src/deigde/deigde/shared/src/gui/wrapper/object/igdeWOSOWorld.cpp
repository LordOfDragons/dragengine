/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include "igdeWOSOWorld.h"
#include "igdeWOSOVisitor.h"
#include "../igdeWObject.h"
#include "../../../engine/igdeEngineController.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../gamedefinition/igdeGameDefinition.h"
#include "../../../gamedefinition/class/igdeGDClass.h"
#include "../../../gamedefinition/class/world/igdeGDCWorld.h"
#include "../../../gameproject/igdeGameProject.h"
#include "../../../resourceloader/igdeResourceLoaderTask.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentTexture.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkinManager.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableColor.h>


#define LOGSOURCE "IGDE"


// LoadObjectResources
////////////////////////

igdeWOSOWorld::LoadObjectResources::Texture::Texture(ChildObject &aobject, ChildObjectTexture &atexture) :
object(&aobject),
texture(&atexture){
}

igdeWOSOWorld::LoadObjectResources::LoadObjectResources(igdeWOSOWorld &owner) :
pOwner(&owner){
}

void igdeWOSOWorld::LoadObjectResources::Drop(){
	pOwner = nullptr;
}

void igdeWOSOWorld::LoadObjectResources::LoadTexture(ChildObject &object, ChildObjectTexture &texture){
	const decString &path = texture.pathSkin;
	pTextures.Add(Texture::Ref::New(object, texture));
	if(!pSkins.Has(path)){
		pSkins.Add(path);
		pOwner->GetWrapper().GetEnvironment().AsyncLoadResource(path, deResourceLoader::ertSkin, this);
	}
}

void igdeWOSOWorld::LoadObjectResources::LoadingFinished(
const igdeResourceLoaderTask &task, deFileResource *resource){
	const decString &path = task.GetFilename();
	if(task.GetResourceType() == deResourceLoader::ertSkin && pSkins.Has(path)){
		pUpdateTextures(path, (deSkin*)resource);
		pSkins.Remove(path);
	}
}

void igdeWOSOWorld::LoadObjectResources::LoadingFailed(const igdeResourceLoaderTask &task){
	const decString &path = task.GetFilename();
	if(task.GetResourceType() == deResourceLoader::ertSkin && pSkins.Has(path)){
		pUpdateTextures(path, nullptr);
		pSkins.Remove(path);
	}
}

void igdeWOSOWorld::LoadObjectResources::pUpdateTextures(const char *path, deSkin *skin){
	pTextures.RemoveIf([&](const Texture &t) {
		if(t.texture->pathSkin != path){
			return false;
		}
		
		t.texture->skin = skin;
		if(skin && pOwner){
			pOwner->UpdateChildComponentTexture(t.object, t.texture);
		}
		return true;
	});
}


// ChildObjectTexture
///////////////////////

igdeWOSOWorld::ChildObjectTexture::ChildObjectTexture(const decString &aname) :
name(aname),
texture(0),
hasTCTransform(false){
}


// ChildObject
////////////////

igdeWOSOWorld::ChildObject::ChildObject(igdeEnvironment &environment) :
pWrapper(igdeWObject::Ref::New(environment)){
}

void igdeWOSOWorld::ChildObject::AddTexture(const ChildObjectTexture::Ref &texture){
	DEASSERT_NOTNULL(texture)
	pTextures.Add(texture);
}


// LoadXmlWorld
/////////////////

igdeWOSOWorld::LoadXmlWorld::LoadXmlWorld(igdeWOSOWorld &owner) :
igdeBaseXML(owner.GetEnvironment().GetLogger(), LOGSOURCE),
pOwner(owner),
pEnvironment(owner.GetEnvironment()){
}

void igdeWOSOWorld::LoadXmlWorld::LoadWorld(const decString &path){
	const decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::New());
	
	decXmlParser(GetLogger()).ParseXml(pEnvironment.GetFileSystemGame()->OpenFileForReading(
		decPath::CreatePathUnix(path)), xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	const decXmlElementTag * const xmlRoot = xmlDoc->GetRoot();
	DEASSERT_NOTNULL(xmlRoot)
	DEASSERT_TRUE(xmlRoot->GetName() == "world")
	
	pReadWorld(*xmlRoot);
}

void igdeWOSOWorld::LoadXmlWorld::pReadWorld(const decXmlElementTag &root){
	const int elementCount = root.GetElementCount();
	int i;
	for(i=0; i<elementCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "object"){
			const ChildObject::Ref object(ChildObject::Ref::New(pEnvironment));
			pReadObject(*tag, object);
			pOwner.AddChildObject(object);
		}
	}
}

void igdeWOSOWorld::LoadXmlWorld::pReadObject(const decXmlElementTag &root, ChildObject &object){
	const int elementCount = root.GetElementCount();
	igdeWObject &wo = object.GetWrapper();
	int i;
	
	for(i=0; i<elementCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "classname"){
			wo.SetGDClassName(GetCDataString(*tag));
			
		}else if(tagName == "position"){
			decDVector vector;
			ReadDVector(*tag, vector);
			wo.SetPosition(vector);
			
		}else if(tagName == "scaling"){
			decVector vector;
			ReadVector(*tag, vector);
			wo.SetScaling(vector);
			
		}else if(tagName == "rotation"){
			decVector vector;
			ReadVector(*tag, vector);
			wo.SetOrientation(decQuaternion::CreateFromEuler(vector * DEG2RAD));
			
		}else if(tagName == "property"){
			wo.SetProperty(GetAttributeString(*tag, "key"), ReadMultilineString(*tag));
			
		}else if(tagName == "texture"){
			ChildObjectTexture::Ref texture(ChildObjectTexture::Ref::New(
				GetAttributeString(*tag, "name")));
			pReadObjectTexture(*tag, object, texture);
			object.AddTexture(texture);
		}
	}
	
	object.originalMatrix.SetWorld(wo.GetPosition(), wo.GetOrientation(), wo.GetScaling());
}

void igdeWOSOWorld::LoadXmlWorld::pReadObjectTexture(const decXmlElementTag &root,
ChildObject &object, ChildObjectTexture &texture){
	const int elementCount = root.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "skin"){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if(cdata){
				texture.pathSkin = cdata->GetData();
				if(!texture.pathSkin.IsEmpty()){
					pOwner.LoadTextureSkin(object, texture);
				}
			}
			
		}else if(tagName == "transform"){
			pReadObjectTextureTransform(*tag, object, texture);
			
		}else if(tagName == "tint"){
			decColor color;
			ReadColor(*tag, color);
			
			texture.dynamicSkin = pOwner.GetEnvironment().GetEngineController()->
				GetEngine()->GetDynamicSkinManager()->CreateDynamicSkin();
			
			deDSRenderableColor * const renderable = new deDSRenderableColor("tint");
			renderable->SetColor(color);
			texture.dynamicSkin->AddRenderable(renderable);
		}
	}
}

void igdeWOSOWorld::LoadXmlWorld::pReadObjectTextureTransform(const decXmlElementTag &root,
ChildObject &object, ChildObjectTexture &texture){
	const int elementCount = root.GetElementCount();
	decVector2 translation, scaling(1.0f, 1.0f);
	float rotation = 0.0f;
	int i;
	
	for(i=0; i<elementCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "translation"){
			translation.x = GetAttributeFloat(*tag, "u");
			translation.y = GetAttributeFloat(*tag, "v");
			
		}else if(tagName == "scaling"){
			scaling.x = GetAttributeFloat(*tag, "u");
			if(scaling.x == 0.0f){
				scaling.x = 1.0f;
			}
			
			scaling.y = GetAttributeFloat(*tag, "v");
			if(scaling.y == 0.0f){
				scaling.y = 1.0f;
			}
			
		}else if(tagName == "rotation"){
			rotation = strtof(GetCDataString(*tag), nullptr) * DEG2RAD;
		}
	}
	
	texture.tcTransform = decTexMatrix2::CreateScale(scaling)
		* decTexMatrix2::CreateRotation(rotation)
		* decTexMatrix2::CreateTranslation(translation);
	texture.hasTCTransform = true;
}


// Class ChildAsyncFinished
/////////////////////////////

igdeWOSOWorld::ChildAsyncFinished::ChildAsyncFinished(igdeWOSOWorld &owner) :
pOwner(owner){
}

void igdeWOSOWorld::ChildAsyncFinished::LoadFinished(igdeWObject &wrapper, bool succeeded){
	ChildObject * const co = pOwner.GetChildObjects().FindOrDefault([&](ChildObject &o){
		return o.GetWrapper() == &wrapper;
	});
	if(co){
		pOwner.ChildObjectFinishedAsyncLoad(*co);
	}
}

void igdeWOSOWorld::ChildAsyncFinished::ExtendsChanged(igdeWObject &wrapper){
	ChildObject * const co = pOwner.GetChildObjects().FindOrDefault([&](ChildObject &o){
		return o.GetWrapper() == &wrapper;
	});
	if(co){
		pOwner.ChildObjectExtendsChanged(*co);
	}
}


// Class igdeWOSOWorld
////////////////////////

// Constructor, destructor
////////////////////////////

igdeWOSOWorld::igdeWOSOWorld(igdeWObject &wrapper, const igdeGDCWorld &gdcWorld,
	const decString &prefix) :
igdeWOSubObject(wrapper, prefix),
pGDWorld(gdcWorld),
pChildAsyncFinished(*this),
pLoadObjectResources(LoadObjectResources::Ref::New(*this)),
pNoUpdateAnyContentVisibile(false)
{
	wrapper.SubObjectFinishedLoading(*this, true);
}

igdeWOSOWorld::~igdeWOSOWorld(){
	pDestroyWorld();
	
	if(pLoadObjectResources){
		pLoadObjectResources->Drop();
	}
}


// Management
///////////////

void igdeWOSOWorld::UpdateParameters(){
	GetWrapper().SubObjectFinishedLoading(*this, true);
}

void igdeWOSOWorld::OnAllSubObjectsFinishedLoading(){
	pUpdateWorld();
}

void igdeWOSOWorld::UpdateVisibility(){
	const bool visible = GetWrapper().GetVisible();
	pChildObjects.Visit([&](ChildObject &o){
		o.GetWrapper()->SetVisible(visible);
	});
}

void igdeWOSOWorld::UpdateGeometry(){
	const igdeWObject &w = GetWrapper();
	const decDMatrix transform(decDMatrix::CreateWorld(pPosition, pOrientation)
		* decDMatrix::CreateWorld(w.GetPosition(), w.GetOrientation(), w.GetScaling()));
	pChildObjects.Visit([&](ChildObject &o){
		igdeWObject &wo = o.GetWrapper();
		const decDMatrix matrix(o.originalMatrix * transform);
		
		wo.SetScaling(matrix.GetScale());
		wo.SetPosition(matrix.GetPosition());
		if(!wo.GetScaling().IsZero()){
			wo.SetOrientation(matrix.Normalized().ToQuaternion());
		}
	});
}

void igdeWOSOWorld::UpdateCollisionFilter(){
	const igdeWObject &w = GetWrapper();
	const decCollisionFilter &cf = w.GetCollisionFilter();
	const decCollisionFilter &cfFallback = w.GetCollisionFilterFallback();
	const decCollisionFilter &cfForceFields = w.GetCollisionFilterForceFields();
	const decCollisionFilter &cfInteract = w.GetCollisionFilterInteract();
	const decCollisionFilter &cfParticles = w.GetCollisionFilterParticles();
	int lmAudio = w.GetAudioLayerMask();
	int lmRender = w.GetRenderLayerMask();
	pChildObjects.Visit([&](ChildObject &o){
		igdeWObject &wo = o.GetWrapper();
		wo.SetCollisionFilter(cf);
		wo.SetCollisionFilterFallback(cfFallback);
		wo.SetCollisionFilterForceFields(cfForceFields);
		wo.SetCollisionFilterInteract(cfInteract);
		wo.SetCollisionFilterParticles(cfParticles);
		wo.SetAudioLayerMask(lmAudio);
		wo.SetRenderLayerMask(lmRender);
	});
}

void igdeWOSOWorld::OutlineSkinChanged(){
	const igdeWObject &w = GetWrapper();
	deSkin * const skin = w.GetOutlineSkin();
	const decColor &color = w.GetOutlineColor();
	pChildObjects.Visit([&](ChildObject &o){
		igdeWObject &wo = o.GetWrapper();
		wo.SetOutlineSkin(skin);
		wo.SetOutlineColor(color);
	});
}

void igdeWOSOWorld::ColliderUserPointerChanged(){
	void * const userPointer = GetWrapper().GetColliderUserPointer();
	pChildObjects.Visit([&](ChildObject &o){
		o.GetWrapper()->SetColliderUserPointer(userPointer);
	});
}

void igdeWOSOWorld::Visit(igdeWOSOVisitor &visitor){
	visitor.VisitWorld(*this);
}


void igdeWOSOWorld::AddChildObject(const ChildObject::Ref &object){
	DEASSERT_NOTNULL(object)
	
	igdeWObject &wo = object->GetWrapper();
	wo.SetAsyncLoadFinished(&pChildAsyncFinished);
	wo.SetWorld(GetWrapper().GetWorld());
	pChildObjects.Add(object);
}

void igdeWOSOWorld::RemoveAllChildObjects(){
	pChildObjects.Visit([](ChildObject &o){
		igdeWObject &wo = o.GetWrapper();
		wo.SetWorld(nullptr);
		wo.SetAsyncLoadFinished(nullptr);
	});
	pChildObjects.RemoveAll();
}

void igdeWOSOWorld::ChildObjectFinishedAsyncLoad(ChildObject &object){
	pUpdateChildComponentTextures(object);
	pUpdateExtends();
	if(!pNoUpdateAnyContentVisibile){
		GetWrapper().UpdateAnyContentVisibile();
	}
}

void igdeWOSOWorld::ChildObjectExtendsChanged(ChildObject &object){
	pUpdateExtends();
}

void igdeWOSOWorld::LoadTextureSkin(ChildObject &object, ChildObjectTexture &texture){
	pLoadObjectResources->LoadTexture(object, texture);
}

void igdeWOSOWorld::UpdateChildComponentTexture(ChildObject &object, const ChildObjectTexture &texture){
	igdeWObject &wo = object.GetWrapper();
	deComponent * const component = wo.GetComponent();
	if(!component || !component->GetModel()){
		return;
	}
	
	const deModel * const engModel = component->GetModel();
	const int index = engModel->IndexOfTextureNamed(texture.name);
	if(index == -1){
		return;
	}
	
	deComponentTexture &ctex = component->GetTextureAt(index);
	if(texture.skin){
		ctex.SetSkin(texture.skin);
		ctex.SetTexture(texture.texture);
	}
	if(texture.hasTCTransform){
		ctex.SetTransform(texture.tcTransform);
	}
	if(texture.dynamicSkin){
		ctex.SetDynamicSkin(texture.dynamicSkin);
	}
	component->NotifyTextureChanged(index);
}

bool igdeWOSOWorld::IsContentVisible(){
	return pChildObjects.HasMatching([](ChildObject &o){
		return o.GetWrapper()->IsAnyContentVisible();
	});
}

bool igdeWOSOWorld::AllSubObjectsFinishedLoading() const{
	return !pChildObjects.HasMatching([](ChildObject &o){
		return !o.GetWrapper()->AllSubObjectsFinishedLoading();
	});
}


// Protected Functions
////////////////////////

void igdeWOSOWorld::AttachToCollider(){
}

void igdeWOSOWorld::DetachFromCollider(){
}


// Private Functions
//////////////////////

void igdeWOSOWorld::pUpdateWorld(){
	decString pathWorld(GetStringProperty(
		pGDWorld.GetPropertyName(igdeGDCWorld::epPath), pGDWorld.GetPath()));
	if(pathWorld != pPathWorld){
		pDestroyWorld();
	}
	
	const decDVector position(GetVectorProperty(
		pGDWorld.GetPropertyName(igdeGDCWorld::epPosition), pGDWorld.GetPosition()));
	const decQuaternion orientation(GetRotationProperty(
		pGDWorld.GetPropertyName(igdeGDCWorld::epRotation), pGDWorld.GetOrientation()));
	
	if(!position.IsEqualTo(pPosition) || !orientation.IsEqualTo(pOrientation)){
		// TODO move objects
		pPosition = position;
		pOrientation = orientation;
	}
	
	if(pathWorld != pPathWorld){
		try{
			pLoadWorld(pathWorld);
			
		}catch(const deException &e){
			GetEnvironment().GetLogger()->LogException("IGDE", e);
			pDestroyWorld();
		}
		pPathWorld = pathWorld;
		
		UpdateGeometry();
		UpdateCollisionFilter();
		OutlineSkinChanged();
		ColliderUserPointerChanged();
	}
}

void igdeWOSOWorld::pDestroyWorld(){
	RemoveAllChildObjects();
}

void igdeWOSOWorld::pLoadWorld(const decString &path){
	const decString pathWorld(GetStringProperty(
		pGDWorld.GetPropertyName(igdeGDCWorld::epPath), pGDWorld.GetPath()));
	if(pathWorld.IsEmpty()){
		return;
	}
	
	pNoUpdateAnyContentVisibile = true;
	try{
		LoadXmlWorld(*this).LoadWorld(path);
		pNoUpdateAnyContentVisibile = false;
		
	}catch(const deException &){
		pNoUpdateAnyContentVisibile = false;
		throw;
	}
}

void igdeWOSOWorld::pUpdateChildComponentTextures(ChildObject &object){
	igdeWObject &wo = object.GetWrapper();
	deComponent * const component = wo.GetComponent();
	if(!component || !component->GetModel()){
		return;
	}
	
	object.GetTextures().Visit([&](const ChildObjectTexture &t){
		UpdateChildComponentTexture(object, t);
	});
}


void igdeWOSOWorld::pUpdateExtends(){
	const decDMatrix invMatrix(GetWrapper().GetInverseMatrix().QuickMultiply(
		decDMatrix::CreateWorld(pPosition, pOrientation).QuickInvert()));
	decVector boxMinExtend, boxMaxExtend, cboxCorners[8];
	bool hasBoxExtends = false;
	
	pChildObjects.Visit([&](ChildObject &o){
		igdeWObject &wo = o.GetWrapper();
		if(!wo.GetHasBoxExtends()){
			return;
		}
		
		const decDVector a(wo.GetBoxMinExtend()), b(wo.GetBoxMaxExtend());
		const decDVector boxCorners[8]{
			{a.x, a.y, a.z}, {a.x, b.y, a.z}, {b.x, b.y, a.z}, {b.x, a.y, a.z},
			{a.x, a.y, b.z}, {a.x, b.y, b.z}, {b.x, b.y, b.z}, {b.x, a.y, b.z}};
		
		const decDMatrix matrix(wo.GetMatrix() * invMatrix);
		int i;
		for(i=0; i<8; i++){
			cboxCorners[i] = matrix * boxCorners[i];
		}
		
		decVector cboxMinExtend(cboxCorners[0]), cboxMaxExtend(cboxCorners[0]);
		for(i=1; i<8; i++){
			cboxMinExtend.SetSmallest(cboxCorners[i]);
			cboxMaxExtend.SetLargest(cboxCorners[i]);
		}
		
		if(hasBoxExtends){
			boxMinExtend.SetSmallest(cboxMinExtend);
			boxMaxExtend.SetLargest(cboxMaxExtend);
			
		}else{
			boxMinExtend = cboxMinExtend;
			boxMaxExtend = cboxMaxExtend;
			hasBoxExtends = true;
		}
	});
	
	if(hasBoxExtends){
		const decVector boxSize(boxMaxExtend - boxMinExtend);
		if(boxSize.x < 0.001f){
			boxMinExtend.x -= 0.0005f;
			boxMaxExtend.x += 0.0005f;
		}
		if(boxSize.y < 0.001f){
			boxMinExtend.y -= 0.0005f;
			boxMaxExtend.y += 0.0005f;
		}
		if(boxSize.z < 0.001f){
			boxMinExtend.z -= 0.0005f;
			boxMaxExtend.z += 0.0005f;
		}
		SetBoxExtends(boxMinExtend, boxMaxExtend);
		
	}else{
		ClearBoxExtends();
	}
}
