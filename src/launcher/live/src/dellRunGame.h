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

#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/systems/deModuleSystem.h>

class dellLauncher;
class dellGame;
class dellGameProfile;
class dellGPModuleList;
class decStringList;


/**
 * \brief Run Game Action.
 */
class dellRunGame{
private:
	dellLauncher &pLauncher;
	
	decString pDelgaFile;
	decString pProfileName;
	
	dellGame *pGame;
	dellGameProfile *pProfile;
	dellGPModuleList *pModuleParameters;
	decString pRunArguments;
	int pRunWidth;
	int pRunHeight;
	bool pRunFullScreen;
	decString pWindowTitle;
	decUnicodeArgumentList pGameArgs;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create run game. */
	dellRunGame( dellLauncher &launcher );
	
	/** \brief Clean up run game. */
	~dellRunGame();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Launcher. */
	inline dellLauncher &GetLauncher() const{ return pLauncher; }
	
	/** \brief Parse arguments. */
	bool ParseArguments();
	
	/** \brief Parse module parameter argument. */
	bool ParseModuleParameter( const decString &value );
	
	/** \brief Init launcher. */
	void InitLauncher();
	
	/** \brief Locate game to run. */
	bool LocateGame();
	
	/** \brief Locate and test profile to run and update parameters. */
	bool LocateProfile();
	
	/** \brief Show problems with the game. */
	void ShowGameProblems();
	
	/** \brief Show problems with the profile. */
	void ShowProfileProblems();
	
	/** \brief Show module problem if there are any. */
	void ShowModuleProblem(const char *moduleName, deModuleSystem::eModuleTypes moduleType );
	
	/** \brief Activate script module. */
	void ActivateScriptModule();
	
	/** \brief Add disk directory to virtual file system. */
	void VFSAddDiskDir( const char *pathRoot, const char *pathDisk, bool readOnly );
	
	/** \brief Add containers to the engine virtual file system. */
	void InitVFS();
	
	/** \brief Copy game arguments to engine argument list. */
	void CopyGameArguments();
	
	/** \brief Run game. */
	void Run();
	
	/** \brief Process module parameter changes. */
	void StoreCustomConfig( const dellGPModuleList &changedModules );
	/*@}*/
};

#endif
