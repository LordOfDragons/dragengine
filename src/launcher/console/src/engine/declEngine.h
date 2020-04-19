/* 
 * Drag[en]gine Console Launcher
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

#ifndef _DECLENGINE_H_
#define _DECLENGINE_H_

#include <unistd.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

#include "modules/declEngineModuleList.h"

class declLauncher;
class deEngine;
class deLogger;



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
class declEngine{
private:
	declLauncher *pLauncher;
	
	decString pPathConfig;
	decString pPathShare;
	decString pPathLib;
	declEngineModuleList pModuleList;
	
	deEngine *pEngine;
	
	decPoint pCurrentResolution;
	int pResolutionCount;
	decPoint *pResolutions;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new engine. */
	declEngine( declLauncher *launcher );
	/** Cleans up the engine. */
	~declEngine();
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
	
	/** Retrieves the module list. */
	inline declEngineModuleList &GetModuleList(){ return pModuleList; }
	inline const declEngineModuleList &GetModuleList() const{ return pModuleList; }
	
	/** Retrieves the engine or NULL if not existing yet. */
	inline deEngine *GetEngine() const{ return pEngine; }
	
	/** \brief Start engine. */
	void Start( deLogger *logger, const char *cacheAppID );
	
	/** Stop the engine. */
	void Stop();
	/** Load list of modules from module directory and checking them if they are working. */
	void LoadModuleList();
	/** Add modules found in the given directory. */
	void AddModulesFrom( const char *directory, int type );
	/** Retrieves the best module for the given module type. */
	declEngineModule *GetBestModuleForType( int moduleType );
	/** Check modules and retrieves parameter lists. */
	void CheckModules();
	
	/** Get engine path and add virtual file system containers for them. */
	void PutEngineIntoVFS();
	
	/** Retrieves the text for a module type. */
	const char *GetModuleTypeText( int moduleType ) const;
	
	/** Load configuration. */
	void LoadConfig();
	
	/** \brief Count of display resolutions. */
	inline int GetResolutionCount() const{ return pResolutionCount; }
	
	/** \brief Display resolution at index. */
	const decPoint &GetResolutionAt( int index ) const;
	
	/** \brief Current display resolution. */
	inline const decPoint &GetCurrentResolution() const{ return pCurrentResolution; }
	
	/** \brief Get resolution list from engine. */
	void UpdateResolutions();
	/*@}*/
};

#endif
