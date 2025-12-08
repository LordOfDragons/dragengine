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

#include "meLoadSaveSystem.h"
#include "meLSWorld.h"
#include "meLSXMLWorld.h"
#include "heightterrain/meLSHeightTerrain.h"
#include "heightterrain/meLSXMLHeightTerrain.h"
#include "pfcache/meLSPFCache.h"
#include "meLoadSaveNavTest.h"
#include "../world/meWorld.h"
#include "../world/terrain/meHeightTerrain.h"
#include "../gui/meWindowMain.h"

#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/filedialog/igdeFilePattern.h>
#include <deigde/gui/igdeStepableTask.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/engine/igdeEngineController.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/systems/modules/skin/deBaseSkinModule.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define LOGSOURCE "World Editor"



// class meLoadSaveSystem
///////////////////////////

// Constructor, destructor
////////////////////////////

meLoadSaveSystem::meLoadSaveSystem(meWindowMain *wndMain){
	if(!wndMain) DETHROW(deeInvalidParam);
	
	pWndMain = wndMain;
	
	pLSWorlds = NULL;
	pLSWorldCount = 0;
	pLSWorldSize = 0;
	
	pLSHTs = NULL;
	pLSHTCount = 0;
	pLSHTSize = 0;
	
	pLSPFCache = NULL;
	
	pLSNavTest = NULL;
	
	try{
		AddLSWorld(new meLSXMLWorld(this));
		AddLSHeightTerrain(new meLSXMLHeightTerrain(this));
		
		pLSPFCache = new meLSPFCache;
		
		pLSNavTest = new meLoadSaveNavTest(this, wndMain->GetEnvironment().GetLogger(), LOGSOURCE);
		
		pRebuildFilePatternLists();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meLoadSaveSystem::~meLoadSaveSystem(){
	pCleanUp();
}
	


// World Management
/////////////////////

meLSWorld *meLoadSaveSystem::GetLSWorldAt(int index) const{
	if(index < 0 || index >= pLSWorldCount) DETHROW(deeInvalidParam);
	return pLSWorlds[index];
}

meLSWorld *meLoadSaveSystem::FindLSWorldMatching(const char *filename) const{
	if(!filename) DETHROW(deeInvalidParam);
	// hack!
	return pLSWorlds[0];
}

void meLoadSaveSystem::AddLSWorld(meLSWorld *lsWorld){
	if(!lsWorld) DETHROW(deeInvalidParam);
	
	if(pLSWorldCount == pLSWorldSize){
		int i, newSize = pLSWorldSize * 3 / 2 + 1;
		meLSWorld **newArray = new meLSWorld*[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
		if(pLSWorlds){
			for(i=0; i<pLSWorldSize; i++) newArray[i] = pLSWorlds[i];
			delete [] pLSWorlds;
		}
		pLSWorlds = newArray;
		pLSWorldSize = newSize;
	}
	
	pLSWorlds[pLSWorldCount] = lsWorld;
	pLSWorldCount++;
}

meWorld *meLoadSaveSystem::LoadWorld(const char *filename,
igdeGameDefinition *gameDefinition, igdeStepableTask **task){
	if(!filename || !gameDefinition || !task){
		DETHROW(deeInvalidParam);
	}
	
	meLSWorld * const lsWorld = FindLSWorldMatching(filename);
	if(!lsWorld){
		DETHROW(deeInvalidParam);
	}
	
	meWorld * const world.TakeOver(new meWorld(*pWndMain, &pWndMain->GetEnvironment()));
	world->SetFilePath(filename);
	try{
		*task = lsWorld->CreateLoadTask(world, decBaseFileReader::Ref::New(
			pWndMain->GetEnvironment().GetFileSystemGame()->OpenFileForReading(
				decPath::CreatePathUnix(filename))));
		return world;
		
	}catch(const deException &){
		throw;
	}
}

void meLoadSaveSystem::SaveWorld(meWorld::Ref world, const char *filename){
	if(!world || !filename) DETHROW(deeInvalidParam);
	
	meLSWorld * const lsWorld = FindLSWorldMatching(filename);
	if(!lsWorld) DETHROW(deeInvalidParam); // hack
	
	lsWorld->SaveWorld(*this, *world, decBaseFileWriter::Ref::New(
		pWndMain->GetEnvironment().GetFileSystemGame()->OpenFileForWriting(
			decPath::CreatePathUnix(filename))));
}



// Height Terrain Management
//////////////////////////////

meLSHeightTerrain *meLoadSaveSystem::GetLSHeightTerrainAt(int index) const{
	if(index < 0 || index >= pLSHTCount) DETHROW(deeInvalidParam);
	
	return pLSHTs[index];
}

meLSHeightTerrain *meLoadSaveSystem::FindLSHeightTerrainMatching(const char *filename) const{
	if(!filename) DETHROW(deeInvalidParam);
	
	// hack!
	return pLSHTs[0];
}

void meLoadSaveSystem::AddLSHeightTerrain(meLSHeightTerrain *loader){
	if(!loader) DETHROW(deeInvalidParam);
	
	if(pLSHTCount == pLSHTSize){
		int i, newSize = pLSHTSize * 3 / 2 + 1;
		meLSHeightTerrain **newArray = new meLSHeightTerrain*[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
		if(pLSHTs){
			for(i=0; i<pLSHTSize; i++) newArray[i] = pLSHTs[i];
			delete [] pLSHTs;
		}
		pLSHTs = newArray;
		pLSHTSize = newSize;
	}
	
	pLSHTs[pLSHTCount] = loader;
	pLSHTCount++;
}

void meLoadSaveSystem::LoadHeightTerrain(meHeightTerrain &heightTerrain, const char *filename){
	meLSHeightTerrain * const loader = FindLSHeightTerrainMatching(filename);
	if(!loader){
		DETHROW(deeInvalidParam);
	}
	
	loader->LoadFromFile(heightTerrain, decBaseFileReader::Ref::New(
		pWndMain->GetEnvironment().GetFileSystemGame()->OpenFileForReading(
			decPath::CreatePathUnix(filename))));
}

void meLoadSaveSystem::SaveHeightTerrain(meHeightTerrain &heightTerrain, const char *filename){
	meLSHeightTerrain * const loader = FindLSHeightTerrainMatching(filename);
	if(!loader){
		DETHROW(deeInvalidParam);
	}
	
	loader->SaveToFile(heightTerrain, decBaseFileWriter::Ref::New(
		pWndMain->GetEnvironment().GetFileSystemGame()->OpenFileForWriting(
			decPath::CreatePathUnix(filename))));
}



// Prop Field Cache
/////////////////////

void meLoadSaveSystem::LoadPFCache(meHeightTerrainSector &sector, const char *filename){
	pLSPFCache->LoadFromFile(sector, decBaseFileReader::Ref::New(
		pWndMain->GetEnvironment().GetFileSystemGame()->OpenFileForReading(
			decPath::CreatePathUnix(filename))));
}

void meLoadSaveSystem::SavePFCache(meHeightTerrainSector &sector, const char *filename){
	pLSPFCache->SaveToFile(sector, decBaseFileWriter::Ref::New(
		pWndMain->GetEnvironment().GetFileSystemGame()->OpenFileForWriting(
			decPath::CreatePathUnix(filename))));
}



void meLoadSaveSystem::LoadNavTest(const char *filename, meWorld &world){
	if(!filename){
		DETHROW(deeInvalidParam);
	}
	
	pLSNavTest->LoadNavTest(world, decBaseFileReader::Ref::New(
		pWndMain->GetEnvironment().GetFileSystemGame()->OpenFileForReading(
			decPath::CreatePathUnix(filename))));
}

void meLoadSaveSystem::SaveNavTest(const char *filename, meWorld &world){
	if(!filename){
		DETHROW(deeInvalidParam);
	}
	
	pLSNavTest->SaveNavTest(world, decBaseFileWriter::Ref::New(
		pWndMain->GetEnvironment().GetFileSystemGame()->OpenFileForWriting(
			decPath::CreatePathUnix(filename))));
}



// Private Functions
//////////////////////

void meLoadSaveSystem::pCleanUp(){
	if(pLSNavTest){
		delete pLSNavTest;
	}
	
	if(pLSPFCache) delete pLSPFCache;
	
	if(pLSHTs){
		while(pLSHTCount > 0){
			delete pLSHTs[pLSHTCount - 1];
			pLSHTCount--;
		}
		delete [] pLSHTs;
	}
	
	if(pLSWorlds){
		while(pLSWorldCount > 0){
			delete pLSWorlds[pLSWorldCount - 1];
			pLSWorldCount--;
		}
		delete [] pLSWorlds;
	}
}

void meLoadSaveSystem::pRebuildFilePatternLists(){
//	char *newPattern = NULL;
//	const char *lsPattern;
//	int lenFDPattern = 0;
//	const char *lsName;
//	int lenLSPattern;
//	int lenLSName;
//	int newLen;
	
	igdeFilePattern *filePattern = NULL;
	decString pattern;
	int i;
	
	pFilePatternList.RemoveAllFilePatterns();
	
	try{
		for(i=0; i<pLSWorldCount; i++){
			pattern.Format("*%s", pLSWorlds[i]->GetPattern().GetString());
			filePattern = new igdeFilePattern(pLSWorlds[i]->GetName(), pattern, pLSWorlds[i]->GetPattern());
			pFilePatternList.AddFilePattern(filePattern);
			filePattern = NULL;
		}
		
	}catch(const deException &){
		if(filePattern){
			delete filePattern;
		}
		throw;
	}
	
	/*
	newPattern = new char[1];
	if(!newPattern) DETHROW(deeOutOfMemory);
	newPattern[0] = '\0';
	
	if(pFDPattern) delete [] pFDPattern;
	pFDPattern = newPattern;
	
	for(i=0; i<pLSSkinCount; i++){
		lsName = pLSSkins[i]->GetName();
		lenLSName = strlen(lsName);
		lsPattern = pLSSkins[i]->GetPattern();
		lenLSPattern = strlen(lsPattern) + 1;
		
		newLen = lenFDPattern + lenLSName + lenLSPattern + 3;
		if(i > 0) newLen++;
		
		newPattern = new char[newLen + 1];
		if(!newPattern) DETHROW(deeOutOfMemory);
		
		if(i > 0){
			sprintf(newPattern, "\n%s (*%s)", lsName, lsPattern);
			
		}else{
			sprintf(newPattern, "%s (*%s)", lsName, lsPattern);
		}
		
		if(pFDPattern) delete [] pFDPattern;
		pFDPattern = newPattern;
		lenFDPattern = newLen;
	}
	*/
	
	
	// build prop field cache file patterns
	try{
		pattern.Format("*%s", pLSPFCache->GetPattern().GetString());
		filePattern = new igdeFilePattern(pLSPFCache->GetName(), pattern, pLSPFCache->GetPattern());
		pPropFieldCacheFilePatterns.AddFilePattern(filePattern);
		
		pattern.Format("*%s", pLSNavTest->GetPattern().GetString());
		filePattern = new igdeFilePattern(pLSNavTest->GetName(), pattern, pLSNavTest->GetPattern());
		pFPNavTest.AddFilePattern(filePattern);
		
	}catch(const deException &){
		if(filePattern){
			delete filePattern;
		}
		throw;
	}
}
