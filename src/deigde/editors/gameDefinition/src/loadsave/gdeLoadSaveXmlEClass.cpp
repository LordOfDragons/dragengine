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

#include "gdeLoadSaveXmlEClass.h"
#include "../gamedef/gdeGameDefinition.h"
#include "../gamedef/objectClass/gdeObjectClass.h"
#include "../gamedef/objectClass/component/gdeOCComponent.h"
#include "../gamedef/objectClass/component/gdeOCComponentTexture.h"
#include "../gamedef/objectClass/inherit/gdeOCInherit.h"
#include "../gamedef/property/gdeProperty.h"
#include "../gui/gdeWindowMain.h"

#include <deigde/codec/igdeCodecPropertyString.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decPointerDictionary.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>



// Class gdeLoadSaveXmlEClass
///////////////////////////////

// Constructor, destructor
////////////////////////////

gdeLoadSaveXmlEClass::gdeLoadSaveXmlEClass(gdeLoadSaveSystem &loadSaveSystem,
deLogger *logger, const char *loggerSource) :
igdeBaseXML(logger, loggerSource),
pLoadSaveSystem(loadSaveSystem),
pName("XML Element Class"),
pPattern(".deeclass"){
}



// Loading and saving
///////////////////////

gdeObjectClass *gdeLoadSaveXmlEClass::LoadXmlEClass(decBaseFileReader &reader){
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	
	decXmlParser(GetLogger()).ParseXml(&reader, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if(!root){
		DETHROW(deeInvalidParam);
	}
	
	if(strcmp(root->GetName(), "elementClass") != 0){
		DETHROW(deeInvalidParam);
	}
	
	return pReadElementClass(*root);
}

void gdeLoadSaveXmlEClass::SaveXmlEClass(const gdeGameDefinition &gameDefinition,
const gdeObjectClass &objectClass, decBaseFileWriter &writer){
	decXmlWriter xmlWriter(&writer);
	xmlWriter.WriteXMLDeclaration();
	pWriteElementClass(xmlWriter, gameDefinition, objectClass);
}



// Private Functions
//////////////////////

gdeObjectClass *gdeLoadSaveXmlEClass::pReadElementClass(const decXmlElementTag &root){
#if 0
	const int elementCount = root.GetElementCount();
	int i;
	
	gdeObjectClass::Ref objectClass = NULL;
	try{
		objectClass.TakeOver(new gdeObjectClass);
		
		for(i=0; i<elementCount; i++){
			const decXmlElementTag * const tag = root.GetElementIfTag(i);
			if(!tag){
				continue;
			}
			
			const decString tagName(tag->GetName());
			
			if(tagName == "name"){
				gameDefinition.SetName(GetCDataString(*tag));
				
			}else if(tagName == "basePath"){
				gameDefinition.SetBasePath(GetCDataString(*tag));
				
			}else if(tagName == "defaultClass"){
				gameDefinition.SetDefaultObjectClass(GetCDataString(*tag));
				
			}else if(tagName == "defaultSkin"){
				gameDefinition.SetDefaultSkin(GetCDataString(*tag));
				
			}else if(tagName == "defaultSky"){
				gameDefinition.SetDefaultSky(GetCDataString(*tag));
				
			}else if(tagName == "class"){
				pReadObjectClass(*tag, gameDefinition);
				
			}else if(tagName == "skin"){
				pReadSkin(*tag, gameDefinition);
				
			}else if(tagName == "sky"){
				pReadSky(*tag, gameDefinition);
				
			}else if(tagName == "particleEmitter"){
				pReadParticleEmitter(*tag, gameDefinition);
				
			}else if(tagName == "categories"){
				pReadCategories(*tag, gameDefinition);
				
			}else if(tagName == "property"){
				pReadProperty(*tag, gameDefinition.GetWorldProperties());
				
			}else if(tagName == "decalProperty"){
				pReadProperty(*tag, gameDefinition.GetDecalProperties());
				
			}else{
				LogWarnUnknownTag(root, *tag);
			}
		}
		
	}catch(const deException &){
		throw;
	}
	
	return objectClass;
#endif
	
	// TODO
	DETHROW(deeInvalidParam);
}



void gdeLoadSaveXmlEClass::pWriteElementClass(decXmlWriter &writer,
const gdeGameDefinition &gameDefinition, const gdeObjectClass &objectClass){
	// element class tag
	writer.WriteOpeningTagStart("elementClass");
	
	writer.WriteAttributeString("name", objectClass.GetName());
	
	if(objectClass.GetInherits().GetCount() == 0){
		DETHROW(deeInvalidParam);
	}
	
	writer.WriteAttributeString("class", objectClass.GetInherits().GetAt(0)->GetName());
	
	writer.WriteOpeningTagEnd();
	
	// we can only guess what properties the element class supports since this is not required
	// to be the same as the properties defined for the element class instances. the properties
	// defined in the object class and the parent classes are ignored since the parent class
	// (which has to be a script class not an xml element class) define these values already
	// in when adding element class properties. what we do here is only exporting the property
	// values.
	writer.WriteComment("These are the property values set in this element class.");
	writer.WriteComment("The names and types are guessed from the game definition.");
	writer.WriteComment("Check and fix them before using.");
	writer.WriteNewline();
	
	const decStringDictionary &properties = objectClass.GetPropertyValues();
	const decStringList names(properties.GetKeys());
	const int count = names.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const decString &name = names.GetAt(i);
		pWritePropertyValue(writer, objectClass, false, name, properties.GetAt(name));
	}
	
	// write texture replacements
	gdeOCComponentTextureList textures;
	pCollectTextures(gameDefinition, objectClass, textures);
	
	const int textureCount = textures.GetCount();
	if(textureCount > 0){
		writer.WriteOpeningTagStart("map");
		writer.WriteAttributeString("name", "textureReplacements");
		writer.WriteOpeningTagEnd();
		
		for(i=0; i<textureCount; i++){
			pWritePropertyTextureReplacement(writer, objectClass, *textures.GetAt(i));
		}
		
		writer.WriteClosingTag("map");
	}
	
	// end of element class tag
	writer.WriteClosingTag("elementClass");
}

