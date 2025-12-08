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

#include "meLoadXMLWorldTask.h"
#include "meLoadSaveSystem.h"
#include "../gui/meWindowMain.h"
#include "../world/meWorld.h"
#include "../world/decal/meDecal.h"
#include "../world/navspace/meNavigationSpace.h"
#include "../world/object/meObject.h"
#include "../world/object/texture/meObjectTexture.h"
#include "../world/terrain/meHeightTerrain.h"
#include "../world/terrain/meHeightTerrainSector.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/wrapper/igdeWSky.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyController.h>



// Definitions
////////////////

#define LOGSOURCE "World Editor"



// Class meLoadXMLWorldTask
//////////////////////////////

// Constructor, destructor
////////////////////////////
#include <dragengine/logger/deLogger.h>

meLoadXMLWorldTask::meLoadXMLWorldTask(meLoadSaveSystem *lssys, meWorld *world, decBaseFileReader *file) :
igdeBaseXML(lssys->GetWindowMain()->GetEnvironment().GetLogger(), LOGSOURCE),
pLSSys(lssys),
pWorld(world),
pReader(NULL),
pXMLDocument(NULL),
pXMLRoot(NULL),
pNextTag(0)
{
	if(!lssys || !world || !file){
		DETHROW(deeInvalidParam);
	}
	
	decString message;
	message.Format("Loading world from '%s'", world->GetFilePath().GetString());
	SetMessage(message);
	SetProgress(0.0f);
	
	pReader = file;
	file->AddReference();
}

meLoadXMLWorldTask::~meLoadXMLWorldTask(){
	pCleanUp();
}



// Management
///////////////

