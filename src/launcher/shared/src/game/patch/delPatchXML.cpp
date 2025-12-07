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

#include "delPatch.h"
#include "delPatchXML.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>



// Class delPatchXML
//////////////////////

// Constructors and Destructors
/////////////////////////////////

delPatchXML::delPatchXML(deLogger *logger, const char *loggerSource) :
delBaseXML(logger, loggerSource){
}

delPatchXML::~delPatchXML(){
}



// Management
///////////////

void delPatchXML::ReadFromFile(decBaseFileReader &reader, delPatch &patch){
	const decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	decXmlParser(GetLogger()).ParseXml(&reader, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if(! root || root->GetName() != "depatch"){
		DETHROW_INFO(deeInvalidParam, "missing root tag 'depatch'");
	}
	
	pReadPatch(*root, patch);
}



// Private Functions
//////////////////////

void delPatchXML::pReadPatch(const decXmlElementTag &root, delPatch &patch){
	const int count = root.GetElementCount();
	int i;
	
	for(i=0; i<count; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(! tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "identifier"){
			patch.SetIdentifier(decUuid(GetCDataString(*tag), false));
			
		}else if(tagName == "name"){
			patch.SetName(decUnicodeString::NewFromUTF8(GetCDataString(*tag)));
			
		}else if(tagName == "description"){
			patch.SetDescription(decUnicodeString::NewFromUTF8(GetCDataString(*tag)));
			
		}else if(tagName == "creator"){
			patch.SetCreator(decUnicodeString::NewFromUTF8(GetCDataString(*tag)));
			
		}else if(tagName == "homepage"){
			patch.SetHomepage(GetCDataString(*tag));
			
		}else if(tagName == "patchDir"){
			patch.SetPatchDirectory(GetCDataString(*tag));
			
		}else if(tagName == "dataDir"){
			patch.SetDataDirectory(GetCDataString(*tag));
			
		}else if(tagName == "gameId"){
			patch.SetGameID(decUuid(GetCDataString(*tag), false));
			
		}else if(tagName == "requiredPatch"){
			patch.GetRequiredPatches().Add(decUuid(GetCDataString(*tag), false));
			
		}else if(tagName == "hiddenPath"){
			patch.GetHiddenPath().Add(GetCDataString(*tag));
			
		}else{
			ErrorUnknownTag(root, *tag);
		}
	}
	
	if(! patch.GetIdentifier()){
		ErrorMissingTag(root, "identifier");
		DETHROW_INFO(deeInvalidParam, "missing tag");
	}
	if(! patch.GetGameID()){
		ErrorMissingTag(root, "gameId");
		DETHROW_INFO(deeInvalidParam, "missing tag");
	}
}
