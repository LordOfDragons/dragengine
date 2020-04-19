/* 
 * Drag[en]gine IGDE Skin Editor
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

#ifndef _SELOADSAVESYSTEM_H_
#define _SELOADSAVESYSTEM_H_

class seLoadSaveSkin;
class seWindowMain;
class seSkin;
class igdeGameDefinition;



/**
 * @brief Load/Save System.
 */
class seLoadSaveSystem{
private:
	seWindowMain &pWindowMain;
	
	seLoadSaveSkin **pLSSkins;
	int pLSSkinCount;
	int pLSSkinSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new load/save system object. */
	seLoadSaveSystem( seWindowMain &windowMain );
	/** Cleans up the load/save system object. */
	~seLoadSaveSystem();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the main window. */
	inline seWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** Retrieves the number of load save skins. */
	inline int GetLSSkinCount() const{ return pLSSkinCount; }
	/** Retrieves the load save skin at the given index. */
	seLoadSaveSkin *GetLSSkinAt( int index ) const;
	/** Retrieves the index of the load save skin. */
	int IndexOfLSSkin( seLoadSaveSkin *lsSkin ) const;
	/** Determines if the save skin exists. */
	bool HasLSSkin( seLoadSaveSkin *lsSkin ) const;
	/** Retrieves the index of the load save skin matching the given filename. */
	int IndexOfLSSkinMatching( const char *filename );
	/** Adds a load save skin. */
	void AddLSSkin( seLoadSaveSkin *lsSkin );
	/** Removes a load save skin. */
	void RemoveLSSkin( seLoadSaveSkin *lsSkin );
	/** Removes all load save skins. */
	void RemoveAllLSSkins();
	/** Updates the load save skin list from the engine. */
	void UpdateLSSkins();
	
	/** Loads the skin from file if possible. */
	seSkin *LoadSkin( const char *filename, igdeGameDefinition *gameDefinition );
	/** Saves the skin to file if possible. */
	void SaveSkin( seSkin *skin, const char *filename );
	/*@}*/
};

#endif
