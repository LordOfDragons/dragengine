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

// include only once
#ifndef _MESAVESUPPORT_H_
#define _MESAVESUPPORT_H_

#include <deigde/gui/filedialog/igdeFilePattern.h>

class meWindowMain;
class meWorld;
class meHeightTerrainSector;
class meHeightTerrainTexture;
class meHeightTerrainNavSpace;
class meNavigationSpace;



/**
 * Support system for saving files.
 */
class meSaveSupport{
private:
	meWindowMain *pWindowMain;
	
public:
	// constructor, destructor
	meSaveSupport(meWindowMain *windowMain);
	~meSaveSupport();
	
	// management
	/** Saves a world including all dependencies. */
	void SaveWorldWithDependencies(meWorld *world, bool forceAskForFilename);
	
	/** Saves a world. */
	void SaveWorld(meWorld *world, bool forceAskForFilename);
	/** Saves a height terrain. */
	void SaveHeightTerrain(meWorld &world, bool forceAskForFilename);
	/** Saves a height terrain height map image. */
	void SaveHTHeightImage(meWorld *world, meHeightTerrainSector *sector, bool forceAskForFilename);
	/** Saves a height terrain visibility image. */
	void SaveHTVisibilityImage(meWorld *world, meHeightTerrainSector *sector, bool forceAskForFilename);
	/** Saves a height terrain texture mask image. */
	void SaveHTTextureMaskImage(meWorld *world, meHeightTerrainSector *sector, meHeightTerrainTexture *texture, bool forceAskForFilename);
	
	/** \brief Save height terrain navigation space. */
	void SaveHTNavSpace(meWorld &world, meHeightTerrainNavSpace &navspace, bool forceAskForFilename);
	
	/** Saves a height terrain prop field cache. */
	void SaveHTPFCache(meWorld *world, meHeightTerrainSector *sector, bool forceAskForFilename);
	/** Saves a navigation space. */
	void SaveNavigationSpace(meWorld *world, meNavigationSpace *navspace, bool forceAskForFilename);
	
	/** Fills the given file pattern list with file patterns for world files. */
	void GetWorldPatterns(igdeFilePattern::List &patternList) const;
	/** Fills the given file pattern list with file patterns for height terrain sector files. */
	void GetHeightTerrainSectorPatterns(igdeFilePattern::List &patternList) const;
};

// end of include only once
#endif
