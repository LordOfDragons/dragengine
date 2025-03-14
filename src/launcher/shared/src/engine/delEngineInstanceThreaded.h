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

#ifndef _DELENGINEINSTANCETHREADED_H_
#define _DELENGINEINSTANCETHREADED_H_

#include <dragengine/dragengine_configuration.h>

#ifndef OS_ANDROID

#include "delEngineInstance.h"

#ifdef OS_W32
#include <dragengine/app/include_windows.h>
#endif


/**
 * \brief Instance of game engine running in different process.
 * \version 1.10
 */
class DE_DLL_EXPORT delEngineInstanceThreaded : public delEngineInstance{
public:
	/** \brief Factory creating engine instances. */
	class DE_DLL_EXPORT Factory : public delEngineInstance::Factory{
	public:
		/** \brief Factory constructor. */
		Factory();
		
	protected:
		/** \brief Factory destructor. */
		virtual ~Factory();
		
	public:
		/** \brief Create engine instance. */
		virtual delEngineInstance *CreateEngineInstance( delLauncher &launcher, const char *logfile );
	};
	
	
	
private:
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
	static decString pDefaultExecutableName;
#else
	int pPipeIn;
	int pPipeOut;
	pid_t pProcessID;
#endif
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create engine instance. */
	delEngineInstanceThreaded( delLauncher &launcher, const char *logfile );
	
protected:
	/** \brief Clean up engine instance. */
	~delEngineInstanceThreaded() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	#ifdef OS_W32
	/** \brief Executable name. */
	inline const decString &GetExecutableName() const{ return pExecutableName; }
	
	/** \brief Set executable name. */
	void SetExecutableName( const char *executableName );
	
	/** \brief Default executable name. */
	inline static const decString &GetDefaultExecutableName(){ return pDefaultExecutableName; }
	
	/** \brief Set default executable name. */
	static void SetDefaultExecutableName( const char *executableName );
	#endif
	
	
	
	/** \brief Engine is running. */
	bool IsEngineRunning() const override;
	
	/** \brief Start engine if not running. */
	bool StartEngine() override;
	
	/** \brief Stop engine if not running. */
	void StopEngine() override;
	
	/** \brief Kill engine if running. */
	void KillEngine() override;
	
	
	
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
	/*@}*/
};

#endif

#endif // OS_ANDROID
