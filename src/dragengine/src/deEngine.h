/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEENGINE_H_
#define _DEENGINE_H_

#include "common/string/decString.h"
#include "common/utils/decPRNG.h"

class deAISystem;
class deAnimationManager;
class deAnimatorManager;
class deAnimatorInstanceManager;
class deAnimatorSystem;
class deArchiveManager;
class deAudioSystem;
class deBaseSystem;
class deBillboardManager;
class deCanvasManager;
class deCameraManager;
class deCaptureCanvasManager;
class deCmdLineArgs;
class deColliderManager;
class deComponentManager;
class deConnectionManager;
class deCrashRecoverySystem;
class deDebugDrawerManager;
class deDecalManager;
class deDynamicSkinManager;
class deEffectManager;
class deEnvMapProbeManager;
class deErrorTrace;
class deFontManager;
class deForceFieldManager;
class deGraphicSystem;
class deHeightTerrainManager;
class deImageManager;
class deInputSystem;
class deLanguagePackManager;
class deLightManager;
class deLogger;
class deLumimeterManager;
class deMicrophoneManager;
class deModelManager;
class deModuleSystem;
class deNavigationBlockerManager;
class deNavigationSpaceManager;
class deNavigatorManager;
class deNetworkStateManager;
class deNetworkSystem;
class deOcclusionMeshManager;
class deOS;
class deParallelProcessing;
class deParticleEmitterManager;
class deParticleEmitterInstanceManager;
class dePhysicsSystem;
class dePropFieldManager;
class deRenderWindowManager;
class deResourceLoader;
class deResourceManager;
class deRigManager;
class deScriptingSystem;
class deServerManager;
class deSkinManager;
class deSkyManager;
class deSkyInstanceManager;
class deSmokeEmitterManager;
class deSoundManager;
class deSoundLevelMeterManager;
class deSpeakerManager;
class deSynthesizerManager;
class deSynthesizerInstanceManager;
class deSynthesizerSystem;
class deTouchSensorManager;
class deVideoManager;
class deVideoPlayerManager;
class deVirtualFileSystem;
class deVRSystem;
class deWorldManager;

class decTimer;


/**
 * \brief Drag[en]gine Game Engine Class.
 *
 * Manages the runtime
 * of the entire game engine. Create one instance of
 * this class for each game running. The class is safe
 * to be used in multiple instances but this behaviour
 * can be restricted by single type modules grabbing
 * certain devices.
 *
 * To launch successfully a game with this engine create
 * first one instance of this class. Then set your game
 * specific information to the shared data directory
 * of the engine and your game data directory. You are
 * now ready to launch the game by calling the run
 * function. You need to specify the script directory
 * inside your data directory where the scripting module
 * has to look for your game scripts. The run function
 * catches exceptions itself hence there is no need to
 * enclose it in a try-catch block.
 *
 * If you plan to use the library in slave mode hence
 * only for rendering and you provide the run loop then
 * you can do parts of the run() function yourself. Be
 * carefull though as wrong setting will result in
 * crashes. Wrapping all in an try-catch clause should
 * be enough for testing to be safe.
 */
class deEngine{
private:
	// application
	deCmdLineArgs *pArgs;
	deOS *pOS;
	deVirtualFileSystem *pOSFileSystem;
	
	// crash prevention system
	deErrorTrace *pErrorTrace;
	bool pScriptFailed;
	bool pSystemFailed;
	deLogger *pLogger;
	
	// systems
	deModuleSystem *pModSys;
	deBaseSystem **pSystems;
	deParallelProcessing *pParallelProcessing;
	deResourceLoader *pResLoader;
	
	// resource managers
	deResourceManager **pResMgrs;
	
	// files
	decString pPathData; // the path to the data files
	decString pCacheAppID; // unique catch directory identifier for the application
	deVirtualFileSystem *pVFS;
	decString pPathCapture;
	decString pPathOverlay;
	decString pPathConfig;
	
	// frame timer
	decTimer *pFrameTimer;
	float pElapsedTime;
	float pAccumElapsedTime;
	
	// prng
	decPRNG pPRNG;
	
	// fps
	float pFPSAccum;
	int pFPSFrames;
	int pFPSRate;
	
	// os events
	bool pRequestQuit;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create game engine instance.
	 * 
	 * OS file system is used if the engine installation is located outside
	 * the simple confines of an installation directory.
	 * If not NULL the file system has to provide the
	 * following structure:
	 * - /engine : read-only container with engine module libraries and definitions
	 * - /share : read-only container with shared engine and module files
	 * - /config : read-write container with engine and module config files
	 * 
	 * \param[in] os OS object to use. Deleted during destructor.
	 * \param[in] fileSystem OS file system or NULL if not used.
	 */
	deEngine( deOS *os, deVirtualFileSystem *fileSystem = NULL );
	
