/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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

#ifdef OS_ANDROID
#include <dragengine/app/deOSAndroid.h>
#endif

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
class DE_DLL_EXPORT delLauncher{
public:
#ifdef OS_ANDROID
	/**
	 * \brief Configuration.
	 */
	struct sConfig{
		/**
		 * \brief Logger source.
		 */
		decString loggerSource = "Launcher";
		
		/**
		 * \brief Engine log file title.
		 */
		decString engineLogFileTitle = "launcher-engine";
		
		/**
		 * \brief Path to launcher installation directory.
		 * 
		 * Typically this is File(context.filesDir, "delauncher").absolutePath .
		 */
		decString pathLauncher;
		
		/**
		 * \brief Path to games directory.
		 * 
		 * Typically this is File(context.filesDir, "delauncher-games").absolutePath .
		 */
		decString pathGames;
		
		/**
		 * \brief Path to user configuration directory.
		 * 
		 * This directory includes launcher configurations and log files.
		 * 
		 * Typically this is File(context.filesDir, "delauncher-config").absolutePath
		 * or File(context.getExternalFilesDir(null), "delauncher-config").absolutePath .
		 */
		decString pathConfig;
		
		/**
		 * \brief OS configuration.
		 */
		deOSAndroid::sConfig osConfig;
	};
#endif
	
	
	
private:
#ifdef OS_ANDROID
	const sConfig pConfig;
#endif
	
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
#ifdef OS_ANDROID
	delLauncher(const sConfig &config);
#else
	delLauncher(const char *loggerSource = "Launcher", const char *engineLogFileTitle = "launcher-engine");
#endif
	
	/** \brief Clean up launcher support. */
	virtual ~delLauncher();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
#ifdef OS_ANDROID
	/** \brief Configuration. */
	inline const sConfig &GetConfig() const{return pConfig;}
#endif
	
	/** \brief System config path. */
	inline const decString &GetPathConfigSystem() const{return pPathConfigSystem;}
	
	/** \brief User config path. */
	inline const decString &GetPathConfigUser() const{return pPathConfigUser;}
	
	/** \brief Shares path. */
	inline const decString &GetPathShares() const{return pPathShares;}
	
	/** \brief Games path. */
	inline const decString &GetPathGames() const{return pPathGames;}
	
	/** \brief Logs path. */
	inline const decString &GetPathLogs() const{return pPathLogs;}
	
	
	
	/** \brief Virtual file system. */
	inline deVirtualFileSystem *GetVFS() const{return pVFS;}
	
	/** \brief Logger. */
	inline deLoggerChain *GetLogger() const{return pLogger;}
	
	/** \brief Logger history. */
	inline delLoggerHistory *GetLoggerHistory() const{return pLoggerHistory;}
	
	/** \brief Log source. */
	inline const decString &GetLogSource() const{return pLogSource;}
	
	/**
	 * \brief Add file logger.
	 * 
	 * File title is relative to launcher logging directory. The path will be of the
	 * form "logdir/{filetitle}.log".
	 */
	void AddFileLogger(const char *filetitle);
	
	
	
	/** \brief Engine. */
	inline delEngine &GetEngine(){return pEngine;}
	inline const delEngine &GetEngine() const{return pEngine;}
	
	/** \brief Game manager. */
	inline delGameManager &GetGameManager(){return pGameManager;}
	inline const delGameManager &GetGameManager() const{return pGameManager;}
	
	/** \brief Patch manager. */
	inline delPatchManager &GetPatchManager(){return pPatchManager;}
	inline const delPatchManager &GetPatchManager() const{return pPatchManager;}
	
	/**
	 * \brief Engine instance factory.
	 * \version 1.10
	 */
	inline delEngineInstance::Factory &GetEngineInstanceFactory() const{return pEngineInstanceFactory;}
	
	/**
	 * \brief Set engine instance factory.
	 * \version 1.10
	 */
	void SetEngineInstanceFactory(delEngineInstance::Factory *factory);
	
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
	virtual delGameProfile *CreateGameProfile(const delGameProfile *copyFrom = nullptr);
	
	/**
	 * \brief Create game icon instance.
	 * 
	 * Default implementation creates instance of delGameIcon.
	 */
	virtual delGameIcon *CreateGameIcon(int size, const char *path);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pLocatePath();
	void pInitVFS();
};

#endif
