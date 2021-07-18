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

#ifndef _DELLAUNCHERSUPPORT_H_
#define _DELLAUNCHERSUPPORT_H_

#include "game/delGameManager.h"
#include "game/patch/delPatchManager.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLoggerChain.h>


/**
 * \brief Drag[en]gine launching support.
 * 
 * Main class managing Drag[en]gine launching support.
 */
class delLauncherSupport{
private:
	decString pPathConfigSystem;
	decString pPathConfigUser;
	decString pPathShares;
	decString pPathGames;
	decString pPathLogs;
	
	deVirtualFileSystem::Ref pVFS;
	deLoggerChain::Ref pLogger;
	decString pLogSource;
	
	delGameManager pGameManager;
	delPatchManager pPatchManager;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create launcher support.
	 * 
	 * Creates logger chain with color console logger. Then locates required file path.
	 * Then creates virtual file system with containers for the found path:
	 * - "/config/system" : GetPathConfigSystem() , read-only
	 * - "/config/user" : GetPathConfigUser(), read-write
	 * - "/data" : GetPathShares(), read-write
	 * - "/logs" : GetPathLogs(), read-write
	 */
	delLauncherSupport( const char *loggerSource = "Launcher" );
	
	/** \brief Clean up launcher support. */
	virtual ~delLauncherSupport();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief System config path. */
	inline const decString &GetPathConfigSystem() const{ return pPathConfigSystem; }
	
	/** \brief User config path. */
	inline const decString &GetPathConfigUser() const{ return pPathConfigUser; }
	
	/** \brief Shares path. */
	inline const decString &GetPathShares() const{ return pPathShares; }
	
	/** \brief Games path. */
	inline const decString &GetPathGames() const{ return pPathGames; }
	
	/** \brief Logs path. */
	inline const decString &GetPathLogs() const{ return pPathLogs; }
	
	
	
	/** \brief Virtual file system. */
	inline deVirtualFileSystem *GetVFS() const{ return pVFS; }
	
	/** \brief Logger. */
	inline deLoggerChain *GetLogger() const{ return pLogger; }
	
	/** \brief Log source. */
	inline const decString &GetLogSource() const{ return pLogSource; }
	
	/**
	 * \brief Add file logger.
	 * 
	 * File title is relative to launcher logging directory. The path will be of the
	 * form "logdir/{filetitle}.log".
	 */
	void AddFileLogger( const char *filetitle );
	
	/**
	 * \brief Log initial parameters.
	 * 
	 * Call this after calling AddFileLogger() to log initial parameters useful for debuggin.
	 */
	void LogInitialParameters();
	
	
	
	/** \brief Game manager. */
	inline delGameManager &GetGameManager(){ return pGameManager; }
	inline const delGameManager &GetGameManager() const{ return pGameManager; }
	
	/** \brief Patch manager. */
	inline delPatchManager &GetPatchManager(){ return pPatchManager; }
	inline const delPatchManager &GetPatchManager() const{ return pPatchManager; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pLocatePath();
	void pInitVFS();
};

#endif
