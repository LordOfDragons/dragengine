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

#include "deFontModule.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontGlyph.h>



// Export definition
//////////////////////

#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *DEFontCreateModule(deLoadableModule *loadableModule);
#ifdef  __cplusplus
}
#endif
#endif


// Entry function
///////////////////

deBaseModule *DEFontCreateModule(deLoadableModule *loadableModule) {
	deBaseModule *module = NULL;
	
	try {
		module = new deFontModule(*loadableModule);
		
	} catch (const deException &) {
	}
	
	return module;
}



// Class deFontModule
///////////////////////

// Constructor, destructor
////////////////////////////

deFontModule::deFontModule(deLoadableModule &loadableModule) :
deBaseFontModule(loadableModule) {
}

deFontModule::~deFontModule() {
}



// Loading and saving
///////////////////////

void deFontModule::LoadFont(decBaseFileReader &file, deFont &font) {
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	
	decXmlParser(GetGameEngine()->GetLogger()).ParseXml(&file, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if (!root || strcmp(root->GetName(), "font") != 0){
		DETHROW(deeInvalidParam);
	}
	
	pParseFont(root, font);
}

void deFontModule::SaveFont(decBaseFileWriter &file, const deFont &font) {
	decXmlWriter writer(&file);
	writer.WriteXMLDeclaration();
	pWriteFont(writer, font);
}



// Private functions
//////////////////////

const char *deFontModule::pGetAttributeString(decXmlElementTag *tag, const char *name) {
	decXmlAttValue *value = tag->FindAttribute(name);
	if (value) {
		return value->GetValue();
	} else {
		LogErrorFormat("Missing Attribute '%s' in tag '%s'\n", name, tag->GetName().GetString());
		DETHROW(deeInvalidParam);
	}
}

bool deFontModule::pHasAttributeString(decXmlElementTag *tag, const char *name) {
	return tag->FindAttribute(name) != NULL;
}

int deFontModule::pGetAttributeInt(decXmlElementTag *tag, const char *name) {
	decXmlAttValue *value = tag->FindAttribute(name);
	if (value) {
		return (int)strtol(value->GetValue(), NULL, 10);
	} else {
		LogErrorFormat("Missing Attribute '%s' in tag '%s'\n", name, tag->GetName().GetString());
		DETHROW(deeInvalidParam);
	}
}
float deFontModule::pGetAttributeFloat(decXmlElementTag *tag, const char *name) {
	decXmlAttValue *value = tag->FindAttribute(name);
	if (value) {
		return strtof(value->GetValue(), NULL);
	} else {
		LogErrorFormat("Missing Attribute '%s' in tag '%s'\n", name, tag->GetName().GetString());
		DETHROW(deeInvalidParam);
	}
}

void deFontModule::pParseFont(decXmlElementTag *root, deFont &font){
	decXmlElementTag *tag;
	int i;
	
	int glyphCount = 0;
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(tag && strcmp(tag->GetName(), "glyph") == 0){
			glyphCount++;
		}
	}
	
	font.SetGlyphCount(glyphCount);
	bool hasBaseLine = false;
	int glyphIndex = 0;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = root->GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(strcmp(tag->GetName(), "image") == 0){
			font.SetImagePath(pGetAttributeString(tag, "src"));
			
		}else if(strcmp(tag->GetName(), "colorFont") == 0){
			font.SetIsColorFont(true);
			
		}else if(strcmp(tag->GetName(), "lineHeight") == 0){
			font.SetLineHeight(tag->GetFirstData()->GetData().ToInt());
			if(!hasBaseLine){
				font.SetBaseLine(font.GetLineHeight() * 3 / 4);
			}
			
		}else if(tag->GetName() == "baseLine"){
			font.SetBaseLine(tag->GetFirstData()->GetData().ToInt());
			hasBaseLine = true;
			
		}else if(strcmp(tag->GetName(), "defaults") == 0){
			deFontGlyph &glyph = font.GetUndefinedGlyph();
			
			if(pHasAttributeString(tag, "u")){
				// DEPRECATED old version
				glyph.SetX(pGetAttributeInt(tag, "u"));
				glyph.SetY(pGetAttributeInt(tag, "v"));
				glyph.SetWidth(pGetAttributeInt(tag, "width"));
				glyph.SetHeight(font.GetLineHeight());
				glyph.SetAdvance(glyph.GetWidth());
				
			}else{
				glyph.SetX(pGetAttributeInt(tag, "x"));
				glyph.SetY(pGetAttributeInt(tag, "y"));
				glyph.SetZ(pGetAttributeInt(tag, "z"));
				glyph.SetWidth(pGetAttributeInt(tag, "width"));
				glyph.SetBearing(pGetAttributeInt(tag, "bearing"));
				glyph.SetAdvance(pGetAttributeInt(tag, "advance"));
				
				if(pHasAttributeString(tag, "height")){
					glyph.SetHeight(pGetAttributeInt(tag, "width"));
					
				}else{
					glyph.SetHeight(font.GetLineHeight());
				}
				
				if(pHasAttributeString(tag, "bearingY")){
					glyph.SetBearingY(pGetAttributeInt(tag, "bearingY"));
				}
			}
			
		}else if(strcmp(tag->GetName(), "glyph") == 0) {
			deFontGlyph &glyph = font.GetGlyphAt(glyphIndex++);
			
			if(pHasAttributeString(tag, "code")){
				// DEPRECATED old version
				glyph.SetUnicode(pGetAttributeInt(tag, "code"));
				glyph.SetX(pGetAttributeInt(tag, "u"));
				glyph.SetY(pGetAttributeInt(tag, "v"));
				
			}else{
				glyph.SetUnicode(pGetAttributeInt(tag, "unicode"));
				glyph.SetX(pGetAttributeInt(tag, "x"));
				glyph.SetY(pGetAttributeInt(tag, "y"));
				glyph.SetZ(pGetAttributeInt(tag, "z"));
			}
			
			glyph.SetWidth(pGetAttributeInt(tag, "width"));
			glyph.SetBearing(pGetAttributeInt(tag, "bearing"));
			glyph.SetAdvance(pGetAttributeInt(tag, "advance"));
			
			if(pHasAttributeString(tag, "height")){
				glyph.SetHeight(pGetAttributeInt(tag, "width"));
				
			}else{
				glyph.SetHeight(font.GetLineHeight());
			}
			
			if(pHasAttributeString(tag, "bearingY")){
				glyph.SetBearingY(pGetAttributeInt(tag, "bearingY"));
			}
		}
	}
}



