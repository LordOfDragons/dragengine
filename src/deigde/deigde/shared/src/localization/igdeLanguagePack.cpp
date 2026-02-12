/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "igdeLanguagePack.h"
#include "../utils/igdeBaseXML.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/logger/deLogger.h>


// Class igdeLanguagePack
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeLanguagePack::igdeLanguagePack() = default;
igdeLanguagePack::~igdeLanguagePack() = default;


// Management
///////////////

void igdeLanguagePack::SetLanguage(const decString &language){
	pLanguage = language;
}

void igdeLanguagePack::SetName(const decUnicodeString &name){
	pName = name;
}


class cLanguagePackLoader : public igdeBaseXML{
public:
	explicit cLanguagePackLoader(deLogger *logger) : igdeBaseXML(logger, "LanguagePackLoader"){}
	~cLanguagePackLoader() override = default;
	
	void LoadLanguagePack(decBaseFileReader &reader, igdeLanguagePack &languagePack){
		try{
			auto document = decXmlDocument::Ref::New();
			decXmlParser(GetLogger()).ParseXml(&reader, document);
			
			document->StripComments();
			document->CleanCharData();
			
			const decXmlElementTag * const root = document->GetRoot();
			DEASSERT_NOTNULL(root)
			DEASSERT_TRUE(root->GetName() == "languagePack")
			
			pReadLanguagePack(*root, languagePack);
			
		}catch(const deException &e){
			GetLogger()->LogInfoFormat(GetLoggerSource(),
				"Failed loading language pack from file '%s'", reader.GetFilename());
			GetLogger()->LogException(GetLoggerSource(), e);
			throw;
		}
	}
	
private:
	void pReadLanguagePack(const decXmlElementTag &root, igdeLanguagePack &languagePack){
		const int count = root.GetElementCount();
		int i;
		
		for(i=0; i<count; i++){
			const decXmlElementTag * const tag = root.GetElementIfTag(i);
			if(!tag){
				continue;
			}
			
			if(tag->GetName() == "identifier"){
				languagePack.SetLanguage(GetCDataString(*tag));
				
			}else if(tag->GetName() == "name"){
				languagePack.SetName(decUnicodeString::NewFromUTF8(GetCDataString(*tag)));
				
			}else if(tag->GetName() == "translation"){
				languagePack.GetEntries().SetAt(GetAttributeString(*tag, "name"),
					decUnicodeString::NewFromUTF8(GetCDataString(*tag)));
			}
		}
	}
};

void igdeLanguagePack::LoadFromFile(deLogger &logger, decBaseFileReader &reader){
	cLanguagePackLoader(&logger).LoadLanguagePack(reader, *this);
}
