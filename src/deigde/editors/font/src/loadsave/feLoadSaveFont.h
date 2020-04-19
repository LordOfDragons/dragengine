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
#ifndef _FELOADSAVEFONT_H_
#define _FELOADSAVEFONT_H_

// includes
#include <dragengine/common/string/decString.h>

// predefinitions
class feFont;
class deBaseFontModule;
class decBaseFileReader;
class decBaseFileWriter;



/**
 * @brief Load/Save Engine Font.
 * Loads and saves fonts using engine font modules.
 */
class feLoadSaveFont{
private:
	deBaseFontModule *pModule;
	
	decString pName;
	decString pPattern;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new loader for the given module. */
	feLoadSaveFont( deBaseFontModule *module );
	/** Cleans up the loader. */
	~feLoadSaveFont();
	/*@}*/

	/** @name Management */
	/*@{*/
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the name. */
	void SetName( const char *name );
	/** Retrieves the pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	/** Sets the pattern. */
	void SetPattern( const char *pattern );
	
	/** Loads the font from file. */
	void LoadFont( const char *virtualPath, feFont *font, decBaseFileReader *file );
	/** Saves the font to file. */
	void SaveFont( const char *virtualPath, feFont *font, decBaseFileWriter *file );
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
