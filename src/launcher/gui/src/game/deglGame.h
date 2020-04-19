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

#ifndef _DEGLGAME_H_
#define _DEGLGAME_H_

#include "fileformat/deglFileFormatList.h"
#include "profile/deglGPModuleList.h"

#include <dragengine/deObject.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/common/file/decMemoryFileReference.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/utils/decUuid.h>


class deglEngineInstance;
class deglGameProfile;
class deglGameRunParams;
class deglGPModuleList;
class deglLauncher;
class deglPatchList;
class deglSharedIcon;


/**
 * @brief Installed Game.
 * Describes an installed game. The launch name is the identifier to be used
 * on the command line if the user wishes to run the game directory. The title
 * stores the full game title. The description contains a formatted description
 * of the game without length restriction. The creator stores the name of the
 * company, team or person that created the game. The homepage stores the URL
 * of the game homepage or optionally the creator homepage.
 * 
 * @par Installation
 * Once installed a game has a game directory under which the game data is
 * installed. The data directory defines the directory relative to the game
 * directory containing the game data. The script directory defines the directory
 * relative to the game directory containing the game scripts.
 * 
 * @par Running
 * For running the game each game object has an engine instance object. This way
 * the game runs in a separate child process and does not take down the entire
 * launcher in case of an unexpected failure. Only one instance of a game can be
 * running at the same time. This might be changed later on if required.
 */
class deglGame : public deObject{
private:
	deglLauncher &pLauncher;
	
	decUuid pIdentifier;
	decString pAliasIdentifier;
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
	decString pScriptModuleVersion;
	decPoint pWindowSize;
	deglFileFormatList pFileFormatList;
	
	decString pPathConfig;
	decString pPathCapture;
	
	decString pIconPath;
	int pIconSize;
	decMemoryFileReference pIconFileContent;
	deObjectReference pFoxIconMini;
	deObjectReference pFoxIconBig;
	
	deglEngineInstance *pEngineInstance;
	deglGPModuleList pCollectChangedParams;
	deObjectReference pCollectChangedParamsProfile;
	
	bool pAllFormatsSupported;
	bool pScriptModuleFound;
	decString pScriptModuleFoundVersion;
	bool pGameUpToDate;
	bool pCanRun;
	
	deObjectReference pCustomProfile;
	deObjectReference pActiveProfile;
	
	decString pRunArguments;
	
	decString pLogFile;
	bool pLogToConsole;
	
	decString pDelgaFile;
	
	bool pUseLatestPatch;
	decUuid pUseCustomPatch;
	
	
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new game. */
	deglGame( deglLauncher &launcher );
	/** Cleans up the game. */
	virtual ~deglGame();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Identifier. */
	inline const decUuid &GetIdentifier() const{ return pIdentifier; }
	
	/** \brief Set identifier. */
	void SetIdentifier( const decUuid &identifier );
	
	/** \brief Alias identifier. */
	inline const decString &GetAliasIdentifier() const{ return pAliasIdentifier; }
	
	/** \brief Set alias identifier. */
	void SetAliasIdentifier( const char *identifier );
	
	/** Retrieves the game title. */
	inline const decUnicodeString &GetTitle() const{ return pTitle; }
	/** Sets the game title. */
	void SetTitle( const decUnicodeString &title );
	/** Retrieves the description. */
	inline const decUnicodeString &GetDescription() const{ return pDescription; }
	/** Sets the description. */
	void SetDescription( const decUnicodeString &description );
	/** Retrieves the creator. */
	inline const decUnicodeString &GetCreator() const{ return pCreator; }
	/** Sets the creator. */
	void SetCreator( const decUnicodeString &creator );
	/** Retrieves the homepage. */
	inline const decString &GetHomepage() const{ return pHomepage; }
	/** Sets the homepage. */
	void SetHomepage( const char *homepage );
	
	/** Retrieves the game directory. */
	inline const decString &GetGameDirectory() const{ return pGameDirectory; }
	/** Sets the game directory. */
	void SetGameDirectory( const char *directory );
	
	/** \brief Data directory relative to game directory. */
	inline const decString &GetDataDirectory() const{ return pDataDirectory; }
	
	/** \brief Set data directory relative to game directory. */
	void SetDataDirectory( const char *directory );
	
	/** \brief Script directory relative to data directory. */
	inline const decString &GetScriptDirectory() const{ return pScriptDirectory; }
	
	/** \brief Set script directory relative to data directory. */
	void SetScriptDirectory( const char *directory );
	
	/** \brief Game object. */
	inline const decString &GetGameObject() const{ return pGameObject; }
	
	/** \brief Set game object. */
	void SetGameObject( const char *gameObject );
	
	/** \brief Script module to use. */
	inline const decString &GetScriptModule() const{ return pScriptModule; }
	
	/** \brief Set script module to use. */
	void SetScriptModule( const char *module );
	
	/** \brief Script module minimum version to use or empty string. */
	inline const decString &GetScriptModuleVersion() const{ return pScriptModuleVersion; }
	
	/** \brief Set script module minimum version to use or empty string. */
	void SetScriptModuleVersion( const char *version );
	
	/** \brief Window size or 0 to run fullscreen if possible. */
	inline const decPoint &GetWindowSize() const{ return pWindowSize; }
	
	/** \brief Set window size or 0 to run fullscreen if possible. */
	void SetWindowSize( const decPoint &size );
	
	/** Retrieves the file format list. */
	inline deglFileFormatList &GetFileFormatList(){ return pFileFormatList; }
	inline const deglFileFormatList &GetFileFormatList() const{ return pFileFormatList; }
	
