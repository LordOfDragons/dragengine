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

#include "projConfiguration.h"
#include "projConfigurationXml.h"
#include "../gui/projWindowMain.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>


// Class projConfigurationXml
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

projConfigurationXml::projConfigurationXml(deLogger *logger, const char *loggerSource) :
igdeBaseXML(logger, loggerSource){
}

projConfigurationXml::~projConfigurationXml(){
}


// Management
///////////////

void projConfigurationXml::ReadFromFile(decBaseFileReader &reader, projConfiguration &config){
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	
	decXmlParser(GetLogger()).ParseXml(&reader, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	DEASSERT_NOTNULL(root)
	DEASSERT_TRUE(root->GetName() == "projectEditor")
	
	pReadConfig(*root, config);
}

void projConfigurationXml::WriteToFile(decBaseFileWriter &writer, const projConfiguration &config){
	decXmlWriter xmlWriter(&writer);
	
	xmlWriter.WriteXMLDeclaration();
	
	pWriteConfig(xmlWriter, config);
}


// Private Functions
//////////////////////

void projConfigurationXml::pWriteConfig(decXmlWriter &writer, const projConfiguration &config){
	writer.WriteOpeningTag("projectEditor", false, true);
	
	pWriteRemoteLauncher(writer, config);
	
	writer.WriteClosingTag( "projectEditor", true );
}

void projConfigurationXml::pWriteRemoteLauncher(decXmlWriter &writer,
const projConfiguration &config){
	writer.WriteOpeningTag("remoteLauncher");
	
	writer.WriteDataTagString("address", config.GetRemoteAddress());
	
	writer.WriteClosingTag("remoteLauncher");
}


void projConfigurationXml::pReadConfig(const decXmlElementTag &root, projConfiguration &config){
	const int count = root.GetElementCount();
	int i;
	
	for(i=0; i<count; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(tag->GetName() == "remoteLauncher" ){
			pReadRemoteLauncher(*tag, config);
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}

void projConfigurationXml::pReadRemoteLauncher(const decXmlElementTag &root,
projConfiguration &config){
	const int count = root.GetElementCount();
	int i;
	
	for(i=0; i<count; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(tag->GetName() == "address"){
			config.SetRemoteAddress(GetCDataString(*tag));
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}
