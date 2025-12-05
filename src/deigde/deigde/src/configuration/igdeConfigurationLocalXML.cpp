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

#include "igdeConfigurationLocal.h"
#include "igdeConfigurationLocalXML.h"
#include "../gui/igdeWindowMain.h"
#include "../environment/igdeEnvironmentIGDE.h"
#include "../module/igdeEditorModuleDefinition.h"
#include "../module/igdeEditorModuleManager.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
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


// Class igdeConfigurationLocalXML
////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

igdeConfigurationLocalXML::igdeConfigurationLocalXML(deLogger *logger, const char *loggerSource) :
igdeBaseXML(logger, loggerSource){
}

igdeConfigurationLocalXML::~igdeConfigurationLocalXML(){
}


// Management
///////////////

void igdeConfigurationLocalXML::ReadFromFile(decBaseFileReader &reader, igdeConfigurationLocal &config){
	const decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	
	decXmlParser parser(GetLogger());
	parser.ParseXml(&reader, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	DEASSERT_NOTNULL(root)
	DEASSERT_TRUE(root->GetName() == "deigde")
	
	pReadConfig(*root, config);
}

void igdeConfigurationLocalXML::WriteToFile(decBaseFileWriter &writer, const igdeConfigurationLocal &config){
	decXmlWriter xmlWriter(&writer);
	xmlWriter.WriteXMLDeclaration();
	pWriteConfig(xmlWriter, config);
}


// Private Functions
//////////////////////

void igdeConfigurationLocalXML::pReadConfig(const decXmlElementTag &root, igdeConfigurationLocal &config){
	const int count = root.GetElementCount();
	int i;
	
	for(i=0; i<count; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(tag->GetName() == "recentEditorFiles"){
			config.GetRecentEditorFiles().ReadFromXml(*tag);
			
		}else if(tag->GetName() == "recentEditors"){
			pReadRecentEditors(*tag, config);
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}

void igdeConfigurationLocalXML::pReadRecentEditors(const decXmlElementTag &root, igdeConfigurationLocal &config){
	igdeEditorModuleManager &moduleManger = config.GetWindowMain().GetModuleManager();
	const int count = root.GetElementCount();
	int i, position = 0;
	
	for(i=0; i<count; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(tag->GetName() == "editor"){
			igdeEditorModuleDefinition * const module = moduleManger.GetModuleWithID(GetCDataString(*tag));
			if(module){
				moduleManger.ChangeModuleRecentUsedPosition(module, position++);
			}
		}
	}
}


void igdeConfigurationLocalXML::pWriteConfig(decXmlWriter &writer, const igdeConfigurationLocal &config){
	writer.WriteOpeningTag("deigde", false, true);
	
	config.GetRecentEditorFiles().WriteToXml(writer, "recentEditorFiles");
	pWriteRecentEditors(writer, config);
	
	writer.WriteClosingTag("deigde", true);
}

void igdeConfigurationLocalXML::pWriteRecentEditors(decXmlWriter &writer, const igdeConfigurationLocal &config){
	igdeEditorModuleManager &moduleManger = config.GetWindowMain().GetModuleManager();
	const int count = moduleManger.GetModuleCount();
	int i;
	
	writer.WriteOpeningTag("recentEditors");
	
	for(i=0; i<count; i++){
		writer.WriteDataTagString("editor", moduleManger.GetRecentModuleAt(i)->GetID());
	}
	
	writer.WriteClosingTag("recentEditors");
}
