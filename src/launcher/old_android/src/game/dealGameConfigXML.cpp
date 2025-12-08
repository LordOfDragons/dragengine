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

#include "dealGame.h"
#include "dealGameManager.h"
#include "dealGameConfigXML.h"
#include "profile/dealGameProfile.h"
#include "profile/dealGPModule.h"
#include "profile/dealGPMParameter.h"
#include "profile/dealGPDisableModuleVersion.h"
#include "profile/dealGPDisableModuleVersionList.h"

#include "../logger/deLogger.h"
#include "../common/file/decBaseFileReader.h"
#include "../common/file/decBaseFileWriter.h"
#include "../common/xmlparser/decXmlWriter.h"
#include "../common/xmlparser/decXmlDocument.h"
#include "../common/xmlparser/decXmlCharacterData.h"
#include "../common/xmlparser/decXmlElementTag.h"
#include "../common/xmlparser/decXmlAttValue.h"
#include "../common/xmlparser/decXmlVisitor.h"
#include "../common/xmlparser/decXmlParser.h"
#include "../common/exceptions.h"



// Class dealGameConfigXML
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dealGameConfigXML::dealGameConfigXML(deLogger* logger, const char* loggingSource,
dealGameManager &gameManager) :
dealBaseXML(logger, loggingSource),
pGameManager(gameManager){
}

dealGameConfigXML::~dealGameConfigXML(){
}



// Management
///////////////

void dealGameConfigXML::ReadFromFile(decBaseFileReader &reader, dealGame &game){
	decXmlDocument::Ref xmldoc((decXmlDocument::Ref::NewWith()));
	
	decXmlParser parser(GetLogger());
	
	parser.ParseXml(&reader, xmldoc);
	
	xmldoc->StripComments();
	xmldoc->CleanCharData();
	
	decXmlElementTag * const root = xmldoc->GetRoot();
	if(!root || strcmp(root->GetName(), "gameConfig") != 0) DETHROW(deeInvalidParam);
	
	pReadConfig(*root, game);
}

void dealGameConfigXML::WriteToFile(decBaseFileWriter &writer, const dealGame &game){
	decXmlWriter xmlWriter(&writer);
	xmlWriter.WriteXMLDeclaration();
	pWriteConfig(xmlWriter, game);
}



// Private Functions
//////////////////////

void dealGameConfigXML::pWriteConfig(decXmlWriter &writer, const dealGame &game){
	writer.WriteOpeningTag("gameConfig", false, true);
	
	pWriteProfiles(writer, game);
	
	if(game.GetGlobalProfile()){
		writer.WriteDataTagString("globalProfile", game.GetGlobalProfile()->GetName());
		
	}else{
		writer.WriteDataTagString("globalProfile", "");
	}
	
	if(game.GetGameProfile()){
		writer.WriteDataTagString("gameProfile", game.GetGameProfile()->GetName());
		
	}else{
		writer.WriteDataTagString("gameProfile", "");
	}
	
	writer.WriteDataTagString("runArguments", game.GetRunArguments());
	
	writer.WriteClosingTag("gameConfig", true);
}

void dealGameConfigXML::pWriteProfiles(decXmlWriter &writer, const dealGame &game){
	const dealGameProfileList &profileList = game.GetProfileList();
	int i, count = profileList.GetCount();
	
	writer.WriteOpeningTag("profiles", false, true);
	
	for(i=0; i<count; i++){
		pWriteProfile(writer, *profileList.GetAt(i));
	}
	
	writer.WriteClosingTag("profiles", true);
}

void dealGameConfigXML::pWriteProfile(decXmlWriter &writer, const dealGameProfile &profile){
	writer.WriteOpeningTagStart("profile");
	writer.WriteAttributeString("name", profile.GetName());
	writer.WriteOpeningTagEnd();
	
	pWriteProfileSystems(writer, profile);
	pWriteProfileDisableModuleVersions(writer, profile);
	pWriteProfileModules(writer, profile);
	
	writer.WriteDataTagString("runArguments", profile.GetRunArguments());
	writer.WriteDataTagInt("replaceRunArguments", profile.GetReplaceRunArguments() ? 1 : 0);
	pWriteProfileWindow(writer, profile);
	
	writer.WriteClosingTag("profile", true);
}

