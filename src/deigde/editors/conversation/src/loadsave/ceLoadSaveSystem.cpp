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

#include "ceLoadSaveSystem.h"
#include "ceLoadSaveConversation.h"
#include "ceLoadSaveCTS.h"
#include "ceLoadSaveCTA.h"
#include "ceLoadSaveCTGS.h"
#include "ceLoadSaveLangPack.h"
#include "../conversation/ceConversation.h"
#include "../gui/ceWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/filedialog/igdeFilePattern.h>
#include <deigde/gui/filedialog/igdeFilePatternList.h>
#include <deigde/module/igdeEditorModule.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/filesystem/dePatternList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/systems/modules/langpack/deBaseLanguagePackModule.h>



// Class ceLoadSaveSystem
////////////////////////////

// Constructor, destructor
////////////////////////////

ceLoadSaveSystem::ceLoadSaveSystem(ceWindowMain &windowMain) :
pWindowMain(windowMain),

pLSConversation(nullptr),
pLSCTS(nullptr),
pLSCTA(nullptr),
	pLSCTGS(nullptr)
{
	deLogger *const logger = windowMain.GetEnvironment().GetLogger();
	const decString &loggingName = windowMain.GetEditorModule().GetLoggingName();
	
	pLSConversation = new ceLoadSaveConversation(this, logger, loggingName);
	pLSCTS = new ceLoadSaveCTS(this, logger, loggingName);
	pLSCTA = new ceLoadSaveCTA(*this, logger, loggingName);
	pLSCTGS = new ceLoadSaveCTGS(*this, logger, loggingName);
	
	UpdateLSLangPacks();
	pBuildFilePattern();
}

ceLoadSaveSystem::~ceLoadSaveSystem(){
	if(pLSCTGS){
		delete pLSCTGS;
	}
	if(pLSCTA){
		delete pLSCTA;
	}
	if(pLSCTS){
		delete pLSCTS;
	}
	if(pLSConversation){
		delete pLSConversation;
	}
}



// Management
///////////////

ceConversation::Ref ceLoadSaveSystem::LoadConversation(const char *filename){
	ceConversation::Ref conversation;
	
	try{
		conversation = ceConversation::Ref::New(&pWindowMain.GetEnvironment());
		pLSConversation->LoadConversation(*conversation, pWindowMain.GetEnvironment().
			GetFileSystemGame()->OpenFileForReading(decPath::CreatePathUnix(filename)), filename);
		
	}catch(const deException &){
		throw;
	}
	
	return conversation;
}

void ceLoadSaveSystem::SaveConversation(ceConversation *conversation, const char *filename){
	pLSConversation->SaveConversation(*conversation, pWindowMain.GetEnvironment().
		GetFileSystemGame()->OpenFileForWriting(decPath::CreatePathUnix(filename)));
}



void ceLoadSaveSystem::LoadCTS(const char *filename, ceConversation &conversation){
	pLSCTS->LoadCTS(conversation, pWindowMain.GetEnvironment().GetFileSystemGame()->
		OpenFileForReading(decPath::CreatePathUnix(filename)));
}

void ceLoadSaveSystem::SaveCTS(const char *filename, ceConversation &conversation){
	pLSCTS->SaveCTS(conversation, pWindowMain.GetEnvironment().GetFileSystemGame()->
		OpenFileForWriting(decPath::CreatePathUnix(filename)));
}



void ceLoadSaveSystem::LoadCTA(const char *filename, ceConversationActor &actor){
	pLSCTA->LoadCTA(actor, pWindowMain.GetEnvironment().GetFileSystemGame()->
		OpenFileForReading(decPath::CreatePathUnix(filename)));
}

void ceLoadSaveSystem::SaveCTA(const char *filename, ceConversationActor &actor){
	pLSCTA->SaveCTA(actor, pWindowMain.GetEnvironment().GetFileSystemGame()->
		OpenFileForWriting(decPath::CreatePathUnix(filename)));
}



void ceLoadSaveSystem::LoadCTGS(const char *filename, ceConversation &conversation){
	pLSCTGS->LoadCTGS(conversation, pWindowMain.GetEnvironment().GetFileSystemGame()->
		OpenFileForReading(decPath::CreatePathUnix(filename)));
}