	/** Retrieves the virtual file system path where the game configuration container is placed at. */
	inline const decString &GetPathConfig() const{ return pPathConfig; }
	/** Sets the virtual file system path where the game configuration container is placed at. */
	void SetPathConfig( const char *path );
	/** Retrieves the virtual file system path where the game capture container is placed at. */
	inline const decString &GetPathCapture() const{ return pPathCapture; }
	/** Sets the virtual file system path where the game capture container is placed at. */
	void SetPathCapture( const char *path );
	
	/** \brief Icon path or empty string. */
	inline const decString &GetIconPath() const{ return pIconPath; }
	
	/** \brief Get icon size. */
	inline int GetIconSize() const{ return pIconSize; }
	
	/** \brief Set icon. */
	void SetIcon( const char *path, int size );
	
	/** \brief Icon file content. */
	inline decMemoryFile &GetIconFileContent() const{ return pIconFileContent; }
	
	/** \brief Set icon file content. */
	void SetIconFileContent( decMemoryFile *content );
	
	/** \brief Create FOX icon from icon file content if possible. */
	void CreateFoxIcons();
	
	/** \brief FOX small icon or NULL. */
	inline deglSharedIcon *GetFoxIconMini() const{ return ( deglSharedIcon* )( deObject* )pFoxIconMini; }
	
	/** \brief FOX big icon or NULL. */
	inline deglSharedIcon *GetFoxIconBig() const{ return ( deglSharedIcon* )( deObject* )pFoxIconBig; }
	
	/** Determines if an engine instance exists. */
	bool HasEngineInstance() const;
	/** Retrieves the engine instance. */
	inline deglEngineInstance *GetEngineInstance() const{ return pEngineInstance; }
	/** Sets the engine instance. */
	void SetEngineInstance( deglEngineInstance *engineInstance );
	
	/** Verifies if all requirements are fullfilled storing the results for quick retrieval. */
	void VerifyRequirements();
	/** Determines if all file formats are supported. */
	inline bool GetAllFormatsSupported() const{ return pAllFormatsSupported; }
	/** Sets if all file formats are supported. */
	void SetAllFormatsSupported( bool supported );
	/** Determines if the script module is found. */
	inline bool GetScriptModuleFound() const{ return pScriptModuleFound; }
	/** Sets if the script module is found. */
	void SetScriptModuleFound( bool found );
	/** Determines if the game is up to date. */
	inline bool GetGameUpToDate() const{ return pGameUpToDate; }
	/** Sets if the game is up to date. */
	void SetGameUpToDate( bool upToDate );
	/** Determines if the game can be run. */
	inline bool GetCanRun() const{ return pCanRun; }
	/** Sets if the game can be run. */
	void SetCanRun( bool canRun );
	
	/** \brief Custom profile or NULL to use global profile. */
	inline deglGameProfile *GetCustomProfile() const{ return ( deglGameProfile* )( deObject* )pCustomProfile; }
	
	/** \brief Set custom profile or NULL to use global profile. */
	void SetCustomProfile( deglGameProfile *profile );
	
	/** \brief Active profile or NULL to use last run profile. */
	inline deglGameProfile *GetActiveProfile() const{ return ( deglGameProfile* )( deObject* )pActiveProfile; }
	
	/** \brief Set active profile or NULL to use last run profile. */
	void SetActiveProfile( deglGameProfile *profile );
	
	/** Retrieves the run arguments. */
	inline const decString &GetRunArguments() const{ return pRunArguments; }
	/** Sets the run arguments. */
	void SetRunArguments( const char *arguments );
	
	/** Retrieves the engine log filename. */
	inline const decString &GetLogFile() const{ return pLogFile; }
	/** Sets the engine log filename. */
	void SetLogFile( const char *path );
	/** Set default log filename which is /logs/delauncher-game-{game-name}. */
	void SetDefaultLogFile();
	/** Determines if the logs are written to the console instead of a file. */
	inline bool GetLogToConsole() const{ return pLogToConsole; }
	/** Sets if the logs are written to the console instead of a file. */
	void SetLogToConsole( bool logToConsole );
	
	/** \brief Delga file name or empty string. */
	inline const decString &GetDelgaFile() const{ return pDelgaFile; }
	
	/** \brief Set delga file name or empty string. */
	void SetDelgaFile( const char *delgaFile );
	
	/** \brief Use latest patch. */
	inline bool GetUseLatestPatch() const{ return pUseLatestPatch; }
	
	/** \brief Set use latest patch. */
	void SetUseLatestPatch( bool useLatestPatch );
	
	/** \brief Custom patch to use or empty string to use unpatched. */
	inline const decUuid &GetUseCustomPatch() const{ return pUseCustomPatch; }
	
	/** \brief Set custom patch to use or empty string to use unpatched. */
	void SetUseCustomPatch( const decUuid &patch );
	
	
	
	
	/** Determines if the game is running. */
	bool IsRunning() const;
	/** Start game. */
	void StartGame( const deglGameRunParams &runParams );
	/** Stop game. */
	void StopGame();
	/** Kill game. */
	void KillGame();
	/** Pulse checking. */
	void PulseChecking();
	/** Load configuration. */
	void LoadConfig();
	/** Save configuration. */
	void SaveConfig();
	/** Determine profile to use. */
	deglGameProfile *GetProfileToUse() const;
	
	/** \brief Find patches for this game. */
	void FindPatches( deglPatchList &list ) const;
	
	/** \brief Sort patches by dependencies so the newest is first. */
	void SortPatches( deglPatchList &sorted, const deglPatchList &patches ) const;
	/*@}*/
	
	
	
private:
	void pStoreCustomConfig();
};

#endif