void gdeLoadSaveXmlEClass::pCollectTextures(const gdeGameDefinition &gameDefinition,
const gdeObjectClass &objectClass, gdeOCComponentTextureList &list){
	const gdeOCComponentTextureList &textures = objectClass.GetTextures();
	const int textureCount = textures.GetCount();
	int i;
	
	for(i=0; i<textureCount; i++){
		gdeOCComponentTexture * const texture = textures.GetAt(i);
		if(!list.HasNamed(texture->GetName())){
			list.Add(texture);
		}
	}
	
	const gdeOCComponentList &components = objectClass.GetComponents();
	const int componentCount = components.GetCount();
	
	for(i=0; i<componentCount; i++){
		const gdeOCComponentTextureList &textures2 = components.GetAt(i)->GetTextures();
		const int textureCount2 = textures2.GetCount();
		for(i=0; i<textureCount2; i++){
			gdeOCComponentTexture * const texture = textures2.GetAt(i);
			if(!list.HasNamed(texture->GetName())){
				list.Add(texture);
			}
		}
	}
	
	const gdeOCInheritList &inherits = objectClass.GetInherits();
	const int inheritCount = inherits.GetCount();
	for(i=0; i<inheritCount; i++){
		const gdeObjectClass * const resolvedClass = gameDefinition.FindObjectClass(inherits.GetAt(i)->GetName());
		if(resolvedClass){
			pCollectTextures(gameDefinition, *resolvedClass, list);
		}
	}
}

void gdeLoadSaveXmlEClass::pWritePropertyValue(decXmlWriter &writer, const gdeObjectClass &objectClass,
bool isMapEntry, const char *name, const decString &value){
	const gdeObjectClass *propertyOwner = NULL;
	const gdeProperty *property = NULL;
	
	if(objectClass.DeepGetNamedProperty(name, propertyOwner, property)){
		pWritePropertyValue(writer, property->GetType(), isMapEntry, name, value);
		
	}else{
		pWritePropertyValueType(writer, isMapEntry, "string", name, value);
	}
}

