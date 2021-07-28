/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DELENGINE_H_
#define _DELENGINE_H_

#include <unistd.h>

#include "modules/delEngineModuleList.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/systems/deModuleSystem.h>

class delLauncher;
class delGameList;
class delPatchList;
class delEngineInstance;


/**
 * \brief Game Engine.
 * 
 * Stores all information about the game engine. The library path stores where the game
 * engine libraries are found. This is usually the system library path. The data path
 * stores where the game engine data directory is located. This is usually under the
 * system data directory. Modules are located under the data directory. The config path
 * stores where the game engine configuration files are stored. This is usually under the
 * system configuration path. Module configuration files are located under the config path.
 */
class delEngine{
private:
	delLauncher &pLauncher;
	
	decString pPathConfig;
	decString pPathShare;
	decString pPathLib;
	decString pPathCache;
	delEngineModuleList pModules;
	decString pLogFile;
	
	decPoint pCurrentResolution;
	int pResolutionCount;
	decPoint *pResolutions;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create engine.
	 * 
	 * Log file is named "{logFileTitle}.log" and is located inside the launcher logging directory.
	 */
	delEngine( delLauncher &launcher, const char *logFileTitle = "launcher-engine" );
	
	/** \brief Clean up engine. */
	~delEngine();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Launcher. */
	inline delLauncher &GetLauncher() const{ return pLauncher; }
	
	/** \brief Config path. */
	inline const decString &GetPathConfig() const{ return pPathConfig; }
	
	/** \brief Set config path. */
	void SetPathConfig( const char *path );
	
	/** \brief Share path. */
	inline const decString &GetPathShare() const{ return pPathShare; }
	
	/** \brief Set share path. */
	void SetPathShare( const char *path );
	
	/** \brief Lib path. */
	inline const decString &GetPathLib() const{ return pPathLib; }
	
	/** \brief Set lib path. */
	void SetPathLib( const char *path );
	
	/** \brief Cache path. */
	inline const decString &GetPathCache() const{ return pPathCache; }
	
	/** \brief Set cache path. */
	void SetPathCache( const char *path );
	
	/** \brief Engine log filename. */
	inline const decString &GetLogFile() const{ return pLogFile; }
	
	/** \brief Set engine log filename. */
	void SetLogFile( const char *path );
	
	/** \brief Modules. */
	inline delEngineModuleList &GetModules(){ return pModules; }
	inline const delEngineModuleList &GetModules() const{ return pModules; }
	
	/** \brief Reload list of modules from module directory. */
	void ReloadModules();
	
	/** \brief Run quick test to check if modules are working. */
	void CheckModules( delEngineInstance &instance );
	
	/** \brief Add modules found in directory. */
	void AddModulesFrom( const char *directory, deModuleSystem::eModuleTypes type );
	
	/** \brief Best module for type. */
	delEngineModule *GetBestModuleForType( deModuleSystem::eModuleTypes moduleType ) const;
	
	/** \brief Get engine path and add virtual file system containers for them. */
	void PutEngineIntoVFS( delEngineInstance &instance );
	
	/** \brief Text for module type. */
	const char *GetModuleTypeText( deModuleSystem::eModuleTypes moduleType ) const;
	
	
	
	/** \brief Get resolution list from engine. */
	void UpdateResolutions( delEngineInstance &instance );
	
	/** \brief Current display resolution. */
	inline const decPoint &GetCurrentResolution() const{ return pCurrentResolution; }
	
	/** \brief Number of resolutions. */
	inline int GetResolutionCount() const{ return pResolutionCount; }
	
	/** \brief Resolution by index. */
	const decPoint &GetResolutionAt( int index ) const;
	
	/** \brief Index of resolution closest but not larger than size or -1 if there are no resolutions. */
	int IndexOfClosestResolutionTo( int width, int height ) const;
	
	
	
	/** \brief Read game definitions from DELGA file. */
	void ReadDelgaGameDefs( delEngineInstance &instance, const char *filename, delGameList &list );
	
	/** \brief Read game patch definitions from DELGA file. */
	void ReadDelgaPatchDefs( delEngineInstance &instance, const char *filename, delPatchList &list );
	
	
	
	/** \brief Load configuration. */
	void LoadConfig();
	
	/** \brief Save configurations. */
	void SaveConfig();
	
	/** \brief Clear. */
	void Clear();
	/*@}*/
};

#endif
