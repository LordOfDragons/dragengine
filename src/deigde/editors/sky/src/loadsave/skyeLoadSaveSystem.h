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

#ifndef _SKYELOADSAVESYSTEM_H_
#define _SKYELOADSAVESYSTEM_H_

#include "skyeLoadSaveSky.h"
#include "../sky/skyeSky.h"

#include <deigde/gui/filedialog/igdeFilePattern.h>

class skyeLoadSaveSky;
class skyeWindowMain;
class igdeGameDefinition;



/**
 * \brief Load/Save System.
 */
class skyeLoadSaveSystem{
private:
	skyeWindowMain &pWindowMain;
	
	skyeLoadSaveSky *pLSSky;
	igdeFilePattern::List pFPSky;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create load/save system. */
	skyeLoadSaveSystem(skyeWindowMain &windowMain);
	
	/** \brief Clean up load/save system. */
	~skyeLoadSaveSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline skyeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Load save sky. */
	inline skyeLoadSaveSky &GetLSSky(){ return *pLSSky; }
	
	/** \brief Load sky from file. */
	skyeSky::Ref LoadSky(const char *filename);
	
	/** \brief Save sky to file. */
	void SaveSky(skyeSky *sky, const char *filename);
	
	/** \brief File pattern list. */
	inline const igdeFilePattern::List *GetSkyFilePatterns() const{ return &pFPSky; }
	/*@}*/
	
	
	
private:
	void pBuildFilePattern();
};

#endif
