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

#ifndef _SAELOADSAVESYSTEM_H_
#define _SAELOADSAVESYSTEM_H_

#include <deigde/gui/filedialog/igdeFilePatternList.h>

#include "saeLoadSaveSAnimation.h"

class saeLoadSaveSAnimation;
class saeWindowMain;
class saeSAnimation;
class igdeGameDefinition;



/**
 * Load/Save System.
 */
class saeLoadSaveSystem{
private:
	saeWindowMain &pWindowMain;
	
	saeLoadSaveSAnimation *pLSSAnim;
	igdeFilePatternList pFilePatternList;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new load/save system. */
	saeLoadSaveSystem(saeWindowMain &windowMain);
	
	/** Cleans up the load/save system. */
	~saeLoadSaveSystem();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the load save speech animation. */
	inline saeLoadSaveSAnimation *GetLSSAnim(){ return pLSSAnim; }
	
	/** Loads the speech animation from file. */
	saeSAnimation *LoadSAnimation(const char *filename);
	/** Saves the speech animation to file. */
	void SaveSAnimation(saeSAnimation *sanimation, const char *filename);
	/** Retrieves the file pattern list. */
	inline const igdeFilePatternList *GetFilePatternList() const{ return &pFilePatternList; }
	/*@}*/
	
private:
	void pBuildFilePattern();
};

#endif
