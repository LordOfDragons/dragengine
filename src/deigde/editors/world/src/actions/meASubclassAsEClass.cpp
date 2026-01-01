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

#include "meASubclassAsEClass.h"
#include "../gui/meWindowMain.h"
#include "../world/meWorld.h"
#include "../world/object/meObject.h"
#include "../world/object/meObjectSelection.h"
#include "../world/object/texture/meObjectTexture.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/filedialog/igdeFilePattern.h>
#include <deigde/gui/filedialog/igdeFilePattern.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gamedefinition/class/igdeGDClassManager.h>
#include <deigde/gamedefinition/property/igdeGDProperty.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>



// Class meWPSObjectListener
//////////////////////////////

// Constructor, destructor
////////////////////////////

meASubclassAsEClass::meASubclassAsEClass(meWindowMain &window) :
igdeAction("Subclass As EClass...",
	window.GetEnvironment().GetStockIcon(igdeEnvironment::esiSaveAs),
	"Create Subclass of selected object saved as XML Element Class"),
pWindow(window)
{
}



// Management
///////////////

void meASubclassAsEClass::OnAction(){
	if(!pWindow.GetWorld()){
		return;
	}
	meWorld &world = *pWindow.GetWorld();
	
	if(!world.GetGameDefinition() || !world.GetSelectionObject().HasActive()){
		return;
	}
	const igdeGameDefinition &gamedefinition = *world.GetGameDefinition();
	meObject &object = *world.GetSelectionObject().GetActive();
	
	if(!object.GetGDClass()){
		return;
	}
	const igdeGDClass &gdclass = *object.GetGDClass();
	
	// ask for class name to use
	const char * const dialogTitle = "Subclass As EClass";
	decString classname(gdclass.GetName());
	if(!igdeCommonDialogs::GetString(&pWindow, dialogTitle, "Object Class:", classname)){
		return;
	}
	
	if(gamedefinition.GetClassManager()->HasNamed(classname)){
		igdeCommonDialogs::Error(&pWindow, dialogTitle, "Object class exists already");
		return;
	}
	
	// ask for filename to save element class to
	igdeFilePattern::List filePatterns;
	filePatterns.Add(igdeFilePattern::Ref::New("XML Element Class", "*.deeclass", ".deeclass"));
	
	decString filename(classname + ".deeclass");
	
	if(!igdeCommonDialogs::GetFileSave(&pWindow, dialogTitle,
	*pWindow.GetEnvironment().GetFileSystemGame(), filePatterns, filename ) ){
		return;
	}
	
	// base path to use to store relative path. using relative path allows to move the
	// created eclass. some path though should stay absolute path. we use here as rule
	// of thumb that all path in the same directory as the eclass or below are relative
	// path. all parent directory path are kept absolute. this is the best choice in
	// most situations leaving the edge cases for the user to edit manually
	decPath pathParentDir;
	pathParentDir.SetFromUnix(filename);
	pathParentDir.RemoveLastComponent();
	
	decString basePath(pathParentDir.GetPathUnix());
	
	if(basePath[basePath.GetLength() - 1] != decPath::PathSeparator()){
		// ensures the path ends with a path separator. this way simple string compare
		// is enough to check if the path in question is in the same directory or below
		basePath += decPath::PathSeparatorString();
	}
	
	// write file
	{
	decXmlWriter xmlWriter(pWindow.GetEnvironment().GetFileSystemGame()->
		OpenFileForWriting(decPath::CreatePathUnix(filename)));
	WriteEClass(object, gdclass, classname, xmlWriter, basePath);
	}
	
	// trigger reloading XML element classes
	pWindow.GetEnvironment().ReloadXMLElementClasses();
}

void meASubclassAsEClass::Update(){
	SetEnabled(pWindow.GetWorld() && pWindow.GetWorld()->GetSelectionObject().HasActive()
		&& pWindow.GetWorld()->GetSelectionObject().GetActive()->GetGDClass());
}



// Protected Functions
////////////////////////

void meASubclassAsEClass::WriteEClass(const meObject &object, const igdeGDClass &gdclass,
const decString &classname, decXmlWriter &writer, const decString &basePath){
	writer.WriteXMLDeclaration();
	
	writer.WriteOpeningTagStart("elementClass");
	writer.WriteAttributeString("name", classname);
	writer.WriteAttributeString("class", gdclass.GetName());
	writer.WriteOpeningTagEnd();
	
	if(object.GetProperties().IsNotEmpty()){
		WriteEClassProperties(object, gdclass, writer, basePath);
	}
	
	if(object.GetTextures().IsNotEmpty()){
		WriteEClassTextureReplacements(object, writer, basePath);
	}
	
	writer.WriteClosingTag("elementClass");
}

