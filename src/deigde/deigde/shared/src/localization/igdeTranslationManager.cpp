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

const char *igdeTranslationManager::FallbackLanguage = "en";


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
	DEASSERT_NOTNULL(pIgdeLanguagePacks.FindByLanguage(FallbackLanguage))
	
	logger.LogInfo("IGDE", "Found IGDE language packs:");
	pIgdeLanguagePacks.Visit([&](const igdeLanguagePack &lp){
		logger.LogInfoFormat("IGDE", "- %s (%s)", lp.GetLanguage().GetString(), lp.GetName().ToUTF8().GetString());
	});
	
	SetActiveLanguage(FallbackLanguage);
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
		
		if(!languagePacks.FindByLanguage(languagePack->GetLanguage())){
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
	
	if(language != FallbackLanguage){
		// add english language pack as fallback if the selected language misses translations
		auto found = pIgdeLanguagePacks.FindByLanguage(FallbackLanguage);
		if(found){
			pActiveLanguagePacks.Add(found);
		}
	}
	
	auto found = pIgdeLanguagePacks.FindByLanguage(language);
	if(found){
		pActiveLanguagePacks.Add(found);
	}
}

void igdeTranslationManager::AddActiveLanguagePack(const igdeLanguagePack::Ref &languagePack){
	pActiveLanguagePacks.Add(languagePack);
}


decUnicodeString igdeTranslationManager::Translate(const decString &entryName) const{
	return Translate(entryName.GetString());
}

decUnicodeString igdeTranslationManager::Translate(const char *entryName) const{
	const decUnicodeString *result = nullptr;
	pActiveLanguagePacks.VisitWhileReverse([&](const igdeLanguagePack &lp){
		return !lp.GetEntries().GetAt(entryName, result);
	});
	return result ? *result : decUnicodeString::NewFromUTF8(entryName);
}

decUnicodeString igdeTranslationManager::Translate(const decString &entryName,
const decUnicodeString &defaultValue) const{
	return Translate(entryName.GetString(), defaultValue);
}

decUnicodeString igdeTranslationManager::Translate(const char *entryName,
const decUnicodeString &defaultValue) const{
	const decUnicodeString *result = nullptr;
	pActiveLanguagePacks.VisitWhileReverse([&](const igdeLanguagePack &lp){
		return !lp.GetEntries().GetAt(entryName, result);
	});
	return result ? *result : defaultValue;
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
