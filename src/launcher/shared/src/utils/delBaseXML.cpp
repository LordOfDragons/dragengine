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

#include "delBaseXML.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>



// Class delBaseXML
/////////////////////

// Constructors and Destructors
/////////////////////////////////

delBaseXML::delBaseXML(deLogger *logger, const char *loggerSource) :
pLogger(logger),
pLoggerSource(loggerSource)
{
	if(!logger){
		DETHROW_INFO(deeNullPointer, "logger");
	}
	if(!loggerSource){
		DETHROW_INFO(deeNullPointer, "loggerSource");
	}
}

delBaseXML::~delBaseXML(){
}



// Management
///////////////

bool delBaseXML::HasAttribute(const decXmlElementTag &tag, const char *name) const{
	return tag.FindAttribute(name) != NULL;
}

const decString &delBaseXML::GetAttributeString(const decXmlElementTag &tag, const char *name) const{
	const decXmlAttValue * const value = tag.FindAttribute(name);
	if(value){
		return value->GetValue();
	}
	
	pLogger->LogErrorFormat(pLoggerSource.GetString(), "Missing Attribute %s in tag %s",
		name, tag.GetName().GetString());
	DETHROW(deeInvalidParam);
}

int delBaseXML::GetAttributeInt(const decXmlElementTag &tag, const char *name) const{
	const decXmlAttValue * const value = tag.FindAttribute(name);
	if(value){
		return value->GetValue().ToInt();
	}
	
	pLogger->LogErrorFormat(pLoggerSource.GetString(), "Missing Attribute %s in tag %s",
		name, tag.GetName().GetString());
	DETHROW(deeInvalidParam);
}

float delBaseXML::GetAttributeFloat(const decXmlElementTag &tag, const char *name) const{
	const decXmlAttValue * const value = tag.FindAttribute(name);
	if(value){
		return value->GetValue().ToFloat();
	}
	
	pLogger->LogErrorFormat(pLoggerSource.GetString(), "Missing Attribute %s in tag %s",
		name, tag.GetName().GetString());
	DETHROW(deeInvalidParam);
}

int delBaseXML::GetCDataInt(const decXmlElementTag &tag) const{
	const decXmlCharacterData * const cdata = tag.GetFirstData();
	return cdata ? cdata->GetData().ToInt() : 0;
}

float delBaseXML::GetCDataFloat(const decXmlElementTag &tag) const{
	const decXmlCharacterData * const cdata = tag.GetFirstData();
	return cdata ? cdata->GetData().ToFloat() : 0.0f;
}

const decString &delBaseXML::GetCDataString(const decXmlElementTag &tag) const{
	const decXmlCharacterData * const cdata = tag.GetFirstData();
	static const decString empty;
	return cdata ? cdata->GetData() : empty;
}

bool delBaseXML::GetCDataBool(const decXmlElementTag &tag) const{
	const decXmlCharacterData * const cdata = tag.GetFirstData();
	return cdata && (cdata->GetData() == "true" || cdata->GetData() == "1");
}



void delBaseXML::ErrorUnknownTag(const decXmlElementTag &root, const decXmlElementTag &tag){
	pLogger->LogErrorFormat(pLoggerSource.GetString(), "%s(%i:%i): Unknown Tag %s",
		root.GetName().GetString(), tag.GetLineNumber(), tag.GetPositionNumber(),
		tag.GetName().GetString());
}

void delBaseXML::ErrorUnknownValue(const decXmlElementTag &tag, const char *value){
	pLogger->LogErrorFormat(pLoggerSource.GetString(), "%s(%i:%i): Unknown Value %s",
		tag.GetName().GetString(), tag.GetLineNumber(), tag.GetPositionNumber(), value);
}

void delBaseXML::ErrorMissingTag(const decXmlElementTag &root, const char *tagName){
	pLogger->LogErrorFormat(pLoggerSource.GetString(), "%s(%i:%i): Missing Tag %s",
		root.GetName().GetString(), root.GetLineNumber(), root.GetPositionNumber(), tagName);
}
