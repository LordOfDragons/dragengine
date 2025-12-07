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
	declRunGame(declLauncher &launcher);
	
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
	bool ParseModuleParameter(const decString &value);
	
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
	void PrintProfileProblems(const delGameProfile &profile);
	
	/** Print module problem if there are any. */
	void PrintModuleProblem(const char *moduleName, deModuleSystem::eModuleTypes moduleType);
	
	/** Run action. */
	void Run();
	/*@}*/
};

#endif
