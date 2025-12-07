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
	feLoadSaveFont(deBaseFontModule *module);
	/** Cleans up the loader. */
	~feLoadSaveFont();
	/*@}*/

	/** @name Management */
	/*@{*/
	/** Retrieves the name. */
	inline const decString &GetName() const{return pName;}
	/** Sets the name. */
	void SetName(const char *name);
	/** Retrieves the pattern. */
	inline const decString &GetPattern() const{return pPattern;}
	/** Sets the pattern. */
	void SetPattern(const char *pattern);
	
	/** Loads the font from file. */
	void LoadFont(const char *virtualPath, feFont *font, decBaseFileReader *file);
	/** Saves the font to file. */
	void SaveFont(const char *virtualPath, feFont *font, decBaseFileWriter *file);
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
