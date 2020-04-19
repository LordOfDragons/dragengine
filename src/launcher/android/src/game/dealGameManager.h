/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DEALGAMEMANAGER_H_
#define _DEALGAMEMANAGER_H_

#include "dealGameList.h"
#include "profile/dealGameProfileList.h"

class dealLauncher;



/**
 * \brief Game Manager.
 * 
 * Handles everything related with installed games, installing games, deinstalling
 * games, keeping games updated and game configurations.
 * 
 */
class dealGameManager{
private:
	dealLauncher &pLauncher;
	dealGameList pGameList;
	
	dealGameProfileList pProfileList;
	dealGameProfile *pActiveProfile;
	dealGameProfile *pDefaultProfile;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game manager. */
	dealGameManager( dealLauncher &launcher );
	
	/** \brief Clean up game manager. */
	~dealGameManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Game list. */
	inline dealGameList &GetGameList(){ return pGameList; }
	inline const dealGameList &GetGameList() const{ return pGameList; }
	
	/** \brief Load list of games from game directory. */
	void LoadGameList();
	
	/** \brief Create default profile from the current engine state. */
	void CreateDefaultProfile();
	
	/** \brief Verify games and profiles. */
	void Verify();
	
	/** \brief Apply changes in the profile list to games. Missing profiles are set to NULL. */
	void ApplyProfileChanges();
	
	/** \brief Load game from a disk file. */
	dealGame *LoadGameFromDisk( const char *path );
	
	/** \brief Profile list. */
	inline dealGameProfileList &GetProfileList(){ return pProfileList; }
	inline const dealGameProfileList &GetProfileList() const{ return pProfileList; }
	
	/** \brief Active profile or \em NULL if none is active. */
	inline dealGameProfile *GetActiveProfile() const{ return pActiveProfile; }
	
	/** \brief Set active profile or \em NULL if none is active. */
	void SetActiveProfile( dealGameProfile *profile );
	
	/** \brief Default profile. */
	inline dealGameProfile *GetDefaultProfile() const{ return pDefaultProfile; }
	
	/** \brief Load game configurations. */
	void LoadGameConfigs();
	
	/** \brief Save game configurations. */
	void SaveGameConfigs();
	/*@}*/
};

#endif