	~deEngine();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Default pseudo rundom number generator. */
	inline decPRNG &GetPRNG(){ return pPRNG; }
	/*@}*/
	
	
	
	/** \name System Accessors */
	/*@{*/
	inline deModuleSystem *GetModuleSystem() const{ return pModSys; }
	
	int GetSystemCount() const;
	deBaseSystem *GetSystemAt( int index ) const;
	
	deGraphicSystem *GetGraphicSystem() const;
	deInputSystem *GetInputSystem() const;
	dePhysicsSystem *GetPhysicsSystem() const;
	deScriptingSystem *GetScriptingSystem() const;
	deCrashRecoverySystem *GetCrashRecoverySystem() const;
	deAnimatorSystem *GetAnimatorSystem() const;
	deAudioSystem *GetAudioSystem() const;
	deNetworkSystem *GetNetworkSystem() const;
	deAISystem *GetAISystem() const;
	deSynthesizerSystem *GetSynthesizerSystem() const;
	deVRSystem *GetVRSystem() const;
	
	/** \brief Scan module directory and loads all modules in there. */
	void LoadModules();
	/*@}*/
	
	
	
	/**
	 * \name Crash Recovery System (CRS)
	 * Signals to the game engine unexpected errors in one of the
	 * engine systems. During the next update cycle the engine
	 * will enter CRS mode to handle the errors.
	 */
	/*@{*/
	/** \brief Error trace object. */
	inline deErrorTrace *GetErrorTrace() const{ return pErrorTrace; }
	
	/** \brief Scripting system failed. */
	inline bool GetScriptFailed() const{ return pScriptFailed; }
	
	/** \brief Signals that the scripting system failed. */
	inline void SignalScriptFailed(){ pScriptFailed = true; }
	
	/** \brief Determines if some other system failed. */
	inline bool GetSystemFailed() const{ return pSystemFailed; }
	
	/** \brief Signals that some other system failed. */
	inline void SignalSystemFailed(){ pSystemFailed = true; }
	
	/** \brief Reset failure flags. */
	void ResetFailureFlags();
	
	/** \brief Logger. */
	inline deLogger *GetLogger() const{ return pLogger; }
	
	/** \brief Set logger. */
	void SetLogger( deLogger *logger );
	/*@}*/
	
	
	
	/**
	 * \name Resource Manager Accessors
	 * Retrieves various resource managers. You can either retrieve
	 * them by index or by specifying the resource type you are
	 * looking for. Furthermore there are convenience functions
	 * to safely access a resource manager of the given type.
	 */
	/*@{*/
	int GetResourceManagerCount() const;
	deResourceManager *GetResourceManagerAt( int index ) const;
	deResourceManager *GetResourceManagerFor( int resourceType ) const;
	
	deAnimationManager *GetAnimationManager() const;
	deAnimatorManager *GetAnimatorManager() const;
	deAnimatorInstanceManager *GetAnimatorInstanceManager() const;
	deArchiveManager *GetArchiveManager() const;
	deBillboardManager *GetBillboardManager() const;
	deCanvasManager *GetCanvasManager() const;
	deCameraManager *GetCameraManager() const;
	deCaptureCanvasManager *GetCaptureCanvasManager() const;
	deColliderManager *GetColliderManager() const;
	deComponentManager *GetComponentManager() const;
	deConnectionManager *GetConnectionManager() const;
	deDebugDrawerManager *GetDebugDrawerManager() const;
	deDecalManager *GetDecalManager() const;
	deDynamicSkinManager *GetDynamicSkinManager() const;
	deEffectManager *GetEffectManager() const;
	deEnvMapProbeManager *GetEnvMapProbeManager() const;
	deFontManager *GetFontManager() const;
	deForceFieldManager *GetForceFieldManager() const;
	deHeightTerrainManager *GetHeightTerrainManager() const;
	deImageManager *GetImageManager() const;
	deLanguagePackManager *GetLanguagePackManager() const;
	deLightManager *GetLightManager() const;
	deLumimeterManager *GetLumimeterManager() const;
	deMicrophoneManager *GetMicrophoneManager() const;
	deModelManager *GetModelManager() const;
	deNavigationBlockerManager *GetNavigationBlockerManager() const;
	deNavigationSpaceManager *GetNavigationSpaceManager() const;
	deNavigatorManager *GetNavigatorManager() const;
	deNetworkStateManager *GetNetworkStateManager() const;
	deOcclusionMeshManager *GetOcclusionMeshManager() const;
	deParticleEmitterManager *GetParticleEmitterManager() const;
	deParticleEmitterInstanceManager *GetParticleEmitterInstanceManager() const;
	dePropFieldManager *GetPropFieldManager() const;
	deRenderWindowManager *GetRenderWindowManager() const;
	deRigManager *GetRigManager() const;
	deServerManager *GetServerManager() const;
	deSkinManager *GetSkinManager() const;
	deSkyManager *GetSkyManager() const;
	deSkyInstanceManager *GetSkyInstanceManager() const;
	deSmokeEmitterManager *GetSmokeEmitterManager() const;
	deSoundManager *GetSoundManager() const;
	deSoundLevelMeterManager *GetSoundLevelMeterManager() const;
	deSpeakerManager *GetSpeakerManager() const;
	deSynthesizerManager *GetSynthesizerManager() const;
	deSynthesizerInstanceManager *GetSynthesizerInstanceManager() const;
	deTouchSensorManager *GetTouchSensorManager() const;
	deVideoManager *GetVideoManager() const;
	deVideoPlayerManager *GetVideoPlayerManager() const;
	deWorldManager *GetWorldManager() const;
	