void dealGameConfigXML::pWriteProfileSystems(decXmlWriter &writer, const dealGameProfile &profile){
	writer.WriteOpeningTag("systems", false, true);
	
	writer.WriteDataTagString("graphic", profile.GetModuleGraphic());
	if(!profile.GetModuleGraphicVersion().IsEmpty()){
		writer.WriteDataTagString("graphicVersion", profile.GetModuleGraphicVersion());
	}
	
	writer.WriteDataTagString("input", profile.GetModuleInput());
	if(!profile.GetModuleInputVersion().IsEmpty()){
		writer.WriteDataTagString("inputVersion", profile.GetModuleInputVersion());
	}
	
	writer.WriteDataTagString("physics", profile.GetModulePhysics());
	if(!profile.GetModulePhysicsVersion().IsEmpty()){
		writer.WriteDataTagString("physicsVersion", profile.GetModulePhysicsVersion());
	}
	
	writer.WriteDataTagString("animator", profile.GetModuleAnimator());
	if(!profile.GetModuleAnimatorVersion().IsEmpty()){
		writer.WriteDataTagString("animatorVersion", profile.GetModuleAnimatorVersion());
	}
	
	writer.WriteDataTagString("ai", profile.GetModuleAI());
	if(!profile.GetModuleAIVersion().IsEmpty()){
		writer.WriteDataTagString("aiVersion", profile.GetModuleAIVersion());
	}
	
	writer.WriteDataTagString("crashRecovery", profile.GetModuleCrashRecovery());
	if(!profile.GetModuleCrashRecoveryVersion().IsEmpty()){
		writer.WriteDataTagString("crashRecoveryVersion", profile.GetModuleCrashRecoveryVersion());
	}
	
	writer.WriteDataTagString("audio", profile.GetModuleAudio());
	if(!profile.GetModuleAudioVersion().IsEmpty()){
		writer.WriteDataTagString("audioVersion", profile.GetModuleAudioVersion());
	}
	
	writer.WriteDataTagString("synthesizer", profile.GetModuleSynthesizer());
	if(!profile.GetModuleSynthesizerVersion().IsEmpty()){
		writer.WriteDataTagString("synthesizerVersion", profile.GetModuleSynthesizerVersion());
	}
	
	writer.WriteDataTagString("network", profile.GetModuleNetwork());
	if(!profile.GetModuleNetworkVersion().IsEmpty()){
		writer.WriteDataTagString("networkVersion", profile.GetModuleNetworkVersion());
	}
	
	writer.WriteClosingTag("systems", true);
}

void dealGameConfigXML::pWriteProfileDisableModuleVersions(decXmlWriter &writer, const dealGameProfile &profile){
	const dealGPDisableModuleVersionList &list = profile.GetDisableModuleVersionList();
	const int count = list.GetCount();
	int i;
	
	if(count == 0){
		return;
	}
	
	writer.WriteOpeningTag("disableModuleVersions", false, true);
	
	for(i=0; i<count; i++){
		const dealGPDisableModuleVersion &version = *list.GetAt(i);
		writer.WriteOpeningTagStart("disableModuleVersion");
		writer.WriteAttributeString("name", version.GetName());
		writer.WriteAttributeString("version", version.GetVersion());
		writer.WriteOpeningTagEnd(true);
	}
	
	writer.WriteClosingTag("disableModuleVersions", true);
}

void dealGameConfigXML::pWriteProfileModules(decXmlWriter &writer, const dealGameProfile &profile){
	const dealGPModuleList &moduleList = profile.GetModuleList();
	int i, count = moduleList.GetModuleCount();
	
	if(count == 0){
		return;
	}
	
	writer.WriteOpeningTag("modules", false, true);
	
	for(i=0; i<count; i++){
		pWriteProfileModule(writer, *moduleList.GetModuleAt(i));
	}
	
	writer.WriteClosingTag("modules", true);
}

void dealGameConfigXML::pWriteProfileModule(decXmlWriter &writer, const dealGPModule &module){
	writer.WriteOpeningTagStart("module");
	writer.WriteAttributeString("name", module.GetName());
	writer.WriteOpeningTagEnd();
	
	pWriteProfileModuleParameters(writer, module);
	
	writer.WriteClosingTag("module", true);
}

void dealGameConfigXML::pWriteProfileModuleParameters(decXmlWriter &writer, const dealGPModule &module){
	const dealGPMParameterList &parametersList = module.GetParameterList();
	int i, count = parametersList.GetParameterCount();
	
	writer.WriteOpeningTag("parameters", false, true);
	
	for(i=0; i<count; i++){
		const dealGPMParameter &parameters = *parametersList.GetParameterAt(i);
		
		writer.WriteOpeningTagStart("parameter");
		writer.WriteAttributeString("name", parameters.GetName());
		writer.WriteOpeningTagEnd(false, false);
		writer.WriteTextString(parameters.GetValue());
		writer.WriteClosingTag("parameter", false);
	}
	
	writer.WriteClosingTag("parameters", true);
}

