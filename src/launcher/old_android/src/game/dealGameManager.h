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
	dealGameManager(dealLauncher &launcher);
	
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
	dealGame *LoadGameFromDisk(const char *path);
	
	/** \brief Profile list. */
	inline dealGameProfileList &GetProfileList(){ return pProfileList; }
	inline const dealGameProfileList &GetProfileList() const{ return pProfileList; }
	
	/** \brief Active profile or \em NULL if none is active. */
	inline dealGameProfile *GetActiveProfile() const{ return pActiveProfile; }
	
	/** \brief Set active profile or \em NULL if none is active. */
	void SetActiveProfile(dealGameProfile *profile);
	
	/** \brief Default profile. */
	inline dealGameProfile *GetDefaultProfile() const{ return pDefaultProfile; }
	
	/** \brief Load game configurations. */
	void LoadGameConfigs();
	
	/** \brief Save game configurations. */
	void SaveGameConfigs();
	/*@}*/
};

#endif
