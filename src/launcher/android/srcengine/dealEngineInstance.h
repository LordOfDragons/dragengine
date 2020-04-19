/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DEALENGINEINSTANCE_H_
#define _DEALENGINEINSTANCE_H_

#include "dealIEngineInstance.h"

#include <android/asset_manager.h>

#include <dragengine/common/string/decString.h>
#include <dragengine/systems/modules/deModuleParameter.h>

class deEngine;
class deOSAndroid;
class deLogger;
class dealVFSZipArchive;
class deVirtualFileSystem;
struct android_app;


/**
 * \brief Engine instance.
 * 
 * Glue library linking against the game engine handling a game engine instance.
 */
class dealEngineInstance : public dealIEngineInstance{
private:
	android_app &pAndroidApp;
	
	deLogger *pLogger;
	
	AAsset *pEngineAsset;
	int pEngineAssetFileDescriptor;
	deVirtualFileSystem *pOSFileSystem;
	
	deOSAndroid *pOSAndroid;
	deEngine *pEngine;
	
	deModuleParameter pModuleParameter;
	
	dealVFSZipArchive *pDelga;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create engine. */
	dealEngineInstance( android_app &androidApp );
	
	/** \brief Clean up engine. */
	virtual ~dealEngineInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Logger or \em null if not existing. */
	inline deLogger *GetLogger() const{ return pLogger; }
	
	
	
	/** Determines if the engine is running. */
	virtual bool IsRunning() const;
	
	/**
	 * \brief Start engine.
	 * \returns \em true if successful or \em false if an exception happened.
	 */
	virtual bool Start( const char *logfile, const char *cacheAppID );
	
	/**
	 * \brief Stop engine.
	 * \returns \em true if successful or \em false if an exception happened.
	 */
	virtual bool Stop();
	
	/** \brief Process input event. */
	virtual void ProcessInputEvent( const AInputEvent &event );
	
	/** \brief Frame update. */
	virtual bool FrameUpdate( bool &keepRunning );
	
	
	
	/** \brief Get module status. */
	virtual bool GetModuleStatus( const char *moduleName, const char *moduleVersion, int &status );
	
	/** \brief Number of module parameters. */
	virtual bool GetModuleParameterCount( const char *moduleName, const char *moduleVersion,
		int &parameterCount );
	
	/** \brief Module parameter information. */
	virtual bool GetModuleParameterData( const char *moduleName, const char *moduleVersion,
		int parameter, const char *&parameterName, const char *&parameterDescription,
		const char*&parameterValue );
	
	/** \brief Set module parameter value. */
	virtual bool SetModuleParameterValue( const char *moduleName, const char *moduleVersion,
		const char *parameter, const char *value );
	
	/** \brief Engine property. */
	virtual bool GetProperty( int property, const char *&value );
	
	/** \brief Activate module. */
	virtual bool ActivateModule( const char *moduleName, const char *moduleVersion );
	
	/** \brief Enable or disable module. */
	virtual bool EnableModule( const char *moduleName, const char *moduleVersion, bool enable );
	
	/** \brief Set command line arguments. */
	virtual bool SetCmdLineArgs( const char *arguments ) const;
	
	/** \brief Set data directory. */
	virtual bool SetDataDirectory( const char *directory );
	
	
	
	/**
	 * \brief Open DELGA file.
	 * 
	 * File descriptor is not claimed and not closed. Caller is responsible to keep
	 * the file descriptor valid as long as the engine instance is running while
	 * not touching it anymore. Once the engine instance closes the caller has to
	 * either close the file descriptor or he can restart using it.
	 * 
	 * \param[in] fileDescriptor File descriptor of open zip archive.
	 * \param[in] fileOffset Offset in bytes where the content begins. Required for asset files.
	 * \param[in] fileLength Length of content in bytes. Required for asset files.
	 */
	virtual bool OpenDelga( int fileDescriptor, long fileOffset, long fileLength );
	
	
	
	/** \brief Add disk directory to virtual file system. */
	virtual bool VFSAddDiskDir( const char *vfsRoot, const char *nativeDirectory, bool readOnly );
	
	/** \brief Add virtual file system container for module shared data. */
	virtual bool VFSAddScriptSharedDataDir();
	
	/**
	 * \brief Add virtual file system container referenceing path in DELGA file.
	 * 
	 * \param[in] vfsRoot Path to show content at in virtual file system.
	 * \param[in] vfsBase Base path relative to zip archive to show in virtual file system.
	 */
	virtual bool VFSAddDelga( const char *vfsRoot, const char *vfsBase );
	
	/** \brief Add redirect directory to virtual file system. */
	virtual bool VFSAddRedirect( const char *root, const char *redirect );
	
	
	
	/** \brief Create render window. */
	virtual bool CreateRenderWindow( int width, int height, bool fullScreen, const char *windowTitle );
	
	/** \brief Start game. */
	virtual bool StartGame( const char *scriptDirectory, const char *gameObject );
	
	/** \brief Stop game. */
	virtual bool StopGame();
	
	/** \brief Determines if the game is still running. */
	virtual int IsGameRunning();
	
	
	
	/** \brief Focus gained. */
	virtual bool FocusGained();
	
	/** \brief Focus lost. */
	virtual bool FocusLost();
	
	/** \brief Freeze application. */
	virtual bool Freeze();
	
	/** \brief Thaw application. */
	virtual bool Thaw();
	
	/** \brief Application window has been created. */
	virtual bool InitAppWindow();
	
	/** \brief Application window has been closed. */
	virtual bool TerminateAppWindow();
	/*@}*/
	
private:
	void pCleanUp();
	
	void pCreateLogger( const char *logfile );
	void pCreateOSFileSystem();
	void pCloseOSFileSystem();
};

#endif
