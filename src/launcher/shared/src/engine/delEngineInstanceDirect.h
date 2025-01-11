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

#ifndef _DELENGINEINSTANCEDIRECT_H_
#define _DELENGINEINSTANCEDIRECT_H_

#include "delEngineInstance.h"

#include <dragengine/logger/deLogger.h>

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/string/decStringDictionary.h>

#ifdef OS_ANDROID
#include <dragengine/app/deOSAndroid.h>
#endif

class deEngine;
class deLoadableModule;


/**
 * \brief Instance of game engine running in the same process.
 * \version 1.10
 */
class DE_DLL_EXPORT delEngineInstanceDirect : public delEngineInstance{
public:
	/** \brief Factory creating engine instances. */
	class DE_DLL_EXPORT Factory : public delEngineInstance::Factory{
	private:
		deLogger::Ref pEngineLogger;
		bool pUseConsole;
#ifdef OS_ANDROID
		deOSAndroid::sConfig pConfig;
#endif
		
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
		
		/** \brief Use console. */
		inline bool GetUseConsole() const{ return pUseConsole; }
		
		/** \brief Set use console. */
		void SetUseConsole( bool useConsole );
		
		/** \brief Create engine instance. */
		virtual delEngineInstance *CreateEngineInstance( delLauncher &launcher, const char *logFile );
		
#ifdef OS_ANDROID
		/** \brief Configuration to use for creating OS instance. */
		inline const deOSAndroid::sConfig &GetConfig() const{ return pConfig; }
		
		/** \brief Set configuration to use for creating OS instance. */
		void SetConfig(const deOSAndroid::sConfig &config);
#endif
	};
	
	
	
private:
	deEngine *pEngine;
	bool pEngineRunning;
	bool pGameRunning;
	deLogger::Ref pLogger;
	deLogger::Ref pEngineLogger;
	
	class cModuleParamState : public deObject{
	public:
		deLoadableModule *module;
		decStringDictionary parameters;
		cModuleParamState(deLoadableModule *amodule);
	};
	decObjectList pModuleParamStates;
	
#ifdef OS_ANDROID
	deOSAndroid::sConfig pConfig;
	delGPModuleList *pGameCollectChangedParams;
#endif
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create engine instance. */
	delEngineInstanceDirect( delLauncher &launcher, const char *logfile );
	
protected:
	/** \brief Clean up engine instance. */
	~delEngineInstanceDirect() override;
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
		
#ifdef OS_ANDROID
	/** \brief Configuration to use for creating OS instance. */
	inline const deOSAndroid::sConfig &GetConfig() const{ return pConfig; }
	
	/** \brief Set configuration to use for creating OS instance. */
	void SetConfig(const deOSAndroid::sConfig &config);
#endif
	
	
	
	/** \brief Engine is running. */
	bool IsEngineRunning() const override;
	
	/** \brief Start engine if not running. */
	bool StartEngine() override;
	
	/** \brief Stop engine if not running. */
	void StopEngine() override;
	
	/** \brief Kill engine if running. */
	void KillEngine() override;
	
	/** \brief Get property from engine. */
	void GetProperty( int property, decString &value ) override;
	
	/** \brief Load modules. */
	void LoadModules() override;
	
	/** \brief Command get module status. */
	int GetModuleStatus( const char *moduleName, const char *moduleVersion ) override;
	
	/** \brief Get module parameter list. */
	void GetModuleParams ( delEngineModule &module ) override;
	
	/** \brief Set module parameter. */
	void SetModuleParameter( const char *moduleName, const char *moduleVersion,
		const char *parameter, const char *value ) override;
	
	/** \brief Activate module. */
	void ActivateModule( const char *moduleName, const char *moduleVersion ) override;
	
	/** \brief Enable or disable module. */
	void EnableModule( const char *moduleName, const char *moduleVersion, bool enable ) override;
	
	/** \brief Set data directory. */
	void SetDataDirectory( const char *directory ) override;
	
	/** \brief Set cache application identifier. */
	void SetCacheAppID( const char *cacheAppID ) override;
	
	/**
	 * \brief Set overlay directory.
	 * \version 1.7
	 */
	void SetPathOverlay( const char *path ) override;
	
	/**
	 * \brief Set capture directory.
	 * \version 1.7
	 */
	void SetPathCapture( const char *path ) override;
	
	/**
	 * \brief Set config directory.
	 * \version 1.7
	 */
	void SetPathConfig( const char *path ) override;
	
	/** \brief Add disk directory to virtual file system. */
	void VFSAddDiskDir( const char *vfsRoot, const char *nativeDirectory, bool readOnly ) override;
	
	/**
	 * \brief Add disk directory to virtual file system hidding a set of path.
	 * \version 1.13
	 */
	void VFSAddDiskDir( const char *vfsRoot, const char *nativeDirectory,
		bool readOnly, const decStringSet &hiddenPath ) override;
	