void dealGameConfigXML::pWriteProfileWindow(decXmlWriter &writer, const dealGameProfile &profile){
	writer.WriteOpeningTag("window", false, true);
	
	writer.WriteDataTagInt("width", profile.GetWidth());
	writer.WriteDataTagInt("height", profile.GetHeight());
	writer.WriteDataTagInt("fullScreen", profile.GetFullScreen() ? 1 : 0);
	
	writer.WriteClosingTag("window", true);
}



void dealGameConfigXML::pReadConfig(const decXmlElementTag &root, dealGame &game){
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	const char *profileName;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "profiles") == 0){
				pReadProfiles(*tag, game);
				
			}else if(strcmp(tag->GetName(), "globalProfile") == 0){
				profileName = pGetCDataString(*tag);
				
				if(strlen(profileName) == 0){
					game.SetGlobalProfile(NULL);
					
				}else{
					game.SetGlobalProfile(pGameManager.GetProfileList().GetNamed(profileName));
					if(!game.GetGlobalProfile()){
						GetLogger()->LogWarnFormat(GetLoggerSource(),
							"%s(%i:%i): Global profile '%s' does not exist",
							tag->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), profileName);
					}
				}
				
			}else if(strcmp(tag->GetName(), "gameProfile") == 0){
				profileName = pGetCDataString(*tag);
				
				if(strlen(profileName) == 0){
					game.SetGameProfile(NULL);
					
				}else{
					game.SetGameProfile(game.GetProfileList().GetNamed(profileName));
					if(!game.GetGameProfile()){
						GetLogger()->LogWarnFormat(GetLoggerSource(),
							"%s(%i:%i): Game profile '%s' does not exist",
							tag->GetName().GetString(), tag->GetLineNumber(),
							tag->GetPositionNumber(), profileName);
					}
				}
				
			}else if(strcmp(tag->GetName(), "runArguments") == 0){
				game.SetRunArguments(pGetCDataString(*tag));
				
			}else{
				pErrorUnknownTag(root, *tag);
			}
		}
	}
}

void dealGameConfigXML::pReadProfiles(const decXmlElementTag &root, dealGame &game){
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "profile") == 0){
				pReadProfile(*tag, game);
				
			}else{
				pErrorUnknownTag(root, *tag);
			}
		}
	}
}

void dealGameConfigXML::pReadProfile(const decXmlElementTag &root, dealGame &game){
	int e, elementCount = root.GetElementCount();
	dealGameProfile *profile = NULL;
	const decXmlElementTag *tag;
	decString profileName;
	
	profileName = pGetAttributeString(root, "name");
	
	try{
		profile = new dealGameProfile;
		if(!profile) DETHROW(deeOutOfMemory);
		
		profile->SetName(profileName);
		
		for(e=0; e<elementCount; e++){
			tag = root.GetElementIfTag(e);
			
			if(tag){
				if(strcmp(tag->GetName(), "systems") == 0){
					pReadProfileSystems(*tag, *profile);
					
				}else if(strcmp(tag->GetName(), "disableModuleVersions") == 0){
					pReadProfileDisableModuleVersions(*tag, *profile);
					
				}else if(strcmp(tag->GetName(), "modules") == 0){
					pReadProfileModules(*tag, *profile);
					
				}else if(strcmp(tag->GetName(), "runArguments") == 0){
					profile->SetRunArguments(pGetCDataString(*tag));
					
				}else if(strcmp(tag->GetName(), "replaceRunArguments") == 0){
					profile->SetReplaceRunArguments(pGetCDataInt(*tag) != 0);
					
				}else if(strcmp(tag->GetName(), "window") == 0){
					pReadProfileWindow(*tag, *profile);
					
				}else{
					pErrorUnknownTag(root, *tag);
				}
			}
		}
		
		game.GetProfileList().Add(profile);
		
	}catch(const deException &){
		if(profile) profile->FreeReference();;
		throw;
	}
}