void gdeLoadSaveXmlEClass::pWritePropertyValue(decXmlWriter& writer,
gdeProperty::ePropertyTypes type, bool isMapEntry, const char *name, const decString &value){
	switch(type){
	case gdeProperty::eptString:
		pWritePropertyValueType(writer, isMapEntry, "string", name, value);
		break;
		
	case gdeProperty::eptInteger:
		pWritePropertyValueType(writer, isMapEntry, "integer", name, value);
		break;
		
	case gdeProperty::eptPoint2:{
		decVector2 vector;
		pCodec.DecodeVector2(value, vector, decVector2());
		
		writer.WriteOpeningTagStart("point");
		writer.WriteAttributeString(isMapEntry ? "key" : "name", name);
		writer.WriteAttributeInt("x", (int)vector.x);
		writer.WriteAttributeInt("y", (int)vector.y);
		writer.WriteOpeningTagEnd(true);
		}break;
		
	case gdeProperty::eptPoint3:
		// not directly supported yet
		pWritePropertyValueType(writer, isMapEntry, "string", name, value);
		break;
		
	case gdeProperty::eptFloat:
		pWritePropertyValueType(writer, isMapEntry, "float", name, value);
		break;
		
	case gdeProperty::eptVector2:{
		decVector2 vector;
		pCodec.DecodeVector2(value, vector, decVector2());
		
		writer.WriteOpeningTagStart("vector2");
		writer.WriteAttributeString(isMapEntry ? "key" : "name", name);
		writer.WriteAttributeFloat("x", vector.x);
		writer.WriteAttributeFloat("y", vector.y);
		writer.WriteOpeningTagEnd(true);
		}break;
		
	case gdeProperty::eptVector3:{
		decVector vector;
		pCodec.DecodeVector(value, vector, decVector());
		
		writer.WriteOpeningTagStart("vector");
		writer.WriteAttributeString(isMapEntry ? "key" : "name", name);
		writer.WriteAttributeFloat("x", vector.x);
		writer.WriteAttributeFloat("y", vector.y);
		writer.WriteAttributeFloat("z", vector.z);
		writer.WriteOpeningTagEnd(true);
		}break;
		
	case gdeProperty::eptColor:{
		decColor color;
		pCodec.DecodeColor(value, color, decColor());
		
		writer.WriteOpeningTagStart("color");
		writer.WriteAttributeString(isMapEntry ? "key" : "name", name);
		writer.WriteAttributeFloat("r", color.r);
		writer.WriteAttributeFloat("g", color.g);
		writer.WriteAttributeFloat("b", color.b);
		if(fabsf(color.a - 1.0f) > FLOAT_SAFE_EPSILON){
			writer.WriteAttributeFloat("a", color.a);
		}
		writer.WriteOpeningTagEnd(true);
		}break;
		
	case gdeProperty::eptBoolean:
		pWritePropertyValueType(writer, isMapEntry, "boolean", name, value);
		break;
		
	case gdeProperty::eptPath:
		pWritePropertyValueType(writer, isMapEntry, "string", name, value);
		break;
		
	case gdeProperty::eptRange:
		pWritePropertyValueType(writer, isMapEntry, "float", name, value);
		break;
		
	case gdeProperty::eptSelect:
		pWritePropertyValueType(writer, isMapEntry, "string", name, value);
		break;
		
	case gdeProperty::eptList:
		GetLogger()->LogWarn(GetLoggerSource(), "TODO: Implement igdeCodecPropertyString::DecodeStringList\n");
		pWritePropertyValueType(writer, isMapEntry, "string", name, value);
		break;
		
	case gdeProperty::eptTriggerExpression:
		pWritePropertyValueType(writer, isMapEntry, "string", name, value);
		break;
		
	case gdeProperty::eptTriggerTarget:
		pWritePropertyValueType(writer, isMapEntry, "string", name, value);
		break;
		
	case gdeProperty::eptShape:
		pWritePropertyValueType(writer, isMapEntry, "string", name, value);
		break;
		
	case gdeProperty::eptShapeList:
		pWritePropertyValueType(writer, isMapEntry, "string", name, value);
		break;
		
	case gdeProperty::eptIdentifier:
		pWritePropertyValueType(writer, isMapEntry, "string", name, value);
		break;
		
	default:
		pWritePropertyValueType(writer, isMapEntry, "string", name, value);
	}
}

