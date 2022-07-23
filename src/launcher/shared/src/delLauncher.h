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

#ifndef _DELLAUNCHER_H_
#define _DELLAUNCHER_H_

#include "engine/delEngine.h"
#include "engine/delEngineInstance.h"
#include "game/delGameManager.h"
#include "game/patch/delPatchManager.h"
#include "logger/delLoggerHistory.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLoggerChain.h>

class delGameIcon;


/**
 * \brief Drag[en]gine base launcher class.
 * 
 * Manages resources required to launch games. The game engine class stores information
 * about the Drag[en]gine installation on the user system and allows running games through
 * engine instances. The launcher stores games, game patches and game profiles.
 * 
 * Launchers can subclass to overwrite hooks to customize the launching process.
 */
class delLauncher{
private:
	decString pPathConfigSystem;
	decString pPathConfigUser;
	decString pPathShares;
	decString pPathGames;
	decString pPathLogs;
	
	deVirtualFileSystem::Ref pVFS;
	deLoggerChain::Ref pLogger;
	delLoggerHistory::Ref pLoggerHistory;
	decString pLogSource;
	
	delEngine pEngine;
	delGameManager pGameManager;
	delPatchManager pPatchManager;
	delEngineInstance::Factory::Ref pEngineInstanceFactory;
	
	
	
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
	delLauncher( const char *loggerSource = "Launcher", const char *engineLogFileTitle = "launcher-engine" );
	
	/** \brief Clean up launcher support. */
	virtual ~delLauncher();
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
	
	/** \brief Logger history. */
	inline delLoggerHistory *GetLoggerHistory() const{ return pLoggerHistory; }
	
	/** \brief Log source. */
	inline const decString &GetLogSource() const{ return pLogSource; }
	
	/**
	 * \brief Add file logger.
	 * 
	 * File title is relative to launcher logging directory. The path will be of the
	 * form "logdir/{filetitle}.log".
	 */
	void AddFileLogger( const char *filetitle );
	
	
	
	/** \brief Engine. */
	inline delEngine &GetEngine(){ return pEngine; }
	inline const delEngine &GetEngine() const{ return pEngine; }
	
	/** \brief Game manager. */
	inline delGameManager &GetGameManager(){ return pGameManager; }
	inline const delGameManager &GetGameManager() const{ return pGameManager; }
	
	/** \brief Patch manager. */
	inline delPatchManager &GetPatchManager(){ return pPatchManager; }
	inline const delPatchManager &GetPatchManager() const{ return pPatchManager; }
	
	/**
	 * \brief Engine instance factory.
	 * \version 1.10
	 */
	inline delEngineInstance::Factory &GetEngineInstanceFactory() const{ return pEngineInstanceFactory; }
	
	/**
	 * \brief Set engine instance factory.
	 * \version 1.10
	 */
	void SetEngineInstanceFactory( delEngineInstance::Factory *factory );
	
	/**
	 * \brief Prepares launcher loading engine, games and patches.
	 * 
	 * Does the following:
	 * - Logs important launcher path for debugging
	 * - Detect game engine modules
	 * - Logs important game engine path for debugging
	 * - Updates list of available screen resolutions
	 * - Checks usability of game engine modules
	 * - Loads engine configuration from "/config/user/launcher.xml"
	 * - Creates default game profile using available game engine modules
	 * - Load game list
	 * - Load patch list
	 * - Load game configurations from user directory
	 * - Verify games can run
	 */
	void Prepare();
	
	
	
	/**
	 * \brief Create game instance.
	 * 
	 * Default implementation creates instance of delGame.
	 */
	virtual delGame *CreateGame();
	
	/**
	 * \brief Create game profile instance.
	 * 
	 * Default implementation creates instance of delGameProfile.
	 */
	virtual delGameProfile *CreateGameProfile( const delGameProfile *copyFrom = nullptr );
	
	/**
	 * \brief Create game icon instance.
	 * 
	 * Default implementation creates instance of delGameIcon.
	 */
	virtual delGameIcon *CreateGameIcon( int size, const char *path );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pLocatePath();
	void pInitVFS();
};

#endif
