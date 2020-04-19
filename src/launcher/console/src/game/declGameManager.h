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

#ifndef _DECLGAMEMANAGER_H_
#define _DECLGAMEMANAGER_H_

#include "declGameList.h"
#include "profile/declGameProfileList.h"

class declLauncher;
class deVFSDiskDirectory;
class deVirtualFileSystem;
class decPath;


/**
 * \brief Game Manager.
 */
class declGameManager{
private:
	declLauncher *pLauncher;
	declGameList pGameList;
	
	declGameProfileList pProfileList;
	declGameProfile *pActiveProfile;
	declGameProfile *pDefaultProfile;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game manager. */
	declGameManager( declLauncher *launcher );
	
	/** \brief Clean up game manager. */
	~declGameManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Game list. */
	inline declGameList &GetGameList(){ return pGameList; }
	inline const declGameList &GetGameList() const{ return pGameList; }
	
	/** \brief Load list of games from game directory. */
	void LoadGameList();
	
	/** \brief Create default profile from the current engine state. */
	void CreateDefaultProfile();
	
	/** \brief Verifie games and profiles. */
	void Verify();
	
	/** \brief Load a game from a disk file. */
	void LoadGameFromDisk( const decString &path, declGameList &list );
	
	/** \brief Profile list. */
	inline declGameProfileList &GetProfileList(){ return pProfileList; }
	inline const declGameProfileList &GetProfileList() const{ return pProfileList; }
	
	/** \brief Active profile or NULL if none is active. */
	inline declGameProfile *GetActiveProfile() const{ return pActiveProfile; }
	
	/** \brief Set active profile or NULL if none is active. */
	void SetActiveProfile( declGameProfile *profile );
	
	/** \brief Default profile. */
	inline declGameProfile *GetDefaultProfile() const{ return pDefaultProfile; }
	
	/** \brief Load game configurations. */
	void LoadGameConfigs();
	/*@}*/
	
	
private:
	void pScanGameDefFiles( deVirtualFileSystem &vfs, const decPath &baseDir, const decPath &directory );
	void pProcessFoundFiles( const decPath &path );
};

#endif