void deFontModule::pWriteFont(decXmlWriter& writer, const deFont& font) {
	writer.WriteOpeningTag("font", false, true);
	
	if (strlen(font.GetImagePath()) > 0) {
		writer.WriteOpeningTagStart("image");
		writer.WriteAttributeString("src", font.GetImagePath());
		writer.WriteOpeningTagEnd(true);
	}
	
	if (font.GetIsColorFont()) {
		writer.WriteDataTagString("colorFont", "true"); // hack, only presence of tag checked
	}
	
	writer.WriteDataTagInt("lineHeight", font.GetLineHeight());
	writer.WriteDataTagInt("baseLine", font.GetBaseLine());
	
	const deFontGlyph &undefinedGlyph = font.GetUndefinedGlyph();
	writer.WriteOpeningTagStart("defaults");
	writer.WriteAttributeInt("x", undefinedGlyph.GetX());
	writer.WriteAttributeInt("y", undefinedGlyph.GetY());
	writer.WriteAttributeInt("z", undefinedGlyph.GetZ());
	writer.WriteAttributeInt("width", undefinedGlyph.GetWidth());
	writer.WriteAttributeInt("height", undefinedGlyph.GetHeight());
	if(undefinedGlyph.GetBearing() != 0){
		writer.WriteAttributeInt("bearing", undefinedGlyph.GetBearing());
	}
	if(undefinedGlyph.GetBearingY() != 0){
		writer.WriteAttributeInt("bearingY", undefinedGlyph.GetBearingY());
	}
	writer.WriteAttributeInt("advance", undefinedGlyph.GetAdvance());
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteNewline();
	const int glyphCount = font.GetGlyphCount();
	int i;
	
	for (i=0; i<glyphCount; i++) {
		const deFontGlyph &glyph = font.GetGlyphAt(i);
		
		writer.WriteOpeningTagStart("glyph");
		writer.WriteAttributeInt("unicode", glyph.GetUnicode());
		writer.WriteAttributeInt("x", glyph.GetX());
		writer.WriteAttributeInt("y", glyph.GetY());
		writer.WriteAttributeInt("z", glyph.GetZ());
		writer.WriteAttributeInt("width", glyph.GetWidth());
		writer.WriteAttributeInt("height", glyph.GetHeight());
		if(glyph.GetBearing() != 0){
			writer.WriteAttributeInt("bearing", glyph.GetBearing());
		}
		if(glyph.GetBearingY() != 0){
			writer.WriteAttributeInt("bearingY", glyph.GetBearingY());
		}
		writer.WriteAttributeInt("advance", glyph.GetAdvance());
		writer.WriteOpeningTagEnd(true);
	}
	
	writer.WriteClosingTag("font", true);
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

#ifndef MODULE_VERSION
#include "module_version.h"
#endif

class deFontModuleInternal : public deInternalModule{
public:
	typedef deTObjectReference<deFontModuleInternal> Ref;
	
	deFontModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("DEFont");
		SetDescription("Handles fonts in the XML Drag[en]gine font format.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtFont);
		SetDirectoryName("defont");
		GetPatternList().Add(".defont");
		SetDefaultExtension(".defont");
		SetPriority(1);
		SetDefaultLoggingName();
	}
	
	void CreateModule() override{
		SetModule(DEFontCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deTObjectReference<deInternalModule> deFontRegisterInternalModule(deModuleSystem *system){
	return deFontModuleInternal::Ref::NewWith(system);
}
#endif
