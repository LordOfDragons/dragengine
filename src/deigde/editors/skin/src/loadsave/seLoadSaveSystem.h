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

#ifndef _SELOADSAVESYSTEM_H_
#define _SELOADSAVESYSTEM_H_

#include "seLoadSaveSkin.h"
#include "../skin/seSkin.h"

class seWindowMain;
class igdeGameDefinition;



/**
 * @brief Load/Save System.
 */
class seLoadSaveSystem{
private:
	seWindowMain &pWindowMain;
	
	seLoadSaveSkin::List pLSSkins;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new load/save system object. */
	seLoadSaveSystem(seWindowMain &windowMain);
	/** Cleans up the load/save system object. */
	~seLoadSaveSystem();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the main window. */
	inline seWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** Load save skins. */
	inline const seLoadSaveSkin::List &GetLSSkins() const{ return pLSSkins; }
	
	/** Load save skin matching the given filename. */
	seLoadSaveSkin *GetLSSkinMatching(const char *filename);
	
	/** Adds a load save skin. */
	void AddLSSkin(seLoadSaveSkin *lsSkin);
	
	/** Removes a load save skin. */
	void RemoveLSSkin(seLoadSaveSkin *lsSkin);
	
	/** Removes all load save skins. */
	void RemoveAllLSSkins();
	
	/** Updates the load save skin list from the engine. */
	void UpdateLSSkins();
	
	/** Loads the skin from file if possible. */
	seSkin::Ref LoadSkin(const char *filename, igdeGameDefinition *gameDefinition);
	
	/** Saves the skin to file if possible. */
	void SaveSkin(seSkin *skin, const char *filename);
	/*@}*/
};

#endif
