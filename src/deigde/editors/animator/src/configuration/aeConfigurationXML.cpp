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
#include <ctype.h>

#include "aeConfiguration.h"
#include "aeConfigurationXML.h"
#include "../gui/aeWindowMain.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/exceptions.h>



// Class aeConfigurationXML
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

aeConfigurationXML::aeConfigurationXML(deLogger *logger, const char *loggerSource) : igdeBaseXML(logger, loggerSource){
}

aeConfigurationXML::~aeConfigurationXML(){
}



// Management
///////////////

void aeConfigurationXML::ReadFromFile(decBaseFileReader &reader, aeConfiguration &config){
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	
	decXmlParser(GetLogger()).ParseXml(&reader, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if(!root || strcmp(root->GetName(), "animatorEditor") != 0){
		DETHROW(deeInvalidParam);
	}
	
	pReadConfig(*root, config);
}

void aeConfigurationXML::WriteToFile(decBaseFileWriter &writer, const aeConfiguration &config){
	decXmlWriter xmlWriter(&writer);
	
	xmlWriter.WriteXMLDeclaration();
	
	pWriteConfig(xmlWriter, config);
}



// Private Functions
//////////////////////

void aeConfigurationXML::pWriteConfig(decXmlWriter &writer, const aeConfiguration &config){
	writer.WriteOpeningTag("animatorEditor", false, true);
	
	config.GetWindowMain().GetRecentFiles().WriteToXml(writer);
	
	pWriteKey(writer, "locoKeyForward", config.GetLocoKeyForward());
	pWriteKey(writer, "locoKeyBackwards", config.GetLocoKeyBackwards());
	pWriteKey(writer, "locoKeyLeft", config.GetLocoKeyLeft());
	pWriteKey(writer, "locoKeyRight", config.GetLocoKeyRight());
	pWriteKey(writer, "locoKeyCrouch", config.GetLocoKeyCrouch());
	pWriteKey(writer, "locoKeyRun", config.GetLocoKeyRun());
	
	writer.WriteClosingTag("animatorEditor", true);
}

void aeConfigurationXML::pWriteKey(decXmlWriter &writer, const char *name, deInputEvent::eKeyCodes key){
	if(key < deInputEvent::ekcA || key > deInputEvent::ekcZ){
		return;
	}
	
	const char value[2] = {(char)('a' + (key - deInputEvent::ekcA)), 0};
	writer.WriteDataTagString(name, value);
}



void aeConfigurationXML::pReadConfig(const decXmlElementTag &root, aeConfiguration &config){
	const int count = root.GetElementCount();
	int i;
	
	for(i=0; i<count; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(tag->GetName() == "recentFiles"){
			config.GetWindowMain().GetRecentFiles().ReadFromXml(*tag);
			
		}else if(tag->GetName() == "locoKeyForward"){
			config.SetLocoKeyForward(pReadKey(*tag, config.GetLocoKeyForward()));
			
		}else if(tag->GetName() == "locoKeyBackwards"){
			config.SetLocoKeyBackwards(pReadKey(*tag, config.GetLocoKeyBackwards()));
			
		}else if(tag->GetName() == "locoKeyLeft"){
			config.SetLocoKeyLeft(pReadKey(*tag, config.GetLocoKeyLeft()));
			
		}else if(tag->GetName() == "locoKeyRight"){
			config.SetLocoKeyRight(pReadKey(*tag, config.GetLocoKeyRight()));
			
		}else if(tag->GetName() == "locoKeyCrouch"){
			config.SetLocoKeyCrouch(pReadKey(*tag, config.GetLocoKeyCrouch()));
			
		}else if(tag->GetName() == "locoKeyRun"){
			config.SetLocoKeyRun(pReadKey(*tag, config.GetLocoKeyRun()));
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}

deInputEvent::eKeyCodes aeConfigurationXML::pReadKey(const decXmlElementTag &root,
deInputEvent::eKeyCodes defaultKey){
	const char * const code = GetCDataString(root);
	if(!code || strlen(code) != 1){
		return defaultKey;
	}
	
	int code2 = tolower(code[0]);
	if(code2 < 'a' || code2 > 'z'){
		return defaultKey;
	}
	
	return (deInputEvent::eKeyCodes)(deInputEvent::ekcA + (code2 - 'a'));
}