bool meLoadXMLWorldTask::Step(){
	if(!pXMLDocument){
		pXMLDocument.TakeOver(new decXmlDocument);
		
		decXmlParser parser(pLSSys->GetWindowMain()->GetEnvironment().GetLogger());
		parser.ParseXml(pReader, pXMLDocument);
		
		pXMLDocument->StripComments();
		pXMLDocument->CleanCharData();
		
		pXMLRoot = pXMLDocument->GetRoot();
		if(!pXMLRoot){
			DETHROW(deeInvalidParam);
		}
		if(strcmp(pXMLRoot->GetName(), "world") != 0){
			DETHROW(deeInvalidParam);
		}
		
		return true;
	}
	
	float elapsedTime = 0.0f;
	decTimer timer;
	
	while(elapsedTime < 0.5f){
		if(pNextTag == pXMLRoot->GetElementCount()){
			pWorld->SetDepChanged(false);
			pWorld->SetChanged(false);
			pWorld->SetSaved(true);
			
			// attach objects using the temporary attach id
			const meObjectList &objects = pWorld->GetObjects();
			const int objectCount = objects.GetCount();
			int i;
			
			for(i=0; i<objectCount; i++){
				meObject * const object = objects.GetAt(i);
				if(object->GetAttachedToID().IsEmpty()){
					continue;
				}
				
				meObject *attachTo = pWorld->GetObjectWithID(object->GetAttachedToID());
				if(attachTo){
					object->SetAttachedTo(attachTo);
					
				}else{
					pLSSys->GetWindowMain()->GetLogger()->LogErrorFormat(LOGSOURCE,
						"Object %s at (%g,%g,%g) has attachTo ID %s but no such object exists",
						object->GetID().ToHexString().GetString(), object->GetPosition().x,
						object->GetPosition().y, object->GetPosition().z,
						object->GetAttachedToID().GetString());
				}
			}
			
			// height terrain
			meHeightTerrain * const heightTerrain = pWorld->GetHeightTerrain();
			if(!heightTerrain->GetPathHT().IsEmpty()){
				pLSSys->LoadHeightTerrain(*heightTerrain, decPath::AbsolutePathUnix(
					heightTerrain->GetPathHT(), pWorld->GetDirectoryPath()).GetPathUnix());
			}
			return false;
		}
		
		const decXmlElementTag * const tag = pXMLRoot->GetElementIfTag(pNextTag++);
		if(!tag){
			elapsedTime += timer.GetElapsedTime();
			continue;
		}
		
		if(tag->GetName() == "worldEditor"){
			pLoadWorldEditor(*tag);
			
		}else if(tag->GetName() == "size"){
			pWorld->SetSize(decDVector(GetAttributeDouble(*tag, "x"),
				GetAttributeDouble(*tag, "y"), GetAttributeDouble(*tag, "z")));
			
		}else if(tag->GetName() == "gravity"){
			pWorld->SetGravity(decVector(GetAttributeFloat(*tag, "x"),
				GetAttributeFloat(*tag, "y"), GetAttributeFloat(*tag, "z")));
			
		}else if(tag->GetName() == "heightTerrain"){
			pWorld->GetHeightTerrain()->SetPathHT(GetCDataString(*tag));
			
		}else if(strcmp(tag->GetName(), "nextObjectID") == 0){
			pWorld->SetNextObjectID(decUniqueID(GetCDataString(*tag)));
			
		}else if(strcmp(tag->GetName(), "object") == 0){
			meObject *object = NULL;
			try{
				object = new meObject(pWorld->GetEnvironment());
				pLoadObject(*tag, *object);
				if(object->GetID() == decUniqueID()){
					object->SetID(pWorld->NextObjectID());
				}
				pWorld->AddObject(object);
				object->FreeReference();
				
			}catch(const deException &){
				throw;
			}
			
		}else if(strcmp(tag->GetName(), "decal") == 0){
			meDecal *decal = NULL;
			try{
				decal = new meDecal(pWorld->GetEnvironment());
				pLoadDecal(*tag, *decal);
				pWorld->AddDecal(decal);
				decal->FreeReference();
				
			}catch(const deException &){
				throw;
			}
			
		}else if(strcmp(tag->GetName(), "navigationSpace") == 0){
			meNavigationSpace *navspace = NULL;
			try{
				navspace = new meNavigationSpace(pWorld->GetEnvironment());
				pLoadNavigationSpace(*tag, *navspace);
				pWorld->AddNavSpace(navspace);
				navspace->FreeReference();
				
			}catch(const deException &){
				throw;
			}
			
		}else if(strcmp(tag->GetName(), "property") == 0){
			pWorld->SetProperty(GetAttributeString(*tag, "key"), ReadMultilineString(*tag));
			
		}else{
			pLSSys->GetWindowMain()->GetLogger()->LogWarnFormat(LOGSOURCE,
				"world.sector(%i:%i): Unknown Tag %s, ignoring",
				tag->GetLineNumber(), tag->GetPositionNumber(),
				tag->GetName().GetString());
		}
		
		elapsedTime += timer.GetElapsedTime();
	}
	
	SetProgress((float)pNextTag / (float)pXMLRoot->GetElementCount());
	
	return true;
}



// Private functions
//////////////////////

void meLoadXMLWorldTask::pCleanUp(){
}



void meLoadXMLWorldTask::pLoadWorldEditor(const decXmlElementTag &root){
	int i;
	for(i=0; i<root.GetElementCount(); i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "skyPath"){
			pWorld->GetSky()->SetPath(GetCDataString(*tag));
			
		}else if(tagName == "skyController"){
			const decString &name = GetAttributeString(*tag, "name");
			if(!pWorld->GetSky()->GetSky()){
				continue;
			}
			
			const int index = pWorld->GetSky()->GetSky()->IndexOfControllerNamed(name);
			if(index == -1){
				continue;
			}
			
			pWorld->GetSky()->SetControllerValue(index, GetCDataFloat(*tag));
			
		}else if(tagName == "backgroundObject"){
			pLoadWorldEditorBackgroundObject(*tag);
			
		}else if(tagName == "limitBox"){
			pLoadWorldEditorLimitBox(*tag);
			
		}else{
			pLSSys->GetWindowMain()->GetLogger()->LogWarnFormat(LOGSOURCE,
				"world.worldEditor(%i:%i): Unknown Tag %s, ignoring",
				tag->GetLineNumber(), tag->GetPositionNumber(), tagName.GetString());
		}
	}
}

