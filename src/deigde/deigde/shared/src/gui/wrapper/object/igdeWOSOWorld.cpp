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

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlDocumentReference.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkinManager.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableColor.h>


#define LOGSOURCE "IGDE"


// LoadedObjectTexture
////////////////////////

igdeWOSOWorld::LoadedObjectTexture::LoadedObjectTexture(const decString &aname) :
name(aname),
texture(0){
}


// LoadedObject
/////////////////

igdeWOSOWorld::LoadedObject::LoadedObject(igdeEnvironment &environment) :
pWrapper(new igdeWObject(environment)){
}

igdeWOSOWorld::LoadedObject::~LoadedObject(){
	delete pWrapper;
}


// LoadXmlWorld
/////////////////

igdeWOSOWorld::LoadXmlWorld::LoadXmlWorld(igdeWOSOWorld &owner) :
igdeBaseXML(owner.GetEnvironment().GetLogger(), LOGSOURCE),
pOwner(owner),
pEnvironment(owner.GetEnvironment()){
}

void igdeWOSOWorld::LoadXmlWorld::LoadWorld(const decString &path){
	const decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::New(new decXmlDocument));
	
	decXmlParser(GetLogger()).ParseXml(decBaseFileReader::Ref::New(
		pEnvironment.GetFileSystemGame()->OpenFileForReading(decPath::CreatePathUnix(path))),
		xmlDoc);
	
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
			const LoadedObject::Ref object(LoadedObject::Ref::New(new LoadedObject(pEnvironment)));
			pReadObject(*tag, object);
			pOwner.AddObject(object);
		}
	}
}

void igdeWOSOWorld::LoadXmlWorld::pReadObject(const decXmlElementTag &root, LoadedObject &object){
	const int elementCount = root.GetElementCount();
	igdeWObject &wo = object.Wrapper();
	int i;
	
	for(i=0; i<elementCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "classname"){
			wo.SetGDClassName(GetCDataString(*tag));
			
		}else if(tagName == "position" ){
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
			LoadedObjectTexture::Ref texture(LoadedObjectTexture::Ref::New(
				new LoadedObjectTexture(GetAttributeString(*tag, "name"))));
			pReadObjectTexture(*tag, object, texture);
			object.textures.Add(texture);
		}
	}
	
	object.originalMatrix.SetWorld(wo.GetPosition(), wo.GetOrientation(), wo.GetScaling());
}

void igdeWOSOWorld::LoadXmlWorld::pReadObjectTexture(const decXmlElementTag &root,
LoadedObject &object, LoadedObjectTexture &texture){
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
			texture.pathSkin = cdata ? cdata->GetData() : "";
			
		}else if(tagName == "transform"){
			pReadObjectTextureTransform(*tag, object, texture);
			
		}else if(tagName == "tint"){
			decColor color;
			ReadColor(*tag, color);
			
			texture.dynamicSkin.TakeOver(pEnvironment.GetEngineController()->
				GetEngine()->GetDynamicSkinManager()->CreateDynamicSkin());
			
			deDSRenderableColor * const renderable = new deDSRenderableColor("tint");
			renderable->SetColor(color);
			texture.dynamicSkin->AddRenderable(renderable);
		}
	}
}

void igdeWOSOWorld::LoadXmlWorld::pReadObjectTextureTransform(const decXmlElementTag &root,
LoadedObject &object, LoadedObjectTexture &texture){
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
}


// Class igdeWOSOWorld
////////////////////////

// Constructor, destructor
////////////////////////////

igdeWOSOWorld::igdeWOSOWorld(igdeWObject &wrapper, const igdeGDCWorld &gdcWorld,
	const decString &prefix) :
igdeWOSubObject(wrapper, prefix),
pGDWorld(gdcWorld)
{
	wrapper.SubObjectFinishedLoading(*this, true);
}

igdeWOSOWorld::~igdeWOSOWorld(){
	pDestroyWorld();
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
	const int count = pObjects.GetCount();
	int i;
	for(i=0; i<count; i++){
		((LoadedObject*)pObjects.GetAt(i))->Wrapper().SetVisible(visible);
	}
}

void igdeWOSOWorld::UpdateGeometry(){
	const decDMatrix transform(decDMatrix::CreateWorld(pPosition, pOrientation)
		* decDMatrix::CreateWorld(GetWrapper().GetPosition(),
			GetWrapper().GetOrientation(), GetWrapper().GetScaling()));
	const int count = pObjects.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		LoadedObject &object = *((LoadedObject*)pObjects.GetAt(i));
		igdeWObject &wo = object.Wrapper();
		const decDMatrix matrix(object.originalMatrix * transform);
		
		wo.SetScaling(matrix.GetScale());
		wo.SetPosition(matrix.GetPosition());
		if(!wo.GetScaling().IsZero()){
			wo.SetOrientation(matrix.Normalized().ToQuaternion());
		}
	}
}

void igdeWOSOWorld::Visit(igdeWOSOVisitor &visitor){
	visitor.VisitWorld(*this);
}

void igdeWOSOWorld::AddObject(const LoadedObject::Ref &object){
	DEASSERT_NOTNULL(object)
	
	object->Wrapper().SetWorld(GetWrapper().GetWorld());
	pObjects.Add(object);
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
	}
}

void igdeWOSOWorld::pDestroyWorld(){
	pObjects.RemoveAll();
}

void igdeWOSOWorld::pLoadWorld(const decString &path){
	const decString pathWorld(GetStringProperty(
		pGDWorld.GetPropertyName(igdeGDCWorld::epPath), pGDWorld.GetPath()));
	if(pathWorld.IsEmpty()){
		return;
	}
	
	LoadXmlWorld(*this).LoadWorld(path);
}
