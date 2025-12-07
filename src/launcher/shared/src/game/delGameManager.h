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

#ifndef _DELGAMEMANAGER_H_
#define _DELGAMEMANAGER_H_

#include "delGameList.h"
#include "profile/delGameProfile.h"
#include "profile/delGameProfileList.h"

class delLauncher;
class decPath;
class deVFSDiskDirectory;
class deVirtualFileSystem;
class delEngineInstance;


/**
 * \brief Game Manager.
 */
class DE_DLL_EXPORT delGameManager{
private:
	delLauncher &pLauncher;
	delGameList pGames;
	
	delGameProfileList pProfiles;
	delGameProfile::Ref pActiveProfile;
	delGameProfile::Ref pDefaultProfile;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game manager. */
	delGameManager(delLauncher &launcher);
	
	/** \brief Clean up game manager. */
	~delGameManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Launcher. */
	inline delLauncher &GetLauncher() const{ return pLauncher; }
	
	/** \brief Game list. */
	inline delGameList &GetGames(){ return pGames; }
	inline const delGameList &GetGames() const{ return pGames; }
	
	/** \brief Load list of games from game directory. */
	void LoadGames(delEngineInstance &instance);
	
	/** \brief Create default profile from the current engine state. */
	void CreateDefaultProfile();
	
	/** \brief Verifie games and profiles. */
	void Verify();
	
	/** \brief Apply changes in the profile list to games. Missing profiles are set to NULL. */
	void ApplyProfileChanges();
	
	/** \brief Load game from disk file. */
	void LoadGameFromDisk(delEngineInstance &instance, const decString &path, delGameList &list);
	
	/** \brief Profile list. */
	inline delGameProfileList &GetProfiles(){ return pProfiles; }
	inline const delGameProfileList &GetProfiles() const{ return pProfiles; }
	
	/** \brief Active profile or NULL if none is active. */
	inline delGameProfile *GetActiveProfile() const{ return pActiveProfile; }
	
	/** \brief Set active profile or NULL if none is active. */
	void SetActiveProfile(delGameProfile *profile);
	
	/** \brief Default profile. */
	inline delGameProfile *GetDefaultProfile() const{ return pDefaultProfile; }
	
	/** \brief Pulse checking. */
	void PulseChecking();
	
	/** \brief Load game configurations. */
	void LoadGameConfigs();
	
	/** \brief Save game configurations. */
	void SaveGameConfigs();
	
	/** \brief Clear. */
	void Clear();
	/*@}*/
	
	
	
private:
	void pScanGameDefFiles(delEngineInstance &instance, deVirtualFileSystem &vfs,
		const decPath &baseDir, const decPath &directory);
	
	void pProcessFoundFiles(delEngineInstance &instance, const decPath &path);
};

#endif