void meLoadXMLWorldTask::pLoadWorldEditorBackgroundObject(const decXmlElementTag &root){
	igdeWObject &bgObject = *pWorld->GetBgObject();
	const int count = root.GetElementCount();
	int i;
	
	for(i=0; i<count; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if(tagName == "class"){
			bgObject.SetGDClassName(GetCDataString(*tag));
			
		}else if(tagName == "position"){
			bgObject.SetPosition(decDVector(GetAttributeDouble(*tag, "x"),
				GetAttributeDouble(*tag, "y"), GetAttributeDouble(*tag, "z")));
			
		}else if(tagName == "rotation"){
			bgObject.SetOrientation(decQuaternion::CreateFromEuler(
				GetAttributeFloat(*tag, "x") * DEG2RAD, GetAttributeFloat(*tag, "y") * DEG2RAD,
				GetAttributeFloat(*tag, "z") * DEG2RAD));
			
		}else if(tagName == "scaling"){
			bgObject.SetScaling(decVector(GetAttributeFloat(*tag, "x"),
				GetAttributeFloat(*tag, "y"), GetAttributeFloat(*tag, "z")));
			
		}else{
			pLSSys->GetWindowMain()->GetLogger()->LogWarnFormat(LOGSOURCE,
				"world.worldEditor.backgroundObject(%d:%d): Unknown Tag %s, ignoring",
				tag->GetLineNumber(), tag->GetPositionNumber(), tag->GetName().GetString());
		}
	}
}

void meLoadXMLWorldTask::pLoadWorldEditorLimitBox(const decXmlElementTag &root){
	const int count = root.GetElementCount();
	int i;
	
	for(i=0; i<count; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if(tagName == "minExtend"){
			pWorld->SetLimitBoxMinExtend(decVector(GetAttributeFloat(*tag, "x"),
				GetAttributeFloat(*tag, "y"), GetAttributeFloat(*tag, "z")));
			
		}else if(tagName == "maxExtend"){
			pWorld->SetLimitBoxMaxExtend(decVector(GetAttributeFloat(*tag, "x"),
				GetAttributeFloat(*tag, "y"), GetAttributeFloat(*tag, "z")));
			
		}else{
			pLSSys->GetWindowMain()->GetLogger()->LogWarnFormat(LOGSOURCE,
				"world.worldEditor.limitBox(%d:%d): Unknown Tag %s, ignoring",
				tag->GetLineNumber(), tag->GetPositionNumber(), tag->GetName().GetString());
		}
	}
}

void meLoadXMLWorldTask::pLoadObject(const decXmlElementTag &root, meObject &object){
	decStringList attachBehaviors;
	const char *name;
	int i;
	
	if(root.FindAttribute("id")){
		const decString id(root.FindAttribute("id")->GetValue());
		object.SetID(decUniqueID(id));
	}
	
	for(i=0; i<root.GetElementCount(); i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(strcmp(tag->GetName(), "classname") == 0){
			object.SetClassName(GetCDataString(*tag));
			
		}else if(strcmp(tag->GetName(), "position") == 0){
			object.SetPosition(decDVector(GetAttributeDouble(*tag, "x"),
				GetAttributeDouble(*tag, "y"), GetAttributeDouble(*tag, "z")));
			
		}else if(strcmp(tag->GetName(), "scaling") == 0){
			object.SetScaling(decVector(GetAttributeFloat(*tag, "x"),
				GetAttributeFloat(*tag, "y"), GetAttributeFloat(*tag, "z")));
			
		}else if(strcmp(tag->GetName(), "rotation") == 0){
			object.SetRotation(decVector(GetAttributeFloat(*tag, "x"),
				GetAttributeFloat(*tag, "y"), GetAttributeFloat(*tag, "z")));
			
		}else if(strcmp(tag->GetName(), "property") == 0){
			object.SetProperty(GetAttributeString(*tag, "key"), ReadMultilineString(*tag));
			
		}else if(strcmp(tag->GetName(), "texture") == 0){
			name = GetAttributeString(*tag, "name");
			
			meObjectTexture *texture = object.GetTextureNamed(name);else{
				try{
					texture = new meObjectTexture(object.GetEnvironment(), name);
					object.AddTexture(texture);
					
				}catch(const deException &){
					throw;
				}
			}
			
			pLoadObjectTexture(*tag, *texture);
			texture->FreeReference();
			
		}else if(strcmp(tag->GetName(), "attachTo") == 0){
			object.SetAttachedToID(GetCDataString(*tag));
			
		}else if(tag->GetName() == "attachBehavior"){
			attachBehaviors.Add(GetCDataString(*tag));
			
		}else{
			pLSSys->GetWindowMain()->GetLogger()->LogWarnFormat(LOGSOURCE,
				"world.sector.object(%i:%i): Unknown Tag %s, ignoring",
				tag->GetLineNumber(), tag->GetPositionNumber(), tag->GetName().GetString());
		}
	}
	
	object.SetAttachBehaviors(attachBehaviors);
}

