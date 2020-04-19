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

#ifndef _DECLRUNGAME_H_
#define _DECLRUNGAME_H_

#include "../game/patch/declPatchList.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/common/utils/decUuid.h>
#include <dragengine/systems/deModuleSystem.h>

class declLauncher;
class declGame;
class declGameProfile;
class declGPModuleList;


/**
 * @brief Run Game Action.
 */
class declRunGame{
private:
	declLauncher *pLauncher;
	
	decString pGameIdentifier;
	decString pGameDefFile;
	decString pProfileName;
	bool pUseConsole;
	bool pLogAllToConsole;
	
	declGame *pGame;
	declGameProfile *pProfile;
	declGPModuleList *pModuleParameters;
	decUuid pPatchIdentifier;
	bool pHasPatchIdentifier;
	declPatchList pPatches;
	decString pRunArguments;
	int pRunWidth;
	int pRunHeight;
	bool pRunFullScreen;
	decString pWindowTitle;
	decUnicodeArgumentList pGameArgs;
	
	
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new action. */
	declRunGame( declLauncher *launcher );
	/** Cleans up the action. */
	~declRunGame();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the launcher. */
	inline declLauncher *GetLauncher() const{ return pLauncher; }
	
	/** Print syntax. */
	void PrintSyntax();
	/** Parse arguments. */
	bool ParseArguments();
	/** Parse module parameter argument. */
	bool ParseModuleParameter( const decString &value );
	/** Init launcher. */
	void InitLauncher();
	/** Locate game to run. */
	bool LocateGame();
	/** \brief Locate and test profile to run and update parameters. */
	bool LocateProfile();
	
	/** \brief Locate patches to apply. */
	bool LocatePatches();
	
	/** Print the problems with the game. */
	void PrintGameProblems();
	/** Print the problems with the profile. */
	void PrintProfileProblems();
	/** Print module problem if there are any. */
	void PrintModuleProblem( const char *moduleName, deModuleSystem::eModuleTypes moduleType );
	/** Activate script module. */
	void ActivateScriptModule();
	/** Add disk directory to virtual file system. */
	void VFSAddDiskDir( const char *pathRoot, const char *pathDisk, bool readOnly );
	/** Add containers to the engine virtual file system. */
	void InitVFS();
	/** Copy game arguments to engine argument list. */
	void CopyGameArguments();
	
	/** Run action. */
	void Run();
	
	/** \brief Process module parameter changes. */
	void StoreCustomConfig( const declGPModuleList &changedModules );
	/*@}*/
};

#endif
