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
	dellRunGame(dellLauncher &launcher);
	
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
	void ShowModuleProblem(const char *moduleName, deModuleSystem::eModuleTypes moduleType);
	
	/** Run game. */
	void Run();
	/*@}*/
};

#endif
