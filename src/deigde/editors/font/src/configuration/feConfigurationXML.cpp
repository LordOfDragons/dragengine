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

#include "feConfiguration.h"
#include "feConfigurationXML.h"
#include "../gui/feWindowMain.h"

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



// Class feConfigurationXML
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

feConfigurationXML::feConfigurationXML(deLogger *logger, const char *loggerSource) : igdeBaseXML(logger, loggerSource){
}

feConfigurationXML::~feConfigurationXML(){
}



// Management
///////////////

void feConfigurationXML::ReadFromFile(decBaseFileReader &reader, feConfiguration &config){
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	
	decXmlParser(GetLogger()).ParseXml(&reader, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if(!root || strcmp(root->GetName(), "fontEditor") != 0){
		DETHROW(deeInvalidParam);
	}
	
	pReadConfig(*root, config);
}

void feConfigurationXML::WriteToFile(decBaseFileWriter &writer, const feConfiguration &config){
	decXmlWriter xmlWriter(&writer);
	
	xmlWriter.WriteXMLDeclaration();
	
	pWriteConfig(xmlWriter, config);
}



// Private Functions
//////////////////////

void feConfigurationXML::pWriteConfig(decXmlWriter &writer, const feConfiguration &config){
	writer.WriteOpeningTag("fontEditor", false, true);
	
	config.GetWindowMain().GetRecentFiles().WriteToXml(writer);
	
	writer.WriteClosingTag("fontEditor", true);
}



void feConfigurationXML::pReadConfig(const decXmlElementTag &root, feConfiguration &config){
	const int count = root.GetElementCount();
	int i;
	
	for(i=0; i<count; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(tag->GetName() == "recentFiles"){
			config.GetWindowMain().GetRecentFiles().ReadFromXml(*tag);
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}
