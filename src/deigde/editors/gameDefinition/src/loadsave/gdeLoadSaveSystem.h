/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDELOADSAVESYSTEM_H_
#define _GDELOADSAVESYSTEM_H_

#include <deigde/gui/filedialog/igdeFilePatternList.h>

#include "gdeLoadSaveGameDefinition.h"
#include "gdeLoadSaveXmlEClass.h"

class gdeWindowMain;
class gdeGameDefinition;
class gdeObjectClass;
class igdeGameDefinition;



/**
 * \brief Load/save system.
 */
class gdeLoadSaveSystem{
private:
	gdeWindowMain &pWindowMain;
	
	gdeLoadSaveGameDefinition pLSGameDef;
	igdeFilePatternList pFPGameDef;
	
	gdeLoadSaveXmlEClass pLSXmlEClass;
	igdeFilePatternList pFPXmlEClass;
	
	decString pFoxFPLGameDef;
	decString pFoxFPLXmlEClass;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create load/save system. */
	gdeLoadSaveSystem( gdeWindowMain &windowMain );
	
	/** \brief Clean up load/save system. */
	~gdeLoadSaveSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Load save game definition. */
	inline gdeLoadSaveGameDefinition &GetLSGameDefinition(){ return pLSGameDef; }
	
	/** \brief Load game definition from file. */
	gdeGameDefinition *LoadGameDefinition( const char *filename );
	
	/** \brief Save game definition to file. */
	void SaveGameDefinition( const gdeGameDefinition &gameDefinition, const char *filename );
	
	/** \brief File pattern list. */
	inline const igdeFilePatternList &GetGameDefFilePatterns() const{ return pFPGameDef; }
	
	/** \brief FOX game definition file pattern list. */
	inline const decString &GetFoxFPLGameDefinition() const{ return pFoxFPLGameDef; }
	
	
	
	/** \brief Load save xml element class. */
	inline gdeLoadSaveXmlEClass &GetLSXmlEClass(){ return pLSXmlEClass; }
	
	/** \brief Load xml element class from file. */
	gdeObjectClass *LoadXmlEClass( const char *filename );
	
	/** \brief Save xml element class to file. */
	void SaveXmlEClass( const gdeObjectClass &objectClass, const char *filename );
	
	/** \brief XML element class file pattern list. */
	inline const igdeFilePatternList &GetXmlEClassFilePatterns() const{ return pFPXmlEClass; }
	
	/** \brief FOX XML element class file pattern list. */
	inline const decString &GetFoxFPLXmlEClass() const{ return pFoxFPLXmlEClass; }
	/*@}*/
	
	
	
private:
	void pBuildFilePattern();
	void pConvertToFOX( const igdeFilePatternList &fpl, decString &foxfpl );
};

#endif
