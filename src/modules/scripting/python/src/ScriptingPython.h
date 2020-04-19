/* 
 * Drag[en]gine Python Script Module
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

#ifndef _SCRIPTINGPYTHON_H_
#define _SCRIPTINGPYTHON_H_

#include "spCommon.h"

#include <dragengine/systems/modules/scripting/deBaseScriptingModule.h>
#include <dragengine/common/string/decString.h>

class spTypeCanvas;
class spScriptFileList;
class spBaseModule;
class spModuleEngine;
class spTypeWorld;
class spModuleGraphicSystem;
class spTypeModuleLoader;



/**
 * \brief Python Scripting Module.
 */
class ScriptingPython : public deBaseScriptingModule{
private:
	PyObject *pModuleGame;
	PyObject *pObjectGame;
	
	spBaseModule *pNamespaceDragengine;
	spBaseModule *pNamespaceScenery;
	spBaseModule *pNamespaceGui;
	
	spTypeModuleLoader *pTypeModuleLoader;
	spModuleEngine *pModuleEngine;
	spTypeCanvas *pTypeCanvas;
	spModuleGraphicSystem *pModuleGraphicSystem;
	spTypeWorld *pTypeWorld;
	
	decString pScriptPath;
	
	spScriptFileList *pScriptFileList;
	
	static ScriptingPython *pSingleton;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new MODULE. */
	ScriptingPython( deLoadableModule &loadableModule );
	/** \brief Cleans up the MODULE. */
	virtual ~ScriptingPython();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/**
	 * \brief Directory relative to the module share directory where shared module data is located.
	 * \returns \em NULL if no such directory is used.
	 */
	virtual const char *GetSharedDataDir() const;
	
	/**
	 * \brief VFS directory where the module share directory is shown at.
	 * \returns \em NULL if no such directory is used.
	 */
	virtual const char *GetVFSSharedDataDir() const;
	
	/**
	 * \brief Prepares the scripting module to operate the game.
	 * \details Checks the specified script directory to contain suitable scripts
	 *          and allocates needed objects or memory. A startable package usually
	 *          needs certain classes or functions to be present to be startable.
	 *          See the individual manuals of the scripting modules to learn about
	 *          those requirements. After the Init function has exited the scripting
	 *          module is ready to operate the game.
	 */
	virtual bool Init( const char *scriptDirectory, const char *gameObject );
	/** \brief Shuts down the scripting module and frees all resources. */
	virtual void ShutDown();
	
	/** \brief Creates a peer for the given collider object. */
	virtual deBaseScriptingCollider *CreateCollider( deCollider *collider );
	/** \brief Creates a peer for the given server object. */
	virtual deBaseScriptingServer *CreateServer( deServer *server );
	/** \brief Creates a peer for the given connection object. */
	virtual deBaseScriptingConnection *CreateConnection( deConnection *connection );
	/** \brief Creates a peer for the given network state object. */
	virtual deBaseScriptingNetworkState *CreateNetworkState( deNetworkState *state );
	/** \brief Creates a peer for the given touch sensor object. */
	virtual deBaseScriptingTouchSensor *CreateTouchSensor( deTouchSensor *touchSensor );
	/** \brief Creates a peer for the given prop field object. */
	virtual deBaseScriptingPropField *CreatePropField( dePropField *propField );
	
	/** \brief Create peer for the given particle emitter instance object or NULL if not used. */
	virtual deBaseScriptingParticleEmitterInstance *CreateParticleEmitterInstance(
		deParticleEmitterInstance *instance );
	
	/** \brief Create deSoundLevelMeter peer. */
	virtual deBaseScriptingSoundLevelMeter *CreateSoundLevelMeter( deSoundLevelMeter *meter );
	
	/** \brief Create deSpeaker peer. */
	virtual deBaseScriptingSpeaker *CreateSpeaker( deSpeaker *speaker );
	
	/**
	 * \brief Initializes the game scripts.
	 * \details This usually involves creating the game objects and calling
	 *          init functions on them.
	 */
	virtual bool InitGame();
	/**
	 * \brief Exits the game scripts.
	 * \details This usually involves calling exit functions and freeing game objects.
	 */
	virtual bool ExitGame();
	/**
	 * \brief Called at the beginning of each update cycle.
	 * \details This gives the game scripts the chance to do frame update related tasks
	 *          that do not involve updating the screen. You can update skins or offscreen
	 *          render targets but you should not update the screen itself.
	 * \returns true if the call has been successfull or false otherwise
	 */
	virtual bool OnFrameUpdate();
	/** \brief Called after the render window changed size. */
	virtual bool OnResizeRenderWindow();
	/**
	 * \brief Send the given event to game scripts.
	 * \returns true if the call has been successfull or false otherwise
	 */
	virtual bool SendEvent( deInputEvent *event );
	
	/** \brief Python cleanup. */
	void PythonCleanUp();
	
	/** \brief Retrieves the singleton script module. */
	static ScriptingPython *GetSP();
	
	/** \brief Calls a function without any arguments. */
	bool CallFunction( const char *functionName );
	
	/** \brief Add python exception to the module trace. */
	void SetErrorTracePython();
	
	/** \brief Log python exception. */
	void LogExceptionPython();
	
	/** \brief Retrieve a namespace. */
	inline spBaseModule *GetNamespaceDragengine() const{ return pNamespaceDragengine; }
	inline spBaseModule *GetNamespaceScenery() const{ return pNamespaceScenery; }
	inline spBaseModule *GetNamespaceGui() const{ return pNamespaceGui; }
	
	/** \brief Retrieve a type. */
	inline spTypeModuleLoader *GetTypeModuleLoader() const{ return pTypeModuleLoader; }
	inline spModuleEngine *GetModuleEngine() const{ return pModuleEngine; }
	inline spModuleGraphicSystem *GetModuleGraphicSystem() const{ return pModuleGraphicSystem; }
	inline spTypeCanvas *GetTypeCanvas() const{ return pTypeCanvas; }
	inline spTypeWorld *GetTypeWorld() const{ return pTypeWorld; }
	
	/** \brief Retrieves the script directory. */
	inline const decString &GetScriptPath() const{ return pScriptPath; }
	
	/** \brief Retrieves the list of script files. */
	inline spScriptFileList &GetScriptFileList() const{ return *pScriptFileList; }
	/*@}*/
	
private:
	void pCreateModules();
	void pLoadGameScript( const char *directory, const char *gameClass );
};

#endif
