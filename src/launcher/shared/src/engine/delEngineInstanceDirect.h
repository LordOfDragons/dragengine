/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _DELENGINEINSTANCEDIRECT_H_
#define _DELENGINEINSTANCEDIRECT_H_

#include "delEngineInstance.h"

#include <dragengine/logger/deLogger.h>

class deEngine;


/**
 * \brief Instance of game engine running in the same process.
 * \version 1.10
 */
class delEngineInstanceDirect : public delEngineInstance{
public:
	/** \brief Factory creating engine instances. */
	class Factory : public delEngineInstance::Factory{
	private:
		deLogger::Ref pEngineLogger;
		
	public:
		/** \brief Type holding strong reference. */
		typedef deTObjectReference<Factory> Ref;
		
		/** \brief Factory constructor. */
		Factory( deLogger *engineLogger = nullptr );
		
	protected:
		/** \brief Factory destructor. */
		virtual ~Factory();
		
	public:
		/** \brief Logger to use for new engine instances or nullptr. */
		inline const deLogger::Ref &GetEngineLogger() const{ return pEngineLogger; }
		
		/** \brief Set logger to use for new engine instance or nullptr. */
		void SetEngineLogger( deLogger *logger );
		
		/** \brief Create engine instance. */
		virtual delEngineInstance *CreateEngineInstance( delLauncher &launcher, const char *logFile );
	};
	
	
	
private:
	deEngine *pEngine;
	bool pEngineRunning;
	deLogger::Ref pLogger;
	deLogger::Ref pEngineLogger;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create engine instance. */
	delEngineInstanceDirect( delLauncher &launcher, const char *logfile );
	
protected:
	/** \brief Clean up engine instance. */
	virtual ~delEngineInstanceDirect();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Engine or nullptr if not running. */
	inline deEngine *GetEngine() const{ return pEngine; }
	
	/** \brief Logger to use for new engine instances. */
	inline const deLogger::Ref &GetLogger() const{ return pLogger; }
	
	/** \brief Set logger to use for new engine instances. */
	void SetLogger( deLogger *logger );
	
	/** \brief Logger to use for new engine or nullptr to use engine instance logger. */
	inline const deLogger::Ref &GetEngineLogger() const{ return pEngineLogger; }
	
	/** \brief Set logger to use for new engine or nullptr to use engine instance logger. */
	void SetEngineLogger( deLogger *logger );
	
	
	
	/** \brief Engine is running. */
	virtual bool IsEngineRunning() const;
	
	/** \brief Start engine if not running. */
	virtual bool StartEngine();
	
	/** \brief Stop engine if not running. */
	virtual void StopEngine();
	
	/** \brief Kill engine if running. */
	virtual void KillEngine();
	
	/** \brief Get property from engine. */
	virtual void GetProperty( int property, decString &value );
	
	/** \brief Load modules. */
	virtual void LoadModules();
	
	/** \brief Command get module status. */
	virtual int GetModuleStatus( const char *moduleName, const char *moduleVersion );
	
	/** \brief Get module parameter list. */
	virtual void GetModuleParams ( delEngineModule &module );
	
	/** \brief Set module parameter. */
	virtual void SetModuleParameter( const char *moduleName, const char *moduleVersion,
		const char *parameter, const char *value );
	
	/** \brief Activate module. */
	virtual void ActivateModule( const char *moduleName, const char *moduleVersion );
	
	/** \brief Enable or disable module. */
	virtual void EnableModule( const char *moduleName, const char *moduleVersion, bool enable );
	
	/** \brief Set data directory. */
	virtual void SetDataDirectory( const char *directory );
	
	/** \brief Set cache application identifier. */
	virtual void SetCacheAppID( const char *cacheAppID );
	
	/**
	 * \brief Set overlay directory.
	 * \version 1.7
	 */
	virtual void SetPathOverlay( const char *path );
	
