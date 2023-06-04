/* 
 * Drag[en]gine IGDE Speech Animation Editor
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
	saeLoadSaveSystem( saeWindowMain &windowMain );
	
	/** Cleans up the load/save system. */
	~saeLoadSaveSystem();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the load save speech animation. */
	inline saeLoadSaveSAnimation *GetLSSAnim(){ return pLSSAnim; }
	
	/** Loads the speech animation from file. */
	saeSAnimation *LoadSAnimation( const char *filename );
	/** Saves the speech animation to file. */
	void SaveSAnimation( saeSAnimation *sanimation, const char *filename );
	/** Retrieves the file pattern list. */
	inline const igdeFilePatternList *GetFilePatternList() const{ return &pFilePatternList; }
	/*@}*/
	
private:
	void pBuildFilePattern();
};

#endif
