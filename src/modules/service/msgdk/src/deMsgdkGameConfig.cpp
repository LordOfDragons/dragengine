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

#include "deMsgdkGameConfig.h"
#include "deMicrosoftGdk.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileReader.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>


// Class deMsgdkGameConfig
////////////////////////////

// Constructor, destructor
////////////////////////////

deMsgdkGameConfig::deMsgdkGameConfig(deMicrosoftGdk& module, const decString& config) :
pModule(module),
advancedUserModel(false)
{
	pLoadFromXml(config);
}

// Management
///////////////

decXmlElementTag *deMsgdkGameConfig::pGetTagAt(decXmlElementTag &tag, int index)
{
	decXmlElement &element = *tag.GetElementAt(index);
	if(element.CanCastToElementTag())
	{
		return element.CastToElementTag();
	}
	return nullptr;
}

const decXmlAttValue *deMsgdkGameConfig::pFindAttribute(decXmlElementTag &tag, const char *name)
{
	const int count = tag.GetElementCount();
	int i;
	for(i=0; i<count; i++)
	{
		decXmlElement * const element = tag.GetElementAt(i);
		if(element->CanCastToAttValue())
		{
			const decXmlAttValue * const value = element->CastToAttValue();
			if(value->GetName() == name)
			{
				return value;
			}
		}
	}
	return nullptr;
}

const char *deMsgdkGameConfig::pGetAttributeString(decXmlElementTag &tag, const char *name)
{
	const decXmlAttValue * const value = pFindAttribute(tag, name);
	if(value)
	{
		return value->GetValue();
	}

	pModule.LogErrorFormat("Missing Attribute '%s' in tag '%s'.", name, tag.GetName().GetString());
	DETHROW(deeInvalidParam);
}

int deMsgdkGameConfig::pGetAttributeInt(decXmlElementTag &tag, const char *name)
{
	const decXmlAttValue * const value = pFindAttribute(tag, name);
	if(value)
	{
		return value->GetValue().ToIntValid();
	}

	pModule.LogErrorFormat("Missing Attribute '%s' in tag '%s'.", name, tag.GetName().GetString());
	DETHROW( deeInvalidParam );
}

float deMsgdkGameConfig::pGetAttributeFloat(decXmlElementTag &tag, const char *name)
{
	const decXmlAttValue * const value = pFindAttribute(tag, name);
	if(value)
	{
		return value->GetValue().ToFloatValid();
	}

	pModule.LogErrorFormat("Missing Attribute '%s' in tag '%s'.", name, tag.GetName().GetString());
	DETHROW(deeInvalidParam);
}

void deMsgdkGameConfig::pLoadFromXml(const decString& config)
{
	const decMemoryFile::Ref file(decMemoryFile::Ref::NewWith("/MicrosoftGame.config"));
	file->Resize(config.GetLength());
	memcpy((char*)file->GetPointer(), config.GetString(), file->GetLength());

	const decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	const decMemoryFileReader::Ref reader(decMemoryFileReader::Ref::NewWith(file));
	decXmlParser(pModule.GetGameEngine()->GetLogger()).ParseXml(reader, xmlDoc);

	xmlDoc->StripComments();
	xmlDoc->CleanCharData();

	decXmlElementTag * const root = xmlDoc->GetRoot();
	DEASSERT_NOTNULL(root)
	DEASSERT_TRUE(root->GetName() == "Game")

	pReadTagGame(*root);

	DEASSERT_FALSE(scid.IsEmpty())
}

void deMsgdkGameConfig::pReadTagGame(decXmlElementTag& root)
{
	const int count = root.GetElementCount();
	int i;
	for(i=0; i<count; i++)
	{
		decXmlElementTag * const tag = pGetTagAt(root, i);
		if(!tag)
		{
			continue;
		}

		const decString &tagName = tag->GetName();
		if(tagName == "ExtendedAttributeList")
		{
			pReadTagExtendedAttributeList(*tag);
		}
		else if(tagName == "AdvancedUserModel")
		{
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			DEASSERT_NOTNULL(cdata)
			advancedUserModel = cdata->GetData() == "true";
		}
	}
}

void deMsgdkGameConfig::pReadTagExtendedAttributeList(decXmlElementTag& root)
{
	const int count = root.GetElementCount();
	int i;
	for(i=0; i<count; i++)
	{
		decXmlElementTag * const tag = pGetTagAt(root, i);
		if(!tag)
		{
			continue;
		}

		const decString &tagName = tag->GetName();
		if(tagName == "ExtendedAttribute")
		{
			const decString &attrName = pGetAttributeString(*tag, "Name");
			if(attrName == "Scid")
			{
				scid = pGetAttributeString(*tag, "Value");
			}
		}
	}
}