void meLoadXMLWorldTask::pLoadObjectTexture(const decXmlElementTag &root, meObjectTexture &texture){
	int i;
	
	for(i=0; i<root.GetElementCount(); i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(strcmp(tag->GetName(), "skin") == 0){
			decXmlCharacterData * const cdata = tag->GetFirstData();
			if(cdata){
				texture.SetSkinPath(cdata->GetData());
				
			}else{
				texture.SetSkinPath("");
			}
			
		}else if(strcmp(tag->GetName(), "transform") == 0){
			pLoadObjectTextureTransform(*tag, texture);
			
		}else if(strcmp(tag->GetName(), "tint") == 0){
			texture.SetColorTint(decColor(GetAttributeFloat(*tag, "r"),
				GetAttributeFloat(*tag, "g"), GetAttributeFloat(*tag, "b")));
			
		}else if(strcmp(tag->GetName(), "property") == 0){
			texture.SetProperty(GetAttributeString(*tag, "key"), ReadMultilineString(*tag));
			
		}else{
			pLSSys->GetWindowMain()->GetLogger()->LogWarnFormat(LOGSOURCE,
				"texture(%i:%i): Unknown Tag %s, ignoring",
				tag->GetLineNumber(), tag->GetPositionNumber(), tag->GetName().GetString());
		}
	}
}

void meLoadXMLWorldTask::pLoadObjectTextureTransform(const decXmlElementTag &root, meObjectTexture &texture){
	const int tagCount = root.GetElementCount();
	int i;
	
	for(i=0; i<tagCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(strcmp(tag->GetName(), "translation") == 0){
			texture.SetTexCoordOffset(decVector2(GetAttributeFloat(*tag, "u"),
				GetAttributeFloat(*tag, "v")));
			
		}else if(strcmp(tag->GetName(), "scaling") == 0){
			texture.SetTexCoordScaling(decVector2(GetAttributeFloat(*tag, "u"),
				GetAttributeFloat(*tag, "v")));
			
		}else if(strcmp(tag->GetName(), "rotation") == 0){
			texture.SetTexCoordRotation(strtof(GetCDataString(*tag), NULL));
			
		}else{
			pLSSys->GetWindowMain()->GetLogger()->LogWarnFormat(LOGSOURCE,
				"%s(%i:%i): Unknown Tag %s, ignoring", root.GetName().GetString(),
				tag->GetLineNumber(), tag->GetPositionNumber(), tag->GetName().GetString());
		}
	}
}