void dealGameConfigXML::pReadProfileSystems(const decXmlElementTag &root, dealGameProfile &profile){
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "graphic") == 0){
				profile.SetModuleGraphic(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "graphicVersion") == 0){
				profile.SetModuleGraphicVersion(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "input") == 0){
				profile.SetModuleInput(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "inputVersion") == 0){
				profile.SetModuleInputVersion(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "physics") == 0){
				profile.SetModulePhysics(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "physicsVersion") == 0){
				profile.SetModulePhysicsVersion(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "animator") == 0){
				profile.SetModuleAnimator(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "animatorVersion") == 0){
				profile.SetModuleAnimatorVersion(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "ai") == 0){
				profile.SetModuleAI(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "aiVersion") == 0){
				profile.SetModuleAIVersion(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "crashRecovery") == 0){
				profile.SetModuleCrashRecovery(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "crashRecoveryVersion") == 0){
				profile.SetModuleCrashRecoveryVersion(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "audio") == 0){
				profile.SetModuleAudio(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "audioVersion") == 0){
				profile.SetModuleAudioVersion(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "network") == 0){
				profile.SetModuleNetwork(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "synthesizer") == 0){
				profile.SetModuleSynthesizer(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "networkVersion") == 0){
				profile.SetModuleNetworkVersion(pGetCDataString(*tag));
				
			}else{
				pErrorUnknownTag(root, *tag);
			}
		}
	}
}

void dealGameConfigXML::pReadProfileDisableModuleVersions(const decXmlElementTag &root, dealGameProfile &profile){
	const int elementCount = root.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		
		if(tag){
			if(strcmp(tag->GetName(), "disableModuleVersion") == 0){
				dealGPDisableModuleVersion *entry = NULL;
				const char * const name = pGetAttributeString(*tag, "name");
				const char * const version = pGetAttributeString(*tag, "version");
				
				try{
					entry = new dealGPDisableModuleVersion(name, version);
					profile.GetDisableModuleVersionList().Add(entry);
					entry->FreeReference();
					
				}catch(const deException &){
					if(entry){
						entry->FreeReference();
					}
					throw;
				}
				
			}else{
				pErrorUnknownTag(root, *tag);
			}
		}
	}
}

void dealGameConfigXML::pReadProfileModules(const decXmlElementTag &root, dealGameProfile &profile){
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "module") == 0){
				pReadProfileModule(*tag, profile);
				
			}else{
				pErrorUnknownTag(root, *tag);
			}
		}
	}
}

void dealGameConfigXML::pReadProfileModule(const decXmlElementTag &root, dealGameProfile &profile){
	int e, elementCount = root.GetElementCount();
	dealGPModule *module = NULL;
	const decXmlElementTag *tag;
	decString moduleName;
	
	moduleName = pGetAttributeString(root, "name");
	
	try{
		module = new dealGPModule;
		if(!module) DETHROW(deeOutOfMemory);
		
		module->SetName(moduleName);
		
		for(e=0; e<elementCount; e++){
			tag = root.GetElementIfTag(e);
			
			if(tag){
				if(strcmp(tag->GetName(), "parameters") == 0){
					pReadProfileModuleParameters(*tag, *module);
					
				}else{
					pErrorUnknownTag(root, *tag);
				}
			}
		}
		
		profile.GetModuleList().AddModule(module);
		
	}catch(const deException &){
		if(module) module->FreeReference();;
		throw;
	}
}

void dealGameConfigXML::pReadProfileModuleParameters(const decXmlElementTag &root, dealGPModule &module){
	dealGPMParameterList &parametersList = module.GetParameterList();
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	dealGPMParameter *parameters;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "parameter") == 0){
				parameters = NULL;
				
				try{
					parameters = new dealGPMParameter;
					if(!parameters) DETHROW(deeOutOfMemory);
					
					parameters->SetName(pGetAttributeString(*tag, "name"));
					parameters->SetValue(pGetCDataString(*tag));
					
					parametersList.AddParameter(parameters);
					
				}catch(const deException &){
					if(parameters) parameters->FreeReference();
					throw;
				}
				
			}else{
				pErrorUnknownTag(root, *tag);
			}
		}
	}
}

void dealGameConfigXML::pReadProfileWindow(const decXmlElementTag &root, dealGameProfile &profile){
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "width") == 0){
				profile.SetWidth(pGetCDataInt(*tag));
				
			}else if(strcmp(tag->GetName(), "height") == 0){
				profile.SetHeight(pGetCDataInt(*tag));
				
			}else if(strcmp(tag->GetName(), "fullScreen") == 0){
				profile.SetFullScreen(pGetCDataInt(*tag) != 0);
				
			}else{
				pErrorUnknownTag(root, *tag);
			}
		}
	}
}
