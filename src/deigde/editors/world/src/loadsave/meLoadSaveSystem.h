/* 
 * Drag[en]gine IGDE World Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	meLoadSaveSystem( meWindowMain *wndMain );
	~meLoadSaveSystem();
	
	/** Retrieves the main window. */
	inline meWindowMain *GetWindowMain() const{ return pWndMain; }
	
	// world management
	inline int GetLSWorldCount() const{ return pLSWorldCount; }
	meLSWorld *GetLSWorldAt( int index ) const;
	meLSWorld *FindLSWorldMatching( const char *filename ) const;
	void AddLSWorld( meLSWorld *world );
	meWorld *LoadWorld( const char *filename, igdeGameDefinition *gameDefinition,
		igdeStepableTask **task );
	void SaveWorld( meWorld *world, const char *filename );
	
	// height terrain sectors
	/** Retrieves the number of height terrain loaders. */
	inline int GetLSHeightTerrainCount() const{ return pLSHTCount; }
	/** Retrieves the height terrain loader at the given index. */
	meLSHeightTerrain *GetLSHeightTerrainAt( int index ) const;
	/** Retrieves the matching height terrain loader. */
	meLSHeightTerrain *FindLSHeightTerrainMatching( const char *filename ) const;
	/** Adds a height terrain loader. */
	void AddLSHeightTerrain( meLSHeightTerrain *loader );
	/** Loads a height terrain using the matching loader. */
	void LoadHeightTerrain( meHeightTerrain &heightTerrain, const char *filename );
	/** Saves a height terrain using the matching loader. */
	void SaveHeightTerrain( meHeightTerrain &heightTerrain, const char *filename );
	
	/** Retrieves the prop field cache loader. */
	inline meLSPFCache *GetLSPFCache() const{ return pLSPFCache; }
	/** Loads a prop field cache using the matching loader. */
	void LoadPFCache( meHeightTerrainSector &sector, const char *filename );
	/** Saves a prop field cache using the matching loader. */
	void SavePFCache( meHeightTerrainSector &sector, const char *filename );
	
	/** Retrieves the file pattern list. */
	inline const igdeFilePatternList &GetFilePatternList() const{ return pFilePatternList; }
	
	/** Retrieves the prop field cache file patterns. */
	inline const igdeFilePatternList &GetPropFieldCacheFilePatterns() const{ return pPropFieldCacheFilePatterns; }
	
	/** \brief Retrieves the load save navigation test. */
	inline meLoadSaveNavTest *GetLSNavTest(){ return pLSNavTest; }
	/** \brief Loads the navigation test from file. */
	void LoadNavTest( const char *filename, meWorld &world );
	/** \brief Saves the navigation test to file. */
	void SaveNavTest( const char *filename, meWorld &world );
	/** \brief Retrieves the navigation test file pattern list. */
	inline const igdeFilePatternList *GetNavTestFilePatterns() const{ return &pFPNavTest; }
	
private:
	void pCleanUp();
	void pRebuildFilePatternLists();
};

// end of include only once
#endif
