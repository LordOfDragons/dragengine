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

#include "seLoadSaveSystem.h"
#include "seLoadSaveSkin.h"
#include "../skin/seSkin.h"
#include "../gui/seWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/systems/modules/skin/deBaseSkinModule.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/exceptions.h>



// Class seLoadSaveSystem
///////////////////////////

// Constructor, destructor
////////////////////////////

seLoadSaveSystem::seLoadSaveSystem(seWindowMain &windowMain) :
pWindowMain(windowMain)
{
	pLSSkins = NULL;
	pLSSkinCount = 0;
	pLSSkinSize = 0;
	
	UpdateLSSkins();
}

seLoadSaveSystem::~seLoadSaveSystem(){
	RemoveAllLSSkins();
	if(pLSSkins) delete [] pLSSkins;
}



// Management
///////////////

seLoadSaveSkin *seLoadSaveSystem::GetLSSkinAt(int index) const{
	if(index < 0 || index >= pLSSkinCount) DETHROW(deeInvalidParam);
	
	return pLSSkins[index];
}

int seLoadSaveSystem::IndexOfLSSkin(seLoadSaveSkin *lsSkin) const{
	if(!lsSkin) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pLSSkinCount; i++){
		if(lsSkin == pLSSkins[i]){
			return i;
		}
	}
	
	return -1;
}

bool seLoadSaveSystem::HasLSSkin(seLoadSaveSkin *lsSkin) const{
	if(!lsSkin) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pLSSkinCount; i++){
		if(lsSkin == pLSSkins[i]){
			return true;
		}
	}
	
	return false;
}

int seLoadSaveSystem::IndexOfLSSkinMatching(const char *filename){
	const decString testFilename(filename);
	int i;
	
	for(i=0; i<pLSSkinCount; i++){
		if(testFilename.MatchesPattern(pLSSkins[i]->GetPattern())){
			return i;
		}
	}
	
	return -1;
}

void seLoadSaveSystem::AddLSSkin(seLoadSaveSkin *lsSkin){
	if(HasLSSkin(lsSkin)) DETHROW(deeInvalidParam);
	
	if(pLSSkinCount == pLSSkinSize){
		int newSize = pLSSkinSize * 3 / 2 + 1;
		seLoadSaveSkin **newArray = new seLoadSaveSkin*[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
		if(pLSSkins){
			memcpy(newArray, pLSSkins, sizeof(seLoadSaveSkin*) * pLSSkinSize);
			delete [] pLSSkins;
		}
		pLSSkins = newArray;
		pLSSkinSize = newSize;
	}
	
	pLSSkins[pLSSkinCount] = lsSkin;
	pLSSkinCount++;
}

void seLoadSaveSystem::RemoveLSSkin(seLoadSaveSkin *lsSkin){
	int i, index = IndexOfLSSkin(lsSkin);
	if(index == -1) DETHROW(deeInvalidParam);
	
	for(i=index+1; i<pLSSkinCount; i++){
		pLSSkins[i - 1] = pLSSkins[i];
	}
	pLSSkinCount--;
	
	delete lsSkin;
}

void seLoadSaveSystem::RemoveAllLSSkins(){
	while(pLSSkinCount > 0){
		pLSSkinCount--;
		delete pLSSkins[pLSSkinCount];
	}
}

void seLoadSaveSystem::UpdateLSSkins(){
	deEngine *engine = pWindowMain.GetEngineController().GetEngine();
	deModuleSystem *modSys = engine->GetModuleSystem();
	int m, moduleCount = modSys->GetModuleCount();
	deLoadableModule *loadableModule;
	seLoadSaveSkin *lsSkin = NULL;
	
	// remove all load save skins
	RemoveAllLSSkins();
	
	try{
		// add a new load save skin for each skin module found in the engine that is also
		// running and usable therefore
		for(m=0; m<moduleCount; m++){
			loadableModule = modSys->GetModuleAt(m);
			
			if(loadableModule->GetType() != deModuleSystem::emtSkin) continue;
			if(!loadableModule->IsLoaded()) continue;
			
			lsSkin = new seLoadSaveSkin((deBaseSkinModule*)loadableModule->GetModule());
			if(!lsSkin) DETHROW(deeOutOfMemory);
			
			AddLSSkin(lsSkin);
			lsSkin = NULL;
		}
		
	}catch(const deException &){
		if(lsSkin) delete lsSkin;
		throw;
	}
}



seSkin *seLoadSaveSystem::LoadSkin(const char *filename, igdeGameDefinition *gameDefinition){
	if(!filename || !gameDefinition) DETHROW(deeInvalidParam);
	deEngine *engine = pWindowMain.GetEngineController().GetEngine();
	decBaseFileReader *fileReader = NULL;
	seSkin *skin = NULL;
	decPath path;
	int lsIndex;
	
	lsIndex = IndexOfLSSkinMatching(filename);
	if(lsIndex == -1) DETHROW(deeInvalidParam);
	
	path.SetFromUnix(filename);
	
	try{
		fileReader = engine->GetVirtualFileSystem()->OpenFileForReading(path);
		
		skin = new seSkin(&pWindowMain.GetEnvironment());
		skin->SetFilePath(filename); // required here so the relative path can be resolved properly
		
		pLSSkins[lsIndex]->LoadSkin(skin, fileReader, *pWindowMain.GetEnvironment().GetTexturePropertyList());
		fileReader->FreeReference();
	
	}catch(const deException &){
		throw;
	}
	
	skin->SetSaved(true);
	skin->SetChanged(false);
	
	return skin;
}

void seLoadSaveSystem::SaveSkin(seSkin *skin, const char *filename){
	if(!skin || !filename) DETHROW(deeInvalidParam);
	deEngine *engine = pWindowMain.GetEngineController().GetEngine();
	decBaseFileWriter *fileWriter = NULL;
	decPath path;
	int lsIndex;
	
	lsIndex = IndexOfLSSkinMatching(filename);
	if(lsIndex == -1) DETHROW(deeInvalidParam);
	
	path.SetFromUnix(filename);
	
	try{
		fileWriter = engine->GetVirtualFileSystem()->OpenFileForWriting(path);
		
		skin->SetFilePath(filename); // required here so the relative path can be resolved properly
		
		pLSSkins[lsIndex]->SaveSkin(skin, fileWriter);
		
		fileWriter->FreeReference();
		
	}catch(const deException &){
		throw;
	}
	
	skin->SetSaved(true);
	skin->SetChanged(false);
}
