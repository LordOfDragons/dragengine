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

#ifndef _MELOADSAVESYSTEM_H_
#define _MELOADSAVESYSTEM_H_

#include "meLSWorld.h"
#include "heightterrain/meLSHeightTerrain.h"
#include "../world/meWorld.h"

#include <deigde/gui/igdeStepableTask.h>
#include <deigde/gui/filedialog/igdeFilePattern.h>

class meLoadSaveNavTest;
class meLSPFCache;
class meWindowMain;
class meHeightTerrain;
class meHeightTerrainSector;
class igdeGameDefinition;



/**
 * Load/Save System for the editor.
 */
class meLoadSaveSystem{
private:
	meWindowMain *pWndMain;
	
	meLSWorld::List pLSWorlds;
	meLSHeightTerrain::List pLSHTs;
	
	meLoadSaveNavTest *pLSNavTest;
	igdeFilePattern::List pFPNavTest;
	
	meLSPFCache *pLSPFCache;
	
	igdeFilePattern::List pFilePatternList;
	igdeFilePattern::List pPropFieldCacheFilePatterns;
	
public:
	// constructor, destructor
	meLoadSaveSystem(meWindowMain *wndMain);
	~meLoadSaveSystem();
	
	/** Retrieves the main window. */
	inline meWindowMain *GetWindowMain() const{ return pWndMain; }
	
	// world management
	inline const meLSWorld::List &GetLSWorlds() const{ return pLSWorlds; }
	
	meLSWorld *FindLSWorldMatching(const char *filename) const;
	
	void AddLSWorld(meLSWorld *world);
	
	meWorld::Ref LoadWorld(const char *filename, igdeGameDefinition *gameDefinition,
		igdeStepableTask::Ref &task);
	
	void SaveWorld(meWorld *world, const char *filename);
	
	// height terrain sectors
	inline const meLSHeightTerrain::List &GetLSHeightTerrains() const{ return pLSHTs; }
	
	/** Retrieves the matching height terrain loader. */
	meLSHeightTerrain *FindLSHeightTerrainMatching(const char *filename) const;
	
	/** Adds a height terrain loader. */
	void AddLSHeightTerrain(meLSHeightTerrain *loader);
	
	/** Loads a height terrain using the matching loader. */
	void LoadHeightTerrain(meHeightTerrain &heightTerrain, const char *filename);
	
	/** Saves a height terrain using the matching loader. */
	void SaveHeightTerrain(meHeightTerrain &heightTerrain, const char *filename);
	
	/** Retrieves the prop field cache loader. */
	inline meLSPFCache *GetLSPFCache() const{ return pLSPFCache; }
	/** Loads a prop field cache using the matching loader. */
	void LoadPFCache(meHeightTerrainSector &sector, const char *filename);
	/** Saves a prop field cache using the matching loader. */
	void SavePFCache(meHeightTerrainSector &sector, const char *filename);
	
	/** Retrieves the file pattern list. */
	inline const igdeFilePattern::List &GetFilePatternList() const{ return pFilePatternList; }
	
	/** Retrieves the prop field cache file patterns. */
	inline const igdeFilePattern::List &GetPropFieldCacheFilePatterns() const{ return pPropFieldCacheFilePatterns; }
	
	/** \brief Retrieves the load save navigation test. */
	inline meLoadSaveNavTest *GetLSNavTest(){ return pLSNavTest; }
	/** \brief Loads the navigation test from file. */
	void LoadNavTest(const char *filename, meWorld &world);
	/** \brief Saves the navigation test to file. */
	void SaveNavTest(const char *filename, meWorld &world);
	/** \brief Retrieves the navigation test file pattern list. */
	inline const igdeFilePattern::List *GetNavTestFilePatterns() const{ return &pFPNavTest; }
	
private:
	void pCleanUp();
	void pRebuildFilePatternLists();
};

// end of include only once
#endif
