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

#ifndef _FELOADSAVESYSTEM_H_
#define _FELOADSAVESYSTEM_H_

#include "../font/feFont.h"

#include <dragengine/common/collection/decTOrderedSet.h>

class feLoadSaveFont;
class feWindowMain;
class igdeGameDefinition;



/**
 * @brief Load/Save System.
 * Provides loading and saving support as well as providing file
 * pattern lists for various file formats.
 */
class feLoadSaveSystem{
public:
	typedef decTObjectOrderedSet<feLoadSaveFont> LSFontList;
	
	
private:
	feWindowMain *pWndMain;
	
	LSFontList pLSFonts;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new load/save system. */
	feLoadSaveSystem(feWindowMain *wndMain);
	/** Cleans up the load/save system. */
	~feLoadSaveSystem();
	/*@}*/

	/** @name Management */
	/*@{*/
	/** Retrieves the main window. */
	inline feWindowMain *GetWindowMain() const{ return pWndMain; }
	
	/** Load save fonts. */
	LSFontList &GetLSFonts(){ return pLSFonts; }
	const LSFontList &GetLSFonts() const{ return pLSFonts; }
	
	/** Updates the load save font list from the engine. */
	void UpdateLSFonts();
	
	/** Loads the font from file if possible. */
	feFont::Ref LoadFont(const char *filename, igdeGameDefinition *gameDefinition);
	
	/** Saves the font to file if possible. */
	void SaveFont(feFont *font, const char *filename);
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
