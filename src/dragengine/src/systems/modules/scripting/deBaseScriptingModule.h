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

#ifndef _DEBASESCRIPTINGMODULE_H_
#define _DEBASESCRIPTINGMODULE_H_

#include "../deBaseModule.h"
#include "../../../common/math/decMath.h"

class deBaseScriptingCollider;
class deBaseScriptingComponent;
class deBaseScriptingConnection;
class deBaseScriptingNetworkState;
class deBaseScriptingParticleEmitterInstance;
class deBaseScriptingPropField;
class deBaseScriptingServer;
class deBaseScriptingSpeaker;
class deBaseScriptingSoundLevelMeter;
class deBaseScriptingTouchSensor;
class deCollider;
class deComponent;
class deConnection;
class deInputEvent;
class deNetworkState;
class deParticleEmitterInstance;
class dePropField;
class deServer;
class deSoundLevelMeter;
class deSpeaker;
class deTouchSensor;


/**
 * \brief Base Scripting Module.
 *
 * The scripting module provides the interface of the engine systems to a
 * scripting or programming language. The scripting module defines also
 * how much of the power of the engine is exposed to the scripter.
 */
class DE_DLL_EXPORT deBaseScriptingModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new module. */
	deBaseScriptingModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deBaseScriptingModule();
	/*@}*/
	
	
	
	/** \name Modeul Management */
	/*@{*/
	/**
	 * \brief Directory relative to the module share directory where shared module data is located.
	 * 
	 * \returns NULL if no such directory is used.
	 */
	virtual const char *GetSharedDataDir() const = 0;
	
	/**
	 * \brief VFS directory where the module share directory is shown at.
	 * 
	 * \returns NULL if no such directory is used.
	 */
	virtual const char *GetVFSSharedDataDir() const = 0;
	
	/**
	 * Prepares the scripting module to operate the game by checking the
	 * specified script directory to contain suitable scripts and
	 * allocates needed objects or memory. A startable package usually
	 * needs certain classes or functions to be present to be startable.
	 * See the individual manuals of the scripting modules to learn about
	 * those requirements. After the Init function has exited the scripting
	 * module is ready to operate the game.
	 */
	virtual bool Init( const char *scriptDirectory, const char *gameObject ) = 0;
	
	/** \brief Shut down scripting module and frees all resources. */
	virtual void ShutDown() = 0;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create peer for the given collider object. */
	virtual deBaseScriptingCollider *CreateCollider( deCollider *collider ) = 0;
	
	/** \brief Create peer for the given server object. */
	virtual deBaseScriptingServer *CreateServer( deServer *server ) = 0;
	
	/** \brief Create peer for the given connection object. */
	virtual deBaseScriptingConnection *CreateConnection( deConnection *connection ) = 0;
	
	/** \brief Create peer for the given network state object. */
	virtual deBaseScriptingNetworkState *CreateNetworkState( deNetworkState *state ) = 0;
	
	/** \brief Create peer for the given touch sensor object. */
	virtual deBaseScriptingTouchSensor *CreateTouchSensor( deTouchSensor *touchSensor ) = 0;
	
	/** \brief Create peer for the given prop field object. */
	virtual deBaseScriptingPropField *CreatePropField( dePropField *propField ) = 0;
	
	/** \brief Create peer for the given particle emitter instance object or NULL if not used. */
	virtual deBaseScriptingParticleEmitterInstance *CreateParticleEmitterInstance(
		deParticleEmitterInstance *instance ) = 0;
	
	/** \brief Create deSoundLevelMeter peer. */
	virtual deBaseScriptingSoundLevelMeter *CreateSoundLevelMeter( deSoundLevelMeter *meter ) = 0;
	
	/** \brief Create deSpeaker peer. */
	virtual deBaseScriptingSpeaker *CreateSpeaker( deSpeaker *speaker ) = 0;
	
	/**
	 * \brief Initialize game scripts.
	 * 
	 * This usually involves creating the game objects and calling init functions on them.
	 */
	virtual bool InitGame() = 0;
	
	/**
	 * \brief Exit game scripts.
	 * 
	 * This usually involves calling exit functions and freeing game objects.
	 */
	virtual bool ExitGame() = 0;
	
	/**
	 * \brief Called at the beginning of each update cycle.
	 * 
	 * This gives the game scripts the chance to do frame update related tasks that do not
	 * involve updating the screen. You can update skins or offscreen render targets but
	 * you should not update the screen itself.
	 * 
	 * \return true if the call has been successfull or false otherwise
	 */
	virtual bool OnFrameUpdate() = 0;
	
	/**
	 * \brief Called after the render window changed size.
	 */
	virtual bool OnResizeRenderWindow() = 0;
	
	/**
	 * \brief Send the given event to game scripts.
	 * \return true if the call has been successfull or false otherwise
	 */
	virtual bool SendEvent( deInputEvent *event ) = 0;
	
	/**
	 * \brief User requested window to be closed.
	 * 
	 * Default implementation calls deEngine.Quit().
	 */
	virtual void UserRequestQuit();
	/*@}*/
};

#endif
