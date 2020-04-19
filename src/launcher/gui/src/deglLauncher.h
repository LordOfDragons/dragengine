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

#ifndef _DEGLLAUNCHER_H_
#define _DEGLLAUNCHER_H_

#include "game/patch/deglPatchManager.h"

#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/common/string/decString.h>

class deglConfiguration;
class deglEngine;
class deVirtualFileSystem;
class deglGameManager;
class deglGame;
class deglLoggerHistory;
class deLogger;
class deglWindowMain;



/**
 * @brief Main Launcher Class.
 */
class deglLauncher{
private:
	deglWindowMain *pWindowMain;
	
	decUnicodeArgumentList pArgList;
	deVirtualFileSystem *pFileSystem;
	deglConfiguration *pConfiguration;
	deglEngine *pEngine;
	deglGameManager *pGameManager;
	deglPatchManager pPatchManager;
	deglGame *pCmdLineGame;
	deLogger *pLogger;
	deglLoggerHistory *pLoggerHistory;
	
	decString pRunGame;
	decUnicodeArgumentList pRunGameArgList;
	decString pRunProfileName;
	decString pCmdLineInstallDelga;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new launcher. */
	deglLauncher( deglWindowMain *windowMain, int argc, char **argv );
	/** Cleans up the launcher. */
	~deglLauncher();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the command line arguments. */
	inline const decUnicodeArgumentList &GetArgumentList() const{ return pArgList; }
	/** Retrieves the file system. */
	inline deVirtualFileSystem *GetFileSystem() const{ return pFileSystem; }
	/** Retrieves the configuration. */
	inline deglConfiguration *GetConfiguration() const{ return pConfiguration; }
	/** Retrieves the engine. */
	inline deglEngine *GetEngine() const{ return pEngine; }
	/** Retrieves the game manager. */
	inline deglGameManager *GetGameManager() const{ return pGameManager; }
	
	/** \brief Patch manager. */
	inline deglPatchManager &GetPatchManager(){ return pPatchManager; }
	inline const deglPatchManager &GetPatchManager() const{ return pPatchManager; }
	
	/** Retrieves the logger. */
	inline deLogger *GetLogger() const{ return pLogger; }
	/** Retrieves the logger history. */
	inline deglLoggerHistory *GetLoggerHistory() const{ return pLoggerHistory; }
	
	/** \brief Install delga command line argument. */
	inline const decString &GetCommandLineInstallDelga() const{ return pCmdLineInstallDelga; }
	
	/** Determines if a game has to be run due to command line arguments. */
	bool HasCommandLineRunGame() const;
	
	/**
	 * \brief Run game from command line.
	 * \returns True to start the application loop or false to exit
	 */
	bool RunCommandLineGame();
	/**
	 * Run game from command line stop check. Called when a game stops. Checks if the last
	 * game stopped running and shuts down the application if run from the command line.
	 */
	void RunCommandLineGameStopCheck();
	
	/** Pulse checking. */
	void PulseChecking();
	/*@}*/
	
private:
	void pParseArguments();
	void pInitLogger();
	void pRegisterSignals();
};

#endif
