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

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "lpeLoadSaveSystem.h"
#include "lpeLoadSaveLangPack.h"
#include "../langpack/lpeLangPack.h"
#include "../gui/lpeWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gui/filedialog/igdeFilePattern.h>
#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/systems/modules/langpack/deBaseLanguagePackModule.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/exceptions.h>



// Class lpeLoadSaveSystem
///////////////////////////

// Constructor, destructor
////////////////////////////

lpeLoadSaveSystem::lpeLoadSaveSystem(lpeWindowMain *windowMain){
	if(!windowMain){
		DETHROW(deeInvalidParam);
	}
	
	pWindowMain = windowMain;
	
	pLSLangPacks = nullptr;
	pLSLangPackCount = 0;
	pLSLangPackSize = 0;
	
	UpdateLSLangPacks();
	pRebuildFPListLangPack();
}

lpeLoadSaveSystem::~lpeLoadSaveSystem(){
	RemoveAllLSLangPacks();
	
	if(pLSLangPacks){
		delete [] pLSLangPacks;
	}
}



// Management
///////////////

lpeLoadSaveLangPack *lpeLoadSaveSystem::GetLSLangPackAt(int index) const{
	if(index < 0 || index >= pLSLangPackCount){
		DETHROW(deeInvalidParam);
	}
	
	return pLSLangPacks[index];
}

int lpeLoadSaveSystem::IndexOfLSLangPack(lpeLoadSaveLangPack *lsLangPack) const{
	if(!lsLangPack){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	
	for(i=0; i<pLSLangPackCount; i++){
		if(lsLangPack == pLSLangPacks[i]){
			return i;
		}
	}
	
	return -1;
}

bool lpeLoadSaveSystem::HasLSLangPack(lpeLoadSaveLangPack *lsLangPack) const{
	if(!lsLangPack){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	
	for(i=0; i<pLSLangPackCount; i++){
		if(lsLangPack == pLSLangPacks[i]){
			return true;
		}
	}
	
	return false;
}

int lpeLoadSaveSystem::IndexOfLSLangPackMatching(const char *filename){
	const decString testFilename(filename);
	int i;
	
	for(i=0; i<pLSLangPackCount; i++){
		if(testFilename.MatchesPattern(pLSLangPacks[i]->GetPattern())){
			return i;
		}
	}
	
	return -1;
}

void lpeLoadSaveSystem::AddLSLangPack(lpeLoadSaveLangPack *lsLangPack){
	if(HasLSLangPack(lsLangPack)){
		DETHROW(deeInvalidParam);
	}
	
	if(pLSLangPackCount == pLSLangPackSize){
		int newSize = pLSLangPackSize * 3 / 2 + 1;
		lpeLoadSaveLangPack **newArray = new lpeLoadSaveLangPack*[newSize];		if(pLSLangPacks){
			memcpy(newArray, pLSLangPacks, sizeof(lpeLoadSaveLangPack*) * pLSLangPackSize);
			delete [] pLSLangPacks;
		}
		pLSLangPacks = newArray;
		pLSLangPackSize = newSize;
	}
	
	pLSLangPacks[pLSLangPackCount] = lsLangPack;
	pLSLangPackCount++;
}

void lpeLoadSaveSystem::RemoveLSLangPack(lpeLoadSaveLangPack *lsLangPack){
	int i, index = IndexOfLSLangPack(lsLangPack);
	if(index == -1) DETHROW(deeInvalidParam);
	
	for(i=index+1; i<pLSLangPackCount; i++){
		pLSLangPacks[i - 1] = pLSLangPacks[i];
	}
	pLSLangPackCount--;
	
	delete lsLangPack;
}

void lpeLoadSaveSystem::RemoveAllLSLangPacks(){
	while(pLSLangPackCount > 0){
		pLSLangPackCount--;
		delete pLSLangPacks[pLSLangPackCount];
	}
}

void lpeLoadSaveSystem::UpdateLSLangPacks(){
	deEngine *engine = pWindowMain->GetEngineController().GetEngine();
	deModuleSystem *modSys = engine->GetModuleSystem();
	int m, moduleCount = modSys->GetModuleCount();
	deLoadableModule *loadableModule;
	lpeLoadSaveLangPack *lsLangPack = nullptr;
	
	// remove all load save langpacks
	RemoveAllLSLangPacks();
	
	try{
		// add a new load save langpack for each langpack module found in the engine that is also
		// running and usable therefore
		for(m=0; m<moduleCount; m++){
			loadableModule = modSys->GetModuleAt(m);
			
			if(loadableModule->GetType() != deModuleSystem::emtLanguagePack) continue;
			if(!loadableModule->IsLoaded()) continue;
			
			lsLangPack = new lpeLoadSaveLangPack((deBaseLanguagePackModule*)loadableModule->GetModule());
			if(!lsLangPack) DETHROW(deeOutOfMemory);
			
			AddLSLangPack(lsLangPack);
			lsLangPack = NULL;
		}
		
	}catch(const deException &){
		if(lsLangPack) delete lsLangPack;
		throw;
	}
}



lpeLangPack::Ref lpeLoadSaveSystem::LoadLangPack(const char *filename){
	if(!filename) DETHROW(deeInvalidParam);
	const int lsIndex = IndexOfLSLangPackMatching(filename);
	if(lsIndex == -1) DETHROW(deeInvalidParam);
	
	const lpeLangPack::Ref langpack(lpeLangPack::Ref::New(&pWindowMain->GetEnvironment()));
	langpack->SetFilePath(filename); // required here so the relative path can be resolved properly
	
	pLSLangPacks[lsIndex]->LoadLangPack(langpack, pWindowMain->GetEnvironment().
		GetFileSystemGame()->OpenFileForReading(decPath::CreatePathUnix(filename)));
	
	langpack->SetSaved(true);
	langpack->SetChanged(false);
	
	return langpack;
}

void lpeLoadSaveSystem::SaveLangPack(lpeLangPack *langpack, const char *filename){
	if(!langpack || !filename) DETHROW(deeInvalidParam);
	const int lsIndex = IndexOfLSLangPackMatching(filename);
	if(lsIndex == -1) DETHROW(deeInvalidParam);
	
	langpack->SetFilePath(filename); // required here so the relative path can be resolved properly
	
	pLSLangPacks[lsIndex]->SaveLangPack(langpack, pWindowMain->GetEnvironment().
		GetFileSystemGame()->OpenFileForWriting(decPath::CreatePathUnix(filename)));
	
	langpack->SetSaved(true);
	langpack->SetChanged(false);
}



// Private Functions
//////////////////////

void lpeLoadSaveSystem::pRebuildFPListLangPack(){
	igdeFilePattern *filePattern = nullptr;
	decString pattern;
	int i;
	
	pFPListLangPack.RemoveAllFilePatterns();
	
	try{
		for(i=0; i<pLSLangPackCount; i++){
			pattern.Format("*%s", pLSLangPacks[i]->GetPattern().GetString());
			
			filePattern = new igdeFilePattern(pLSLangPacks[i]->GetName(),
				pattern, pLSLangPacks[i]->GetPattern());
			
			pFPListLangPack.AddFilePattern(filePattern);
			filePattern = nullptr;
		}
		
	}catch(const deException &){
		if(filePattern){
			delete filePattern;
		}
		throw;
	}
}
