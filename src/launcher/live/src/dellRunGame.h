/* 
 * Drag[en]gine Live Launcher
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

#ifndef _DELLRUNGAME_H_
#define _DELLRUNGAME_H_

#include <delauncher/game/delGame.h>
#include <delauncher/game/delGameRunParams.h>

#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/systems/deModuleSystem.h>

class dellLauncher;
class dellGame;
class dellGameProfile;
class dellGPModuleList;
class decStringList;


/**
 * Run Game Action.
 */
class dellRunGame{
private:
	dellLauncher &pLauncher;
	
	decString pDelgaFile;
	decString pProfileName;
	
	delGame::Ref pGame;
	delGameRunParams pRunParams;
	decUnicodeArgumentList pGameArgs;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create run game. */
	dellRunGame( dellLauncher &launcher );
	
	/** Clean up run game. */
	~dellRunGame();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Launcher. */
	inline dellLauncher &GetLauncher() const{ return pLauncher; }
	
	/** Parse arguments. */
	bool ParseArguments();
	
	/** Locate game to run. */
	bool LocateGame();
	
	/** Locate and test profile to run and update parameters. */
	bool LocateProfile();
	
	/** Update game arguments. */
	void UpdateRunArguments();
	
	/** Show problems with the game. */
	void ShowGameProblems();
	
	/** Show problems with the profile. */
	void ShowProfileProblems();
	
	/** Show module problem if there are any. */
	void ShowModuleProblem(const char *moduleName, deModuleSystem::eModuleTypes moduleType );
	
	/** Run game. */
	void Run();
	/*@}*/
};

#endif
