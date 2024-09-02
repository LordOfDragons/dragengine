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

#ifndef _SCRIPTINGSMALLTALK_H_
#define _SCRIPTINGSMALLTALK_H_

#include "stCommon.h"

#include <dragengine/systems/modules/scripting/deBaseScriptingModule.h>

class stClassCanvas;
class stClassColor;
class stClassEngine;
class stClassGraphicSystem;
class stClassPoint;
class stClassScripting;
class stClassVector2;
class stClassWorld;

class deVirtualFileSystem;



/**
 * \brief Smalltalk scripting module.
 */
class ScriptingSmalltalk : public deBaseScriptingModule{
private:
	OOP pObjNil;
	OOP pClsCharacter;
	OOP pClsOrderedCollection;
	OOP pSelectorNew;
	OOP pSelectorValue;
	OOP pSelectorAdd;
	OOP pSelectorInitialize;
	OOP pSelectorAt;
	
	OOP pSelectorInitGame;
	OOP pSelectorCleanUp;
	OOP pSelectorOnFrameUpdate;
	OOP pSelectorOnResizeRenderWindow;
	OOP pSelectorEventKeyPress;
	OOP pSelectorEventKeyRelease;
	OOP pSelectorEventKeyType;
	OOP pSelectorEventMousePress;
	OOP pSelectorEventMouseRelease;
	OOP pSelectorEventMouseMove;
	
	OOP pClsGame;
	OOP pObjGame;
	
	OOP pClsDragengine;
	OOP pClsScenery;
	OOP pClsGui;
	
	stClassCanvas *pClsCanvas;
	stClassColor *pClsColor;
	stClassEngine *pClsEngine;
	stClassGraphicSystem *pClsGraphicSystem;
	stClassScripting *pClsScripting;
	stClassVector2 *pClsVector2;
	stClassPoint *pClsPoint;
	stClassWorld *pClsWorld;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create scripting module. */
	ScriptingSmalltalk( deLoadableModule &loadableModule );
	
	/** \brief Clean up scripting module. */
	virtual ~ScriptingSmalltalk();
	/*@}*/
	
	
	
	/** \name Management */
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
	 * \brief Prepare scripting module to operate the game.
	 * \details Checks the specified script directory to contain suitable scripts
	 *          and allocates needed objects or memory. A startable package usually
	 *          needs certain classes or functions to be present to be startable.
	 *          See the individual manuals of the scripting modules to learn about
	 *          those requirements. After the Init function has exited the scripting
	 *          module is ready to operate the game.
	 */
	virtual bool Init( const char *scriptDirectory, const char *gameObject );
	
	/** \brief Shut down the scripting module and frees all resources. */
	virtual void ShutDown();
	
	
	
	/** \brief Create peer for the given collider object. */
	virtual deBaseScriptingCollider *CreateCollider( deCollider *collider );
	
	/** \brief Create peer for the given server object. */
	virtual deBaseScriptingServer *CreateServer( deServer *server );
	
	/** \brief Create peer for the given connection object. */
	virtual deBaseScriptingConnection *CreateConnection( deConnection *connection );
	
	/** \brief Create peer for the given network state object. */
	virtual deBaseScriptingNetworkState *CreateNetworkState( deNetworkState *state );
	
	/** \brief Create peer for the given touch sensor object. */
	virtual deBaseScriptingTouchSensor *CreateTouchSensor( deTouchSensor *touchSensor );
	
	/** \brief Create peer for the given prop field object. */
	virtual deBaseScriptingPropField *CreatePropField( dePropField *propField );
	
	/** \brief Create peer for the given particle emitter instance object or NULL if not used. */
	virtual deBaseScriptingParticleEmitterInstance *CreateParticleEmitterInstance(
		deParticleEmitterInstance *instance );
	
	/** \brief Create deSoundLevelMeter peer. */
	virtual deBaseScriptingSoundLevelMeter *CreateSoundLevelMeter( deSoundLevelMeter *meter );
	