void ceLoadSaveSystem::SaveCTGS(const char *filename, const ceConversation &conversation){
	pLSCTGS->SaveCTGS(conversation, pWindowMain.GetEnvironment().GetFileSystemGame()->
		OpenFileForWriting(decPath::CreatePathUnix(filename)));
}



void ceLoadSaveSystem::UpdateLSLangPacks(){
	const deEngine &engine = *pWindowMain.GetEngineController().GetEngine();
	const deModuleSystem &modsys = *engine.GetModuleSystem();
	const int count = modsys.GetModuleCount();
	int i;
	
	pLSLangPacks.RemoveAll();
	
	for(i=0; i<count; i++){
		const deLoadableModule &lmodule = *modsys.GetModuleAt(i);
		
		if(lmodule.GetType() == deModuleSystem::emtLanguagePack && lmodule.IsLoaded()){
			pLSLangPacks.Add(ceLoadSaveLangPack::Ref::New(
				*((deBaseLanguagePackModule*)lmodule.GetModule())));
		}
	}
}

ceLangPack::Ref ceLoadSaveSystem::LoadLangPack(const char *filename){
	const ceLangPack::Ref langpack(ceLangPack::Ref::New(filename));
	
	pLSLangPacks.FindOrDefault([&](const ceLoadSaveLangPack &lp){
		return decString::StringMatchesPattern(filename, lp.GetPattern());
	})->LoadLangPack(langpack, pWindowMain.GetEnvironment().GetFileSystemGame()->OpenFileForReading(
		decPath::CreatePathUnix(filename)));
	
	return langpack;
}

void ceLoadSaveSystem::SaveLangPack(ceLangPack &langpack){
	pLSLangPacks.FindOrDefault([&](const ceLoadSaveLangPack &lp){
		return langpack.GetPath().MatchesPattern(lp.GetPattern());
	})->SaveLangPack(langpack, pWindowMain.GetEnvironment().GetFileSystemGame()->OpenFileForWriting(
		decPath::CreatePathUnix(langpack.GetPath())));
	
	langpack.SetChanged(false);
}



// Private Functions
//////////////////////

void ceLoadSaveSystem::pBuildFilePattern(){
	igdeFilePattern *filePattern = nullptr;
	decString pattern;
	
	try{
		pattern.Format("*%s", pLSConversation->GetPattern().GetString());
		filePattern = new igdeFilePattern(pLSConversation->GetName(),
			pattern, pLSConversation->GetPattern());
		pFPConversation.AddFilePattern(filePattern);
		filePattern = nullptr;
		
		pattern.Format("*%s", pLSCTS->GetPattern().GetString());
		filePattern = new igdeFilePattern(pLSCTS->GetName(), pattern, pLSCTS->GetPattern());
		pFPCTS.AddFilePattern(filePattern);
		filePattern = nullptr;
		
		pattern.Format("*%s", pLSCTA->GetPattern().GetString());
		filePattern = new igdeFilePattern(pLSCTA->GetName(), pattern, pLSCTA->GetPattern());
		pFPCTA.AddFilePattern(filePattern);
		filePattern = nullptr;
		
		pattern.Format("*%s", pLSCTGS->GetPattern().GetString());
		filePattern = new igdeFilePattern(pLSCTGS->GetName(), pattern, pLSCTGS->GetPattern());
		pFPCTGS.AddFilePattern(filePattern);
		filePattern = nullptr;
		
		// language pack
		pFPListLangPack.RemoveAllFilePatterns();
		
		pLSLangPacks.Visit([&](const ceLoadSaveLangPack &lslp){
			pattern.Format("*%s", lslp.GetPattern().GetString());
			filePattern = new igdeFilePattern(lslp.GetName(), pattern, lslp.GetPattern());
			pFPListLangPack.AddFilePattern(filePattern);
			filePattern = nullptr;
		});
		
	}catch(const deException &){
		if(filePattern){
			delete filePattern;
		}
		throw;
	}
}
