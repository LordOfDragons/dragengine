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

#include <deigde/gui/filedialog/igdeFilePatternList.h>

#include "seLoadSaveSky.h"

class seLoadSaveSky;
class seWindowMain;
class seSky;
class igdeGameDefinition;



/**
 * \brief Load/Save System.
 */
class seLoadSaveSystem{
private:
	seWindowMain &pWindowMain;
	
	seLoadSaveSky *pLSSky;
	igdeFilePatternList pFPSky;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create load/save system. */
	seLoadSaveSystem(seWindowMain &windowMain);
	
	/** \brief Clean up load/save system. */
	~seLoadSaveSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline seWindowMain &GetWindowMain() const{return pWindowMain;}
	
	/** \brief Load save sky. */
	inline seLoadSaveSky &GetLSSky(){return *pLSSky;}
	
	/** \brief Load sky from file. */
	seSky *LoadSky(const char *filename);
	
	/** \brief Save sky to file. */
	void SaveSky(seSky *sky, const char *filename);
	
	/** \brief File pattern list. */
	inline const igdeFilePatternList *GetSkyFilePatterns() const{return &pFPSky;}
	/*@}*/
	
	
	
private:
	void pBuildFilePattern();
};

#endif
