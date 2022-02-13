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

#include <delauncher/game/delGame.h>
#include <delauncher/game/delGameRunParams.h>

#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/common/utils/decUuid.h>
#include <dragengine/systems/deModuleSystem.h>

class declLauncher;
class delGameProfile;
class delGPModuleList;


/**
 * Game Action.
 */
class declRunGame{
private:
	declLauncher &pLauncher;
	
	decString pGameIdentifier;
	decString pGameDefFile;
	decString pProfileName;
	bool pUseConsole;
	bool pLogAllToConsole;
	decUnicodeArgumentList pGameArgs;
	
	delGame::Ref pGame;
	delGPModuleList *pModuleParameters;
	decUuid pPatchIdentifier;
	bool pHasPatchIdentifier;
	delGameRunParams pRunParams;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create action. */
	declRunGame( declLauncher &launcher );
	
	/** Clean up action. */
	~declRunGame();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Launcher. */
	inline declLauncher &GetLauncher() const{ return pLauncher; }
	
	/** Print syntax. */
	void PrintSyntax();
	
	/** Parse arguments. */
	bool ParseArguments();
	
	/** Parse module parameter argument. */
	bool ParseModuleParameter( const decString &value );
	
	/** Locate game to run. */
	bool LocateGame();
	
	/** Locate and test profile to run and update parameters. */
	bool LocateProfile();
	
	/** Update run arguments. */
	void UpdateRunArguments();
	
	/** Apply custom module parameters. */
	void ApplyCustomModuleParameters();
	
	/** Print the problems with the game. */
	void PrintGameProblems();
	
	/** Print the problems with the profile. */
	void PrintProfileProblems( const delGameProfile &profile );
	
	/** Print module problem if there are any. */
	void PrintModuleProblem( const char *moduleName, deModuleSystem::eModuleTypes moduleType );
	
	/** Run action. */
	void Run();
	/*@}*/
};

#endif