void meASubclassAsEClass::WriteEClassProperties(const meObject &object,
const igdeGDClass &gdclass, decXmlWriter &writer, const decString &basePath){
	object.GetProperties().Visit([&](const decString &key, const decString &value){
		WriteEClassProperty(key, value, gdclass.GetPropertyNamed(key), writer, basePath);
	});
}

void meASubclassAsEClass::WriteEClassProperty(const decString &property, const decString &value,
const igdeGDProperty *gdProperty, decXmlWriter &writer, const decString &basePath){
	igdeGDProperty::ePropertyTypes gdPropertyType = gdProperty
		? gdProperty->GetType() : igdeGDProperty::eptString;
	
	switch(gdPropertyType){
	case igdeGDProperty::eptInteger:
		writer.WriteOpeningTagStart("integer");
		writer.WriteAttributeString("name", property);
		writer.WriteOpeningTagEnd(false, false);
		writer.WriteTextString(value);
		writer.WriteClosingTag("integer", false);
		break;
		
	case igdeGDProperty::eptPoint2:{
		int values[2] = {0, 0};
		pCodec.DecodeFixedIntList(value, &values[0], 2);
		writer.WriteOpeningTagStart("point");
		writer.WriteAttributeString("name", property);
		writer.WriteAttributeInt("x", values[0]);
		writer.WriteAttributeInt("y", values[1]);
		writer.WriteOpeningTagEnd(true);
		}break;
		
	case igdeGDProperty::eptPoint3:{
		int values[3] = {0, 0, 0};
		pCodec.DecodeFixedIntList(value, &values[0], 3);
		writer.WriteOpeningTagStart("point3");
		writer.WriteAttributeString("name", property);
		writer.WriteAttributeInt("x", values[0]);
		writer.WriteAttributeInt("y", values[1]);
		writer.WriteAttributeInt("z", values[2]);
		writer.WriteOpeningTagEnd(true);
		}break;
		
	case igdeGDProperty::eptFloat:
	case igdeGDProperty::eptRange:
		writer.WriteOpeningTagStart("float");
		writer.WriteAttributeString("name", property);
		writer.WriteOpeningTagEnd(false, false);
		writer.WriteTextString(value);
		writer.WriteClosingTag("float", false);
		break;
		
	case igdeGDProperty::eptVector2:{
		float values[2] = {0.0f, 0.0f};
		pCodec.DecodeFixedFloatList(value, &values[0], 2);
		writer.WriteOpeningTagStart("vector2");
		writer.WriteAttributeString("name", property);
		writer.WriteAttributeFloat("x", values[0]);
		writer.WriteAttributeFloat("y", values[1]);
		writer.WriteOpeningTagEnd(true);
		}break;
		
	case igdeGDProperty::eptVector3:{
		float values[3] = {0.0f, 0.0f, 0.0f};
		pCodec.DecodeFixedFloatList(value, &values[0], 3);
		writer.WriteOpeningTagStart("vector");
		writer.WriteAttributeString("name", property);
		writer.WriteAttributeFloat("x", values[0]);
		writer.WriteAttributeFloat("y", values[1]);
		writer.WriteAttributeFloat("z", values[2]);
		writer.WriteOpeningTagEnd(true);
		}break;
		
	case igdeGDProperty::eptColor:{
		float values[4] = {0.0f, 0.0f, 0.0f, 0.0f};
		const int parsedCount = pCodec.DecodeFixedFloatList(value, &values[0], 4);
		writer.WriteOpeningTagStart("color");
		writer.WriteAttributeString("name", property);
		writer.WriteAttributeFloat("r", values[0]);
		writer.WriteAttributeFloat("g", values[1]);
		writer.WriteAttributeFloat("b", values[2]);
		if(parsedCount > 3){
			writer.WriteAttributeFloat("a", values[3]);
		}
		writer.WriteOpeningTagEnd(true);
		}break;
		
	case igdeGDProperty::eptBoolean:
		writer.WriteOpeningTagStart("boolean");
		writer.WriteAttributeString("name", property);
		writer.WriteOpeningTagEnd(false, false);
		writer.WriteTextString(value);
		writer.WriteClosingTag("boolean", false);
		break;
		
	case igdeGDProperty::eptList:{
		writer.WriteOpeningTagStart("list");
		writer.WriteAttributeString("name", property);
		writer.WriteOpeningTagEnd();
		
		value.Split(",").Visit([&](const decString &part){
			writer.WriteDataTagString("string", part);
		});
		
		writer.WriteClosingTag("list");
		}break;
		
	case igdeGDProperty::eptPath:
		writer.WriteOpeningTagStart("string");
		writer.WriteAttributeString("name", property);
		writer.WriteOpeningTagEnd(false, false);
		
		// relative path for all path in or below base directory is safe and useful
		writer.WriteTextString(value.BeginsWith(basePath)
			? value.GetMiddle(basePath.GetLength()) : value);
		
		writer.WriteClosingTag("string", false);
		break;
		
	case igdeGDProperty::eptString:
	case igdeGDProperty::eptIdentifier:
	case igdeGDProperty::eptSelect:
	case igdeGDProperty::eptTriggerExpression:
	case igdeGDProperty::eptTriggerTarget:
	case igdeGDProperty::eptShape:
	case igdeGDProperty::eptShapeList:
	default:
		writer.WriteOpeningTagStart("string");
		writer.WriteAttributeString("name", property);
		writer.WriteOpeningTagEnd(false, false);
		writer.WriteTextString(value);
		writer.WriteClosingTag("string", false);
		break;
	}
}

