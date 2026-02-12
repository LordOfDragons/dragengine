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

#include "igdeTranslationManager.h"
#include "../environment/igdeEnvironment.h"
#include "../module/igdeEditorModule.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/filesystem/deCollectFileSearchVisitor.h>


// Class igdeLanguagePack
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeTranslationManager::igdeTranslationManager(igdeEnvironment &environment) :
pEnvironment(environment){
}


// Management
///////////////

void igdeTranslationManager::LoadIgdeLanguagePacks(const decPath &path){
	deLogger &logger = *pEnvironment.GetLogger();
	logger.LogInfo("IGDE", "Loading IGDE language packs...");
	
	LoadLanguagePacks(path, pIgdeLanguagePacks);
	DEASSERT_TRUE(pIgdeLanguagePacks.IsNotEmpty())
	
	const bool hasEnglish = pIgdeLanguagePacks.HasMatching([](const igdeLanguagePack &lp){
		return lp.GetLanguage() == "en";
	});
	DEASSERT_TRUE(hasEnglish)
	
	logger.LogInfo("IGDE", "Found IGDE language packs:");
	pIgdeLanguagePacks.Visit([&](const igdeLanguagePack &lp){
		logger.LogInfoFormat("IGDE", "- %s (%s)", lp.GetLanguage().GetString(), lp.GetName().ToUTF8().GetString());
	});
	
	SetActiveLanguage("en");
}

void igdeTranslationManager::LoadLanguagePacks(const decPath &path,
igdeLanguagePack::List &languagePacks) const{
	deVirtualFileSystem &vfs = *pEnvironment.GetFileSystemIGDE();
	
	deCollectFileSearchVisitor collect("*.delangpack", true);
	vfs.SearchFiles(path, collect);
	
	languagePacks.RemoveAll();
	
	collect.GetFiles().Visit([&](const decPath &file){
		auto languagePack = igdeLanguagePack::Ref::New();
		languagePack->LoadFromFile(*pEnvironment.GetLogger(), vfs.OpenFileForReading(file));
		
		if(!languagePacks.HasMatching([&](const igdeLanguagePack &lp){
			return lp.GetLanguage() == languagePack->GetLanguage();
		})){
			languagePacks.Add(languagePack);
		}
	});
}

void igdeTranslationManager::SetActiveLanguage(const decString &language){
	if(language == pActiveLanguage){
		return;
	}
	
	pActiveLanguagePacks.RemoveAll();
	pActiveLanguage = language;
	
	if(language != "en"){
		// add english language pack as fallback if the selected language misses translations
		pIgdeLanguagePacks.Visit([&](const igdeLanguagePack::Ref &languagePack){
			if(languagePack->GetLanguage() == "en"){
				pActiveLanguagePacks.Add(languagePack);
			}
		});
	}
	
	pIgdeLanguagePacks.Visit([&](const igdeLanguagePack::Ref &languagePack){
		if(languagePack->GetLanguage() == language){
			pActiveLanguagePacks.Add(languagePack);
		}
	});
}

void igdeTranslationManager::AddActiveLanguagePack(const igdeLanguagePack::Ref &languagePack){
	pActiveLanguagePacks.Add(languagePack);
}


decUnicodeString igdeTranslationManager::Translate(const decString &entryName) const{
	return Translate(entryName.GetString());
}

decUnicodeString igdeTranslationManager::Translate(const char *entryName) const{
	const decUnicodeString *result;
	for(const auto &languagePack : pActiveLanguagePacks){
		if(languagePack->GetEntries().GetAt(entryName, result)){
			return *result;
		}
	}
	
	return decUnicodeString::NewFromUTF8(entryName);
}

decUnicodeString igdeTranslationManager::Translate(const decString &entryName,
const decUnicodeString &defaultValue) const{
	return Translate(entryName.GetString(), defaultValue);
}

decUnicodeString igdeTranslationManager::Translate(const char *entryName,
const decUnicodeString &defaultValue) const{
	const decUnicodeString *result;
	for(const auto &languagePack : pActiveLanguagePacks){
		if(languagePack->GetEntries().GetAt(entryName, result)){
			return *result;
		}
	}
	
	return defaultValue;
}

decUnicodeString igdeTranslationManager::TranslateIf(
const decUnicodeString &text, int translationCharacter) const{
	if(!text.IsEmpty() && text.GetAt(0) == translationCharacter){
		return Translate(text.GetRight(text.GetLength() - 1).ToUTF8());
	}
	return text;
}

decUnicodeString igdeTranslationManager::TranslateIf(const char *text, int translationCharacter) const{
	DEASSERT_NOTNULL(text)
	if(text[0] == translationCharacter){
		return Translate(text + 1);
	}
	return decUnicodeString::NewFromUTF8(text);
}

decUnicodeString igdeTranslationManager::TranslateIf(const decUnicodeString &text,
const decUnicodeString &defaultValue, int translationCharacter) const{
	if(!text.IsEmpty() && text.GetAt(0) == translationCharacter){
		return Translate(text.GetRight(text.GetLength() - 1).ToUTF8(), defaultValue);
	}
	return text;
}

decUnicodeString igdeTranslationManager::TranslateIf(const char *text,
const decUnicodeString &defaultValue, int translationCharacter) const{
	DEASSERT_NOTNULL(text)
	if(text[0] == translationCharacter){
		return Translate(text + 1, defaultValue);
	}
	return decUnicodeString::NewFromUTF8(text);
}
