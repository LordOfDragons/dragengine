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

#ifndef _DELENGINEINSTANCE_H_
#define _DELENGINEINSTANCE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringSet.h>

#ifdef OS_BEOS
#include <MessageQueue.h>
#endif

#ifdef OS_ANDROID
#include <dragengine/filesystem/deVFSContainer.h>
struct android_input_buffer;
#endif

class delEngineModule;
class delEngineModuleList;
class delLauncher;
class decStringList;
class delGPModuleList;
class decObjectOrderedSet;


/**
 * \brief Game engine instance.
 */
class DE_DLL_EXPORT delEngineInstance : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<delEngineInstance> Ref;
	
	
public:
	/**
	 * \brief Factory creating engine instances.
	 * \version 1.10
	 */
	class DE_DLL_EXPORT Factory : public deObject{
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
	
	/** \brief List of internal modules. */
	virtual void GetInternalModules(delEngineModuleList &list) = 0;
	
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
	
	/**
	 * \brief Make modules add stage specific containers to virtual file system.
	 * \version 1.23
	 */
	virtual void ModulesAddVFSContainers( const char *stage ) = 0;
	
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
	 * \brief Current global scaling factor for display.
	 * 
	 * Value of 100 represents scaling of 100%. Value step size is 25.
	 */
	virtual int GetDisplayCurrentScaleFactor(int display) = 0;
	
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
	
#ifdef OS_ANDROID
	/**
	 * \brief Read game definitions from DELGA file using VFS container.
	 * 
	 * Replaces \em list with content of all found files.
	 */
	virtual void ReadDelgaGameDefsVfs(const deVFSContainer::Ref &container,
		const char *delgaFile, decStringList &list);
	
	/**
	 * \brief Read files from DELGA file file using VFS container.
	 * 
	 * Stores content of files to \em filesContent as instances of decMemoryFile.
	 */
	virtual void ReadDelgaFilesVfs(const deVFSContainer::Ref &container, const char *delgaFile,
		const decStringList &filenames, decObjectOrderedSet &filesContent);
	
	/**
	 * \brief Add DELGA file to virtual file system as root container.
	 * 
	 * Container maps the content of \em archivePath into the virtual file system.
	 */
	virtual void VFSAddDelgaFileVfs(const deVFSContainer::Ref &container,
		const char *delgaFile, const char *archivePath, const decStringSet &hiddenPath);
	
	/** \brief Run single game frame update. */
	virtual void RunSingleFrameUpdate();
	
	/** \brief App gained/lost focus. */
	virtual void SetAppActive(bool active);
	
	/** \brief App paused/resumed. */
	virtual void SetAppPaused(bool paused);
	
	/** \brief Update content rect. */
	virtual void UpdateContentRect(const decBoundary &contentRect);
	
	/** \brief An event processed by the application event loop. */
	virtual void InputEvent(const android_input_buffer &inputBuffer);
#endif
	/*@}*/
};

#endif
