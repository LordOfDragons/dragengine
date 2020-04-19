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

#ifndef _DEGLENGINEINSTANCE_H_
#define _DEGLENGINEINSTANCE_H_

#include <unistd.h>
#ifdef OS_W32
#include <dragengine/app/include_windows.h>
#endif

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class deglEngineModule;
class deglLauncher;
class decStringList;
class deglGPModuleList;
class decObjectOrderedSet;


/**
 * @brief Game Engine Instance.
 * Instance of a game engine running in a different process for safety.
 */
class deglEngineInstance{
private:
	deglLauncher &pLauncher;
	decString pLogFile;
	bool pStopping;
	deglGPModuleList *pGameCollectChangedParams;
	
#ifdef OS_W32
	HANDLE pPipeIn;
	HANDLE pPipeOut;
	HANDLE pProcessHandle;
	HANDLE pThreadHandle;
	DWORD pProcessID;
	DWORD pThreadID;
#else
	int pPipeIn;
	int pPipeOut;
	pid_t pProcessID;
#endif
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new engine instance. */
	deglEngineInstance( deglLauncher &launcher, const char *logfile );
	/** Cleans up the engine instance. */
	~deglEngineInstance();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Determines if the engine is running. */
	bool IsEngineRunning() const;
	/** Starts the engine if not running. */
	bool StartEngine();
	/** Stops the engine if not running. */
	void StopEngine();
	/** Kills the process. */
	void KillProcess();
	
	/** Write unsigned char to pipe. */
	void WriteUCharToPipe( int value );
	/** Write unsigned short to pipe. */
	void WriteUShortToPipe( int value );
	/** Write float to pipe. */
	void WriteFloatToPipe( float value );
	/** Write string to pipe using unsigned short as length. */
	void WriteString16ToPipe( const char *string );
	/** Write data to pipe. */
	void WriteToPipe( const void *data, int length );
	
	/** Read unsigned char from pipe. */
	int ReadUCharFromPipe();
	/** Read unsigned short from pipe. */
	int ReadUShortFromPipe();
	
	/** \brief Read int from pipe. */
	int ReadIntFromPipe();
	
	/** Read float from pipe. */
	float ReadFloatFromPipe();
	/** Read string from pipe using unsigned short as length. */
	void ReadString16FromPipe( decString &string );
	/** Read data from pipe. */
	void ReadFromPipe( void *data, int length );
	/** \brief Check if data can be read from pipe. */
	bool CheckCanReadPipe();
	/** Drain read pipe. */
	void DrainReadPipe();
	
	/** Sends the stop process command to the engine process. */
	bool StopProcess();
	/** Retrieves a property from the engine. */
	void GetProperty( int property, decString &value );
	/** Load modules. */
	void LoadModules();
	/** Command get module status. */
	int GetModuleStatus( const char *moduleName, const char *moduleVersion );
	/** Get module parameter list. */
	void GetModuleParamList( deglEngineModule &module );
	/** Set module parameter. */
	void SetModuleParameter( const char *moduleName, const char *moduleVersion,
		const char *parameter, const char *value );
	/** Activate module. */
	void ActivateModule( const char *moduleName, const char *moduleVersion );
	/** \brief Enable or disable module. */
	void EnableModule( const char *moduleName, const char *moduleVersion, bool enable );
	/** Set data directory. */
	void SetDataDirectory( const char *directory );
	/** \brief Set cache application identifier. */
	void SetCacheAppID( const char *cacheAppID );
	/** Add disk directory to virtual file system. */
	void VFSAddDiskDir( const char *vfsRoot, const char *nativeDirectory, bool readOnly );
	
	/** \brief Add virtual file system container for module shared data. */
	void VFSAddScriptSharedDataDir();
	
	/**
	 * \brief Add DELGA file to virtual file system as root container.
	 * 
	 * Container maps the content of \em archivePath into the virtual file system.
	 */
	void VFSAddDelgaFile( const char *delgaFile, const char *archivePath );
	
	/** Set command line arguments. */
	void SetCmdLineArgs( const char *arguments );
	/** Create render window. */
	void CreateRenderWindow( int width, int height, bool fullScreen,
		const char *windowTitle, const char *iconPath );
	/** Start game. */
	void StartGame( const char *scriptDirectory, const char *gameObject,
		deglGPModuleList *collectChangedParams = NULL );
	/** Stop game. */
	void StopGame();
	/** Determines if the game is still running. */
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

#endif // _DEGLGAME_H_
