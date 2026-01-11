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

#ifndef _IGDENULLSCRIPTMODULE_H_
#define _IGDENULLSCRIPTMODULE_H_

#include <dragengine/systems/modules/deInternalModule.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingModule.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>
#include <dragengine/common/math/decMath.h>



/**
 * \brief nullptr Script Module.
 */
class DE_DLL_EXPORT igdeNullScriptModule : public deBaseScriptingModule{
public:
	class DE_DLL_EXPORT cModule : public deInternalModule{
	public:
		using Ref = deTObjectReference<cModule>;
		cModule(deModuleSystem *system);
		
	protected:
		~cModule() override;
		
	public:
		void CreateModule() override;
	};
	
	
	
private:
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	igdeNullScriptModule(deLoadableModule &loadableModule);
	
	/** \brief Clean up module. */
	~igdeNullScriptModule() override;
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/**
	 * \brief Directory relative to the module share directory where shared module data is located.
	 * 
	 * \returns nullptr if no such directory is used.
	 */
	const char *GetSharedDataDir() const override;
	
	/**
	 * \brief VFS directory where the module share directory is shown at.
	 * 
	 * \returns nullptr if no such directory is used.
	 */
	const char *GetVFSSharedDataDir() const override;
	
	/**
	 * Prepares the scripting module to operate the game by checking the
	 * specified script directory to contain suitable scripts and
	 * allocates needed objects or memory. A startable package usually
	 * needs certain classes or functions to be present to be startable.
	 * See the individual manuals of the scripting modules to learn about
	 * those requirements. After the Init function has exited the scripting
	 * module is ready to operate the game.
	 */
	bool Init(const char *scriptDirectory, const char *gameObject) override;
	
	/** \brief Shut down scripting module and frees all resources. */
	void ShutDown() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create peer for the given collider object. */
	deBaseScriptingCollider *CreateCollider(deCollider *collider) override;
	
	/** \brief Create peer for the given server object. */
	deBaseScriptingServer *CreateServer(deServer *server) override;
	
	/** \brief Create peer for the given connection object. */
	deBaseScriptingConnection *CreateConnection(deConnection *connection) override;
	
	/** \brief Create peer for the given network state object. */
	deBaseScriptingNetworkState *CreateNetworkState(deNetworkState *state) override;
	
	/** \brief Create peer for the given touch sensor object. */
	deBaseScriptingTouchSensor *CreateTouchSensor(deTouchSensor *touchSensor) override;
	
	/** \brief Create peer for the given prop field object. */
	deBaseScriptingPropField *CreatePropField(dePropField *propField) override;
	
	/** \brief Create peer for the given particle emitter instance object or nullptr if not used. */
	deBaseScriptingParticleEmitterInstance *CreateParticleEmitterInstance(
		deParticleEmitterInstance *instance) override;
	
	/** \brief Create deSoundLevelMeter peer. */
	deBaseScriptingSoundLevelMeter *CreateSoundLevelMeter(deSoundLevelMeter *meter) override;
	
	/** \brief Create deSpeaker peer. */
	deBaseScriptingSpeaker *CreateSpeaker(deSpeaker *speaker) override;
	
	/**
	 * \brief Initialize game scripts.
	 * 
	 * This usually involves creating the game objects and calling init functions on them.
	 */
	bool InitGame() override;
	
	/**
	 * \brief Exit game scripts.
	 * 
	 * This usually involves calling exit functions and freeing game objects.
	 */
	bool ExitGame() override;
	
	/**
	 * \brief Called at the beginning of each update cycle.
	 * 
	 * This gives the game scripts the chance to do frame update related tasks that do not
	 * involve updating the screen. You can update skins or offscreen render targets but
	 * you should not update the screen itself.
	 * 
	 * \return true if the call has been successfull or false otherwise
	 */
	bool OnFrameUpdate() override;
	
	/**
	 * \brief Called after the render window changed size.
	 */
	bool OnResizeRenderWindow() override;
	
	/**
	 * \brief Send the given event to game scripts.
	 * \return true if the call has been successfull or false otherwise
	 */
	bool SendEvent(deInputEvent *event) override;
	
	/**
	 * \brief User requested window to be closed.
	 * 
	 * Default implementation calls deEngine.Quit().
	 */
	void UserRequestQuit() override;
	/*@}*/
};

#endif
