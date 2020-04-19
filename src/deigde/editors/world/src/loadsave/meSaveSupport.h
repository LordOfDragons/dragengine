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

// include only once
#ifndef _MESAVESUPPORT_H_
#define _MESAVESUPPORT_H_

class meWindowMain;
class meWorld;
class meHeightTerrainSector;
class meHeightTerrainTexture;
class meHeightTerrainNavSpace;
class meNavigationSpace;
class igdeFilePatternList;



/**
 * Support system for saving files.
 */
class meSaveSupport{
private:
	meWindowMain *pWindowMain;
	
public:
	// constructor, destructor
	meSaveSupport( meWindowMain *windowMain );
	~meSaveSupport();
	
	// management
	/** Saves a world including all dependencies. */
	void SaveWorldWithDependencies( meWorld *world, bool forceAskForFilename );
	
	/** Saves a world. */
	void SaveWorld( meWorld *world, bool forceAskForFilename );
	/** Saves a height terrain. */
	void SaveHeightTerrain( meWorld &world, bool forceAskForFilename );
	/** Saves a height terrain height map image. */
	void SaveHTHeightImage( meWorld *world, meHeightTerrainSector *sector, bool forceAskForFilename );
	/** Saves a height terrain visibility image. */
	void SaveHTVisibilityImage( meWorld *world, meHeightTerrainSector *sector, bool forceAskForFilename );
	/** Saves a height terrain texture mask image. */
	void SaveHTTextureMaskImage( meWorld *world, meHeightTerrainSector *sector, meHeightTerrainTexture *texture, bool forceAskForFilename );
	
	/** \brief Save height terrain navigation space. */
	void SaveHTNavSpace( meWorld &world, meHeightTerrainNavSpace &navspace, bool forceAskForFilename );
	
	/** Saves a height terrain prop field cache. */
	void SaveHTPFCache( meWorld *world, meHeightTerrainSector *sector, bool forceAskForFilename );
	/** Saves a navigation space. */
	void SaveNavigationSpace( meWorld *world, meNavigationSpace *navspace, bool forceAskForFilename );
	
	/** Fills the given file pattern list with file patterns for world files. */
	void GetWorldPatterns( igdeFilePatternList &patternList ) const;
	/** Fills the given file pattern list with file patterns for height terrain sector files. */
	void GetHeightTerrainSectorPatterns( igdeFilePatternList &patternList ) const;
};

// end of include only once
#endif
