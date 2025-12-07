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

#include "dealBaseXML.h"
#include "logger/deLogger.h"
#include "common/xmlparser/decXmlWriter.h"
#include "common/xmlparser/decXmlDocument.h"
#include "common/xmlparser/decXmlCharacterData.h"
#include "common/xmlparser/decXmlElementTag.h"
#include "common/xmlparser/decXmlAttValue.h"
#include "common/xmlparser/decXmlVisitor.h"
#include "common/xmlparser/decXmlParser.h"
#include "common/exceptions.h"



// Class dealBaseXML
//////////////////////

// Constructors and Destructors
/////////////////////////////////

dealBaseXML::dealBaseXML(deLogger *logger, const char *loggerSource){
	if(!logger || !loggerSource) DETHROW(deeInvalidParam);
	
	pLoggerSource = loggerSource;
	
	pLogger = logger;
	logger->AddReference();
}

dealBaseXML::~dealBaseXML(){
	pLogger->FreeReference();
}



// Management
///////////////


bool dealBaseXML::pHasAttribute(const decXmlElementTag &tag, const char *name) const{
	return tag.FindAttribute(name) != NULL;
}

const char *dealBaseXML::pGetAttributeString(const decXmlElementTag &tag, const char *name) const{
	const decXmlAttValue *value = tag.FindAttribute(name);
	
	if(value){
		return value->GetValue();
		
	}else{
		pLogger->LogErrorFormat(pLoggerSource.GetString(),
			"Missing Attribute %s in tag %s", name, tag.GetName().GetString());
		DETHROW(deeInvalidParam);
	}
}

int dealBaseXML::pGetAttributeInt(const decXmlElementTag &tag, const char *name) const{
	const decXmlAttValue *value = tag.FindAttribute(name);
	
	if(value){
		return (int)strtol(value->GetValue(), NULL, 10);
		
	}else{
		pLogger->LogErrorFormat(pLoggerSource.GetString(),
			"Missing Attribute %s in tag %s", name, tag.GetName().GetString());
		DETHROW(deeInvalidParam);
	}
}

float dealBaseXML::pGetAttributeFloat(const decXmlElementTag &tag, const char *name) const{
	const decXmlAttValue *value = tag.FindAttribute(name);
	
	if(value){
		return strtof(value->GetValue(), NULL);
		
	}else{
		pLogger->LogErrorFormat(pLoggerSource.GetString(),
			"Missing Attribute %s in tag %s", name, tag.GetName().GetString());
		DETHROW(deeInvalidParam);
	}
}

int dealBaseXML::pGetCDataInt(const decXmlElementTag &tag) const{
	decXmlCharacterData *cdata = tag.GetFirstData();
	
	if(cdata){
		return (int)strtol(cdata->GetData(), NULL, 10);
	}
	
	return 0;
}

float dealBaseXML::pGetCDataFloat(const decXmlElementTag &tag) const{
	decXmlCharacterData *cdata = tag.GetFirstData();
	
	if(cdata){
		return strtof(cdata->GetData(), NULL);
	}
	
	return 0.0f;
}

const char *dealBaseXML::pGetCDataString(const decXmlElementTag &tag) const{
	decXmlCharacterData *cdata = tag.GetFirstData();
	
	if(cdata){
		return cdata->GetData();
	}
	
	return "";
}



void dealBaseXML::pErrorUnknownTag(const decXmlElementTag &root, const decXmlElementTag &tag){
	pLogger->LogErrorFormat(pLoggerSource.GetString(), "%s(%i:%i): Unknown Tag %s",
		root.GetName().GetString(), tag.GetLineNumber(), tag.GetPositionNumber(),
		tag.GetName().GetString());
}

void dealBaseXML::pErrorUnknownValue(const decXmlElementTag &tag, const char *value){
	pLogger->LogErrorFormat(pLoggerSource.GetString(), "%s(%i:%i): Unknown Value %s",
		tag.GetName().GetString(), tag.GetLineNumber(), tag.GetPositionNumber(), value);
}

void dealBaseXML::pErrorMissingTag(const decXmlElementTag &root, const char *tagName){
	pLogger->LogErrorFormat(pLoggerSource.GetString(), "%s(%i:%i): Missing Tag %s",
		root.GetName().GetString(), root.GetLineNumber(), root.GetPositionNumber(), tagName);
}
