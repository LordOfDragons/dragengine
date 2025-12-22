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
	
	UpdateLSLangPacks();
	pRebuildFPListLangPack();
}

lpeLoadSaveSystem::~lpeLoadSaveSystem(){
}



// Management
///////////////

void lpeLoadSaveSystem::UpdateLSLangPacks(){
	deEngine *engine = pWindowMain->GetEngineController().GetEngine();
	deModuleSystem *modSys = engine->GetModuleSystem();
	int m, moduleCount = modSys->GetModuleCount();
	deLoadableModule *loadableModule;
	
	pLSLangPacks.RemoveAll();
	
	// add a new load save langpack for each langpack module found in the engine that is also
	// running and usable therefore
	for(m=0; m<moduleCount; m++){
		loadableModule = modSys->GetModuleAt(m);
		
		if(loadableModule->GetType() != deModuleSystem::emtLanguagePack) continue;
		if(!loadableModule->IsLoaded()) continue;
		
		pLSLangPacks.Add(lpeLoadSaveLangPack::Ref::New(
			(deBaseLanguagePackModule*)loadableModule->GetModule()));
	}
}



lpeLangPack::Ref lpeLoadSaveSystem::LoadLangPack(const char *filename){
	if(!filename) DETHROW(deeInvalidParam);
	
	lpeLoadSaveLangPack &lslp = pLSLangPacks.FindOrDefault([&](const lpeLoadSaveLangPack &l){
		return decString::StringMatchesPattern(filename, l.GetPattern());
	});
	
	const lpeLangPack::Ref langpack(lpeLangPack::Ref::New(&pWindowMain->GetEnvironment()));
	langpack->SetFilePath(filename); // required here so the relative path can be resolved properly
	
	lslp.LoadLangPack(langpack, pWindowMain->GetEnvironment().GetFileSystemGame()->
		OpenFileForReading(decPath::CreatePathUnix(filename)));
	
	langpack->SetSaved(true);
	langpack->SetChanged(false);
	
	return langpack;
}

void lpeLoadSaveSystem::SaveLangPack(lpeLangPack *langpack, const char *filename){
	if(!langpack || !filename) DETHROW(deeInvalidParam);
	
	lpeLoadSaveLangPack &lslp = pLSLangPacks.FindOrDefault([&](const lpeLoadSaveLangPack &l){
		return decString::StringMatchesPattern(filename, l.GetPattern());
	});
	
	langpack->SetFilePath(filename); // required here so the relative path can be resolved properly
	
	lslp.SaveLangPack(langpack, pWindowMain->GetEnvironment().GetFileSystemGame()->
		OpenFileForWriting(decPath::CreatePathUnix(filename)));
	
	langpack->SetSaved(true);
	langpack->SetChanged(false);
}



// Private Functions
//////////////////////

void lpeLoadSaveSystem::pRebuildFPListLangPack(){
	decString pattern;
	
	pFPListLangPack.RemoveAll();
	
	pLSLangPacks.Visit([&](const lpeLoadSaveLangPack &lslp){
		pattern.Format("*%s", lslp.GetPattern().GetString());
		pFPListLangPack.Add(igdeFilePattern::Ref::New(lslp.GetName(), pattern, lslp.GetPattern()));
	});
}
