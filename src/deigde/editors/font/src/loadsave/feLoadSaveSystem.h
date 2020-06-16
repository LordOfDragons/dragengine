/* 
 * Drag[en]gine IGDE Font Editor
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
#ifndef _FELOADSAVESYSTEM_H_
#define _FELOADSAVESYSTEM_H_

// includes
#include <deigde/gui/filedialog/igdeFilePatternList.h>

// predefinitions
class feLoadSaveFont;
class feWindowMain;
class feFont;
class igdeGameDefinition;



/**
 * @brief Load/Save System.
 * Provides loading and saving support as well as providing file
 * pattern lists for various file formats.
 */
class feLoadSaveSystem{
private:
	feWindowMain *pWndMain;
	
	feLoadSaveFont **pLSFonts;
	int pLSFontCount;
	int pLSFontSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new load/save system. */
	feLoadSaveSystem( feWindowMain *wndMain );
	/** Cleans up the load/save system. */
	~feLoadSaveSystem();
	/*@}*/

	/** @name Management */
	/*@{*/
	/** Retrieves the main window. */
	inline feWindowMain *GetWindowMain() const{ return pWndMain; }
	
	/** Retrieves the number of load save fonts. */
	inline int GetLSFontCount() const{ return pLSFontCount; }
	/** Retrieves the load save font at the given index. */
	feLoadSaveFont *GetLSFontAt( int index ) const;
	/** Retrieves the index of the load save font. */
	int IndexOfLSFont( feLoadSaveFont *lsFont ) const;
	/** Determines if the save font exists. */
	bool HasLSFont( feLoadSaveFont *lsFont ) const;
	/** Retrieves the index of the load save font matching the given filename. */
	int IndexOfLSFontMatching( const char *filename );
	/** Adds a load save font. */
	void AddLSFont( feLoadSaveFont *lsFont );
	/** Removes a load save font. */
	void RemoveLSFont( feLoadSaveFont *lsFont );
	/** Removes all load save fonts. */
	void RemoveAllLSFonts();
	/** Updates the load save font list from the engine. */
	void UpdateLSFonts();
	
	/** Loads the font from file if possible. */
	feFont *LoadFont( const char *filename, igdeGameDefinition *gameDefinition );
	/** Saves the font to file if possible. */
	void SaveFont( feFont *font, const char *filename );
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