	/** \brief Add virtual file system container for module shared data. */
	void VFSAddScriptSharedDataDir() override;
	
	/**
	 * \brief Add DELGA file to virtual file system as root container.
	 * 
	 * Container maps the content of \em archivePath into the virtual file system.
	 */
	void VFSAddDelgaFile( const char *delgaFile, const char *archivePath ) override;
	
	/**
	 * \brief Add DELGA file to virtual file system as root container.
	 * \version 1.13
	 * 
	 * Container maps the content of \em archivePath into the virtual file system.
	 */
	void VFSAddDelgaFile( const char *delgaFile, const char *archivePath,
		const decStringSet &hiddenPath ) override;
	
	/**
	 * \brief Make modules add stage specific containers to virtual file system.
	 * \version 1.23
	 */
	void ModulesAddVFSContainers( const char *stage ) override;
	
	/** \brief Set command line arguments. */
	void SetCmdLineArgs( const char *arguments ) override;
	
	/** \brief Create render window. */
	void CreateRenderWindow( int width, int height, bool fullScreen,
		const char *windowTitle, const char *iconPath ) override;
	
	/**
	 * \brief Start game.
	 * \deprecated Use StartGame(const char*, const char*, const char*, delGPModuleList*).
	 */
	void StartGame( const char *scriptDirectory, const char *gameObject,
		delGPModuleList *collectChangedParams = NULL ) override;
	
	/**
	 * \brief Start game.
	 * \version 1.9
	 */
	void StartGame( const char *scriptDirectory, const char *scriptVersion, const char *gameObject,
		delGPModuleList *collectChangedParams = NULL ) override;
	
	/** \brief Stop game. */
	void StopGame() override;
	
	/** \brief Game is still running. */
	int IsGameRunning() override;
	
	/** \brief Current display resolution. */
	decPoint GetDisplayCurrentResolution( int display ) override;
	
	/**
	 * \brief Resolution for display.
	 * 
	 * If \em resolution is \em NULL and \em resolutionCount is 0 returns the size of \em resolutions
	 * needed to store resolution data. If \em resolution is not \em NULL it contains the buffer to
	 * fill with information. \em resolutionCount indicates the size of \em resolutions. The number
	 * of written entries is returned which can be less than \em resolutionCount.
	 */
	int GetDisplayResolutions( int display, decPoint *resolutions, int resolutionCount ) override;
	
	/**
	 * \brief Current global scaling factor for display.
	 * 
	 * Value of 100 represents scaling of 100%. Value step size is 25.
	 */
	int GetDisplayCurrentScaleFactor(int display) override;
	
	/**
	 * \brief Read game definitions from DELGA file.
	 * 
	 * Replaces \em list with content of all found files.
	 */
	void ReadDelgaGameDefs( const char *delgaFile, decStringList &list ) override;
	
	/**
	 * \brief Read game definitions from DELGA file.
	 * 
	 * Replaces \em list with content of all found files.
	 */
	void ReadDelgaPatchDefs( const char *delgaFile, decStringList &list ) override;
	
	/**
	 * \brief Read files from DELGA file.
	 * 
	 * Stores content of files to \em filesContent as instances of decMemoryFile.
	 */
	void ReadDelgaFiles( const char *delgaFile, const decStringList &filenames,
		decObjectOrderedSet &filesContent ) override;

	
#ifdef OS_BEOS
	/**
	 * \brief Message received.
	 * 
	 * Required for direct engine instance on BeOS only.
	 */
	void BeosMessageReceived( BMessage *message ) override;
#endif
	
#ifdef OS_ANDROID
	/**
	 * \brief Read game definitions from DELGA file using VFS container.
	 * 
	 * Replaces \em list with content of all found files.
	 */
	void ReadDelgaGameDefsVfs(const deVFSContainer::Ref &container,
		const char *delgaFile, decStringList &list) override;
	
	/**
	 * \brief Read files from DELGA file file using VFS container.
	 * 
	 * Stores content of files to \em filesContent as instances of decMemoryFile.
	 */
	void ReadDelgaFilesVfs(const deVFSContainer::Ref &container, const char *delgaFile,
		const decStringList &filenames, decObjectOrderedSet &filesContent) override;
	
	/**
	 * \brief Add DELGA file to virtual file system as root container.
	 * 
	 * Container maps the content of \em archivePath into the virtual file system.
	 */
	void VFSAddDelgaFileVfs(const deVFSContainer::Ref &container, const char *delgaFile,
		const char *archivePath, const decStringSet &hiddenPath) override;
	
	/** \brief Run single game frame update. */
	void RunSingleFrameUpdate() override;
	
	/** \brief App gained/lost focus. */
	void SetAppActive(bool active) override;
	
	/** \brief App paused/resumed. */
	void SetAppPaused(bool paused) override;
	
	/** \brief An event processed by the application event loop. */
	void InputEvent(const android_input_buffer &inputBuffer) override;
#endif
/*@}*/
};

#endif
