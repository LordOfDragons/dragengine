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

#ifndef _DELGAMEMANAGER_H_
#define _DELGAMEMANAGER_H_

#include "delGameList.h"
#include "profile/delGameProfile.h"
#include "profile/delGameProfileList.h"

class delLauncherSupport;
class decPath;
class deVFSDiskDirectory;
class deVirtualFileSystem;
class delEngineInstance;


/**
 * \brief Game Manager.
 */
class delGameManager{
private:
	delLauncherSupport &pSupport;
	delGameList pGames;
	
	delGameProfileList pProfileList;
	delGameProfile::Ref pActiveProfile;
	delGameProfile::Ref pDefaultProfile;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game manager. */
	delGameManager( delLauncherSupport &support );
	
	/** \brief Clean up game manager. */
	~delGameManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Game list. */
	inline delGameList &GetGameList(){ return pGames; }
	inline const delGameList &GetGameList() const{ return pGames; }
	
	/** \brief Load list of games from game directory. */
	void LoadGameList( delEngineInstance &instance );
	
	/** \brief Create default profile from the current engine state. */
	void CreateDefaultProfile();
	
	/** \brief Verifie games and profiles. */
	void Verify();
	
	/** \brief Apply changes in the profile list to games. Missing profiles are set to NULL. */
	void ApplyProfileChanges();
	
	/** \brief Load game from disk file. */
	void LoadGameFromDisk( delEngineInstance &instance, const decString &path, delGameList &list );
	
	/** \brief Profile list. */
	inline delGameProfileList &GetProfileList(){ return pProfileList; }
	inline const delGameProfileList &GetProfileList() const{ return pProfileList; }
	
	/** \brief Active profile or NULL if none is active. */
	inline delGameProfile *GetActiveProfile() const{ return pActiveProfile; }
	
	/** \brief Set active profile or NULL if none is active. */
	void SetActiveProfile( delGameProfile *profile );
	
	/** \brief Default profile. */
	inline delGameProfile *GetDefaultProfile() const{ return pDefaultProfile; }
	
	/** \brief Pulse checking. */
	void PulseChecking();
	
	/** \brief Load game configurations. */
	void LoadGameConfigs();
	
	/** \brief Save game configurations. */
	void SaveGameConfigs();
	/*@}*/
	
	
	
private:
	void pScanGameDefFiles( delEngineInstance &instance, deVirtualFileSystem &vfs,
		const decPath &baseDir, const decPath &directory );
	
	void pProcessFoundFiles( delEngineInstance &instance, const decPath &path );
};

#endif