void meLoadXMLWorldTask::pLoadDecal(const decXmlElementTag &root, meDecal &decal){
	int i;
	
	for(i=0; i<root.GetElementCount(); i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(strcmp(tag->GetName(), "skin") == 0){
			decXmlCharacterData * const cdata = tag->GetFirstData();
			if(cdata){
				decal.SetSkinPath(cdata->GetData());
			}
			
		}else if(strcmp(tag->GetName(), "texture") == 0){
			decXmlCharacterData * const cdata = tag->GetFirstData();
			if(cdata){
				decal.SetTextureName(cdata->GetData());
			}
			
		}else if(strcmp(tag->GetName(), "position") == 0){
			decal.SetPosition(decDVector(GetAttributeDouble(*tag, "x"),
				GetAttributeDouble(*tag, "y"), GetAttributeDouble(*tag, "z")));
			
		}else if(strcmp(tag->GetName(), "size") == 0){
			decal.SetSize(decVector(GetAttributeFloat(*tag, "x"),
				GetAttributeFloat(*tag, "y"), GetAttributeFloat(*tag, "z")));
			
		}else if(strcmp(tag->GetName(), "rotation") == 0){
			decal.SetRotation(decVector(GetAttributeFloat(*tag, "x"),
				GetAttributeFloat(*tag, "y"), GetAttributeFloat(*tag, "z")));
			
		}else if(strcmp(tag->GetName(), "visible") == 0){
			decal.SetVisible(strcmp(GetCDataString(*tag), "true") == 0);
			
		}else if(strcmp(tag->GetName(), "transform") == 0){
			pLoadDecalTransform(*tag, decal);
			
		}else if(strcmp(tag->GetName(), "tint") == 0){
			decal.SetColorTint(decColor(GetAttributeFloat(*tag, "r"),
				GetAttributeFloat(*tag, "g"), GetAttributeFloat(*tag, "b")));
			
		}else if(strcmp(tag->GetName(), "property") == 0){
			decal.SetProperty(GetAttributeString(*tag, "key"), ReadMultilineString(*tag));
			
		}else{
			pLSSys->GetWindowMain()->GetLogger()->LogWarnFormat(LOGSOURCE,
				"decal(%i:%i): Unknown Tag %s, ignoring",
				tag->GetLineNumber(), tag->GetPositionNumber(), tag->GetName().GetString());
		}
	}
}

void meLoadXMLWorldTask::pLoadDecalTransform(const decXmlElementTag &root, meDecal &decal){
	const int tagCount = root.GetElementCount();
	int i;
	
	for(i=0; i<tagCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(strcmp(tag->GetName(), "translation") == 0){
			decal.SetTexCoordOffset(decVector2(GetAttributeFloat(*tag, "u"),
				GetAttributeFloat(*tag, "v")));
			
		}else if(strcmp(tag->GetName(), "scaling") == 0){
			decal.SetTexCoordScaling(decVector2(GetAttributeFloat(*tag, "u"),
				GetAttributeFloat(*tag, "v")));
			
		}else if(strcmp(tag->GetName(), "rotation") == 0){
			decal.SetTexCoordRotation(strtof(GetCDataString(*tag), NULL));
			
		}else{
			pLSSys->GetWindowMain()->GetLogger()->LogWarnFormat(LOGSOURCE,
				"%s(%i:%i): Unknown Tag %s, ignoring", root.GetName().GetString(),
				tag->GetLineNumber(), tag->GetPositionNumber(), tag->GetName().GetString());
		}
	}
}

void meLoadXMLWorldTask::pLoadNavigationSpace(const decXmlElementTag &root, meNavigationSpace &navspace){
	int i;
	
	for(i=0; i<root.GetElementCount(); i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(strcmp(tag->GetName(), "position") == 0){
			navspace.SetPosition(decDVector(GetAttributeDouble(*tag, "x"),
				GetAttributeDouble(*tag, "y"), GetAttributeDouble(*tag, "z")));
			
		}else if(strcmp(tag->GetName(), "orientation") == 0){
			navspace.SetOrientation(decVector(GetAttributeFloat(*tag, "x"),
				GetAttributeFloat(*tag, "y"), GetAttributeFloat(*tag, "z")));
			
		}else if(strcmp(tag->GetName(), "filename") == 0){
			decXmlCharacterData * const cdata = tag->GetFirstData();
			if(cdata){
				navspace.SetFilename(cdata->GetData());
			}
			
		}else{
			pLSSys->GetWindowMain()->GetLogger()->LogWarnFormat(LOGSOURCE,
				"%s(%i:%i): Unknown Tag %s, ignoring", root.GetName().GetString(),
				tag->GetLineNumber(), tag->GetPositionNumber(), tag->GetName().GetString());
		}
	}
}
