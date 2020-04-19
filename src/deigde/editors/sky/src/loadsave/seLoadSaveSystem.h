/* 
 * Drag[en]gine IGDE Sky Editor
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
	seLoadSaveSystem( seWindowMain &windowMain );
	
	/** \brief Clean up load/save system. */
	~seLoadSaveSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline seWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Load save sky. */
	inline seLoadSaveSky &GetLSSky(){ return *pLSSky; }
	
	/** \brief Load sky from file. */
	seSky *LoadSky( const char *filename );
	
	/** \brief Save sky to file. */
	void SaveSky( seSky *sky, const char *filename );
	
	/** \brief File pattern list. */
	inline const igdeFilePatternList *GetSkyFilePatterns() const{ return &pFPSky; }
	/*@}*/
	
	
	
private:
	void pBuildFilePattern();
};

#endif