	/**
	 * \brief Set capture directory.
	 * \version 1.7
	 */
	virtual void SetPathCapture( const char *path );
	
	/**
	 * \brief Set config directory.
	 * \version 1.7
	 */
	virtual void SetPathConfig( const char *path );
	
	/** \brief Add disk directory to virtual file system. */
	virtual void VFSAddDiskDir( const char *vfsRoot, const char *nativeDirectory, bool readOnly );
	
	/**
	 * \brief Add disk directory to virtual file system hidding a set of path.
	 * \version 1.13
	 */
	virtual void VFSAddDiskDir( const char *vfsRoot, const char *nativeDirectory,
		bool readOnly, const decStringSet &hiddenPath );
	
	/** \brief Add virtual file system container for module shared data. */
	virtual void VFSAddScriptSharedDataDir();
	
	/**
	 * \brief Add DELGA file to virtual file system as root container.
	 * 
	 * Container maps the content of \em archivePath into the virtual file system.
	 */
	virtual void VFSAddDelgaFile( const char *delgaFile, const char *archivePath );
	
	/**
	 * \brief Add DELGA file to virtual file system as root container.
	 * \version 1.13
	 * 
	 * Container maps the content of \em archivePath into the virtual file system.
	 */
	virtual void VFSAddDelgaFile( const char *delgaFile, const char *archivePath,
		const decStringSet &hiddenPath );
	
	/** \brief Set command line arguments. */
	virtual void SetCmdLineArgs( const char *arguments );
	
	/** \brief Create render window. */
	virtual void CreateRenderWindow( int width, int height, bool fullScreen,
		const char *windowTitle, const char *iconPath );
	
	/**
	 * \brief Start game.
	 * \deprecated Use StartGame(const char*, const char*, const char*, delGPModuleList*).
	 */
	virtual void StartGame( const char *scriptDirectory, const char *gameObject,
		delGPModuleList *collectChangedParams = NULL );
	
	/**
	 * \brief Start game.
	 * \version 1.9
	 */
	virtual void StartGame( const char *scriptDirectory, const char *scriptVersion, const char *gameObject,
		delGPModuleList *collectChangedParams = NULL );
	
	/** \brief Stop game. */
	virtual void StopGame();
	
	/** \brief Game is still running. */
	virtual int IsGameRunning();
	
	/** \brief Current display resolution. */
	virtual decPoint GetDisplayCurrentResolution( int display );
	
	/**
	 * \brief Resolution for display.
	 * 
	 * If \em resolution is \em NULL and \em resolutionCount is 0 returns the size of \em resolutions
	 * needed to store resolution data. If \em resolution is not \em NULL it contains the buffer to
	 * fill with information. \em resolutionCount indicates the size of \em resolutions. The number
	 * of written entries is returned which can be less than \em resolutionCount.
	 */
	virtual int GetDisplayResolutions( int display, decPoint *resolutions, int resolutionCount );
	
	/**
	 * \brief Read game definitions from DELGA file.
	 * 
	 * Replaces \em list with content of all found files.
	 */
	virtual void ReadDelgaGameDefs( const char *delgaFile, decStringList &list );
	
	/**
	 * \brief Read game definitions from DELGA file.
	 * 
	 * Replaces \em list with content of all found files.
	 */
	virtual void ReadDelgaPatchDefs( const char *delgaFile, decStringList &list );
	
	/**
	 * \brief Read files from DELGA file.
	 * 
	 * Stores content of files to \em filesContent as instances of decMemoryFile.
	 */
	virtual void ReadDelgaFiles( const char *delgaFile, const decStringList &filenames,
		decObjectOrderedSet &filesContent );

	
#ifdef OS_BEOS
	/**
	 * \brief Message received.
	 * 
	 * Required for direct engine instance on BeOS only.
	 */
	virtual void BeosMessageReceived( BMessage *message );
#endif	/*@}*/
};

#endif
