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

#include "dealEngine.h"
#include "dealEngineConfigXML.h"
#include "../dealLauncher.h"
#include "../game/dealGameManager.h"
#include "../game/profile/dealGameProfile.h"
#include "../game/profile/dealGPModule.h"
#include "../game/profile/dealGPMParameter.h"
#include "../game/profile/dealGPDisableModuleVersion.h"
#include "../game/profile/dealGPDisableModuleVersionList.h"

#include "../logger/deLogger.h"
#include "../logger/deLoggerConsole.h"
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



// Class dealEngineConfigXML
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dealEngineConfigXML::dealEngineConfigXML(deLogger *logger, const char *loggerSource) :
dealBaseXML(logger, loggerSource){
}

dealEngineConfigXML::~dealEngineConfigXML(){
}



// Management
///////////////

void dealEngineConfigXML::ReadFromFile(decBaseFileReader &reader, dealLauncher &launcher){
	decXmlDocument::Ref xmldoc(decXmlDocument::Ref::NewWith());
	
	decXmlParser parser(&launcher.GetLogger());
	
	parser.ParseXml(&reader, xmldoc);
	
	xmldoc->StripComments();
	xmldoc->CleanCharData();
	
	decXmlElementTag * const root = xmldoc->GetRoot();
	if(!root || strcmp(root->GetName(), "engineConfig") != 0) DETHROW(deeInvalidParam);
	
	pReadConfig(*root, launcher);
}

void dealEngineConfigXML::WriteToFile(decBaseFileWriter &writer, const dealLauncher &launcher){
	decXmlWriter xmlWriter(&writer);
	xmlWriter.WriteXMLDeclaration();
	pWriteConfig(xmlWriter, launcher);
}



// Private Functions
//////////////////////

void dealEngineConfigXML::pWriteConfig(decXmlWriter &writer, const dealLauncher &launcher){
	const dealGameManager &gameManager = launcher.GetGameManager();
	
	writer.WriteOpeningTag("engineConfig", false, true);
	
	pWriteProfiles(writer, launcher);
	
	if(gameManager.GetActiveProfile()){
		writer.WriteDataTagString("activeProfile", gameManager.GetActiveProfile()->GetName().GetString());
	}
	
	writer.WriteClosingTag("engineConfig", true);
}

void dealEngineConfigXML::pWriteProfiles(decXmlWriter &writer, const dealLauncher &launcher){
	const dealGameManager &gameManager = launcher.GetGameManager();
	const dealGameProfileList &profileList = gameManager.GetProfileList();
	int i, count = profileList.GetCount();
	
	writer.WriteOpeningTag("profiles", false, true);
	
	for(i=0; i<count; i++){
		pWriteProfile(writer, *profileList.GetAt(i));
	}
	
	writer.WriteClosingTag("profiles", true);
}

void dealEngineConfigXML::pWriteProfile(decXmlWriter &writer, const dealGameProfile &profile){
	writer.WriteOpeningTagStart("profile");
	writer.WriteAttributeString("name", profile.GetName().GetString());
	writer.WriteOpeningTagEnd();
	
	pWriteProfileSystems(writer, profile);
	pWriteProfileDisableModuleVersions(writer, profile);
	pWriteProfileModules(writer, profile);
	
	writer.WriteDataTagString("runArguments", profile.GetRunArguments().GetString());
	writer.WriteDataTagInt("replaceRunArguments", profile.GetReplaceRunArguments() ? 1 : 0);
	pWriteProfileWindow(writer, profile);
	
	writer.WriteClosingTag("profile", true);
}

void dealEngineConfigXML::pWriteProfileSystems(decXmlWriter &writer, const dealGameProfile &profile){
	writer.WriteOpeningTag("systems", false, true);
	
	writer.WriteDataTagString("graphic", profile.GetModuleGraphic());
	writer.WriteDataTagString("input", profile.GetModuleInput());
	writer.WriteDataTagString("physics", profile.GetModulePhysics());
	writer.WriteDataTagString("animator", profile.GetModuleAnimator());
	writer.WriteDataTagString("ai", profile.GetModuleAI());
	writer.WriteDataTagString("crashRecovery", profile.GetModuleCrashRecovery());
	writer.WriteDataTagString("audio", profile.GetModuleAudio());
	writer.WriteDataTagString("synthesizer", profile.GetModuleSynthesizer());
	writer.WriteDataTagString("network", profile.GetModuleNetwork());
	
	writer.WriteClosingTag("systems", true);
}

