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

#ifndef _DELGAME_H_
#define _DELGAME_H_

#include "icon/delGameIconList.h"
#include "fileformat/delFileFormatList.h"
#include "profile/delGameProfile.h"
#include "profile/delGPModuleList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/utils/decUuid.h>


class delEngineInstance;
class delGameRunParams;
class delGPModuleList;
class delLauncherSupport;
class delPatchList;
class delSharedIcon;


/**
 * \brief Game.
 * 
 * Describes a game. This can be either an installed game or a not installed game for which
 * a directory exists from a previous run. The launch name is the identifier to be used
 * on the command line if the user wishes to run the game directory. The title stores the
 * full game title. The description contains a formatted description of the game without
 * length restriction. The creator stores the name of the company, team or person that
 * created the game. The homepage stores the URL of the game homepage or optionally the
 * creator homepage. This information is stored in the game configuration directory in
 * case the DELGA file becomes missing
 * 
 * \par Running
 * 
 * For running the game an engine instance is required. This allows to run games in a
 * separate child process and does not take down the entire launcher in case of an
 * unexpected failure.
 */
class delGame : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<delGame> Ref;
	
	
	
private:
	delLauncherSupport &pSupport;
	
	decUuid pIdentifier;
	decString pAliasIdentifier;
	decUnicodeString pTitle;
	decUnicodeString pDescription;
	decUnicodeString pCreator;
	decString pHomepage;
	delGameIconList pIcons;
	
	decString pGameDirectory;
	decString pDataDirectory;
	decString pScriptDirectory;
	decString pGameObject;
	decString pScriptModule;
	decString pScriptModuleVersion;
	decPoint pWindowSize;
	delFileFormatList pFileFormats;
	
	decString pPathConfig;
	decString pPathCapture;
	
	delEngineInstance *pEngineInstance;
	delGPModuleList pCollectChangedParams;
	delGameProfile::Ref pCollectChangedParamsProfile;
	
	bool pAllFormatsSupported;
	bool pScriptModuleFound;
	decString pScriptModuleFoundVersion;
	bool pGameUpToDate;
	bool pCanRun;
	
	delGameProfile::Ref pCustomProfile;
	delGameProfile::Ref pActiveProfile;
	
	decString pRunArguments;
	
	decString pLogFile;
	bool pLogToConsole;
	
	decString pDelgaFile;
	
	bool pUseLatestPatch;
	decUuid pUseCustomPatch;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game. */
	delGame( delLauncherSupport &support );
	
protected:
	/** \brief Clean up game. */
	virtual ~delGame();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Identifier. */
	inline const decUuid &GetIdentifier() const{ return pIdentifier; }
	
	/** \brief Set identifier. */
	void SetIdentifier( const decUuid &identifier );
	
	/** \brief Alias identifier. */
	inline const decString &GetAliasIdentifier() const{ return pAliasIdentifier; }
	
	/** \brief Set alias identifier. */
	void SetAliasIdentifier( const char *identifier );
	
	/** \brief Game title. */
	inline const decUnicodeString &GetTitle() const{ return pTitle; }
	
	/** \brief Set game title. */
	void SetTitle( const decUnicodeString &title );
	
	/** \brief Description. */
	inline const decUnicodeString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description. */
	void SetDescription( const decUnicodeString &description );
	
	/** \brief Creator. */
	inline const decUnicodeString &GetCreator() const{ return pCreator; }
	
	/** \brief Set creator. */
	void SetCreator( const decUnicodeString &creator );
	
	/** \brief Homepage. */
	inline const decString &GetHomepage() const{ return pHomepage; }
	
	/** \brief Set homepage. */
	void SetHomepage( const char *homepage );
	
	/** \brief Icons. */
	inline delGameIconList &GetIcons(){ return pIcons; }
	inline const delGameIconList &GetIcons() const{ return pIcons; }
	
	
	
	/** \brief Game directory. */
	inline const decString &GetGameDirectory() const{ return pGameDirectory; }
	
	/** \brief Set game directory. */
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
	inline delFileFormatList &GetFileFormats(){ return pFileFormats; }
	inline const delFileFormatList &GetFileFormats() const{ return pFileFormats; }
	
	
	
	/** \brief Virtual file system path where the game configuration container is placed at. */
	inline const decString &GetPathConfig() const{ return pPathConfig; }
	
	/** \brief Set virtual file system path where the game configuration container is placed at. */
	void SetPathConfig( const char *path );
	
	/** \brief Virtual file system path where the game capture container is placed at. */
	inline const decString &GetPathCapture() const{ return pPathCapture; }
	
	/** \brief Set virtual file system path where the game capture container is placed at. */
	void SetPathCapture( const char *path );
	
	
	
	/** Determines if an engine instance exists. */
	bool HasEngineInstance() const;
	/** Retrieves the engine instance. */
	inline delEngineInstance *GetEngineInstance() const{ return pEngineInstance; }
	/** Sets the engine instance. */
	void SetEngineInstance( delEngineInstance *engineInstance );
	
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
	inline delGameProfile *GetCustomProfile() const{ return pCustomProfile; }
	
	/** \brief Set custom profile or NULL to use global profile. */
	void SetCustomProfile( delGameProfile *profile );
	
	/** \brief Active profile or NULL to use last run profile. */
	inline delGameProfile *GetActiveProfile() const{ return pActiveProfile; }
	
	/** \brief Set active profile or NULL to use last run profile. */
	void SetActiveProfile( delGameProfile *profile );
	
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
	void StartGame( const delGameRunParams &runParams );
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
	delGameProfile *GetProfileToUse() const;
	
	/** \brief Find patches for this game. */
	void FindPatches( delPatchList &list ) const;
	
	/** \brief Sort patches by dependencies so the newest is first. */
	void SortPatches( delPatchList &sorted, const delPatchList &patches ) const;
	/*@}*/
	
	
	
private:
	void pStoreCustomConfig();
};

#endif
