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

#include "deglConfigXML.h"
#include "deglConfigWindow.h"
#include "deglConfiguration.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>


// Class deglConfigXML
////////////////////////

// Constructors and Destructors
/////////////////////////////////

deglConfigXML::deglConfigXML(deLogger *logger, const char *loggerSource) :
delBaseXML(logger, loggerSource){
}

deglConfigXML::~deglConfigXML(){
}



// Management
///////////////

void deglConfigXML::ReadFromFile(decBaseFileReader &reader, deglConfiguration &config){
	const decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	decXmlParser(GetLogger()).ParseXml(&reader, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	DEASSERT_NOTNULL(root)
	DEASSERT_TRUE(root->GetName() == "delaunchergui")
	
	pReadConfig(*root, config);
}

void deglConfigXML::WriteToFile( decBaseFileWriter &writer, const deglConfiguration &config ){
	decXmlWriter xmlWriter( &writer );
	xmlWriter.WriteXMLDeclaration();
	pWriteConfig( xmlWriter, config );
}



// Private Functions
//////////////////////

void deglConfigXML::pWriteConfig(decXmlWriter &writer, const deglConfiguration &config){
	writer.WriteOpeningTag("delaunchergui", false, true);
	
	pWriteWindow(writer, config.GetWindowMain(), "windowMain");
	
	writer.WriteClosingTag("delaunchergui", true);
}

void deglConfigXML::pWriteWindow(decXmlWriter &writer, const deglConfigWindow &window, const char *tagName){
	writer.WriteOpeningTag(tagName, false, true);
	
	// NOTE windows has a tendency to somehow produce -32000 as coordinates. if this is
	//      saved the user is no more able to get the window back visible. prevent this
	//      from happening.
#ifdef OS_W32
	writer.WriteDataTagInt("x", decMath::max(window.GetX(), 0));
	writer.WriteDataTagInt("y", decMath::max(window.GetY(), 10));
#else
	writer.WriteDataTagInt("x", window.GetX());
	writer.WriteDataTagInt("y", window.GetY());
#endif
	
	writer.WriteDataTagInt("width", window.GetWidth());
	writer.WriteDataTagInt("height", window.GetHeight());
	
	writer.WriteClosingTag(tagName, true);
}


void deglConfigXML::pReadConfig(const decXmlElementTag &root, deglConfiguration &config){
	const int count = root.GetElementCount();
	int i;
	
	for(i=0; i<count; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(tag->GetName() == "windowMain"){
			pReadWindow(*tag, config.GetWindowMain());
			
		}else{
			ErrorUnknownTag(root, *tag);
		}
	}
}

void deglConfigXML::pReadWindow(const decXmlElementTag &root, deglConfigWindow &window){
	const int count = root.GetElementCount();
	int i;
	
	for(i=0; i<count; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(tag->GetName() == "x"){
			window.SetX(GetCDataInt(*tag));
			
		}else if(tag->GetName() == "y"){
			window.SetY(GetCDataInt(*tag));
			
		}else if(tag->GetName() == "width"){
			window.SetWidth(GetCDataInt(*tag));
			
		}else if(tag->GetName() == "height"){
			window.SetHeight(GetCDataInt(*tag));
			
		}else{
			ErrorUnknownTag(root, *tag);
		}
	}
}
