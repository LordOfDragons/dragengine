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
	
	pLSPFCache = nullptr;
	
	pLSNavTest = nullptr;
	
	try{
		AddLSWorld(meLSXMLWorld::Ref::New(this));
		AddLSHeightTerrain(meLSXMLHeightTerrain::Ref::New(this));
		
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

meLSWorld *meLoadSaveSystem::FindLSWorldMatching(const char *filename) const{
	DEASSERT_NOTNULL(filename)
	
	return pLSWorlds.FindOrDefault([&](meLSWorld *lsWorld){
		return decString::StringMatchesPattern(filename, lsWorld->GetPattern());
	});
}

void meLoadSaveSystem::AddLSWorld(meLSWorld *lsWorld){
	DEASSERT_NOTNULL(lsWorld)
	pLSWorlds.Add(lsWorld);
}

meWorld::Ref meLoadSaveSystem::LoadWorld(const char *filename,
igdeGameDefinition *gameDefinition, igdeStepableTask::Ref &task){
	DEASSERT_NOTNULL(filename)
	DEASSERT_NOTNULL(gameDefinition)
	
	meLSWorld * const lsWorld = FindLSWorldMatching(filename);
	DEASSERT_NOTNULL(lsWorld)
	
	const meWorld::Ref world(meWorld::Ref::New(*pWndMain, &pWndMain->GetEnvironment()));
	world->SetFilePath(filename);
	task = lsWorld->CreateLoadTask(world, pWndMain->GetEnvironment().GetFileSystemGame()->
		OpenFileForReading(decPath::CreatePathUnix(filename)));
	return world;
}

void meLoadSaveSystem::SaveWorld(meWorld *world, const char *filename){
	DEASSERT_NOTNULL(world)
	
	meLSWorld * const lsWorld = FindLSWorldMatching(filename);
	DEASSERT_NOTNULL(lsWorld)
	
	lsWorld->SaveWorld(*this, *world, pWndMain->GetEnvironment().GetFileSystemGame()->
		OpenFileForWriting(decPath::CreatePathUnix(filename)));
}



// Height Terrain Management
//////////////////////////////

meLSHeightTerrain *meLoadSaveSystem::FindLSHeightTerrainMatching(const char *filename) const{
	DEASSERT_NOTNULL(filename)
	
	return pLSHTs.FindOrDefault([&](meLSHeightTerrain *lsHT){
		return decString::StringMatchesPattern(filename, lsHT->GetPattern());
	});
}

void meLoadSaveSystem::AddLSHeightTerrain(meLSHeightTerrain *loader){
	DEASSERT_NOTNULL(loader)
	pLSHTs.Add(loader);
}

void meLoadSaveSystem::LoadHeightTerrain(meHeightTerrain &heightTerrain, const char *filename){
	meLSHeightTerrain * const loader = FindLSHeightTerrainMatching(filename);
	DEASSERT_NOTNULL(loader)
	
	loader->LoadFromFile(heightTerrain, pWndMain->GetEnvironment().GetFileSystemGame()->
		OpenFileForReading(decPath::CreatePathUnix(filename)));
}

void meLoadSaveSystem::SaveHeightTerrain(meHeightTerrain &heightTerrain, const char *filename){
	meLSHeightTerrain * const loader = FindLSHeightTerrainMatching(filename);
	if(!loader){
		DETHROW(deeInvalidParam);
	}
	
	loader->SaveToFile(heightTerrain, pWndMain->GetEnvironment().GetFileSystemGame()->
		OpenFileForWriting(decPath::CreatePathUnix(filename)));
}



// Prop Field Cache
/////////////////////

void meLoadSaveSystem::LoadPFCache(meHeightTerrainSector &sector, const char *filename){
	pLSPFCache->LoadFromFile(sector, pWndMain->GetEnvironment().GetFileSystemGame()->
		OpenFileForReading(decPath::CreatePathUnix(filename)));
}

void meLoadSaveSystem::SavePFCache(meHeightTerrainSector &sector, const char *filename){
	pLSPFCache->SaveToFile(sector, pWndMain->GetEnvironment().GetFileSystemGame()->
		OpenFileForWriting(decPath::CreatePathUnix(filename)));
}



void meLoadSaveSystem::LoadNavTest(const char *filename, meWorld &world){
	pLSNavTest->LoadNavTest(world, pWndMain->GetEnvironment().GetFileSystemGame()->
		OpenFileForReading(decPath::CreatePathUnix(filename)));
}

void meLoadSaveSystem::SaveNavTest(const char *filename, meWorld &world){
	pLSNavTest->SaveNavTest(world, pWndMain->GetEnvironment().GetFileSystemGame()->
		OpenFileForWriting(decPath::CreatePathUnix(filename)));
}



// Private Functions
//////////////////////

void meLoadSaveSystem::pCleanUp(){
	if(pLSNavTest){
		delete pLSNavTest;
	}
	
	if(pLSPFCache){
		delete pLSPFCache;
	}
}

void meLoadSaveSystem::pRebuildFilePatternLists(){
	decString pattern;
	
	pFilePatternList.RemoveAll();
	
	pLSWorlds.Visit([&](const meLSWorld &ls){
		pattern.Format("*%s", ls.GetPattern().GetString());
		pFilePatternList.Add(igdeFilePattern::Ref::New(ls.GetName(), pattern, ls.GetPattern()));
	});
	
	// build prop field cache file patterns
	pattern.Format("*%s", pLSPFCache->GetPattern().GetString());
	pPropFieldCacheFilePatterns.Add(igdeFilePattern::Ref::New(
		pLSPFCache->GetName(), pattern, pLSPFCache->GetPattern()));
	
	pattern.Format("*%s", pLSNavTest->GetPattern().GetString());
	pFPNavTest.Add(igdeFilePattern::Ref::New(
		pLSNavTest->GetName(), pattern, pLSNavTest->GetPattern()));
}
