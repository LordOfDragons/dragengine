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

#ifndef _DELGAME_H_
#define _DELGAME_H_

#include "icon/delGameIconList.h"
#include "fileformat/delFileFormatList.h"
#include "patch/delPatchList.h"
#include "profile/delGameProfile.h"
#include "profile/delGPModuleList.h"
#include "../engine/delEngineInstance.h"

#include <dragengine/deObject.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringDictionary.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/utils/decUuid.h>


class delGameRunParams;
class delGPModuleList;
class delLauncher;
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
class DE_DLL_EXPORT delGame : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<delGame> Ref;
	
	
	
private:
	delLauncher &pLauncher;
	
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
	
	delEngineInstance::Ref pEngineInstance;
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
	delPatchList pLocalPatches;
	
	decStringDictionary pCustomProperties;
	
#ifdef OS_ANDROID
	deVFSContainer::Ref pVFSDelgaContainer;
#endif
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game. */
	delGame( delLauncher &launcher );
	
protected:
	/** \brief Clean up game. */
	virtual ~delGame();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Launcher. */
	inline delLauncher &GetLauncher() const{ return pLauncher; }
	
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
	
	/** \brief Display scaled window size or 0 to run fullscreen if possible. */
	decPoint GetDisplayScaledWindowSize() const;
	
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
	
	
	
	/** \brief Engine instance is present. */
	bool HasEngineInstance() const;
	
	/** \brief Engine instance or nullptr. */
	inline const delEngineInstance::Ref &GetEngineInstance() const{ return pEngineInstance; }
	
	/** \brief Set engine instance or nullptr. */
	void SetEngineInstance( delEngineInstance *engineInstance );
	
	/** \brief Verify all requirements are fullfilled storing results. */
	void VerifyRequirements();
	
	/** \brief All file formats are supported. */
	inline bool GetAllFormatsSupported() const{ return pAllFormatsSupported; }
	
	/** \brief Set all file formats are supported. */
	void SetAllFormatsSupported( bool supported );
	
	/** \brief Script module is found. */
	inline bool GetScriptModuleFound() const{ return pScriptModuleFound; }
	
	/** \brief Set if script module is found. */
	void SetScriptModuleFound( bool found );
	
	/** \brief Found script module version. */
	inline const decString &GetScriptModuleFoundVersion() const{ return pScriptModuleFoundVersion; }
	
	/** \brief Game is up to date. */
	inline bool GetGameUpToDate() const{ return pGameUpToDate; }
	
	/** \brief Set if game is up to date. */
	void SetGameUpToDate( bool upToDate );
	
	/** \brief Game can run. */
	inline bool GetCanRun() const{ return pCanRun; }
	
	/** \brief Set if game can run. */
	void SetCanRun( bool canRun );
	
	/** \brief Custom profile or nullptr to use global profile. */
	inline delGameProfile *GetCustomProfile() const{ return pCustomProfile; }
	
	/** \brief Set custom profile or nullptr to use global profile. */
	void SetCustomProfile( delGameProfile *profile );
	
	/** \brief Active profile or nullptr to use last run profile. */
	inline delGameProfile *GetActiveProfile() const{ return pActiveProfile; }
	
	/** \brief Set active profile or nullptr to use last run profile. */
	void SetActiveProfile( delGameProfile *profile );
	
	/** \brief Run arguments. */
	inline const decString &GetRunArguments() const{ return pRunArguments; }
	
	/** \brief Set run arguments. */
	void SetRunArguments( const char *arguments );
	
	/** \brief Engine log filename. */
	inline const decString &GetLogFile() const{ return pLogFile; }
	
	/** \brief Set engine log filename. */
	void SetLogFile( const char *path );
	
	/** \brief Set default log filename "/logs/delauncher-game-{game-name}". */
	void SetDefaultLogFile();
	
	/** \brief Logs are written to console instead of file. */
	inline bool GetLogToConsole() const{ return pLogToConsole; }
	
	/** \brief Set if logs are written to console instead of file. */
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
	
	/**
	 * \brief Patches local to the DELGA file.
	 * \version 1.13
	 */
	inline delPatchList &GetLocalPatches(){ return pLocalPatches; }
	inline const delPatchList &GetLocalPatches() const{ return pLocalPatches; }
	
	/**
	 * \brief Custom properties.
	 * \version 1.25
	 * 
	 * Allows launchers to store custom properties. To avoid name classes best prefix the
	 * property names like "launcher-name:"
	 */
	inline decStringDictionary &GetCustomProperties(){ return pCustomProperties; }
	inline const decStringDictionary &GetCustomProperties() const{ return pCustomProperties; }
	
	
	
#ifdef OS_ANDROID
	/** \brief DELGA VFS container. */
	inline const deVFSContainer::Ref &GetVFSDelgaContainer() const{ return pVFSDelgaContainer; }
	
	/** \brief Set DELGA VFS container. */
	void SetVFSDelgaContainer(const deVFSContainer::Ref &container);
#endif
	
	
	
	/** \brief Game is running. */
	bool IsRunning() const;
	
	/** \brief Start game. */
	void StartGame( const delGameRunParams &runParams );
	
	/**
	 * \brief Start game.
	 * \version 1.10
	 */
	void StartGame( const delGameRunParams &runParams, delEngineInstance::Factory &factory );
	
	/** \brief Stop game. */
	void StopGame();
	
	/** \brief Kill game. */
	void KillGame();
	
	/** \brief Pulse checking. */
	void PulseChecking();
	
	/** \brief Load configuration. */
	void LoadConfig();
	
	/** \brief Save configuration. */
	void SaveConfig();
	
	/** \brief Profile to use. */
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
