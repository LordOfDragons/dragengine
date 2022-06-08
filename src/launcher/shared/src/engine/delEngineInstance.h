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

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringSet.h>

#ifdef OS_BEOS
#include <MessageQueue.h>
#endif

class delEngineModule;
class delLauncher;
class decStringList;
class delGPModuleList;
class decObjectOrderedSet;


/**
 * \brief Game engine instance.
 */
class delEngineInstance : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<delEngineInstance> Ref;
	
	
	
public:
	/**
	 * \brief Factory creating engine instances.
	 * \version 1.10
	 */
	class Factory : public deObject{
	public:
		/** \brief Type holding strong reference. */
		typedef deTObjectReference<Factory> Ref;
		
		/** \brief Factory constructor. */
		Factory();
		
	protected:
		/** \brief Factory destructor. */
		virtual ~Factory();
		
	public:
		/** \brief Create engine instance. */
		virtual delEngineInstance *CreateEngineInstance( delLauncher &launcher, const char *logfile ) = 0;
	};
	
	
	
private:
	delLauncher &pLauncher;
	decString pLogFile;
	bool pUseConsole;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create engine instance. */
	delEngineInstance( delLauncher &launcher, const char *logfile );
	
protected:
	/** \brief Clean up engine instance. */
	virtual ~delEngineInstance();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Launcher. */
	inline delLauncher &GetLauncher() const{ return pLauncher; }
	
	/** \brief Log file. */
	inline const decString &GetLogFile() const{ return pLogFile; }
	
	/** \brief Set log file. */
	void SetLogFile( const decString &logFile );
	
	/** \brief Use console. */
	inline bool GetUseConsole() const{ return pUseConsole; }
	
	/** \brief Set use console. */
	void SetUseConsole( bool useConsole );
	
	
	
	/** \brief Engine is running. */
	virtual bool IsEngineRunning() const = 0;
	
	/** \brief Start engine if not running. */
	virtual bool StartEngine() = 0;
	
	/** \brief Stop engine if not running. */
	virtual void StopEngine() = 0;
	
	/** \brief Kill engine if running. */
	virtual void KillEngine() = 0;
	
	/** \brief Get property from engine. */
	virtual void GetProperty( int property, decString &value ) = 0;
	
	/** \brief Load modules. */
	virtual void LoadModules() = 0;
	
	/** \brief Command get module status. */
	virtual int GetModuleStatus( const char *moduleName, const char *moduleVersion ) = 0;
	
	/** \brief Get module parameter list. */
	virtual void GetModuleParams ( delEngineModule &module ) = 0;
	
	/** \brief Set module parameter. */
	virtual void SetModuleParameter( const char *moduleName, const char *moduleVersion,
		const char *parameter, const char *value ) = 0;
	
	/** \brief Activate module. */
	virtual void ActivateModule( const char *moduleName, const char *moduleVersion ) = 0;
	
	/** \brief Enable or disable module. */
	virtual void EnableModule( const char *moduleName, const char *moduleVersion, bool enable ) = 0;
	
	/** \brief Set data directory. */
	virtual void SetDataDirectory( const char *directory ) = 0;
	
	/** \brief Set cache application identifier. */
	virtual void SetCacheAppID( const char *cacheAppID ) = 0;
	
	/**
	 * \brief Set overlay directory.
	 * \version 1.7
	 */
	virtual void SetPathOverlay( const char *path ) = 0;
	
	/**
	 * \brief Set capture directory.
	 * \version 1.7
	 */
	virtual void SetPathCapture( const char *path ) = 0;
	
	/**
	 * \brief Set config directory.
	 * \version 1.7
	 */
	virtual void SetPathConfig( const char *path ) = 0;
	
	/** \brief Add disk directory to virtual file system. */
	virtual void VFSAddDiskDir( const char *vfsRoot, const char *nativeDirectory, bool readOnly ) = 0;
	
	/**
	 * \brief Add disk directory to virtual file system hidding a set of path.
	 * \version 1.13
	 */
	virtual void VFSAddDiskDir( const char *vfsRoot, const char *nativeDirectory,
		bool readOnly, const decStringSet &hiddenPath ) = 0;
	
	/** \brief Add virtual file system container for module shared data. */
	virtual void VFSAddScriptSharedDataDir() = 0;
	
	/**
	 * \brief Add DELGA file to virtual file system as root container.
	 * 
	 * Container maps the content of \em archivePath into the virtual file system.
	 */
	virtual void VFSAddDelgaFile( const char *delgaFile, const char *archivePath ) = 0;
	
	/**
	 * \brief Add DELGA file to virtual file system as root container.
	 * \version 1.13
	 * 
	 * Container maps the content of \em archivePath into the virtual file system.
	 */
	virtual void VFSAddDelgaFile( const char *delgaFile, const char *archivePath,
		const decStringSet &hiddenPath ) = 0;
	
	/** \brief Set command line arguments. */
	virtual void SetCmdLineArgs( const char *arguments ) = 0;
	
	/** \brief Create render window. */
	virtual void CreateRenderWindow( int width, int height, bool fullScreen,
		const char *windowTitle, const char *iconPath ) = 0;
	
	/**
	 * \brief Start game.
	 * \deprecated Use StartGame(const char*, const char*, const char*, delGPModuleList*).
	 */
	virtual void StartGame( const char *scriptDirectory, const char *gameObject,
		delGPModuleList *collectChangedParams = NULL ) = 0;
	
	/**
	 * \brief Start game.
	 * \version 1.9
	 */
	virtual void StartGame( const char *scriptDirectory, const char *scriptVersion, const char *gameObject,
		delGPModuleList *collectChangedParams = NULL ) = 0;
	
	/** \brief Stop game. */
	virtual void StopGame() = 0;
	
	/** \brief Game is still running. */
	virtual int IsGameRunning() = 0;
	
	/** \brief Current display resolution. */
	virtual decPoint GetDisplayCurrentResolution( int display ) = 0;
	
	/**
	 * \brief Resolution for display.
	 * 
	 * If \em resolution is \em NULL and \em resolutionCount is 0 returns the size of \em resolutions
	 * needed to store resolution data. If \em resolution is not \em NULL it contains the buffer to
	 * fill with information. \em resolutionCount indicates the size of \em resolutions. The number
	 * of written entries is returned which can be less than \em resolutionCount.
	 */
	virtual int GetDisplayResolutions( int display, decPoint *resolutions, int resolutionCount ) = 0;
	
	/**
	 * \brief Read game definitions from DELGA file.
	 * 
	 * Replaces \em list with content of all found files.
	 */
	virtual void ReadDelgaGameDefs( const char *delgaFile, decStringList &list ) = 0;
	
	/**
	 * \brief Read game definitions from DELGA file.
	 * 
	 * Replaces \em list with content of all found files.
	 */
	virtual void ReadDelgaPatchDefs( const char *delgaFile, decStringList &list ) = 0;
	
	/**
	 * \brief Read files from DELGA file.
	 * 
	 * Stores content of files to \em filesContent as instances of decMemoryFile.
	 */
	virtual void ReadDelgaFiles( const char *delgaFile, const decStringList &filenames,
		decObjectOrderedSet &filesContent ) = 0;
	
#ifdef OS_BEOS
	/**
	 * \brief Message received.
	 * 
	 * Required for direct engine instance on BeOS only.
	 */
	virtual void BeosMessageReceived( BMessage *message );
#endif
	/*@}*/
};

#endif
