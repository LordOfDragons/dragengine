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

#include "delGame.h"
#include "delGameXML.h"
#include "icon/delGameIcon.h"
#include "fileformat/delFileFormat.h"
#include "fileformat/delFileFormatList.h"
#include "../delLauncher.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/exceptions.h>



// Class delGameXML
//////////////////////

// Constructors and Destructors
/////////////////////////////////

delGameXML::delGameXML(deLogger *logger, const char *loggerSource) :
delBaseXML(logger, loggerSource){
}

delGameXML::~delGameXML(){
}



// Management
///////////////

void delGameXML::ReadFromFile(decBaseFileReader &reader, delGame &game){
	const decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	decXmlParser(GetLogger()).ParseXml(&reader, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if(! root || root->GetName() != "degame"){
		DETHROW_INFO(deeInvalidParam, "missing root tag 'degame'");
	}
	
	pReadGame(*root, game);
}



// Private Functions
//////////////////////

void delGameXML::pReadGame(const decXmlElementTag &root, delGame &game){
	delFileFormatList &fileFormats = game.GetFileFormats();
	const int count = root.GetElementCount();
	int i;
	
	for(i=0; i<count; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(! tag){
			continue;
		}
		
		if(tag->GetName() == "identifier"){
			game.SetIdentifier(decUuid(GetCDataString(*tag), false));
			
		}else if(tag->GetName() == "aliasIdentifier"){
			game.SetAliasIdentifier(GetCDataString(*tag));
			
		}else if(tag->GetName() == "title"){
			game.SetTitle(decUnicodeString::NewFromUTF8(GetCDataString(*tag)));
			
		}else if(tag->GetName() == "subTitle"){
			game.SetTitle(game.GetTitle() + decUnicodeString::NewFromUTF8(" - ")
				+ decUnicodeString::NewFromUTF8(GetCDataString(*tag)));
			
		}else if(tag->GetName() == "description"){
			game.SetDescription(decUnicodeString::NewFromUTF8(GetCDataString(*tag)));
			
		}else if(tag->GetName() == "icon"){
			game.GetIcons().Add(delGameIcon::Ref::New(game.GetLauncher().CreateGameIcon(
				GetAttributeInt(*tag, "size"), GetCDataString(*tag))));
			
		}else if(tag->GetName() == "creator"){
			game.SetCreator(decUnicodeString::NewFromUTF8(GetCDataString(*tag)));
			
		}else if(tag->GetName() == "homepage"){
			game.SetHomepage(GetCDataString(*tag));
			
		}else if(tag->GetName() == "license"){
			// deprecated
			
		}else if(tag->GetName() == "gameDirectory"){
			game.SetGameDirectory(GetCDataString(*tag));
			
		}else if(tag->GetName() == "dataDirectory"){
			game.SetDataDirectory(GetCDataString(*tag));
			
		}else if(tag->GetName() == "scriptDirectory"){
			game.SetScriptDirectory(GetCDataString(*tag));
			
		}else if(tag->GetName() == "gameObject"){
			game.SetGameObject(GetCDataString(*tag));
			
		}else if(tag->GetName() == "pathConfig"){
			game.SetPathConfig(GetCDataString(*tag));
			
		}else if(tag->GetName() == "pathCapture"){
			game.SetPathCapture(GetCDataString(*tag));
			
		}else if(tag->GetName() == "scriptModule"){
			game.SetScriptModule(GetCDataString(*tag));
			if(HasAttribute(*tag, "version")){
				game.SetScriptModuleVersion(GetAttributeString(*tag, "version"));
			}
			
		}else if(tag->GetName() == "windowSize"){
			game.SetWindowSize(decPoint(GetAttributeInt(*tag, "x"), GetAttributeInt(*tag, "y")));
			
		}else if(tag->GetName() == "requireFormat"){
			const decString typeName(GetAttributeString(*tag, "type"));
			const deModuleSystem::eModuleTypes formatType = deModuleSystem::GetTypeFromString(typeName);
			if(formatType == deModuleSystem::emtUnknown){
				ErrorUnknownValue(*tag, typeName);
				DETHROW_INFO(deeInvalidParam, "invalid tag value");
			}
			
			fileFormats.Add (delFileFormat::Ref::NewWith(formatType, GetCDataString(*tag)));
			
		}else{
			ErrorUnknownTag(root, *tag);
		}
	}
	
	if(! game.GetIdentifier()){
		ErrorMissingTag(root, "identifier");
		DETHROW_INFO(deeInvalidParam, "missing tag");
	}
}
