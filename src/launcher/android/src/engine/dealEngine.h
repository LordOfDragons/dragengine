/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DEALENGINE_H_
#define _DEALENGINE_H_

#include <unistd.h>

#include "modules/dealEngineModuleList.h"
#include "../common/string/decString.h"

struct AInputEvent;
class dealLauncher;
class dealIEngineInstance;



/**
 * \brief Game Engine.
 */
class dealEngine{
private:
	dealLauncher &pLauncher;
	
	decString pPathConfig;
	decString pPathShare;
	decString pPathLib;
	decString pPathCache;
	dealEngineModuleList pModuleList;
	decString pLogFile;
	
	void *pHandleLibDragengine;
	void *pHandleLibEngine;
	dealIEngineInstance *pEngine;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create engine. */
	dealEngine( dealLauncher &launcher );
	
	/** \brief Clean up engine. */
	~dealEngine();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
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
	
	/** \brief Module list. */
	inline dealEngineModuleList &GetModuleList(){ return pModuleList; }
	inline const dealEngineModuleList &GetModuleList() const{ return pModuleList; }
	
	
	
	/** \brief Engine instance or \em NULL if not created. */
	inline dealIEngineInstance *GetEngine() const{ return pEngine; }
	
	
	
	/** \brief Start engine. */
	void Start( const char *cacheAppID );
	
	/** \brief Stop engine. */
	void Stop();
	
	/** \brief Process input event. */
	void ProcessInputEvent( const AInputEvent &event );
	
	
	
	/** \brief Load list of modules from module directory. */
	void LoadModuleList();
	
	/** \brief Run quick test to check if the modules are working. */
	void CheckModules();
	
	/** \brief Add modules found in directory. */
	void AddModulesFrom( const char *directory, int type );
	
	/** \brief Best module for type. */
	dealEngineModule *GetBestModuleForType( int moduleType );
	
	/** \brief Get engine path and add virtual file system containers for them. */
	void PutEngineIntoVFS();
	
	/** \brief Text for type. */
	const char *GetModuleTypeText( int moduleType ) const;
	
	
	
	/** \brief Load configuration. */
	void LoadConfig();
	
	/** \brief Save configurations. */
	void SaveConfig();
	
	
	
	/** \brief Focus gained. */
	void FocusGained();
	
	/** \brief Focus lost. */
	void FocusLost();
	
	/** \brief Freeze application. */
	void Freeze();
	
	/** \brief Thaw application. */
	void Thaw();
	
	/** \brief Application window has been created. */
	void InitAppWindow();
	
	/** \brief Application window has been closed. */
	void TerminateAppWindow();
	/*@}*/
};

#endif
