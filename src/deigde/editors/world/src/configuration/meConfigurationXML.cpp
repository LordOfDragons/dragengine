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

#include "meConfiguration.h"
#include "meConfigurationXML.h"
#include "../gui/meWindowMain.h"

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



// Class meConfigurationXML
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

meConfigurationXML::meConfigurationXML(deLogger *logger, const char *loggerSource) : igdeBaseXML(logger, loggerSource){
}

meConfigurationXML::~meConfigurationXML(){
}



// Management
///////////////

void meConfigurationXML::ReadFromFile(decBaseFileReader &reader, meConfiguration &config){
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	
	decXmlParser(GetLogger()).ParseXml(&reader, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if(! root || strcmp(root->GetName(), "worldEditor") != 0){
		DETHROW(deeInvalidParam);
	}
	
	pReadConfig(*root, config);
}

void meConfigurationXML::WriteToFile(decBaseFileWriter &writer, const meConfiguration &config){
	decXmlWriter xmlWriter(&writer);
	
	xmlWriter.WriteXMLDeclaration();
	
	pWriteConfig(xmlWriter, config);
}



// Private Functions
//////////////////////

void meConfigurationXML::pWriteConfig(decXmlWriter &writer, const meConfiguration &config){
	writer.WriteOpeningTag("worldEditor", false, true);
	
	writer.WriteDataTagFloat("moveStep", config.GetMoveStep());
	writer.WriteDataTagInt("moveSnap", config.GetMoveSnap() ? 1 : 0);
	writer.WriteDataTagFloat("rotateStep", config.GetRotateStep());
	writer.WriteDataTagInt("rotateSnap", config.GetRotateSnap() ? 1 : 0);
	writer.WriteDataTagFloat("scaleStep", config.GetScaleStep());
	writer.WriteDataTagInt("scaleSnap", config.GetScaleSnap() ? 1 : 0);
	writer.WriteDataTagFloat("sensitivity", config.GetSensitivity());
	writer.WriteDataTagBool("enableGI", config.GetEnableGI());
	writer.WriteDataTagBool("enableAuralization", config.GetEnableAuralization());
	config.GetWindowMain().GetRecentFiles().WriteToXml(writer);
	
	writer.WriteClosingTag("worldEditor", true);
}



void meConfigurationXML::pReadConfig(const decXmlElementTag &root, meConfiguration &config){
	const int count = root.GetElementCount();
	int i;
	
	config.GetWindowMain().GetRecentFiles().RemoveAllFiles();
	
	for(i=0; i<count; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(! tag){
			continue;
		}
		
		if(tag->GetName() == "moveStep"){
			config.SetMoveStep(GetCDataFloat(*tag));
			
		}else if(tag->GetName() == "moveSnap"){
			config.SetMoveSnap(GetCDataInt(*tag) != 0);
			
		}else if(tag->GetName() == "rotateStep"){
			config.SetRotateStep(GetCDataFloat(*tag));
			
		}else if(tag->GetName() == "rotateSnap"){
			config.SetRotateSnap(GetCDataInt(*tag) != 0);
			
		}else if(tag->GetName() == "scaleStep"){
			config.SetScaleStep(GetCDataFloat(*tag));
			
		}else if(tag->GetName() == "scaleSnap"){
			config.SetScaleSnap(GetCDataInt(*tag) != 0);
			
		}else if(tag->GetName() == "sensitivity"){
			config.SetSensitivity(GetCDataFloat(*tag));
			
		}else if(tag->GetName() == "enableGI"){
			config.SetEnableGI(GetCDataBool(*tag));
			
		}else if(tag->GetName() == "enableAuralization"){
			config.SetEnableAuralization(GetCDataBool(*tag));
			
		}else if(tag->GetName() == "recentFiles"){
			config.GetWindowMain().GetRecentFiles().ReadFromXml(*tag);
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}