void dealEngineConfigXML::pWriteProfileDisableModuleVersions(decXmlWriter &writer, const dealGameProfile &profile){
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

void dealEngineConfigXML::pWriteProfileModules(decXmlWriter &writer, const dealGameProfile &profile){
	const dealGPModuleList &moduleList = profile.GetModuleList();
	int i, count = moduleList.GetModuleCount();
	
	writer.WriteOpeningTag("modules", false, true);
	
	for(i=0; i<count; i++){
		pWriteProfileModule(writer, *moduleList.GetModuleAt(i));
	}
	
	writer.WriteClosingTag("modules", true);
}

void dealEngineConfigXML::pWriteProfileModule(decXmlWriter &writer, const dealGPModule &module){
	writer.WriteOpeningTagStart("module");
	writer.WriteAttributeString("name", module.GetName().GetString());
	writer.WriteOpeningTagEnd();
	
	pWriteProfileModuleParameters(writer, module);
	
	writer.WriteClosingTag("module", true);
}

void dealEngineConfigXML::pWriteProfileModuleParameters(decXmlWriter &writer, const dealGPModule &module){
	const dealGPMParameterList &parametersList = module.GetParameterList();
	int i, count = parametersList.GetParameterCount();
	
	writer.WriteOpeningTag("parameters", false, true);
	
	for(i=0; i<count; i++){
		const dealGPMParameter &parameters = *parametersList.GetParameterAt(i);
		
		writer.WriteOpeningTagStart("parameter");
		writer.WriteAttributeString("name", parameters.GetName().GetString());
		writer.WriteOpeningTagEnd(false, false);
		writer.WriteTextString(parameters.GetValue());
		writer.WriteClosingTag("parameter", false);
	}
	
	writer.WriteClosingTag("parameters", true);
}

void dealEngineConfigXML::pWriteProfileWindow(decXmlWriter &writer, const dealGameProfile &profile){
	writer.WriteOpeningTag("window", false, true);
	
	writer.WriteDataTagInt("width", profile.GetWidth());
	writer.WriteDataTagInt("height", profile.GetHeight());
	writer.WriteDataTagInt("fullScreen", profile.GetFullScreen() ? 1 : 0);
	
	writer.WriteClosingTag("window", true);
}



void dealEngineConfigXML::pReadConfig(const decXmlElementTag &root, dealLauncher &launcher){
	dealGameManager &gameManager = launcher.GetGameManager();
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	const char *profileName;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "profiles") == 0){
				pReadProfiles(*tag, launcher);
				
			}else if(strcmp(tag->GetName(), "activeProfile") == 0){
				profileName = pGetCDataString(*tag);
				
				if(strlen(profileName) == 0){
					gameManager.SetActiveProfile(NULL);
					
				}else{
					gameManager.SetActiveProfile(gameManager.GetProfileList().GetNamed(profileName));
					if(!gameManager.GetActiveProfile()){
						GetLogger()->LogWarnFormat(GetLoggerSource().GetString(),
							"%s(%i:%i): Profile '%s' does not exist", tag->GetName().GetString(),
							tag->GetLineNumber(), tag->GetPositionNumber(), profileName);
					}
				}
				
			}else{
				pErrorUnknownTag(root, *tag);
			}
		}
	}
}

void dealEngineConfigXML::pReadProfiles(const decXmlElementTag &root, dealLauncher &launcher){
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "profile") == 0){
				pReadProfile(*tag, launcher);
				
			}else{
				pErrorUnknownTag(root, *tag);
			}
		}
	}
}

void dealEngineConfigXML::pReadProfile(const decXmlElementTag &root, dealLauncher &launcher){
	dealGameManager &gameManager = launcher.GetGameManager();
	int e, elementCount = root.GetElementCount();
	dealGameProfile *profile = NULL;
	const decXmlElementTag *tag;
	decString profileName;
	
	profileName = pGetAttributeString(root, "name");
	
	try{
		profile = new dealGameProfile;
		if(!profile) DETHROW(deeOutOfMemory);
		
		profile->SetName(profileName.GetString());
		
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
		
		gameManager.GetProfileList().Add(profile);
		
	}catch(const deException &){
		if(profile) profile->FreeReference();;
		throw;
	}
}

void dealEngineConfigXML::pReadProfileSystems(const decXmlElementTag &root, dealGameProfile &profile){
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "graphic") == 0){
				profile.SetModuleGraphic(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "input") == 0){
				profile.SetModuleInput(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "physics") == 0){
				profile.SetModulePhysics(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "animator") == 0){
				profile.SetModuleAnimator(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "ai") == 0){
				profile.SetModuleAI(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "crashRecovery") == 0){
				profile.SetModuleCrashRecovery(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "audio") == 0){
				profile.SetModuleAudio(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "synthesizer") == 0){
				profile.SetModuleSynthesizer(pGetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "network") == 0){
				profile.SetModuleNetwork(pGetCDataString(*tag));
				
			}else{
				pErrorUnknownTag(root, *tag);
			}
		}
	}
}

void dealEngineConfigXML::pReadProfileDisableModuleVersions(const decXmlElementTag &root, dealGameProfile &profile){
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

void dealEngineConfigXML::pReadProfileModules(const decXmlElementTag &root, dealGameProfile &profile){
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

void dealEngineConfigXML::pReadProfileModule(const decXmlElementTag &root, dealGameProfile &profile){
	int e, elementCount = root.GetElementCount();
	dealGPModule *module = NULL;
	const decXmlElementTag *tag;
	decString moduleName;
	
	moduleName = pGetAttributeString(root, "name");
	
	try{
		module = new dealGPModule;
		if(!module) DETHROW(deeOutOfMemory);
		
		module->SetName(moduleName.GetString());
		
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

void dealEngineConfigXML::pReadProfileModuleParameters(const decXmlElementTag &root, dealGPModule &module){
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

void dealEngineConfigXML::pReadProfileWindow(const decXmlElementTag &root, dealGameProfile &profile){
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
