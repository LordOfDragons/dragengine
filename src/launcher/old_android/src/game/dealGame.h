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

#ifndef _DEALGAME_H_
#define _DEALGAME_H_

#include "fileformat/dealFileFormatList.h"
#include "profile/dealGameProfileList.h"
#include "../deObject.h"
#include "../common/math/decMath.h"
#include "../common/string/decString.h"
#include "../common/string/unicode/decUnicodeString.h"

class dealLauncher;
class dealGameRunParams;



/**
 * \brief Installed Game.
 * Describes an installed game. The launch name is the identifier to be used
 * on the command line if the user wishes to run the game directory. The title
 * stores the full game title. The description contains a formatted description
 * of the game without length restriction. The creator stores the name of the
 * company, team or person that created the game. The homepage stores the URL
 * of the game homepage or optionally the creator homepage.
 * 
 * \par Installation
 * Once installed a game has a game directory under which the game data is
 * installed. The data directory defines the directory relative to the game
 * directory containing the game data. The script directory defines the directory
 * relative to the game directory containing the game scripts.
 * 
 * \par Running
 * For running the game each game object has an engine instance object. This way
 * the game runs in a separate child process and does not take down the entire
 * launcher in case of an unexpected failure. Only one instance of a game can be
 * running at the same time. This might be changed later on if required.
 */
class dealGame : public deObject{
private:
	dealLauncher &pLauncher;
	
	decString pIdentifier;
	decUnicodeString pTitle;
	decUnicodeString pDescription;
	decUnicodeString pCreator;
	decString pHomepage;
	
	//deGameIcon **pIcons;
	//int pIconCount, pIconSize;
	
	decString pGameDirectory;
	decString pDataDirectory;
	decString pScriptDirectory;
	decString pGameObject;
	decString pScriptModule;
	decPoint pWindowSize;
	dealFileFormatList pFileFormatList;
	
	decString pPathConfig;
	decString pPathCapture;
	
	bool pAllFormatsSupported;
	bool pScriptModuleFound;
	bool pGameUpToDate;
	bool pCanRun;
	
	dealGameProfileList pProfileList;
	dealGameProfile *pGlobalProfile;
	dealGameProfile *pGameProfile;
	
	decString pRunArguments;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<dealGame> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game. */
	dealGame(dealLauncher &launcher);
	
	/** \brief Clean up game. */
	virtual ~dealGame();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Identifier. */
	inline const decString &GetIdentifier() const{ return pIdentifier; }
	
	/** \brief Set identifier. */
	void SetIdentifier(const char *identifier);
	
	/** \brief Game title. */
	inline const decUnicodeString &GetTitle() const{ return pTitle; }
	
	/** \brief Set game title. */
	void SetTitle(const decUnicodeString &title);
	
	/** \brief Description. */
	inline const decUnicodeString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description. */
	void SetDescription(const decUnicodeString &description);
	
	/** \brief Creator. */
	inline const decUnicodeString &GetCreator() const{ return pCreator; }
	
	/** \brief Set creator. */
	void SetCreator(const decUnicodeString &creator);
	
	/** \brief Homepage. */
	inline const decString &GetHomepage() const{ return pHomepage; }
	
	/** \brief Set homepage. */
	void SetHomepage(const char *homepage);
	
	
	
	/** \brief Game directory. */
	inline const decString &GetGameDirectory() const{ return pGameDirectory; }
	
	/** \brief Set game directory. */
	void SetGameDirectory(const char *directory);
	
	/** \brief Data directory relative to game directory. */
	inline const decString &GetDataDirectory() const{ return pDataDirectory; }
	
	/** \brief Set data directory relative to game directory. */
	void SetDataDirectory(const char *directory);
	
	/** \brief Script directory relative to data directory. */
	inline const decString &GetScriptDirectory() const{ return pScriptDirectory; }
	
	/** \brief Set script directory relative to data directory. */
	void SetScriptDirectory(const char *directory);
	
	/** \brief Game object. */
	inline const decString &GetGameObject() const{ return pGameObject; }
	
	/** \brief Set game object. */
	void SetGameObject(const char *gameObject);
	
	/** \brief Script module to use. */
	inline const decString &GetScriptModule() const{ return pScriptModule; }
	
	/** \brief Set script module. */
	void SetScriptModule(const char *module);
	
	/** \brief File format list. */
	inline dealFileFormatList &GetFileFormatList(){ return pFileFormatList; }
	inline const dealFileFormatList &GetFileFormatList() const{ return pFileFormatList; }
	
	/** \brief Virtual file system path where the game configuration container is placed at. */
	inline const decString &GetPathConfig() const{ return pPathConfig; }
	
	/** \brief Set virtual file system path where the game configuration container is placed at. */
	void SetPathConfig(const char *path);
	
	/** \brief Virtual file system path where the game capture container is placed at. */
	inline const decString &GetPathCapture() const{ return pPathCapture; }
	
	/** \brief Set virtual file system path where the game capture container is placed at. */
	void SetPathCapture(const char *path);
	
	/** \brief Window size or 0 to run fullscreen if possible. */
	inline const decPoint &GetWindowSize() const{ return pWindowSize; }
	
	/** \brief Set window size or 0 to run fullscreen if possible. */
	void SetWindowSize(const decPoint &size);
	
	
	
	/** \brief Verify if all requirements are fullfilled storing the results for quick retrieval. */
	void VerifyRequirements();
	
	/** \brief All file formats are supported. */
	inline bool GetAllFormatsSupported() const{ return pAllFormatsSupported; }
	
	/** \brief Set if all file formats are supported. */
	void SetAllFormatsSupported(bool supported);
	
	/** \brief Script module is found. */
	inline bool GetScriptModuleFound() const{ return pScriptModuleFound; }
	
	/** \brief Set if script module is found. */
	void SetScriptModuleFound(bool found);
	
	/** \brief Game is up to date. */
	inline bool GetGameUpToDate() const{ return pGameUpToDate; }
	
	/** \brief Set if game is up to date. */
	void SetGameUpToDate(bool upToDate);
	
	/** \brief Game can be run. */
	inline bool GetCanRun() const{ return pCanRun; }
	
	/** \brief Set if game can be run. */
	void SetCanRun(bool canRun);
	
	
	
	/** \brief Profile list. */
	inline dealGameProfileList &GetProfileList(){ return pProfileList; }
	inline const dealGameProfileList &GetProfileList() const{ return pProfileList; }
	
	/** \brief Global profile or \em NULL if not selected. */
	inline dealGameProfile *GetGlobalProfile() const{ return pGlobalProfile; }
	
	/** \brief Set global profile or \em NULL if not selected. */
	void SetGlobalProfile(dealGameProfile *profile);
	
	/** \brief Game profile or \em NULL if not selected. */
	inline dealGameProfile *GetGameProfile() const{ return pGameProfile; }
	
	/** \brief Set game profile or \em NULL if not selected. */
	void SetGameProfile(dealGameProfile *profile);
	
	
	
	/** \brief Run arguments. */
	inline const decString &GetRunArguments() const{ return pRunArguments; }
	
	/** \brief Set run arguments. */
	void SetRunArguments(const char *arguments);
	
	
	
	/** \brief Load configuration. */
	void LoadConfig();
	
	/** \brief Save configuration. */
	void SaveConfig();
	
	/** \brief Apply changes in the profile list to games. Missing profiles are set to NULL. */
	void ApplyProfileChanges();
	
	/** \brief Profile to use. */
	dealGameProfile *GetProfileToUse() const;
	/*@}*/
};

#endif
