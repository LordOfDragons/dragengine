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

#ifndef _DELENGINE_H_
#define _DELENGINE_H_


#include "modules/delEngineModuleList.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/systems/deModuleSystem.h>

#ifdef OS_ANDROID
#include <dragengine/filesystem/deVFSContainer.h>
#endif

class delLauncher;
class delGameList;
class delPatchList;
class delEngineInstance;


/**
 * \brief Game Engine.
 * 
 * Stores all information about the game engine. The library path stores where the game
 * engine libraries are found. This is usually the system library path. The data path
 * stores where the game engine data directory is located. This is usually under the
 * system data directory. Modules are located under the data directory. The config path
 * stores where the game engine configuration files are stored. This is usually under the
 * system configuration path. Module configuration files are located under the config path.
 */
class DE_DLL_EXPORT delEngine{
private:
	delLauncher &pLauncher;
	
	decString pPathConfig;
	decString pPathShare;
	decString pPathLib;
	decString pPathCache;
	delEngineModuleList pModules;
	decString pLogFile;
	
	decPoint pCurrentResolution;
	int pResolutionCount;
	decPoint *pResolutions;
	int pScaleFactor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create engine.
	 * 
	 * Log file is named "{logFileTitle}.log" and is located inside the launcher logging directory.
	 */
	delEngine( delLauncher &launcher, const char *logFileTitle = "launcher-engine" );
	
	/** \brief Clean up engine. */
	~delEngine();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Launcher. */
	inline delLauncher &GetLauncher() const{ return pLauncher; }
	
	/** \brief Config path. */
	inline const decString &GetPathConfig() const{ return pPathConfig; }
	
	/** \brief Set config path. */
	void SetPathConfig( const char *path );
	
	/** \brief Share path. */
	inline const decString &GetPathShare() const{ return pPathShare; }
	
	/** \brief Set share path. */
	void SetPathShare( const char *path );
	
	/** \brief Lib path. */
	inline const decString &GetPathLib() const{ return pPathLib; }
	
	/** \brief Set lib path. */
	void SetPathLib( const char *path );
	
	/** \brief Cache path. */
	inline const decString &GetPathCache() const{ return pPathCache; }
	
	/** \brief Set cache path. */
	void SetPathCache( const char *path );
	
	/** \brief Engine log filename. */
	inline const decString &GetLogFile() const{ return pLogFile; }
	
	/** \brief Set engine log filename. */
	void SetLogFile( const char *path );
	
	/** \brief Modules. */
	inline delEngineModuleList &GetModules(){ return pModules; }
	inline const delEngineModuleList &GetModules() const{ return pModules; }
	
	/** \brief Reload list of modules from module directory. */
	void ReloadModules();
	
	/** \brief Run quick test to check if modules are working. */
	void CheckModules( delEngineInstance &instance );
	
	/** \brief Add modules found in directory. */
	void AddModulesFrom( const char *directory, deModuleSystem::eModuleTypes type );
	
	/** \brief Best module for type. */
	delEngineModule *GetBestModuleForType( deModuleSystem::eModuleTypes moduleType ) const;
	
	/** \brief Get engine path and add virtual file system containers for them. */
	void PutEngineIntoVFS( delEngineInstance &instance );
	
	/** \brief Text for module type. */
	const char *GetModuleTypeText( deModuleSystem::eModuleTypes moduleType ) const;
	
	
	
	/** \brief Get resolution list from engine. */
	void UpdateResolutions( delEngineInstance &instance );
	
	/** \brief Current display resolution. */
	inline const decPoint &GetCurrentResolution() const{ return pCurrentResolution; }
	
	/** \brief Number of resolutions. */
	inline int GetResolutionCount() const{ return pResolutionCount; }
	
	/** \brief Resolution by index. */
	const decPoint &GetResolutionAt( int index ) const;
	
	/** \brief Index of resolution closest but not larger than size or -1 if there are no resolutions. */
	int IndexOfClosestResolutionTo( int width, int height ) const;
	
	
	
	/** \brief Get global scale factor from engine. */
	void UpdateScaleFactor(delEngineInstance &instance);
	
	/** \brief Current global scale factor for display multiplied by 100. */
	inline int GetScaleFactor() const{ return pScaleFactor; }
	
	
	
	/** \brief Read game definitions from DELGA file. */
	void ReadDelgaGameDefs( delEngineInstance &instance, const char *filename, delGameList &list );
	
	/** \brief Read game patch definitions from DELGA file. */
	void ReadDelgaPatchDefs( delEngineInstance &instance, const char *filename, delPatchList &list );
	
#ifdef OS_ANDROID
	/** \brief Read game definitions from DELGA file using VFS container. */
	void ReadDelgaGameDefsVfs(delEngineInstance &instance, const deVFSContainer::Ref &container,
		const char *filename, delGameList &list);
#endif
	
	
	
	/** \brief Load configuration. */
	void LoadConfig();
	
	/** \brief Save configurations. */
	void SaveConfig();
	
	/** \brief Clear. */
	void Clear();
	/*@}*/
};

#endif
