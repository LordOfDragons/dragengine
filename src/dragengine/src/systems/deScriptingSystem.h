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
