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

#include "debpConfiguration.h"
#include "dePhysicsBullet.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>



// Class debpConfiguration
////////////////////////////

// Constructor, destructor
////////////////////////////

debpConfiguration::debpConfiguration(dePhysicsBullet *bullet){
	pBullet = bullet;
	
	pEnableConstraintPoint2Point = true;
	pEnableConstraintHinge = true;
	pEnableConstraintConeTwist = true;
	pEnableConstraintSlider = true;
	
	pSimulatePropFields = true;
}

debpConfiguration::~debpConfiguration(){
}



// Management
///////////////

void debpConfiguration::SetEnableConstraintPoint2Point(bool enable){
	pEnableConstraintPoint2Point = enable;
}

void debpConfiguration::SetEnableConstraintHinge(bool enable){
	pEnableConstraintHinge = enable;
}

void debpConfiguration::SetEnableConstraintConeTwist(bool enable){
	pEnableConstraintConeTwist = enable;
}

void debpConfiguration::SetEnableConstraintSlider(bool enable){
	pEnableConstraintSlider = enable;
}



void debpConfiguration::SetSimulatePropFields(bool simulatePropFields){
	pSimulatePropFields = simulatePropFields;
}



// Loading and Saving
///////////////////////

void debpConfiguration::LoadConfig(){
	deVirtualFileSystem &vfs = pBullet->GetVFS();
	decBaseFileReader *reader = NULL;
	
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	
	// read the configuration file if it exists
	decPath path;
	path.SetFromUnix("/config/bullet.xml");
	
	if(!vfs.ExistsFile(path)){
		return;
	}
	
	try{
		reader = vfs.OpenFileForReading(path);
		if(!reader){
			DETHROW(deeInvalidParam);
		}
		
		decXmlParser(pBullet->GetGameEngine()->GetLogger()).ParseXml(reader, xmlDoc);
		
		reader->FreeReference();
		
	}catch(const deException &){
		if(reader){
			reader->FreeReference();
		}
		throw;
	}
	
	// some cleanup visiting
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	// interpretate xml
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if(!root || strcmp(root->GetName(), "config") != 0){
		DETHROW(deeInvalidParam);
	}
	
	pParseConfig(root);
}

void debpConfiguration::SaveConfig(){
	// TODO
}



// Private Functions
//////////////////////

decXmlElementTag *debpConfiguration::pGetTagAt(decXmlElementTag *tag, int index){
	decXmlElement *element = tag->GetElementAt(index);
	if(element->CanCastToElementTag()){
		return element->CastToElementTag();
	}else{
		return NULL;
	}
}

decXmlAttValue *debpConfiguration::pFindAttribute(decXmlElementTag *tag, const char *name){
	decXmlAttValue *value;
	decXmlElement *element;
	int i;
	for(i=0; i<tag->GetElementCount(); i++){
		element = tag->GetElementAt(i);
		if(element->CanCastToAttValue()){
			value = element->CastToAttValue();
			if(strcmp(value->GetName(), name) == 0){
				return value;
			}
		}
	}
	return NULL;
}

void debpConfiguration::pParseConfig(decXmlElementTag *root){
	decXmlElementTag *tag;
	int i;
	
	for(i=0; i<root->GetElementCount(); i++){
		tag = pGetTagAt(root, i);
		if(tag){
			if(strcmp(tag->GetName(), "property") == 0){
				pParseProperty(tag);
				
			}else{
				pBullet->LogWarnFormat("bullet.xml(%i:%i): Unknown Tag %s, ignoring",
					tag->GetLineNumber(), tag->GetPositionNumber(), tag->GetName().GetString());
			}
		}
	}
}

void debpConfiguration::pParseProperty(decXmlElementTag *root){
	decXmlCharacterData *cdata;
	decXmlAttValue *xmlValue;
	const char *value = NULL;
	const char *name = NULL;
	
	// check attributes
	xmlValue = pFindAttribute(root, "name");
	if(!xmlValue){
		pBullet->LogErrorFormat("bullet.xml(%i:%i): Missing Attribute 'name' in tag '%s'",
			root->GetLineNumber(), root->GetPositionNumber(), root->GetName().GetString());
		DETHROW(deeInvalidParam);
	}
	name = xmlValue->GetValue();
	
	// check cdata
	cdata = root->GetFirstData();
	if(cdata){
		value = cdata->GetData();
		
	}else{
		value = "";
	}
	
	// set property if name is matching anything valid
	if(strcmp(name, "enableConstraintPoint2Point") == 0){
		SetEnableConstraintPoint2Point((int)strtol(value, NULL, 10) != 0);
		
	}else if(strcmp(name, "enableConstraintHinge") == 0){
		SetEnableConstraintHinge((int)strtol(value, NULL, 10) != 0);
		
	}else if(strcmp(name, "enableConstraintConeTwist") == 0){
		SetEnableConstraintConeTwist((int)strtol(value, NULL, 10) != 0);
		
	}else if(strcmp(name, "enableConstraintSlider") == 0){
		SetEnableConstraintSlider((int)strtol(value, NULL, 10) != 0);
		
	}else if(strcmp(name, "simulatePropFields") == 0){
		SetSimulatePropFields((int)strtol(value, NULL, 10) != 0);
		
	}else{
		pBullet->LogWarnFormat("bullet.xml(%i:%i): Invalid property name %s, ignoring",
			root->GetLineNumber(), root->GetPositionNumber(), name);
	}
}
