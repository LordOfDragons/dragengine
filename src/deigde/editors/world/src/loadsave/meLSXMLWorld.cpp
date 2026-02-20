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

#include "meLSXMLWorld.h"
#include "meLoadXMLWorldTask.h"
#include "meLoadSaveSystem.h"
#include "../gui/meWindowMain.h"
#include "../world/meWorld.h"
#include "../world/decal/meDecal.h"
#include "../world/object/meObject.h"
#include "../world/object/texture/meObjectTexture.h"
#include "../world/navspace/meNavigationSpace.h"
#include "../world/terrain/meHeightTerrain.h"
#include "../world/terrain/meHeightTerrainSector.h"
#include "../worldedit.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/wrapper/igdeWSky.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/resources/sky/deSkyController.h>



// Definitions
////////////////

#define LOGSOURCE "World Editor"



// class meLSXMLWorld
///////////////////////

// Constructor, destructor
////////////////////////////

meLSXMLWorld::meLSXMLWorld(meLoadSaveSystem *lssys) :
igdeBaseXML(lssys->GetWindowMain()->GetEnvironment().GetLogger(), LOGSOURCE)
{
	pLSSys = lssys;
	SetName("Drag[en]gine XML World");
	SetPattern("*.deworld");
}

meLSXMLWorld::~meLSXMLWorld(){
}



// Loading and saving
///////////////////////

void meLSXMLWorld::SaveWorld(meLoadSaveSystem &lssys, const meWorld &world, decBaseFileWriter *file){
	DEASSERT_NOTNULL(file)
	
	decXmlWriter writer(file);
	
	writer.WriteXMLDeclaration();
	pWriteWorld(writer, world);
}

igdeStepableTask::Ref meLSXMLWorld::CreateLoadTask(meWorld *world, decBaseFileReader *file){
	DEASSERT_NOTNULL(world)
	DEASSERT_NOTNULL(file)
	return meLoadXMLWorldTask::Ref::New(pLSSys, world, file);
}



// Private functions
//////////////////////

const char *meLSXMLWorld::pGetAttributeString(const decXmlElementTag &tag, const char *name) const{
	const decXmlAttValue * const value = tag.FindAttribute(name);
	if(value){
		return value->GetValue();
	}
	
	pLSSys->GetWindowMain()->GetLogger()->LogErrorFormat(LOGSOURCE,
		"Missing Attribute '%s' in tag '%s'", name, tag.GetName().GetString());
	DETHROW(deeInvalidParam);
}

int meLSXMLWorld::pGetAttributeInt(const decXmlElementTag &tag, const char *name) const{
	const decXmlAttValue * const value = tag.FindAttribute(name);
	if(value){
		return value->GetValue().ToIntValid();
	}
	
	pLSSys->GetWindowMain()->GetLogger()->LogErrorFormat(LOGSOURCE,
		"Missing Attribute '%s' in tag '%s'", name, tag.GetName().GetString());
	DETHROW(deeInvalidParam);
}

float meLSXMLWorld::pGetAttributeFloat(const decXmlElementTag &tag, const char *name) const{
	const decXmlAttValue *value = tag.FindAttribute(name);
	if(value){
		return value->GetValue().ToFloatValid();
	}
	
	pLSSys->GetWindowMain()->GetLogger()->LogErrorFormat(LOGSOURCE,
		"Missing Attribute '%s' in tag '%s'", name, tag.GetName().GetString());
	DETHROW(deeInvalidParam);
}



void meLSXMLWorld::pWriteWorld(decXmlWriter &writer, const meWorld &world){
	writer.WriteOpeningTag("world");
	
	WriteDVector(writer, "size", world.GetSize());
	WriteVector(writer, "gravity", world.GetGravity());
	
	pWriteWorldEditor(writer, world);
	
	pWriteProperties(writer, world.GetProperties());
	
	meHeightTerrain * const heightTerrain = world.GetHeightTerrain();
	if(!heightTerrain->GetPathHT().IsEmpty()){
		writer.WriteDataTagString("heightTerrain", heightTerrain->GetPathHT());
	}
	
	writer.WriteDataTagString("nextObjectID", world.GetNextObjectID().ToHexString());
	
	if(world.GetObjects().IsNotEmpty()){
		writer.WriteNewline();
		world.GetObjects().Visit([&](const meObject &object){
			pWriteObject(writer, object);
		});
	}
	
	if(world.GetDecals().IsNotEmpty()){
		writer.WriteNewline();
		world.GetDecals().Visit([&](const meDecal &decal){
			pWriteDecal(writer, decal);
		});
	}
	
	if(world.GetNavSpaces().IsNotEmpty()){
		writer.WriteNewline();
		world.GetNavSpaces().Visit([&](const meNavigationSpace &navspace){
			pWriteNavigationSystem(writer, navspace);
		});
	}
	
	writer.WriteClosingTag("world");
}

