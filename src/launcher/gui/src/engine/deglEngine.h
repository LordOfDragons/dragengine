/* 
 * Drag[en]gine GUI Launcher
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

#ifndef _DEGLENGINE_H_
#define _DEGLENGINE_H_

#include <unistd.h>

#include "modules/deglEngineModuleList.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class deglLauncher;
class deglGameList;
class deglPatchList;
class deglEngineInstance;



/**
 * @brief Game Engine.
 * Stores all informations about the game engine. The library path stores
 * where the game engine libraries are found. This is usually the system
 * library path. The data path stores where the game engine data directory
 * is located. This is usually under the system data directory. Modules
 * are located under the data directory. The config path stores where the
 * game engine configuration files are stored. This is usually under the
 * system configuration path. Module configuration files are located under
 * the config path.
 */
class deglEngine{
private:
	deglLauncher &pLauncher;
	
	decString pPathConfig;
	decString pPathShare;
	decString pPathLib;
	decString pPathCache;
	deglEngineModuleList pModuleList;
	decString pLogFile;
	
	decPoint pCurrentResolution;
	int pResolutionCount;
	decPoint *pResolutions;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new engine. */
	deglEngine( deglLauncher &launcher );
	/** Cleans up the engine. */
	~deglEngine();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the config path. */
	inline const decString &GetPathConfig() const{ return pPathConfig; }
	/** Sets the config path. */
	void SetPathConfig( const char *path );
	/** Retrieves the share path. */
	inline const decString &GetPathShare() const{ return pPathShare; }
	/** Sets the share path. */
	void SetPathShare( const char *path );
	/** Retrieves the lib path. */
	inline const decString &GetPathLib() const{ return pPathLib; }
	/** Sets the lib path. */
	void SetPathLib( const char *path );
	
	/** \brief Cache path. */
	inline const decString &GetPathCache() const{ return pPathCache; }
	
	/** \brief Set cache path. */
	void SetPathCache( const char *path );
	
	/** Retrieves the engine log filename. */
	inline const decString &GetLogFile() const{ return pLogFile; }
	/** Sets the engine log filename. */
	void SetLogFile( const char *path );
	
	/** Retrieves the module list. */
	inline deglEngineModuleList &GetModuleList(){ return pModuleList; }
	inline const deglEngineModuleList &GetModuleList() const{ return pModuleList; }
	
	/** Reload list of modules from module directory. */
	void ReloadModuleList();
	/** Runs a quick test to check if the modules are working. */
	void CheckModules( deglEngineInstance &instance );
	/** Add modules found in the given directory. */
	void AddModulesFrom( const char *directory, int type );
	/** Retrieves the best module for the given module type. */
	deglEngineModule *GetBestModuleForType( int moduleType );
	
	/** Get engine path and add virtual file system containers for them. */
	void PutEngineIntoVFS( deglEngineInstance &instance );
	
	/** Retrieves the text for a module type. */
	const char *GetModuleTypeText( int moduleType ) const;
	
	
	
	/** \brief Get resolution list from engine. */
	void UpdateResolutions( deglEngineInstance &instance );
	
	/** \brief Current display resolution. */
	inline const decPoint &GetCurrentResolution() const{ return pCurrentResolution; }
	
	/** \brief Number of resolutions. */
	inline int GetResolutionCount() const{ return pResolutionCount; }
	
	/** \brief Resolution by index. */
	const decPoint &GetResolutionAt( int index ) const;
	
	/** \brief Index of resolution closest but not larger than size or -1 if there are no resolutions. */
	int IndexOfClosestResolutionTo( int width, int height ) const;
	
	
	
	/** \brief Read game definitions from DELGA file. */
	void ReadDelgaGameDefs( deglEngineInstance &instance, const char *filename, deglGameList &list );
	
	/** \brief Read game patch definitions from DELGA file. */
	void ReadDelgaPatchDefs( deglEngineInstance &instance, const char *filename, deglPatchList &list );
	
	
	
	/** Load configuration. */
	void LoadConfig();
	/** Save configurations. */
	void SaveConfig();
	/*@}*/
};

#endif // _DEGLGAME_H_