void gdeLoadSaveXmlEClass::pWritePropertyValueType(decXmlWriter &writer,
bool isMapEntry, const char *tagName, const char *name, const decString &value){
	writer.WriteOpeningTagStart(tagName);
	writer.WriteAttributeString(isMapEntry ? "key" : "name", name);
	writer.WriteOpeningTagEnd(false, false);
	
	writer.WriteTextString(value);
	
	writer.WriteClosingTag(tagName, false);
}

void gdeLoadSaveXmlEClass::pWritePropertyTextureReplacement(decXmlWriter &writer,
const gdeObjectClass &objectClass, const gdeOCComponentTexture &texture){
	writer.WriteOpeningTagStart("map");
	writer.WriteAttributeString("key", texture.GetName());
	writer.WriteOpeningTagEnd();
	
	// skin path
	if(!texture.GetPathSkin().IsEmpty()){
		pWritePropertyValueType(writer, true, "string", "skin", texture.GetPathSkin());
	}
	
	// color tint
	if(!texture.GetColorTint().IsEqualTo(decColor(1.0f, 1.0f, 1.0f))){
		const decColor &color = texture.GetColorTint();
		writer.WriteOpeningTagStart("color");
		writer.WriteAttributeString("key", "tint");
		writer.WriteAttributeFloat("r", color.r);
		writer.WriteAttributeFloat("g", color.g);
		writer.WriteAttributeFloat("b", color.b);
		if(fabsf(color.a - 1.0f) > FLOAT_SAFE_EPSILON){
			writer.WriteAttributeFloat("a", color.a);
		}
		writer.WriteOpeningTagEnd(true);
	}
	
	// transform
	const decVector2 &offset = texture.GetOffset();
	const float rotation = texture.GetRotation();
	const decVector2 &scale = texture.GetScale();
	
	const bool hasOffset = !offset.IsEqualTo(decVector2());
	const bool hasRotation = fabsf(rotation) > FLOAT_SAFE_EPSILON;
	const bool hasScale = !scale.IsEqualTo(decVector2(1.0f, 1.0f));
	
	if(hasOffset || hasRotation || hasScale){
		writer.WriteOpeningTagStart("map");
		writer.WriteAttributeString("key", "transform");
		writer.WriteOpeningTagEnd();
		
		if(hasOffset){
			writer.WriteOpeningTagStart("vector2");
			writer.WriteAttributeString("key", "translate");
			writer.WriteAttributeFloat("x", offset.x);
			writer.WriteAttributeFloat("y", offset.y);
			writer.WriteOpeningTagEnd(true);
		}
		
		if(hasRotation){
			writer.WriteOpeningTagStart("float");
			writer.WriteAttributeString("key", "rotate");
			writer.WriteOpeningTagEnd(false, false);
			writer.WriteTextFloat(rotation);
			writer.WriteClosingTag("float", false);
		}
		
		if(hasScale){
			writer.WriteOpeningTagStart("vector2");
			writer.WriteAttributeString("key", "scale");
			writer.WriteAttributeFloat("x", scale.x);
			writer.WriteAttributeFloat("y", scale.y);
			writer.WriteOpeningTagEnd(true);
		}
		
		writer.WriteClosingTag("map");
	}
	
	// additional texture properties. here again we assume they are defined the same way
	// as element class properties as they are in objects properties
	const decStringDictionary &properties = texture.GetProperties();
	const decStringList names(properties.GetKeys());
	const int nameCount = names.GetCount();
	int i;
	
	for(i=0; i<nameCount; i++){
		const decString &name = names.GetAt(i);
		
		const gdeObjectClass *propertyOwner = NULL;
		const gdeProperty *property = NULL;
		if(objectClass.DeepGetNamedProperty(name, propertyOwner, property)){
			pWritePropertyValue(writer, property->GetType(), true, name, properties.GetAt(name));
			
		}else{
			pWritePropertyValueType(writer, true, "string", name, properties.GetAt(name));
		}
	}
	
	writer.WriteClosingTag("map");
}