void meLSXMLWorld::pWriteWorldEditor(decXmlWriter &writer, const meWorld &world){
	writer.WriteOpeningTag("worldEditor");
	
	const igdeWSky &sky = *world.GetSky();
	writer.WriteDataTagString("skyPath", sky.GetPath());
	const int skyControllerCount = sky.GetControllerCount();
	int i;
	for(i=0; i<skyControllerCount; i++){
		const deSkyController &controller = sky.GetControllerAt(i);
		writer.WriteOpeningTagStart("skyController");
		writer.WriteAttributeString("name", controller.GetName());
		writer.WriteOpeningTagEnd(false, false);
		writer.WriteTextFloat(controller.GetCurrentValue());
		writer.WriteClosingTag("skyController", false);
	}
	
	if(world.GetBgObject()->GetGDClass()){
		pWriteWorldEditorBackgroundObject(writer, world);
	}
	
	if(!(world.GetLimitBoxMaxExtend() - world.GetLimitBoxMinExtend()).IsZero()){
		pWriteWorldEditorLimitBox(writer, world);
	}
	
	writer.WriteClosingTag("worldEditor");
}

void meLSXMLWorld::pWriteWorldEditorBackgroundObject(decXmlWriter &writer, const meWorld &world){
	const igdeWObject &bgObject = *world.GetBgObject();
	writer.WriteOpeningTag("backgroundObject");
	
	writer.WriteDataTagString("class", bgObject.GetGDClass()->GetName());
	
	const decVector &position = bgObject.GetPosition();
	if(!position.IsZero()){
		writer.WriteOpeningTagStart("position");
		writer.WriteAttributeDouble("x", position.x);
		writer.WriteAttributeDouble("y", position.y);
		writer.WriteAttributeDouble("z", position.z);
		writer.WriteOpeningTagEnd(true);
	}
	
	const decVector rotation(bgObject.GetOrientation().GetEulerAngles() * RAD2DEG);
	if(!rotation.IsZero()){
		writer.WriteOpeningTagStart("rotation");
		writer.WriteAttributeDouble("x", rotation.x);
		writer.WriteAttributeDouble("y", rotation.y);
		writer.WriteAttributeDouble("z", rotation.z);
		writer.WriteOpeningTagEnd(true);
	}
	
	const decVector &scaling = bgObject.GetScaling();
	if(!scaling.IsEqualTo(decVector(1.0f, 1.0f, 1.0f))){
		writer.WriteOpeningTagStart("scaling");
		writer.WriteAttributeDouble("x", scaling.x);
		writer.WriteAttributeDouble("y", scaling.y);
		writer.WriteAttributeDouble("z", scaling.z);
		writer.WriteOpeningTagEnd(true);
	}
	
	writer.WriteClosingTag("backgroundObject");
}

