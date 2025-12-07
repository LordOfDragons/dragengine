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

#include <deigde/gui/filedialog/igdeFilePatternList.h>

class meLoadSaveNavTest;
class meLSWorld;
class meLSHeightTerrain;
class meLSPFCache;
class meWindowMain;
class meWorld;
class meHeightTerrain;
class meHeightTerrainSector;
class igdeStepableTask;
class igdeGameDefinition;



/**
 * Load/Save System for the editor.
 */
class meLoadSaveSystem{
private:
	meWindowMain *pWndMain;
	
	meLSWorld **pLSWorlds;
	int pLSWorldCount, pLSWorldSize;
	
	meLSHeightTerrain **pLSHTs;
	int pLSHTCount;
	int pLSHTSize;
	
	meLoadSaveNavTest *pLSNavTest;
	igdeFilePatternList pFPNavTest;
	
	meLSPFCache *pLSPFCache;
	
	igdeFilePatternList pFilePatternList;
	igdeFilePatternList pPropFieldCacheFilePatterns;
	
public:
	// constructor, destructor
	meLoadSaveSystem(meWindowMain *wndMain);
	~meLoadSaveSystem();
	
	/** Retrieves the main window. */
	inline meWindowMain *GetWindowMain() const{ return pWndMain; }
	
	// world management
	inline int GetLSWorldCount() const{ return pLSWorldCount; }
	meLSWorld *GetLSWorldAt(int index) const;
	meLSWorld *FindLSWorldMatching(const char *filename) const;
	void AddLSWorld(meLSWorld *world);
	meWorld *LoadWorld(const char *filename, igdeGameDefinition *gameDefinition,
		igdeStepableTask **task);
	void SaveWorld(meWorld *world, const char *filename);
	
	// height terrain sectors
	/** Retrieves the number of height terrain loaders. */
	inline int GetLSHeightTerrainCount() const{ return pLSHTCount; }
	/** Retrieves the height terrain loader at the given index. */
	meLSHeightTerrain *GetLSHeightTerrainAt(int index) const;
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
	inline const igdeFilePatternList &GetFilePatternList() const{ return pFilePatternList; }
	
	/** Retrieves the prop field cache file patterns. */
	inline const igdeFilePatternList &GetPropFieldCacheFilePatterns() const{ return pPropFieldCacheFilePatterns; }
	
	/** \brief Retrieves the load save navigation test. */
	inline meLoadSaveNavTest *GetLSNavTest(){ return pLSNavTest; }
	/** \brief Loads the navigation test from file. */
	void LoadNavTest(const char *filename, meWorld &world);
	/** \brief Saves the navigation test to file. */
	void SaveNavTest(const char *filename, meWorld &world);
	/** \brief Retrieves the navigation test file pattern list. */
	inline const igdeFilePatternList *GetNavTestFilePatterns() const{ return &pFPNavTest; }
	
private:
	void pCleanUp();
	void pRebuildFilePatternLists();
};

// end of include only once
#endif
