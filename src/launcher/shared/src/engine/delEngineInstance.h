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

#ifndef _DELENGINEINSTANCE_H_
#define _DELENGINEINSTANCE_H_

#include <unistd.h>
#ifdef OS_W32
#include <dragengine/app/include_windows.h>
#endif

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class delEngineModule;
class delLauncher;
class decStringList;
class delGPModuleList;
class decObjectOrderedSet;


/**
 * \brief Instance of game engine running in different process.
 */
class delEngineInstance{
private:
	delLauncher &pLauncher;
	decString pLogFile;
	bool pStopping;
	delGPModuleList *pGameCollectChangedParams;
	
#ifdef OS_W32
	HANDLE pPipeIn;
	HANDLE pPipeOut;
	HANDLE pProcessHandle;
	HANDLE pThreadHandle;
	DWORD pProcessID;
	DWORD pThreadID;
	decString pExecutableName;
#else
	int pPipeIn;
	int pPipeOut;
	pid_t pProcessID;
#endif
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create engine instance. */
	delEngineInstance( delLauncher &launcher, const char *logfile );
	
	/** \brief Clean up engine instance. */
	~delEngineInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	#ifdef OS_W32
	/** \brief Executable name. */
	inline const decString &GetExecutableName() const{ return pExecutableName; }
	
	/** \brief Set executable name. */
	void SetExecutableName( const char *executableName );
	#endif
	
	
	
	/** \brief Engine is running. */
	bool IsEngineRunning() const;
	
	/** \brief Start engine if not running. */
	bool StartEngine();
	
	/** \brief Stop engine if not running. */
	void StopEngine();
	
	/** \brief Kill process. */
	void KillProcess();
	
	
	
	/** \brief Write unsigned char to pipe. */
	void WriteUCharToPipe( int value );
	
	/** \brief Write unsigned short to pipe. */
	void WriteUShortToPipe( int value );
	
	/** \brief Write float to pipe. */
	void WriteFloatToPipe( float value );
	
	/** \brief Write string to pipe using unsigned short as length. */
	void WriteString16ToPipe( const char *string );
	
	/** \brief Write data to pipe. */
	void WriteToPipe( const void *data, int length );
	
	
	
	/** \brief Read unsigned char from pipe. */
	int ReadUCharFromPipe();
	
	/** \brief Read unsigned short from pipe. */
	int ReadUShortFromPipe();
	
	/** \brief Read int from pipe. */
	int ReadIntFromPipe();
	
	/** \brief Read float from pipe. */
	float ReadFloatFromPipe();
	
	/** \brief Read string from pipe using unsigned short as length. */
	void ReadString16FromPipe( decString &string );
	
	/** \brief Read data from pipe. */
	void ReadFromPipe( void *data, int length );
	
	
	
	/** \brief Check if data can be read from pipe. */
	bool CheckCanReadPipe();
	
	/** \brief Drain read pipe. */
	void DrainReadPipe();
	
	
	
	/** \brief Send stop process command to the engine process. */
	bool StopProcess();
	
	/** \brief Get property from engine. */
	void GetProperty( int property, decString &value );
	
	/** \brief Load modules. */
	void LoadModules();
	
	/** \brief Command get module status. */
	int GetModuleStatus( const char *moduleName, const char *moduleVersion );
	
	/** \brief Get module parameter list. */
	void GetModuleParamList( delEngineModule &module );
	
	/** \brief Set module parameter. */
	void SetModuleParameter( const char *moduleName, const char *moduleVersion,
		const char *parameter, const char *value );
	
	/** \brief Activate module. */
	void ActivateModule( const char *moduleName, const char *moduleVersion );
	
	/** \brief Enable or disable module. */
	void EnableModule( const char *moduleName, const char *moduleVersion, bool enable );
	
	/** \brief Set data directory. */
	void SetDataDirectory( const char *directory );
	
	/** \brief Set cache application identifier. */
	void SetCacheAppID( const char *cacheAppID );
	
	/** \brief Add disk directory to virtual file system. */
	void VFSAddDiskDir( const char *vfsRoot, const char *nativeDirectory, bool readOnly );
	
	/** \brief Add virtual file system container for module shared data. */
	void VFSAddScriptSharedDataDir();
	
	/**
	 * \brief Add DELGA file to virtual file system as root container.
	 * 
	 * Container maps the content of \em archivePath into the virtual file system.
	 */
	void VFSAddDelgaFile( const char *delgaFile, const char *archivePath );
	
	/** \brief Set command line arguments. */
	void SetCmdLineArgs( const char *arguments );
	
	/** \brief Create render window. */
	void CreateRenderWindow( int width, int height, bool fullScreen,
		const char *windowTitle, const char *iconPath );
	
	/** \brief Start game. */
	void StartGame( const char *scriptDirectory, const char *gameObject,
		delGPModuleList *collectChangedParams = NULL );
	
	/** \brief Stop game. */
	void StopGame();
	
	/** \brief Game is still running. */
	int IsGameRunning();
	
	/** \brief Current display resolution. */
	decPoint GetDisplayCurrentResolution( int display );
	
	/**
	 * \brief Resolution for display.
	 * 
	 * If \em resolution is \em NULL and \em resolutionCount is 0 returns the size of \em resolutions
	 * needed to store resolution data. If \em resolution is not \em NULL it contains the buffer to
	 * fill with information. \em resolutionCount indicates the size of \em resolutions. The number
	 * of written entries is returned which can be less than \em resolutionCount.
	 */
	int GetDisplayResolutions( int display, decPoint *resolutions, int resolutionCount );
	
	/**
	 * \brief Read game definitions from DELGA file.
	 * 
	 * Replaces \em list with content of all found files.
	 */
	void ReadDelgaGameDefs( const char *delgaFile, decStringList &list );
	
	/**
	 * \brief Read game definitions from DELGA file.
	 * 
	 * Replaces \em list with content of all found files.
	 */
	void ReadDelgaPatchDefs( const char *delgaFile, decStringList &list );
	
	/**
	 * \brief Read files from DELGA file.
	 * 
	 * Stores content of files to \em filesContent as instances of decMemoryFile.
	 */
	void ReadDelgaFiles( const char *delgaFile, const decStringList &filenames,
		decObjectOrderedSet &filesContent );
	/*@}*/
};

#endif
