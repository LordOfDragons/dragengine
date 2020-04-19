/* 
 * Drag[en]gine IGDE Project Editor
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

#ifndef _PROJLOADSAVESYSTEM_H_
#define _PROJLOADSAVESYSTEM_H_

#include <deigde/gui/filedialog/igdeFilePatternList.h>

class projWindowMain;
class igdeGameDefinition;
class igdeFilePatternList;



/**
 * \brief Load/Save System.
 */
class projLoadSaveSystem{
private:
	projWindowMain &pWindowMain;
	
	igdeFilePatternList pFPDelga;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create load/save system. */
	projLoadSaveSystem( projWindowMain &windowMain );
	
	/** \brief Clean up load/save system. */
	~projLoadSaveSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline projWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Delga file pattern list. */
	inline const igdeFilePatternList &GetDelgaFilePatterns() const{ return pFPDelga; }
	/*@}*/
	
	
	
private:
	void pBuildFilePattern();
};

#endif
