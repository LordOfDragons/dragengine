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

#include "projProjectXml.h"
#include "../project/projProject.h"
#include "../project/profile/projProfile.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>



// Class projProjectXml
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

projProjectXml::projProjectXml(deLogger *logger, const char *loggerSource) :
igdeBaseXML(logger, loggerSource){
}

projProjectXml::~projProjectXml(){
}



// Management
///////////////

void projProjectXml::ReadFromFile(decBaseFileReader &reader, projProject &project){
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::New());
	
	decXmlParser(GetLogger()).ParseXml(&reader, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if(!root || strcmp(root->GetName(), "project") != 0){
		DETHROW(deeInvalidParam);
	}
	
	pReadProject(*root, project);
}

void projProjectXml::WriteToFile(decBaseFileWriter &writer, const projProject &project){
	decXmlWriter xmlWriter(&writer);
	xmlWriter.WriteXMLDeclaration();
	pWriteProject(xmlWriter, project);
}



// Private Functions
//////////////////////

void projProjectXml::pWriteProject(decXmlWriter &writer, const projProject &project){
	int i;
	
	writer.WriteOpeningTag("project");
	
	const projProfileList &profiles = project.GetProfiles();
	const int profileCount = profiles.GetCount();
	for(i=0; i<profileCount; i++){
		pWriteProfile(writer, *profiles.GetAt(i));
	}
	
	writer.WriteClosingTag("project");
}

void projProjectXml::pWriteProfile(decXmlWriter &writer, const projProfile &profile){
	writer.WriteOpeningTag("profile");
	
	writer.WriteDataTagString("name", profile.GetName());
	WriteMultilineString(writer, "description", profile.GetDescription());
	
	writer.WriteDataTagString("scriptDirectory", profile.GetScriptDirectory());
	writer.WriteDataTagString("gameObject", profile.GetGameObject());
	writer.WriteDataTagString("pathConfig", profile.GetPathConfig());
	writer.WriteDataTagString("pathCapture", profile.GetPathCapture());
	
	writer.WriteDataTagString("identifier", profile.GetIdentifier().ToHexString(false));
	writer.WriteDataTagString("aliasIdentifier", profile.GetAliasIdentifier());
	writer.WriteDataTagString("title", profile.GetTitle());
	WriteMultilineString(writer, "gameDescription", profile.GetGameDescription());
	writer.WriteDataTagString("creator", profile.GetCreator());
	writer.WriteDataTagString("website", profile.GetWebsite());
	
	const decStringSet &icons = profile.GetIcons();
	const int iconCount = icons.GetCount();
	int i;
	for(i=0; i<iconCount; i++){
		writer.WriteDataTagString("icon", icons.GetAt(i));
	}
	
	const decPoint &windowSize = profile.GetWindowSize();
	if(windowSize != decPoint()){
		writer.WriteOpeningTagStart("windowSize");
		writer.WriteAttributeInt("x", windowSize.x);
		writer.WriteAttributeInt("y", windowSize.y);
		writer.WriteOpeningTagEnd(true);
	}
	
	const decStringSet &excludePatterns = profile.GetExcludePatterns();
	const int excludePatternCount = excludePatterns.GetCount();
	for(i=0; i<excludePatternCount; i++){
		writer.WriteDataTagString("excludePattern", excludePatterns.GetAt(i));
	}
	
	const decStringSet &requiredExtensions = profile.GetRequiredExtensions();
	const int requiredExtensionCount = requiredExtensions.GetCount();
	for(i=0; i<requiredExtensionCount; i++){
		writer.WriteDataTagString("requiredExtension", requiredExtensions.GetAt(i));
	}
	
	writer.WriteDataTagString("delgaPath", profile.GetDelgaPath());
	
	if(!profile.GetRunArguments().IsEmpty()){
		writer.WriteDataTagString("runArguments", profile.GetRunArguments());
	}
	
	writer.WriteClosingTag("profile");
}



void projProjectXml::pReadProject(const decXmlElementTag &root, projProject &project){
	const int count = root.GetElementCount();
	int i;
	
	for(i=0; i<count; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if(tagName == "profile"){
			pReadProfile(*tag, project);
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	const projProfileList &profiles = project.GetProfiles();
	if(profiles.GetCount() > 0){
		project.SetActiveProfile(profiles.GetAt(0));
	}
}

void projProjectXml::pReadProfile(const decXmlElementTag &root, projProject &project){
	decStringSet requiredExtensions, excludePatterns, icons;
	const int count = root.GetElementCount();
	int i;
	
	const projProfile::Ref profile(projProfile::Ref::New());
	
	for(i=0; i<count; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if(tagName == "name"){
			profile->SetName(GetCDataString(*tag));
			
		}else if(tagName == "description"){
			profile->SetDescription(ReadMultilineString(*tag));
			
		}else if(tagName == "scriptDirectory"){
			profile->SetScriptDirectory(GetCDataString(*tag));
			
		}else if(tagName == "gameObject"){
			profile->SetGameObject(GetCDataString(*tag));
			
		}else if(tagName == "pathConfig"){
			profile->SetPathConfig(GetCDataString(*tag));
			
		}else if(tagName == "pathCapture"){
			profile->SetPathCapture(GetCDataString(*tag));
			
		}else if(tagName == "identifier"){
			profile->SetIdentifier(decUuid(GetCDataString(*tag), false));
			
		}else if(tagName == "aliasIdentifier"){
			profile->SetAliasIdentifier(GetCDataString(*tag));
			
		}else if(tagName == "title"){
			profile->SetTitle(GetCDataString(*tag));
			
		}else if(tagName == "gameDescription"){
			profile->SetGameDescription(ReadMultilineString(*tag));
			
		}else if(tagName == "creator"){
			profile->SetCreator(GetCDataString(*tag));
			
		}else if(tagName == "website"){
			profile->SetWebsite(GetCDataString(*tag));
			
		}else if(tagName == "windowSize"){
			decPoint size;
			ReadPoint(*tag, size);
			profile->SetWindowSize(size);
			
		}else if(tagName == "icon"){
			icons.Add(GetCDataString(*tag));
			
		}else if(tagName == "excludePattern"){
			excludePatterns.Add(GetCDataString(*tag));
			
		}else if(tagName == "requiredExtension"){
			requiredExtensions.Add(GetCDataString(*tag));
			
		}else if(tagName == "delgaPath"){
			profile->SetDelgaPath(GetCDataString(*tag));
			
		}else if(tagName == "runArguments"){
			profile->SetRunArguments(GetCDataString(*tag));
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	if(profile->GetName().IsEmpty()){
		LogErrorGenericProblem(root, "Profile requires a non-empty name");
	}
	if(!profile->GetIdentifier()){
		LogErrorGenericProblem(root, "Profile requires valid identifier");
	}
	if(project.GetProfiles().HasNamed(profile->GetName())){
		LogErrorGenericProblem(root, "Duplicate profile name");
	}
	
	profile->SetIcons(icons);
	profile->SetExcludePatterns(excludePatterns);
	profile->SetRequiredExtensions(requiredExtensions);
	
	project.AddProfile(profile);
}
