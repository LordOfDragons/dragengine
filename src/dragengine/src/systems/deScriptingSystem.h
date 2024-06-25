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

#ifndef _DESCRIPTINGSYSTEM_H_
#define _DESCRIPTINGSYSTEM_H_

#include "deBaseSystem.h"
#include "../common/math/decMath.h"
#include "../common/string/decString.h"

class deVirtualFileSystem;
class deCollider;
class deServer;
class deConnection;
class deNetworkState;
class deBaseScriptingModule;
class deInputEvent;
class deTouchSensor;
class dePropField;
class deParticleEmitterInstance;
class deSoundLevelMeter;
class deSpeaker;
class deService;


/**
 * \brief Scripting System.
 *
 * Provides support for an external scripting or programming language.
 * This is the only system that can not change during the run of a game
 * not even during engine-break time. The scripts drive the entire
 * engine and should provide an interface ( abstraction layer ) between
 * the engine and engine systems towards the game developer. The amount
 * of access to the engine functionality is depending on the scripting
 * module used.
 *
 * \note
 * Currently the system sends events directly to the scripting system.
 * It would be better to avoid this by giving the scripting system the
 * chance to retrieve the events itself from a queue. The best idea
 * would be to create an event queue in the engine object that the
 * input system writes to and that the scripting system retrieves from.
 * Conversion of events has to be done anyways.
 */
class DE_DLL_EXPORT deScriptingSystem : public deBaseSystem{
private:
	decString pScriptDirectory;
	decString pScriptVersion;
	decString pGameObject;
	deBaseScriptingModule *pActiveModule;
	bool pEditMode;
	bool pGameRunning;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new scripting system linked to the given engine. */
	deScriptingSystem( deEngine *engine );
	
	/** \brief Clean up scripting system. */
	~deScriptingSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Active module. */
	inline deBaseScriptingModule *GetActiveModule() const{ return pActiveModule; }
	
	/** \brief Directory relative to the game directory where the scripts are located. */
	inline const decString &GetScriptDirectory() const{ return pScriptDirectory; }
	
	/** \brief Set directory relative to the game directory where the scripts are located. */
	void SetScriptDirectory( const char *scriptDirectory );
	
	/**
	 * \brief Script version for compatibility.
	 * \version 1.9
	 */
	inline const decString &GetScriptVersion() const{ return pScriptVersion; }
	
	/**
	 * \brief Set script version for compatibility.
	 * \version 1.9
	 */
	void SetScriptVersion( const char *scriptVersion );
	
	/** \brief Initial game object to create. */
	inline const decString &GetGameObject() const{ return pGameObject; }
	
	/** \brief Set initial game object to create. */
	void SetGameObject( const char *gameObject );
	
	/**
	 * \brief Add virtual file system container for module shared data.
	 * 
	 * No container is added if no module is active or the active module has no shared data.
	 */
	void AddVFSSharedDataDir( deVirtualFileSystem &vfs ) const;
	
	/**
	 * \brief Add stage specific VFS containers.
	 * \version 1.23
	 * 
	 * If module requires dynamic modification of containers they should add
	 * add a deVFSRedirect container redirecting to a module owned deVirtualFileSystem.
	 * This way the containers can be modified while the game is running.
	 * 
	 * Stage is defined using a string and depends on the hosting application.
	 * These types are known (but more can be added):
	 * - patches: Game and patch containers are present. Allows modules to add
	 *   containers patching the game. A typical use for this are dynamically
	 *   downloaded patches as used in auto updating online games.
	 * - mods: After 'patches'. Allowes to add containers providing game modifications
	 * - overlay: After 'mods' and before adding the user overlay directory. Allows
	 *   modules to add containers overriding all containers up to this point in time.
	 *   The user overlay directory is always aded after this stage. Hence user
	 *   overlay content is always used first before any other content.
	 * 
	 * Applies to modules of type:
	 * - emtScript
	 * - emtService
	 */
	void AddVFSContainers( deVirtualFileSystem &vfs, const char *stage );
	
	/**
	 * \brief Scripts are executed in edit mode.
	 * 
	 * In this mode the scripts are reduced to what is required by an editing application.
	 * The edit mode can only be changed while the system is not running.
	 */
	inline bool GetEditMode() const{ return pEditMode; }
	
	/**
	 * \brief Set if scripts are executed in edit mode.
	 * 
	 * In this mode the scripts are reduced to what is required by an editing application.
	 * The edit mode can only be changed while the system is not running.
	 */
	void SetEditMode( bool editMode );
	
	/** \brief Inits the game if not running. */
	void InitGame();
	
	/** \brief Exits the game it running. */
	void ExitGame();
	
	/** \brief User requested window to be closed. */
	void UserRequestQuit();
	
	/** \brief A new fram update cycle started. */
	void OnFrameUpdate();
	
	/** \brief Render window has changed size. */
	void OnResizeRenderWindow();
	
	/**
	 * \brief Application received or lost focus.
	 * \version 1.22
	 */
	void OnAppActivate();
	
	/** \brief Event has to be processed. */
	void SendEvent( deInputEvent *event );
	
	/** \brief Create peer object for the given collider using the active module and assigns it. */
	void LoadCollider( deCollider *collider );
	
	/** \brief Create peer object for the given server using the active module and assigns it. */
	void LoadServer( deServer *server );
	
	/** \brief Create peer object for the given connection using the active module and assigns it. */
	void LoadConnection( deConnection *connection );
	
	/** \brief Create peer object for the given network state using the active module and assigns it. */
	void LoadNetworkState( deNetworkState *state );
	
	/** \brief Create peer object for the given touch sensor using the active module and assigns it. */
	void LoadTouchSensor( deTouchSensor *touchSensor );
	
	/** \brief Create peer object for the given prop field using the active module and assigns it. */
	void LoadPropField( dePropField *propField );
	
	/** \brief Create peer object for the given particle emitter instance using the active module and assigns it. */
	void LoadParticleEmitterInstance( deParticleEmitterInstance *instance );
	
	/** \brief Create deSoundLevelMeter peer using active module and assigns it. */
	void LoadSoundLevelMeter( deSoundLevelMeter *meter );
	
	/** \brief Create deSpeaker peer using active module and assigns it. */
	void LoadSpeaker( deSpeaker *speaker );
	
	/** \brief Create deService peer using active module and assigns it. */
	void CreateService( deService *service );
	/*@}*/
	
	
	
	/** \name Overloadables */
	/*@{*/
	/**
	 * \brief Set active loadable module.
	 * 
	 * Do not forget to call the super function.
	 */
	virtual void SetActiveModule( deLoadableModule *module );
	
	/**
	 * \brief Clearcross references and links that could lead to memory leaks.
	 * 
	 * Do not forget to call the super function.
	 */
	virtual void ClearPermanents();
	
	/** \brief Carry out here actions right after the system started up. */
	virtual void PostStart();
	
	/** \brief Carry out here actions right before the system shuts down. */
	virtual void PreStop();
	/*@}*/
};

#endif
