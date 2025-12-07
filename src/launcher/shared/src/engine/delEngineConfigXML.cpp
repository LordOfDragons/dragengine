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

#include "delEngine.h"
#include "delEngineConfigXML.h"
#include "../delLauncher.h"
#include "../game/delGameManager.h"
#include "../game/profile/delGameProfile.h"
#include "../game/profile/delGPModule.h"
#include "../game/profile/delGPMParameter.h"
#include "../game/profile/delGPDisableModuleVersion.h"
#include "../game/profile/delGPDisableModuleVersionList.h"

#include <dragengine/logger/deLogger.h>
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



// Class delEngineConfigXML
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delEngineConfigXML::delEngineConfigXML(deLogger *logger, const char *loggerSource) :
delSharedConfigXML(logger, loggerSource){
}

delEngineConfigXML::~delEngineConfigXML(){
}



// Management
///////////////

void delEngineConfigXML::ReadFromFile(decBaseFileReader &reader, delLauncher &launcher){
	const decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	decXmlParser(launcher.GetLogger()).ParseXml(&reader, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if(! root || root->GetName() != "launcherConfig"){
		DETHROW_INFO(deeInvalidParam, "missing root tag 'launcherConfig'");
	}
	
	pReadConfig(*root, launcher);
}

void delEngineConfigXML::WriteToFile(decBaseFileWriter &writer, const delLauncher &launcher){
	decXmlWriter xmlWriter(&writer);
	xmlWriter.WriteXMLDeclaration();
	pWriteConfig(xmlWriter, launcher);
}



// Private Functions
//////////////////////

void delEngineConfigXML::pWriteConfig(decXmlWriter &writer, const delLauncher &launcher){
	const delGameManager &gameManager = launcher.GetGameManager();
	
	writer.WriteOpeningTag("launcherConfig", false, true);
	
	pWriteProfiles(writer, launcher);
	
	if(gameManager.GetActiveProfile()){
		writer.WriteDataTagString("activeProfile", gameManager.GetActiveProfile()->GetName());
	}
	
	writer.WriteClosingTag("launcherConfig", true);
}

void delEngineConfigXML::pWriteProfiles(decXmlWriter &writer, const delLauncher &launcher){
	const delGameManager &gameManager = launcher.GetGameManager();
	const delGameProfileList &profileList = gameManager.GetProfiles();
	const int count = profileList.GetCount();
	int i;
	
	writer.WriteOpeningTag("profiles", false, true);
	
	for(i=0; i<count; i++){
		WriteProfile(writer, *profileList.GetAt (i), "profile");
	}
	
	writer.WriteClosingTag("profiles", true);
}



void delEngineConfigXML::pReadConfig(const decXmlElementTag &root, delLauncher &launcher){
	delGameManager &gameManager = launcher.GetGameManager();
	const int count = root.GetElementCount();
	int i;
	
	for(i=0; i<count; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(! tag){
			continue;
		}
		
		if(tag->GetName() == "profiles"){
			pReadProfiles(*tag, launcher);
			
		}else if(tag->GetName() == "activeProfile"){
			const decString &profileName = GetCDataString(*tag);
			
			if(profileName.IsEmpty()){
				gameManager.SetActiveProfile(nullptr);
				
			}else{
				gameManager.SetActiveProfile(gameManager.GetProfiles().GetNamed(profileName));
				if(! gameManager.GetActiveProfile()){
					GetLogger()->LogWarnFormat(GetLoggerSource().GetString(),
						"%s(%i:%i): Profile '%s' does not exist", tag->GetName().GetString(),
						tag->GetLineNumber(), tag->GetPositionNumber(), profileName.GetString());
				}
			}
			
		}else{
			ErrorUnknownTag(root, *tag);
		}
	}
}

void delEngineConfigXML::pReadProfiles(const decXmlElementTag &root, delLauncher &launcher){
	delGameManager &gameManager = launcher.GetGameManager();
	const int count = root.GetElementCount();
	int i;
	
	for(i=0; i<count; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(! tag){
			continue;
		}
		
		if(tag->GetName() == "profile"){
			delGameProfile::Ref profile(delGameProfile::Ref::New(launcher.CreateGameProfile()));
			ReadProfile(*tag, profile);
			
			if(! profile->GetName().IsEmpty()
			&& ! gameManager.GetProfiles().HasNamed (profile->GetName())){
				gameManager.GetProfiles().Add (profile);
			}
			
		}else{
			ErrorUnknownTag(root, *tag);
		}
	}
}