	/** \brief Parallel processing. */
	inline deParallelProcessing &GetParallelProcessing(){ return *pParallelProcessing; }
	inline const deParallelProcessing &GetParallelProcessing() const{ return *pParallelProcessing; }
	
	/** \brief Resource loader. */
	inline deResourceLoader *GetResourceLoader() const{ return pResLoader; }
	
	/**
	 * \brief Remove all resources and report them.
	 * 
	 * This function is used to check if a game left behind unreleased resources and to
	 * release the. This ensures a new game can use the same engine without potential left
	 * behind resources of the previous game to pollute the memory. Leaving behind resources
	 * after the game finished is a bug and thus this function lists all culprit resources
	 * as good as possible. The engine call this method before releasing all the resource
	 * managers.
	 */
	void RemoveLeakingResources();
	/*@}*/
	
	
	
	/** \name File System Management */
	/*@{*/
	/**
	 * \brief Game data directory.
	 * 
	 * This directory is considered the virtual root directory of your gama data.
	 * All files handled with the file manager retrieved through GetFileManager are
	 * relative to this directory. The game engine uses this file manager to locate
	 * all resource files. Also the scripting system expects script files to be
	 * somewhere inside this directory.
	 */
	inline const decString &GetDataDir() const{ return pPathData; }
	
	/**
	 * \brief Set game data directory.
	 * 
	 * This directory is considered the virtual root directory of your gama data.
	 * All files handled with the file manager retrieved through GetFileManager are
	 * relative to this directory. The game engine uses this file manager to locate
	 * all resource files. Also the scripting system expects script files to be
	 * somewhere inside this directory.
	 */
	void SetDataDir( const char *dataDir );
	
	/**
	 * \brief Unique catch directory identifier for the application.
	 * 
	 * If the path is empty no application specific cache will be provided.
	 */
	inline const decString &GetCacheAppID() const{ return pCacheAppID; }
	
	/**
	 * \brief Set unique catch directory identifier for the application.
	 * 
	 * If the path is empty no application specific cache will be provided.
	 */
	void SetCacheAppID( const char *cacheAppID );
	
	/** \brief Virtual file system used by the game engine. */
	inline deVirtualFileSystem *GetVirtualFileSystem() const{ return pVFS; }
	
	/**
	 * \brief Overlay directory.
	 * \version 1.7
	 * 
	 * Set by the launcher to indicate to script modules the native path under
	 * which overlay data is stored or an empty string if not available.
	 */
	inline const decString &GetPathOverlay() const{ return pPathOverlay; }
	
	/**
	 * \brief Set overlay directory.
	 * \version 1.7
	 * 
	 * Set by the launcher to indicate to script modules the native path under
	 * which overlay data is stored or an empty string if not available.
	 */
	void SetPathOverlay( const char *path );
	
	/**
	 * \brief Capture directory.
	 * \version 1.7
	 * 
	 * Set by the launcher to indicate to script modules the native path under
	 * which capture data is stored or an empty string if not available.
	 */
	inline const decString &GetPathCapture() const{ return pPathCapture; }
	
	/**
	 * \brief Set capture directory.
	 * \version 1.7
	 * 
	 * Set by the launcher to indicate to script modules the native path under
	 * which capture data is stored or an empty string if not available.
	 */
	void SetPathCapture( const char *path );
	
	/**
	 * \brief Config directory.
	 * \version 1.7
	 * 
	 * Set by the launcher to indicate to script modules the native path under
	 * which config data is stored or an empty string if not available.
	 */
	inline const decString &GetPathConfig() const{ return pPathConfig; }
	
	/**
	 * \brief Set config directory.
	 * \version 1.7
	 * 
	 * Set by the launcher to indicate to script modules the native path under
	 * which config data is stored or an empty string if not available.
	 */
	void SetPathConfig( const char *path );
	/*@}*/
	
	
	
