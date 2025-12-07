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

#include "ceLoadSaveLangPack.h"
#include "../langpack/ceLangPack.h"
#include "../langpack/ceLangPackEntry.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/localization/deLanguagePack.h>
#include <dragengine/resources/localization/deLanguagePackEntry.h>
#include <dragengine/resources/localization/deLanguagePackBuilder.h>
#include <dragengine/resources/localization/deLanguagePackManager.h>
#include <dragengine/systems/modules/langpack/deBaseLanguagePackModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>



// Class ceLoadSaveLangPack
/////////////////////////////

// Constructor, destructor
////////////////////////////

ceLoadSaveLangPack::ceLoadSaveLangPack(deBaseLanguagePackModule &module) :
pModule(module)
{
	const deLoadableModule &loadableModule = module.GetLoadableModule();
	const decStringList &patternList = loadableModule.GetPatternList();
	const int patternCount = patternList.GetCount();
	int i;
	
	pName = loadableModule.GetName();
	
	for(i=0; i<patternCount; i++){
		if(i > 0){
			pPattern.AppendCharacter(',');
		}
		pPattern.AppendCharacter('*');
		pPattern.Append(patternList.GetAt(i));
	}
}

ceLoadSaveLangPack::~ceLoadSaveLangPack(){
}



// Management
///////////////

class cLoadLangPackLoader : public deLanguagePackBuilder{
public:
	cLoadLangPackLoader(){}
	void BuildLanguagePack(deLanguagePack&){}
};

void ceLoadSaveLangPack::LoadLangPack(ceLangPack &langpack, decBaseFileReader &file){
	cLoadLangPackLoader builder;
	const deLanguagePack::Ref engLangPack(deLanguagePack::Ref::New(
		pModule.GetGameEngine()->GetLanguagePackManager()->CreateLanguagePack("", builder)));
	pModule.LoadLanguagePack(file, engLangPack);
	
	langpack.SetIdentifier(engLangPack->GetIdentifier());
	langpack.SetName(engLangPack->GetName());
	langpack.SetDescription(engLangPack->GetDescription());
	langpack.SetMissingText(engLangPack->GetMissingText());
	
	const int entryCount = engLangPack->GetEntryCount();
	int i;
	
	for(i=0; i<entryCount; i++){
		const deLanguagePackEntry &engEntry = engLangPack->GetEntryAt(i);
		
		const ceLangPackEntry::Ref entry(ceLangPackEntry::Ref::NewWith());
		entry->SetName(engEntry.GetName());
		entry->SetText(engEntry.GetText());
		langpack.AddEntry(entry);
	}
}

class cSaveLangPackLoader : public deLanguagePackBuilder{
	const ceLangPack &pLangPack;
	
public:
	cSaveLangPackLoader(const ceLangPack &langpack) : pLangPack(langpack){}
	
	virtual void BuildLanguagePack(deLanguagePack &langPack) override{
		const int entryCount = pLangPack.GetEntryCount();
		int i;
		
		langPack.SetIdentifier(pLangPack.GetIdentifier());
		langPack.SetName(pLangPack.GetName());
		langPack.SetDescription(pLangPack.GetDescription());
		langPack.SetMissingText(pLangPack.GetMissingText());
		
		langPack.SetEntryCount(entryCount);
		for(i=0; i<entryCount; i++){
			const ceLangPackEntry &entry = *pLangPack.GetEntryAt(i);
			deLanguagePackEntry &engEntry = langPack.GetEntryAt(i);
			
			engEntry.SetName(entry.GetName());
			engEntry.SetText(entry.GetText());
		}
	}
};

void ceLoadSaveLangPack::SaveLangPack(const ceLangPack &langpack, decBaseFileWriter &file){
	cSaveLangPackLoader builder(langpack);
	pModule.SaveLanguagePack(file, deLanguagePack::Ref::New(
		pModule.GetGameEngine()->GetLanguagePackManager()->CreateLanguagePack("", builder)));
}