void meLSXMLWorld::pWriteWorldEditorLimitBox(decXmlWriter &writer, const meWorld &world){
	writer.WriteOpeningTag("limitBox");
	
	const decVector &minExtend = world.GetLimitBoxMinExtend();
	writer.WriteOpeningTagStart("minExtend");
	writer.WriteAttributeDouble("x", minExtend.x);
	writer.WriteAttributeDouble("y", minExtend.y);
	writer.WriteAttributeDouble("z", minExtend.z);
	writer.WriteOpeningTagEnd(true);
	
	const decVector &maxExtend = world.GetLimitBoxMaxExtend();
	writer.WriteOpeningTagStart("maxExtend");
	writer.WriteAttributeDouble("x", maxExtend.x);
	writer.WriteAttributeDouble("y", maxExtend.y);
	writer.WriteAttributeDouble("z", maxExtend.z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteClosingTag("limitBox");
}

void meLSXMLWorld::pWriteObject(decXmlWriter &writer, const meObject &object){
	const decDVector &position = object.GetPosition();
	const decVector &rotation = object.GetRotation();
	const decVector &scaling = object.GetScaling();
	int i;
	
	writer.WriteOpeningTagStart("object");
	writer.WriteAttributeString("id", object.GetID().ToHexString());
	writer.WriteOpeningTagEnd();
	
	writer.WriteDataTagString("classname", object.GetClassName().GetString());
	
	writer.WriteOpeningTagStart("position");
	writer.WriteAttributeDouble("x", position.x);
	writer.WriteAttributeDouble("y", position.y);
	writer.WriteAttributeDouble("z", position.z);
	writer.WriteOpeningTagEnd(true);
	
	if(!rotation.IsZero()){
		writer.WriteOpeningTagStart("rotation");
		writer.WriteAttributeDouble("x", rotation.x);
		writer.WriteAttributeDouble("y", rotation.y);
		writer.WriteAttributeDouble("z", rotation.z);
		writer.WriteOpeningTagEnd(true);
	}
	
	if(!scaling.IsEqualTo(decVector(1.0f, 1.0f, 1.0f))){
		writer.WriteOpeningTagStart("scaling");
		writer.WriteAttributeDouble("x", scaling.x);
		writer.WriteAttributeDouble("y", scaling.y);
		writer.WriteAttributeDouble("z", scaling.z);
		writer.WriteOpeningTagEnd(true);
	}
	
	pWriteProperties(writer, object.GetProperties());
	
	object.GetTextures().Visit([&](const meObjectTexture &texture){
		pWriteObjectTexture(writer, texture);
	});
	
	if(object.GetAttachedTo()){
		writer.WriteDataTagString("attachTo", object.GetAttachedTo()->GetID().ToHexString());
	}
	
	const decStringList &attachBehaviors = object.GetAttachBehaviors();
	const int attachBehaviorCount = attachBehaviors.GetCount();
	for(i=0; i<attachBehaviorCount; i++){
		writer.WriteDataTagString("attachBehavior", attachBehaviors.GetAt(i));
	}
	
	writer.WriteClosingTag("object");
}

void meLSXMLWorld::pWriteObjectTexture(decXmlWriter &writer, const meObjectTexture &texture){
	const decVector2 &texCoordOffset = texture.GetTexCoordOffset();
	const decVector2 &texCoordScaling = texture.GetTexCoordScaling();
	const float texCoordRotation = texture.GetTexCoordRotation();
	const decColor &tint = texture.GetColorTint();
	const bool hasTexCoordOffset = !texCoordOffset.IsEqualTo(decVector2(0.0f, 0.0f));
	const bool hasTexCoordScaling = !texCoordScaling.IsEqualTo(decVector2(1.0f, 1.0f));
	const bool hasTexCoordRotation = (fabsf(texCoordRotation) > FLOAT_SAFE_EPSILON);
	const bool hasTint = !tint.IsEqualTo(decColor(1.0f, 1.0f, 1.0f));
	
	writer.WriteOpeningTagStart("texture");
	writer.WriteAttributeString("name", texture.GetName().GetString());
	writer.WriteOpeningTagEnd(false, true);
	
	if(!texture.GetSkinPath().IsEmpty()){
		writer.WriteDataTagString("skin", texture.GetSkinPath().GetString());
	}
	
	if(hasTexCoordOffset || hasTexCoordScaling || hasTexCoordRotation){
		writer.WriteOpeningTag("transform");
		
		if(hasTexCoordOffset){
			writer.WriteOpeningTagStart("translation");
			writer.WriteAttributeFloat("u", texCoordOffset.x);
			writer.WriteAttributeFloat("v", texCoordOffset.y);
			writer.WriteOpeningTagEnd(true);
		}
		
		if(hasTexCoordScaling){
			writer.WriteOpeningTagStart("scaling");
			writer.WriteAttributeFloat("u", texCoordScaling.x);
			writer.WriteAttributeFloat("v", texCoordScaling.y);
			writer.WriteOpeningTagEnd(true);
		}
		
		if(hasTexCoordRotation){
			writer.WriteDataTagFloat("rotation", texCoordRotation);
		}
		
		writer.WriteClosingTag("transform");
	}
	
	if(hasTint){
		writer.WriteOpeningTagStart("tint");
		writer.WriteAttributeFloat("r", tint.r);
		writer.WriteAttributeFloat("g", tint.g);
		writer.WriteAttributeFloat("b", tint.b);
		writer.WriteOpeningTagEnd(true);
	}
	
	pWriteProperties(writer, texture.GetProperties());
	
	writer.WriteClosingTag("texture");
}

void meLSXMLWorld::pWriteDecal(decXmlWriter &writer, const meDecal &decal){
	const decDVector &position = decal.GetPosition();
	const decVector &rotation = decal.GetRotation();
	const decVector &size = decal.GetSize();
	const decVector2 &texCoordOffset = decal.GetTexCoordOffset();
	const decVector2 &texCoordScaling = decal.GetTexCoordScaling();
	const float texCoordRotation = decal.GetTexCoordRotation();
	const decColor &tint = decal.GetColorTint();
	const bool hasTexCoordOffset = !texCoordOffset.IsEqualTo(decVector2(0.0f, 0.0f));
	const bool hasTexCoordScaling = !texCoordScaling.IsEqualTo(decVector2(1.0f, 1.0f));
	const bool hasTexCoordRotation = (fabsf(texCoordRotation) > FLOAT_SAFE_EPSILON);
	const bool hasTint = !tint.IsEqualTo(decColor(1.0f, 1.0f, 1.0f));
	
	writer.WriteOpeningTag("decal");
	
	writer.WriteDataTagString("skin", decal.GetSkinPath());
	//writer.WriteDataTagString( "texture", decal.GetTextureName() );
	
	writer.WriteOpeningTagStart("position");
	writer.WriteAttributeDouble("x", position.x);
	writer.WriteAttributeDouble("y", position.y);
	writer.WriteAttributeDouble("z", position.z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteOpeningTagStart("rotation");
	writer.WriteAttributeDouble("x", rotation.x);
	writer.WriteAttributeDouble("y", rotation.y);
	writer.WriteAttributeDouble("z", rotation.z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteOpeningTagStart("size");
	writer.WriteAttributeDouble("x", size.x);
	writer.WriteAttributeDouble("y", size.y);
	writer.WriteAttributeDouble("z", size.z);
	writer.WriteOpeningTagEnd(true);
	
	if(!decal.GetVisible()){
		writer.WriteDataTagBool("visible", decal.GetVisible());
	}
	
	if(hasTexCoordOffset || hasTexCoordScaling || hasTexCoordRotation){
		writer.WriteOpeningTag("transform");
		
		if(hasTexCoordOffset){
			writer.WriteOpeningTagStart("translation");
			writer.WriteAttributeFloat("u", texCoordOffset.x);
			writer.WriteAttributeFloat("v", texCoordOffset.y);
			writer.WriteOpeningTagEnd(true);
		}
		
		if(hasTexCoordScaling){
			writer.WriteOpeningTagStart("scaling");
			writer.WriteAttributeFloat("u", texCoordScaling.x);
			writer.WriteAttributeFloat("v", texCoordScaling.y);
			writer.WriteOpeningTagEnd(true);
		}
		
		if(hasTexCoordRotation){
			writer.WriteDataTagFloat("rotation", texCoordRotation);
		}
		
		writer.WriteClosingTag("transform");
	}
	
	if(hasTint){
		writer.WriteOpeningTagStart("tint");
		writer.WriteAttributeFloat("r", tint.r);
		writer.WriteAttributeFloat("g", tint.g);
		writer.WriteAttributeFloat("b", tint.b);
		writer.WriteOpeningTagEnd(true);
	}
	
	pWriteProperties(writer, decal.GetProperties());
	
	writer.WriteClosingTag("decal");
}

void meLSXMLWorld::pWriteNavigationSystem(decXmlWriter &writer, const meNavigationSpace &navspace){
	const decDVector &position = navspace.GetPosition();
	const decVector &orientation = navspace.GetOrientation();
	
	writer.WriteOpeningTag("navigationSpace");
	
	writer.WriteOpeningTagStart("position");
	writer.WriteAttributeDouble("x", position.x);
	writer.WriteAttributeDouble("y", position.y);
	writer.WriteAttributeDouble("z", position.z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteOpeningTagStart("orientation");
	writer.WriteAttributeDouble("x", orientation.x);
	writer.WriteAttributeDouble("y", orientation.y);
	writer.WriteAttributeDouble("z", orientation.z);
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteDataTagString("filename", navspace.GetFilename().GetString());
	
	writer.WriteClosingTag("navigationSpace");
}

void meLSXMLWorld::pWriteProperties(decXmlWriter &writer, const decStringDictionary &properties){
	properties.Visit([&](const decString &key, const decString &value){
		writer.WriteOpeningTagStart("property");
		writer.WriteAttributeString("key", key);
		writer.WriteOpeningTagEnd(false, false);
		WriteTextMultilineString(writer, value);
		writer.WriteClosingTag("property", false, true);
	});
}
