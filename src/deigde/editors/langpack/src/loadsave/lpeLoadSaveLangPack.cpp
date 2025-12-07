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

#include "lpeLoadSaveLangPack.h"
#include "../langpack/lpeLangPack.h"
#include "../langpack/lpeLangPackBuilder.h"
#include "../langpack/entry/lpeLangPackEntry.h"
#include "../gui/lpeWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/resources/localization/deLanguagePack.h>
#include <dragengine/resources/localization/deLanguagePackEntry.h>
#include <dragengine/resources/localization/deLanguagePackBuilder.h>
#include <dragengine/resources/localization/deLanguagePackManager.h>
#include <dragengine/systems/modules/langpack/deBaseLanguagePackModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/deEngine.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/exceptions.h>



// Class lpeLoadSaveLangPack
//////////////////////////////

// Constructor, destructor
////////////////////////////

lpeLoadSaveLangPack::lpeLoadSaveLangPack(deBaseLanguagePackModule *module){
	if(! module){
		DETHROW(deeInvalidParam);
	}
	
	const deLoadableModule &loadableModule = module->GetLoadableModule();
	const decStringList &patternList = loadableModule.GetPatternList();
	const int patternCount = patternList.GetCount();
	int i;
	
	pModule = module;
	
	pName = loadableModule.GetName();
	
	for(i=0; i<patternCount; i++){
		if(i > 0){
			pPattern.AppendCharacter(',');
		}
		pPattern.AppendCharacter('*');
		pPattern.Append(patternList.GetAt(i));
	}
}

lpeLoadSaveLangPack::~lpeLoadSaveLangPack(){
}



// Management
///////////////

void lpeLoadSaveLangPack::SetName(const char *name){
	if(! name){
		DETHROW(deeInvalidParam);
	}
	
	pName = name;
}

void lpeLoadSaveLangPack::SetPattern(const char *pattern){
	if(! pattern){
		DETHROW(deeInvalidParam);
	}
	
	pPattern = pattern;
}



class cDirectLangPackLoader : public deLanguagePackBuilder{
public:
	cDirectLangPackLoader(){
	}
	
	void BuildLanguagePack(deLanguagePack &langPack){
	}
};

void lpeLoadSaveLangPack::LoadLangPack(lpeLangPack *langpack, decBaseFileReader *file){
	if(! langpack || ! file){
		DETHROW(deeInvalidParam);
	}
	
	deEngine *engine = pModule->GetGameEngine();
	lpeLangPackEntry *entry = NULL;
	deLanguagePack *engLangPack = NULL;
	int i, entryCount;
	
	cDirectLangPackLoader builder;
	
	try{
		engLangPack = engine->GetLanguagePackManager()->CreateLanguagePack("", builder);
		pModule->LoadLanguagePack(*file, *engLangPack);
		
		langpack->SetIdentifier(engLangPack->GetIdentifier());
		langpack->SetName(engLangPack->GetName());
		langpack->SetDescription(engLangPack->GetDescription());
		langpack->SetMissingText(engLangPack->GetMissingText());
		
		entryCount = engLangPack->GetEntryCount();
		
		for(i=0; i<entryCount; i++){
			const deLanguagePackEntry &engEntry = engLangPack->GetEntryAt(i);
			
			entry = new lpeLangPackEntry;
			entry->SetName(engEntry.GetName());
			entry->SetText(engEntry.GetText());
			
			langpack->AddEntry(entry);
			entry->FreeReference();
			entry = NULL;
		}
		
		engLangPack->FreeReference();
		
	}catch(const deException &){
		if(entry){
			entry->FreeReference();
		}
		if(engLangPack){
			engLangPack->FreeReference();
		}
		throw;
	}
}

void lpeLoadSaveLangPack::SaveLangPack(lpeLangPack *langpack, decBaseFileWriter *file){
	if(! langpack || ! file){
		DETHROW(deeInvalidParam);
	}
	
	lpeLangPackBuilder builder(langpack);
	deLanguagePack *temporaryLangPack = NULL;
	
	try{
		temporaryLangPack = langpack->GetEngine()->GetLanguagePackManager()->CreateLanguagePack("", builder);
		pModule->SaveLanguagePack(*file, *temporaryLangPack);
		temporaryLangPack->FreeReference();
		
	}catch(const deException &){
		if(temporaryLangPack){
			temporaryLangPack->FreeReference();
		}
		throw;
	}
}