	/** \brief Create deSpeaker peer. */
	virtual deBaseScriptingSpeaker *CreateSpeaker( deSpeaker *speaker );
	
	
	
	/**
	 * \brief Initialize game scripts.
	 * \details This usually involves creating the game objects and calling init
	 *          functions on them.
	 */
	virtual bool InitGame();
	
	/**
	 * \brief Exit the game scripts.
	 * \details This usually involves calling exit functions and freeing game objects.
	 */
	virtual bool ExitGame();
	
	/**
	 * \brief Called at the beginning of each update cycle.
	 * \details This gives the game scripts the chance to do frame update related tasks
	 *          that do not involve updating the screen. You can update skins or offscreen
	 *          render targets but you should not update the screen itself.
	 * \return true if the call has been successfull or false otherwise
	 */
	virtual bool OnFrameUpdate();
	
	/**
	 * \brief Called after the render window changed size.
	 */
	virtual bool OnResizeRenderWindow();
	
	/**
	 * \brief Send the given event to game scripts.
	 * \return true if the call has been successfull or false otherwise
	 */
	virtual bool SendEvent( deInputEvent *event );
	
	
	
	/** \brief Smalltalk nil object. */
	inline OOP GetNil() const{ return pObjNil; }
	
	/** \brief Smalltalk ordered collection class. */
	inline OOP GetClassOrderedCollection() const{ return pClsOrderedCollection; }
	
	/** \brief Game object. */
	inline OOP GetObjectGame() const{ return pObjGame; }
	
	
	
	/** \brief Send message to game object. */
	bool SendSTMessage( OOP selector );
	
	/** \brief Create object for a class. */
	OOP CreateNewObject( OOP stclass );
	
	/**
	 * \brief Create object for a given class with specific instance size.
	 */
	OOP CreateNewObject( OOP stclass, int instVarBytes );
	
	/**
	 * \brief Create object for the a class with specific instance size and sending 'initialize'.
	 */
	OOP CreateNewObjectWithInit( OOP stclass, int instVarBytes );
	
	
	
	/** \brief Script classes. */
	inline void *GetClassDragengine() const{ return pClsDragengine; }
	inline void *GetClassScenery() const{ return pClsScenery; }
	inline void *GetClassGui() const{ return pClsGui; }
	
	inline stClassCanvas *GetClassCanvas() const{ return pClsCanvas; }
	inline stClassColor *GetClassColor() const{ return pClsColor; }
	inline stClassEngine *GetClassEngine() const{ return pClsEngine; }
	inline stClassGraphicSystem *GetClassGraphicSystem() const{ return pClsGraphicSystem; }
	inline stClassScripting *GetClassScripting() const{ return pClsScripting; }
	inline stClassVector2 *GetClassVector2() const{ return pClsVector2; }
	inline stClassPoint *GetClassPoint() const{ return pClsPoint; }
	inline stClassWorld *GetClassWorld() const{ return pClsWorld; }
	
	/** \brief Common selectors. */
	inline OOP GetSelectorNew() const{ return pSelectorNew; }
	inline OOP GetSelectorAdd() const{ return pSelectorAdd; }
	inline OOP GetSelectorInitialize() const{ return pSelectorInitialize; }
	inline OOP GetSelectorAt() const{ return pSelectorAt; }
	
	/** \brief Load scripts from a directory. */
	bool LoadScripts( deVirtualFileSystem &vfs, const char *directory );
	/*@}*/
	
private:
	OOP pGetSystemClassByName( const char *name );
	OOP pGetSystemSymbolByName( const char *name );
	
	void pObtainSTStuff();
	void pCreateClasses();
	
	bool pLoadGameScripts( const char *directory, const char *gameClass );
	void pAddScripts( deVirtualFileSystem &vfs, const char *path );
	bool pIsFileType( const char *filename, const char *extension );
};

#endif
