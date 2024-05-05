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

#ifndef _DEALLAUNCHER_H_
#define _DEALLAUNCHER_H_

#include "dealJniHelper.h"
#include "config/dealConfiguration.h"
#include "engine/dealEngine.h"
#include "engine/modules/dealEngineModule.h"
#include "filesystem/deVirtualFileSystem.h"
#include "game/dealGameManager.h"
#include "gui/dealDisplay.h"
#include "common/string/decString.h"
#include "common/string/unicode/decUnicodeArgumentList.h"

struct android_app;
struct AInputEvent;

class dealAssetUnpacker;
class dealDialogMain;
class dealGame;
class dealGameProfile;
class dealGPModuleList;

class deLogger;
class decBaseFileReader;
class decBaseFileWriter;
class decMemoryFile;
class dealGameData;



/**
 * \brief Android launcher.
 */
class dealLauncher{
public:
	/** \brief States. */
	enum eStates{
		/** \brief Restore save state if present. */
		esRestoreState,
		
		/** \brief Process activity intent if present. */
		esProcessActivityIntent,
		
		/** \brief Check initial installation. */
		esInitialCheck,
		
		/** \brief Initial installation. */
		esInitialInstall,
		
		/** \brief Main menu. */
		esMainMenu,
		
		/** \brief Start game. */
		esGameStart,
		
		/** \brief Game running. */
		esGameRunning,
		
		/** \brief Stop game. */
		esGameStop,
		
		/** \brief Quit launcher. */
		esQuit,
		
		/** \brief Wait for application to be destroyed after quit is issued. */
		esWaitDestroy,
		
		/** \brief Application has been destroyed. */
		esDestroyed
	};
	
private:
	android_app &pAndroidApp;
	JNIEnv *pJniEnv;
	
	eStates pState;
	deLogger *pLogger;
	dealAssetUnpacker *pAssetUnpacker;
	
	dealDisplay pDisplay;
	
	deVirtualFileSystem *pFileSystem;
	dealConfiguration pConfiguration;
	dealGameManager pGameManager;
	dealEngine pEngine;
	
	dealDialogMain *pDialogMain;
	
	decString pGameIdentifier;
	decString pProfileName;
	
	dealGame *pGame;
	dealGameProfile *pProfile;
	dealGPModuleList *pModuleParameters;
	decString pRunArguments;
	int pRunWidth;
	int pRunHeight;
	bool pRunFullScreen;
	decString pWindowTitle;
	
	jniGlobalJObject pObjUriIntentData;
	dealGameData *pGameData;
	
	bool pFocused;
	bool pFrozen;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create launcher. */
	dealLauncher( android_app &androidApp );
	
	/** \brief Clean up launcher. */
	~dealLauncher();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Android app. */
	inline android_app &GetAndroidApp() const{ return pAndroidApp; }
	
	/** \brief JNI environment. */
	inline JNIEnv *GetJniEnv() const{ return pJniEnv; }
	
	/** \brief Android application internal path. */
	const char *GetInternalPath() const;
	
	/** \brief Android application external path. */
	const char *GetExternalPath() const;
	
	/** \brief Android application window is present. */
	bool HasAppWindow() const;
	
	/** \brief Logger. */
	deLogger &GetLogger() const;
	
	/** \brief Display. */
	inline dealDisplay &GetDisplay(){ return pDisplay; }
	inline const dealDisplay &GetDisplay() const{ return pDisplay; }
	
	/** \brief File system. */
	inline deVirtualFileSystem *GetFileSystem(){ return pFileSystem; }
	
	/** \brief Configuration. */
	inline dealConfiguration &GetConfiguration(){ return pConfiguration; }
	inline const dealConfiguration &GetConfiguration() const{ return pConfiguration; }
	
	/** \brief Game manager. */
	inline dealGameManager &GetGameManager(){ return pGameManager; }
	inline const dealGameManager &GetGameManager() const{ return pGameManager; }
	
	/** \brief Get engine. */
	inline dealEngine &GetEngine(){ return pEngine; }
	inline const dealEngine &GetEngine() const{ return pEngine; }
	
	
	
	/**
	 * \brief Load asset into memory file.
	 * \param[in] filename Filename relative to asset root without leading slash.
	 * \param[out] memoryFile Memory file to write asset data to.
	 */
	void LoadAsset( const char *filename, decMemoryFile &memoryFile );
	
	
	
	/** \brief Load state. */
	void RestoreState();
	
	/** \brief Process activity intent if present. */
	void ProcessActivityIntent();
	
	/**
	 * \brief Check initial installation.
	 * \returns \em true if installation is ready or \em false if installation is required.
	 */
	bool CheckInitialInstall();
	
	/** \brief Begin initial installation. */
	void StartInitialInstall();
	
	/**
	 * \brief Check initial installation progress.
	 * \returns \em true if installation is still running or \em false if it finished.
	 */
	bool InitialInstallProgress();
	
	/** \brief Stop asset unpacking. */
	void StopAssetUnpacking();
	
	/** \brief Launch game. */
	void LaunchGame();
	
	/** \brief Start game engine if not running. */
	void StartEngine();
	
	/** \brief Start game. */
	void StartGame();
	
	/** \brief Update frame. */
	bool UpdateFrame();
	
	/** \brief Stop game. */
	void StopGame();
	
	/** \brief Stop game engine if running. */
	void StopEngine();
	
	
	
	/** \brief Initialize window. */
	void InitWindow();
	
	/** \brief Terminate window. */
	void TerminateWindow();
	
	/**
	 * \brief Process a frame loop.
	 * \returns \em true to keep running or \em false to exit application.
	 */
	bool FrameLoop();
	
	/** \brief Process input event. */
	void ProcessInputEvent( const AInputEvent &event );
	
	/** \brief Focused. */
	inline bool Focused() const{ return pFocused; }
	
	/** \brief Focus gained. */
	void FocusGained();
	
	/** \brief Focus lost. */
	void FocusLost();
	
	/** \brief Application started. */
	void AppStart();
	
	/** \brief Application stopped. */
	void AppStop();
	
	/** \brief Application is frozen. */
	inline bool Frozen() const{ return pFrozen; }
	
	/** \brief Pause application. */
	void AppPause();
	
	/** \brief Resume application. */
	void AppResume();
	
	/** \brief Save state. */
	void SaveState();
	
	/** \brief Application configuration changed like screen rotated. */
	void ConfigChanged();
	
	/** \brief Application input changed. */
	void InputChanged();
	
	
	
	/**
	 * \brief Exit application.
	 * \details Used to cleanly exit from any state.
	 */
	void ExitApplication();
	/*@}*/
	
private:
	void pCleanUp();
	void pInitLogger();
	void pLocateGame();
	void pLocateProfile();
	void pPrintGameProblems() const;
	void pPrintProfileProblems() const;
	void pPrintModuleProblem( const char *moduleName, dealEngineModule::eModuleTypes moduleType ) const;
	void pReadState( decBaseFileReader &reader );
	void pWriteState( decBaseFileWriter &writer );
};

#endif
