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

#ifndef _DEALIENGINEINSTANCE_H_
#define _DEALIENGINEINSTANCE_H_

struct AInputEvent;


/**
 * \brief Engine instance interface.
 * 
 * Interface to glue library linking against the game engine handling a game engine instance.
 */
class dealIEngineInstance{
public:
	/** \brief Properties. */
	enum eProperties{
		/** \brief Engine config Path. */
		epPathEngineConfig,
		
		/** \brief Engine share Path. */
		epPathEngineShare,
		
		/** \brief Engine lib path. */
		epPathEngineLib,
		
		/** \brief Engine cache path. */
		epPathEngineCache,
	};
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Clean up engine. */
	virtual ~dealIEngineInstance(){};
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Determines if the engine is running. */
	virtual bool IsRunning() const = 0;
	
	/**
	 * \brief Start engine.
	 * \returns \em true if successful or \em false if an exception happened.
	 */
	virtual bool Start( const char *logfile, const char *cacheAppID ) = 0;
	
	/**
	 * \brief Stop engine.
	 * \returns \em true if successful or \em false if an exception happened.
	 */
	virtual bool Stop() = 0;
	
	/** \brief Process input event. */
	virtual void ProcessInputEvent( const AInputEvent &event ) = 0;
	
	/** \brief Frame update. */
	virtual bool FrameUpdate( bool &keepRunning ) = 0;
	
	
	
	/** \brief Get module status. */
	virtual bool GetModuleStatus( const char *moduleName,
		const char *moduleVersion, int &status ) = 0;
	
	/** \brief Number of module parameters. */
	virtual bool GetModuleParameterCount( const char *moduleName,
		const char *moduleVersion, int &parameterCount ) = 0;
	
	/** \brief Module parameter information. */
	virtual bool GetModuleParameterData( const char *moduleName, const char *moduleVersion,
		int parameter, const char *&parameterName, const char *&parameterDescription,
		const char *&parameterValue ) = 0;
	
	/** \brief Set module parameter value. */
	virtual bool SetModuleParameterValue( const char *moduleName, const char *moduleVersion,
		const char *parameter, const char *value ) = 0;
	
	/** \brief Engine property. */
	virtual bool GetProperty( int property, const char *&value ) = 0;
	
	/** \brief Activate module. */
	virtual bool ActivateModule( const char *moduleName, const char *moduleVersion ) = 0;
	
	/** \brief Enable or disable module. */
	virtual bool EnableModule( const char *moduleName, const char *moduleVersion, bool enable ) = 0;
	
	/** \brief Set command line arguments. */
	virtual bool SetCmdLineArgs( const char *arguments ) const = 0;
	
	/** \brief Set data directory. */
	virtual bool SetDataDirectory( const char *directory ) = 0;
	
	
	
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
	virtual bool OpenDelga( int fileDescriptor, long fileOffset, long fileLength ) = 0;
	
	
	
	/** \brief Add disk directory to virtual file system. */
	virtual bool VFSAddDiskDir( const char *vfsRoot, const char *nativeDirectory, bool readOnly ) = 0;
	
	/** \brief Add virtual file system container for module shared data. */
	virtual bool VFSAddScriptSharedDataDir() = 0;
	
	/**
	 * \brief Add virtual file system container referenceing path in DELGA file.
	 * 
	 * \param[in] vfsRoot Path to show content at in virtual file system.
	 * \param[in] vfsBase Base path relative to zip archive to show in virtual file system.
	 */
	virtual bool VFSAddDelga( const char *vfsRoot, const char *vfsBase ) = 0;
	
	/** \brief Add redirect directory to virtual file system. */
	virtual bool VFSAddRedirect( const char *root, const char *redirect ) = 0;
	
	/**
	 * \brief Make modules add stage specific containers to virtual file system.
	 * \version 1.23
	 */
	virtual void ModulesAddVFSContainers( const char *stage ) = 0;
	
	
	
	/** \brief Create render window. */
	virtual bool CreateRenderWindow( int width, int height, bool fullScreen, const char *windowTitle ) = 0;
	
	/** \brief Start game. */
	virtual bool StartGame( const char *scriptDirectory, const char *gameObject ) = 0;
	
	/** \brief Stop game. */
	virtual bool StopGame() = 0;
	
	/** \brief Determines if the game is still running. */
	virtual int IsGameRunning() = 0;
	
	
	
	/** \brief Focus gained. */
	virtual bool FocusGained() = 0;
	
	/** \brief Focus lost. */
	virtual bool FocusLost() = 0;
	
	/** \brief Freeze application. */
	virtual bool Freeze() = 0;
	
	/** \brief Thaw application. */
	virtual bool Thaw() = 0;
	
	/** \brief Application window has been created. */
	virtual bool InitAppWindow() = 0;
	
	/** \brief Application window has been closed. */
	virtual bool TerminateAppWindow() = 0;
	/*@}*/
};

#endif
