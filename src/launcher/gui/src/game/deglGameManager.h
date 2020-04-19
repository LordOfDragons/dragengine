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

#ifndef _DEGLGAMEMANAGER_H_
#define _DEGLGAMEMANAGER_H_

#include "deglGameList.h"
#include "profile/deglGameProfileList.h"

class deglLauncher;
class decPath;
class deVFSDiskDirectory;
class deVirtualFileSystem;
class deglEngineInstance;


/**
 * \brief Game Manager.
 */
class deglGameManager{
private:
	deglLauncher *pLauncher;
	deglGameList pGameList;
	
	deglGameProfileList pProfileList;
	deglGameProfile *pActiveProfile;
	deglGameProfile *pDefaultProfile;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game manager. */
	deglGameManager( deglLauncher *launcher );
	
	/** \brief Clean up game manager. */
	~deglGameManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Game list. */
	inline deglGameList &GetGameList(){ return pGameList; }
	inline const deglGameList &GetGameList() const{ return pGameList; }
	
	/** \brief Load list of games from game directory. */
	void LoadGameList( deglEngineInstance &instance );
	
	/** \brief Create default profile from the current engine state. */
	void CreateDefaultProfile();
	
	/** \brief Verifie games and profiles. */
	void Verify();
	
	/** \brief Apply changes in the profile list to games. Missing profiles are set to NULL. */
	void ApplyProfileChanges();
	
	/** \brief Load game from disk file. */
	void LoadGameFromDisk( deglEngineInstance &instance, const decString &path, deglGameList &list );
	
	/** \brief Profile list. */
	inline deglGameProfileList &GetProfileList(){ return pProfileList; }
	inline const deglGameProfileList &GetProfileList() const{ return pProfileList; }
	
	/** \brief Active profile or NULL if none is active. */
	inline deglGameProfile *GetActiveProfile() const{ return pActiveProfile; }
	
	/** \brief Set active profile or NULL if none is active. */
	void SetActiveProfile( deglGameProfile *profile );
	
	/** \brief Default profile. */
	inline deglGameProfile *GetDefaultProfile() const{ return pDefaultProfile; }
	
	/** \brief Pulse checking. */
	void PulseChecking();
	
	/** \brief Load game configurations. */
	void LoadGameConfigs();
	
	/** \brief Save game configurations. */
	void SaveGameConfigs();
	/*@}*/
	
	
private:
	void pScanGameDefFiles( deglEngineInstance &instance, deVirtualFileSystem &vfs,
		const decPath &baseDir, const decPath &directory );
	void pProcessFoundFiles( deglEngineInstance &instance, const decPath &path );
};

#endif
