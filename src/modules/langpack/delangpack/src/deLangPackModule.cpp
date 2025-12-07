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

#include "deLangPackModule.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/resources/localization/deLanguagePack.h>
#include <dragengine/resources/localization/deLanguagePackEntry.h>


#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *DELangPackCreateModule(deLoadableModule *loadableModule);
#ifdef  __cplusplus
}
#endif
#endif


// Entry function
///////////////////

deBaseModule *DELangPackCreateModule(deLoadableModule *loadableModule){
	deBaseModule *module = NULL;
	
	try{
		module = new deLangPackModule(*loadableModule);
		
	}catch(const deException &){
		return NULL;
	}
	
	return module;
}



// Class deLangPackModule
///////////////////////////

// Constructor, destructor
////////////////////////////

deLangPackModule::deLangPackModule(deLoadableModule &loadableModule) :
deBaseLanguagePackModule(loadableModule){
}

deLangPackModule::~deLangPackModule(){
}



// Management
///////////////

void deLangPackModule::LoadLanguagePack(decBaseFileReader &file, deLanguagePack &languagePack){
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	
	decXmlParser(GetGameEngine()->GetLogger()).ParseXml(&file, xmlDoc);
	
	xmlDoc->StripComments();
// 	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if(! root || strcmp(root->GetName(), "languagePack") != 0){
		DETHROW(deeInvalidParam);
	}
	
	pParseLangPack(*root, languagePack);
}

void deLangPackModule::SaveLanguagePack(decBaseFileWriter &file, const deLanguagePack &languagePack){
	decXmlWriter xmlWriter(&file);
	xmlWriter.WriteXMLDeclaration();
	pWriteLangPack(xmlWriter, languagePack);
}



// Private functions
//////////////////////

const decXmlAttValue *deLangPackModule::pFindAttribute(const decXmlElementTag &tag, const char *name){
	const int elementCount = tag.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		decXmlElement &element = *tag.GetElementAt(i);
		
		if(element.CanCastToAttValue()){
			const decXmlAttValue * const value = element.CastToAttValue();
			
			if(strcmp(value->GetName(), name) == 0){
				return value;
			}
		}
	}
	
	return NULL;
}

const char *deLangPackModule::pGetAttributeString(const decXmlElementTag &tag, const char *name){
	const decXmlAttValue * const value = pFindAttribute(tag, name);
	
	if(value){
		return value->GetValue();
		
	}else{
		LogErrorFormat("Missing Attribute '%s' in tag '%s'", name, tag.GetName().GetString());
		DETHROW(deeInvalidParam);
	}
}



void deLangPackModule::pParseLangPack(const decXmlElementTag &root, deLanguagePack &languagePack){
	const int elementCount = root.GetElementCount();
	int i;
	
	int entryCount = 0;
	for(i=0; i<elementCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(tag && strcmp(tag->GetName(), "translation") == 0){
			entryCount++;
		}
	}
	
	languagePack.SetEntryCount(entryCount);
	int entryIndex = 0;
	
	for(i=0; i<elementCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		
		if(tag){
			if(tag->GetName() == "identifier"){
				const decXmlCharacterData * const cdata = tag->GetFirstData();
				if(cdata){
					languagePack.SetIdentifier(cdata->GetData());
				}
				
			}else if(strcmp(tag->GetName(), "name") == 0){
				const decXmlCharacterData * const cdata = tag->GetFirstData();
				if(cdata){
					languagePack.SetName(decUnicodeString::NewFromUTF8(cdata->GetData()));
				}
				
			}else if(strcmp(tag->GetName(), "description") == 0){
				const decXmlCharacterData * const cdata = tag->GetFirstData();
				if(cdata){
					languagePack.SetDescription(decUnicodeString::NewFromUTF8(cdata->GetData()));
				}
				
			}else if(strcmp(tag->GetName(), "missingText") == 0){
				const decXmlCharacterData * const cdata = tag->GetFirstData();
				if(cdata){
					languagePack.SetMissingText(decUnicodeString::NewFromUTF8(cdata->GetData()));
				}
				
			}else if(strcmp(tag->GetName(), "translation") == 0){
				deLanguagePackEntry &entry = languagePack.GetEntryAt(entryIndex++);
				entry.SetName(pGetAttributeString(*tag, "name"));
				const decXmlCharacterData * const cdata = tag->GetFirstData();
				if(cdata){
					entry.SetText(decUnicodeString::NewFromUTF8(cdata->GetData()));
				}
				
			}else{
				LogWarnFormat("languagePack(%i:%i): Unknown Tag %s, ignoring",
					tag->GetLineNumber(), tag->GetPositionNumber(), tag->GetName().GetString());
			}
		}
	}
	
	// backwards compatibility
	if(languagePack.GetIdentifier().IsEmpty()){
		languagePack.SetIdentifier(languagePack.GetName().ToUTF8());
	}
}



void deLangPackModule::pWriteLangPack(decXmlWriter &writer, const deLanguagePack &languagePack){
	writer.WriteOpeningTag("languagePack", false, true);
	
	writer.WriteDataTagString("identifier", languagePack.GetIdentifier());
	writer.WriteDataTagString("name", languagePack.GetName().ToUTF8());
	writer.WriteDataTagString("description", languagePack.GetDescription().ToUTF8());
	writer.WriteDataTagString("missingText", languagePack.GetMissingText().ToUTF8());
	
	const int entryCount = languagePack.GetEntryCount();
	int i;
	
	for(i=0; i<entryCount; i++){
		pWriteLangPackEntry(writer, languagePack.GetEntryAt(i));
	}
	
	writer.WriteClosingTag("languagePack", true);
}

void deLangPackModule::pWriteLangPackEntry(decXmlWriter &writer, const deLanguagePackEntry &entry){
	writer.WriteOpeningTagStart("translation");
	writer.WriteAttributeString("name", entry.GetName());
	writer.WriteOpeningTagEnd(false, false);
	
	writer.WriteTextString(entry.GetText());
	
	writer.WriteClosingTag("translation", false);
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

#ifndef MODULE_VERSION
#include "module_version.h"
#endif

class delpModuleInternal : public deInternalModule{
public:
	delpModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("DELangPack");
		SetDescription("Handles language packs in the XML Drag[en]gine language pack format.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtLanguagePack);
		SetDirectoryName("delangpack");
		GetPatternList().Add(".delangpack");
		SetDefaultExtension(".delangpack");
		SetPriority(1);
		SetDefaultLoggingName();
	}
	
	void CreateModule() override{
		SetModule(DELangPackCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *delpRegisterInternalModule(deModuleSystem *system){
	return new delpModuleInternal(system);
}
#endif
