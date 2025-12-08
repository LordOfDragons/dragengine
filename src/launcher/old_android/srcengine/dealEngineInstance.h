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

#ifndef _DEALENGINEINSTANCE_H_
#define _DEALENGINEINSTANCE_H_

#include "dealIEngineInstance.h"

#include <android/asset_manager.h>

#include <dragengine/common/string/decString.h>
#include <dragengine/systems/modules/deModuleParameter.h>

class deEngine;
class deOSAndroid;
#include <dragengine/logger/deLogger.h>
#include "dealVFSZipArchive.h"
#include <dragengine/filesystem/deVirtualFileSystem.h>
struct android_app;


/**
 * \brief Engine instance.
 * 
 * Glue library linking against the game engine handling a game engine instance.
 */
class dealEngineInstance : public dealIEngineInstance{
private:
	android_app &pAndroidApp;
	
	deLogger::Ref pLogger;
	
	AAsset *pEngineAsset;
	int pEngineAssetFileDescriptor;
	deVirtualFileSystem::Ref pOSFileSystem;
	
	deOSAndroid *pOSAndroid;
	deEngine *pEngine;
	
	deModuleParameter pModuleParameter;
	
	dealVFSZipArchive::Ref pDelga;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create engine. */
	dealEngineInstance(android_app &androidApp);
	
	/** \brief Clean up engine. */
	~dealEngineInstance() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Logger or \em null if not existing. */
	inline const deLogger::Ref &GetLogger() const{ return pLogger; }
	
	
	
	/** Determines if the engine is running. */
	bool IsRunning() const override;
	
	/**
	 * \brief Start engine.
	 * \returns \em true if successful or \em false if an exception happened.
	 */
	bool Start(const char *logfile, const char *cacheAppID) override;
	
	/**
	 * \brief Stop engine.
	 * \returns \em true if successful or \em false if an exception happened.
	 */
	bool Stop() override;
	
	/** \brief Process input event. */
	void ProcessInputEvent(const AInputEvent &event) override;
	
	/** \brief Frame update. */
	bool FrameUpdate(bool &keepRunning) override;
	
	
	
	/** \brief Get module status. */
	bool GetModuleStatus(const char *moduleName, const char *moduleVersion, int &status) override;
	
	/** \brief Number of module parameters. */
	bool GetModuleParameterCount(const char *moduleName, const char *moduleVersion,
		int &parameterCount) override;
	
	/** \brief Module parameter information. */
	bool GetModuleParameterData(const char *moduleName, const char *moduleVersion,
		int parameter, const char *&parameterName, const char *&parameterDescription,
		const char*&parameterValue) override;
	
	/** \brief Set module parameter value. */
	bool SetModuleParameterValue(const char *moduleName, const char *moduleVersion,
		const char *parameter, const char *value) override;
	
	/** \brief Engine property. */
	bool GetProperty(int property, const char *&value) override;
	
	/** \brief Activate module. */
	bool ActivateModule(const char *moduleName, const char *moduleVersion) override;
	
	/** \brief Enable or disable module. */
	bool EnableModule(const char *moduleName, const char *moduleVersion, bool enable) override;
	
	/** \brief Set command line arguments. */
	bool SetCmdLineArgs(const char *arguments) const override;
	
	/** \brief Set data directory. */
	bool SetDataDirectory(const char *directory) override;
	
	
	
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
	bool OpenDelga(int fileDescriptor, long fileOffset, long fileLength) override;
	
	
	
	/** \brief Add disk directory to virtual file system. */
	bool VFSAddDiskDir(const char *vfsRoot, const char *nativeDirectory, bool readOnly) override;
	
	/** \brief Add virtual file system container for module shared data. */
	bool VFSAddScriptSharedDataDir() override;
	
	/**
	 * \brief Add virtual file system container referenceing path in DELGA file.
	 * 
	 * \param[in] vfsRoot Path to show content at in virtual file system.
	 * \param[in] vfsBase Base path relative to zip archive to show in virtual file system.
	 */
	bool VFSAddDelga(const char *vfsRoot, const char *vfsBase) override;
	
	/** \brief Add redirect directory to virtual file system. */
	bool VFSAddRedirect(const char *root, const char *redirect) override;
	
	/**
	 * \brief Make modules add stage specific containers to virtual file system.
	 * \version 1.23
	 */
	bool ModulesAddVFSContainers(const char *stage) override;
	
	
	
	/** \brief Create render window. */
	bool CreateRenderWindow(int width, int height, bool fullScreen, const char *windowTitle) override;
	
	/** \brief Start game. */
	bool StartGame(const char *scriptDirectory, const char *gameObject) override;
	
	/** \brief Stop game. */
	bool StopGame() override;
	
	/** \brief Determines if the game is still running. */
	int IsGameRunning() override;
	
	
	
	/** \brief Focus gained. */
	bool FocusGained() override;
	
	/** \brief Focus lost. */
	bool FocusLost() override;
	
	/** \brief Freeze application. */
	bool Freeze() override;
	
	/** \brief Thaw application. */
	bool Thaw() override;
	
	/** \brief Application window has been created. */
	bool InitAppWindow() override;
	
	/** \brief Application window has been closed. */
	bool TerminateAppWindow() override;
	/*@}*/
	
private:
	void pCleanUp();
	
	void pCreateLogger(const char *logfile);
	void pCreateOSFileSystem();
	void pCloseOSFileSystem();
};

#endif