void meASubclassAsEClass::WriteEClassTextureReplacements(const meObject &object,
decXmlWriter &writer, const decString &basePath){
	writer.WriteOpeningTagStart("map");
	writer.WriteAttributeString("name", "component.textureReplacements");
	writer.WriteOpeningTagEnd();
	
	object.GetTextures().Visit([&](const meObjectTexture &t){
		WriteEClassTextureReplacements(t, writer, basePath);
	});
	
	writer.WriteClosingTag("map");
}

void meASubclassAsEClass::WriteEClassTextureReplacements(const meObjectTexture &texture,
decXmlWriter &writer, const decString &basePath){
	writer.WriteOpeningTagStart("map");
	writer.WriteAttributeString("key", texture.GetName());
	writer.WriteOpeningTagEnd();
	
	const decString &pathSkin = texture.GetSkinPath();
	if(!pathSkin.IsEmpty()){
		writer.WriteOpeningTagStart("string");
		writer.WriteAttributeString("key", "skin");
		writer.WriteOpeningTagEnd(false, false);
		
		// relative path for skin path in or below base directory is safe and useful
		writer.WriteTextString(pathSkin.BeginsWith(basePath)
			? pathSkin.GetMiddle(basePath.GetLength()) : pathSkin);
		
		writer.WriteClosingTag("string", false);
	}
	
	const decColor &colorTint = texture.GetColorTint();
	if(!colorTint.IsEqualTo(decColor(1.0f, 1.0f, 1.0f))){
		writer.WriteOpeningTagStart("color");
		writer.WriteAttributeString("key", "tint");
		writer.WriteAttributeFloat("r", colorTint.r);
		writer.WriteAttributeFloat("g", colorTint.g);
		writer.WriteAttributeFloat("b", colorTint.b);
		writer.WriteOpeningTagEnd(true);
	}
	
	const decVector2 &tcoffset = texture.GetTexCoordOffset();
	const float tcrotation = texture.GetTexCoordRotation();
	const decVector2 &tcscaling = texture.GetTexCoordScaling();
	const bool hasTCOffset = !tcoffset.IsEqualTo(decVector2());
	const bool hasTCRotation = fabsf(tcrotation) > FLOAT_SAFE_EPSILON;
	const bool hasTCScaling = !tcscaling.IsEqualTo(decVector2(1.0f, 1.0f));
	if(hasTCOffset || hasTCRotation || hasTCScaling){
		writer.WriteOpeningTagStart("map");
		writer.WriteAttributeString("key", "transform");
		writer.WriteOpeningTagEnd();
		
		if(hasTCOffset){
			writer.WriteOpeningTagStart("vector2");
			writer.WriteAttributeString("key", "translate");
			writer.WriteAttributeFloat("x", tcoffset.x);
			writer.WriteAttributeFloat("y", tcoffset.y);
			writer.WriteOpeningTagEnd(true);
		}
		
		if(hasTCRotation){
			writer.WriteOpeningTagStart("float");
			writer.WriteAttributeString("key", "rotate");
			writer.WriteOpeningTagEnd(false, false);
			writer.WriteTextFloat(tcrotation);
			writer.WriteClosingTag("float", false);
		}
		
		if(hasTCScaling){
			writer.WriteOpeningTagStart("vector2");
			writer.WriteAttributeString("key", "scale");
			writer.WriteAttributeFloat("x", tcscaling.x);
			writer.WriteAttributeFloat("y", tcscaling.y);
			writer.WriteOpeningTagEnd(true);
		}
		
		writer.WriteClosingTag("map");
	}
	
	writer.WriteClosingTag("map");
}
