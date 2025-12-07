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

#include "delEngineModule.h"
#include "delEngineModuleXML.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/common/file/decPath.h>
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



// Class delEngineModuleXML
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delEngineModuleXML::delEngineModuleXML(deLogger *logger, const char *loggerSource) :
delBaseXML(logger, loggerSource){
}

delEngineModuleXML::~delEngineModuleXML(){
}



// Management
///////////////

void delEngineModuleXML::ReadFromFile(const char *filename, decBaseFileReader &reader, delEngineModule &module){
	const decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	decXmlParser(GetLogger()).ParseXml(&reader, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if(!root || root->GetName() != "module"){
		DETHROW_INFO(deeInvalidParam, "missing root tag 'module'");
	}
	
	pReadModule(*root, module);
	
	decPath basePath;
	basePath.SetFromUnix(filename);
	basePath.RemoveLastComponent(); // module.xml
	basePath.RemoveLastComponent(); // version
	module.SetDirectoryName(basePath.GetLastComponent());
}



// Private Functions
//////////////////////

void delEngineModuleXML::pReadModule(const decXmlElementTag &root, delEngineModule &module){
	const int count = root.GetElementCount();
	int i;
	
	for(i=0; i<count; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(tag->GetName() == "name"){
			module.SetName(GetCDataString(*tag));
			
		}else if(tag->GetName() == "description"){
			module.SetDescription(decUnicodeString::NewFromUTF8(GetCDataString(*tag)));
			
		}else if(tag->GetName() == "author"){
			module.SetAuthor(decUnicodeString::NewFromUTF8(GetCDataString(*tag)));
			
		}else if(tag->GetName() == "version"){
			module.SetVersion(GetCDataString(*tag));
			
		}else if(tag->GetName() == "type"){
			module.SetType(deModuleSystem::GetTypeFromString(GetCDataString(*tag)));
			
		}else if(tag->GetName() == "pattern"){
			decString pattern(module.GetPattern());
			if(pattern.IsEmpty()){
				pattern.AppendCharacter(',');
			}
			module.SetPattern(pattern + GetCDataString(*tag));
			
		}else if(tag->GetName() == "homepage"){
			// no interest in this tag
			
		}else if(tag->GetName() == "library"){
			pReadModuleLibrary(*tag, module);
			
		}else if(tag->GetName() == "data"){
			// deprecated
			
		}else if(tag->GetName() == "fallback"){
			module.SetIsFallback(true);
			
		}else if(tag->GetName() == "priority"){
			module.SetPriority(GetCDataInt(*tag));
			
		}else{
// 			ErrorUnknownTag( root, *tag );
		}
	}
}

void delEngineModuleXML::pReadModuleLibrary(const decXmlElementTag &root, delEngineModule &module){
	const int count = root.GetElementCount();
	int i;
	
	for(i=0; i<count; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(tag->GetName() == "file"){
			module.SetLibFileName(GetCDataString(*tag));
			
		}else if(tag->GetName() == "size"){
			module.SetLibFileSizeShould(GetCDataInt(*tag));
			
		}else if(tag->GetName() == "sha1"){
			module.SetLibFileHashShould(GetCDataString(*tag));
			
		}else if(tag->GetName() == "entrypoint"){
			module.SetLibFileEntryPoint(GetCDataString(*tag));
			
		}else if(tag->GetName() == "preloadLibrary"){
			// we do not care about this
			
		}else{
			ErrorUnknownTag(root, *tag);
		}
	}
}