	/**
	 * \name Frame Information
	 */
	/*@{*/
	/** \brief Time elapsed since the last frame update in seconds. */
	inline float GetElapsedTime() const{ return pElapsedTime; }
	
	/**
	 * \brief Set the time elapsed since the last frame update in seconds.
	 * 
	 * Can be called instead of UpdateElapsedTime to explicitely set the elapsed time.
	 * Resets the elapsed time timer and updates the FPS Rate.
	 */
	void SetElapsedTime( float elapsed );
	
	/** \brief Frame-per-second rate averaged over the last couple of frames. */
	int GetFPSRate() const;
	/*@}*/
	
	
	
	/**
	 * \name Operating System Bindings
	 */
	/*@{*/
	/** \brief Command line arguments. */
	inline deCmdLineArgs *GetArguments() const{ return pArgs; }
	
	/** \brief Operating system object. */
	inline deOS *GetOS() const{ return pOS; }
	
	/**
	 * \brief OS file system or NULL if not used.
	 * 
	 * OS file system is used if the engine installation is located outside the simple confines
	 * of an installation directory.
	 * 
	 * \warning Do not make this file system available to game scripts. Faulty or malicious
	 * applications could damage the system or the content of the file system.
	 */
	inline deVirtualFileSystem *GetOSFileSystem() const{ return pOSFileSystem; }
	
	/** \brief Determine if the engine received a quit request. */
	inline bool GetQuitRequest() const{ return pRequestQuit; }
	
	/** \brief Requests the engine to quit before the next frame update cycle. */
	void Quit();
	
	/** \brief Reset quit request. */
	void ResetQuitRequest();
	/*@}*/
	
	
	
	/**
	 * \name Game Engine Run Time
	 */
	/*@{*/
	/**
	 * \brief Run game engine.
	 * 
	 * Calling this function the control is handed over to the game engine. The scripts in the
	 * specified directory ( relative to the data directory ) are parsed and executed. This
	 * function returns control to you after a quit-request has been issued or an unrecoverable
	 * error has occurred. The return value indicates if the game engine exited under normal
	 * circumstances or due to a severe error. This function already handles exceptions so there
	 * is no need to enclose it in a try-catch block.
	 *
	 * \param scriptDir Directory relative to data directory containing the script files for your game.
	 * \param gameObject Initial game object to create. Script module specific value.
	 *                   Usually a class or function name to use to create the game object
	 * \returns true if the game engine exited under normal circumstances or false if an
	 *          unrecoverable error occurred
	 */
	bool Run( const char *scriptDirectory, const char *gameObject );
	
	/**
	 * \brief Reset elapsed time counter.
	 * 
	 * This is usefull after lengthy time operations like loading a game. Without this call
	 * the reported amount of seconds elapsed since the last frame update can grow tremendously
	 * huge resulting in strange movements in the first frame after the player takes control.
	 */
	void ResetTimers();
	
	/**
	 * \brief Update elapsed time since the last frame update using the internal timer.
	 * 
	 * Used by launchers taking control of the entire engine run-time instead of handing
	 * it off using the Run function. Use ResetTimers to avoid too long elapsed time after
	 * long operations or use SetElapsedTime to set the elapsed time explicitely. Using this
	 * function though is the better solution. Updates the FPS rate. After this function
	 * RunDoSingleFrame is typically called. Before this function is called ProcessInput
	 * has to be called on the active input module and the quit request checked first.
	 */
	void UpdateElapsedTime();
	
	/**
	 * \brief Run single frame update.
	 * 
	 * Used by launchers taking control of the entire engine run-time instead of handing it
	 * off using the Run function. In particular this does (in this order):<br/>
	 * <ul>
	 * <li>Send input events to the active scripting module.</li>
	 * <li>Update the engine resource loader.</li>
	 * <li>Call OnFrameUpdate function on the active scripting module.</li>
	 * <li>Process audio using the active audio module.</li>
	 * <li>Process networking using the active network module.</li>
	 * <li>Call BeginFrame on the active graphic module.</li>
	 * <li>Call EndFrame on the active graphic module.</li>
	 * </ul>
	 * 
	 * On errors the processing exits the function immediately. Check if GetScriptFailed
	 * or GetSystemFailed returns true and enter error handling if this is the case.
	 * UpdateElapsedTime or SetElapsedTime has to be called before calling this function.
	 * 
	 * \note The frame rate is capped at 200 Hz to avoid very small time steps in case
	 * the main thread components run fast. This frame rate limit is indepedent of frame
	 * rate limits imposed by engine modules.
	 */
	void RunSingleFrame();
	/*@}*/
	
	
	
private:
	void pInit();
	void pInitSystems();
	void pInitResourceManagers();
	void pCleanUp();
	void pUpdateFPSRate();
	bool pClearPermanents();
	bool pStopSystems();
	bool pRecoverFromError();
};

#endif
