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

#ifndef _IGDEENGINECONTROLLER_H_
#define _IGDEENGINECONTROLLER_H_

#include <dragengine/systems/deModuleSystem.h>

class deEngine;
class deLoadableModule;
class igdeMainWindow;
class deRenderWindow;
class igdeGameProject;
class deInternalModule;
class igdeWidget;


/**
 * \brief Engine Controller.
 * Handles a game engine instance.
 */
class DE_DLL_EXPORT igdeEngineController{
public:
	/** \brief List of systems. */
	enum eSystems{
		esCrashRecovery,
		esGraphic,
		esAudio,
		esPhysics,
		esNetwork,
		esInput,
		esAnimator,
		esAI,
		esSynthesizer,
		esVR,
		esScripting
	};
	
	
	
private:
	igdeMainWindow &pMainWindow;
	deEngine *pEngine;
	deRenderWindow *pMainRenderWindow;
	bool pReadyForUse;
	bool pRunning;
	int pRenderCounter;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create engine controller. */
	igdeEngineController( igdeMainWindow &mainWindow );
	
	/** \brief Clean up engine controller. */
	~igdeEngineController();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	
	/** \brief Main window. */
	inline igdeMainWindow &GetMainWindow() const{ return pMainWindow; }
	
	/** \brief Initializes the engine. */
	void InitEngine();
	
	/** \brief Shuts down the engine. */
	void CloseEngine();
	
	/** \brief Resizes the game screen. */
	void ResizeWindow( int width, int height );
	
	/** \brief Create render window. */
	deRenderWindow *CreateRenderWindow();
	
	/** \brief Create render window attached to the given host window. */
	deRenderWindow *CreateRenderWindow( igdeWidget &hostWindow );
	
	/** \brief Unparent main render window. */
	void UnparentMainRenderWindow();
	
	/** \brief Update engine virtual file system using game definition if present. */
	void UpdateEngine( const igdeGameProject *gameProject,
		const char *pathIGDEData, const char *pathIGDEModuleData );
	
	/** \brief Adds an internal module to the engine. */
	void AddInternalModule( deInternalModule *module );
	
	/** \brief Activates a module by name. */
	void ActivateModule( int system, const char *name );
	
	/** \brief Engine is running. */
	inline bool GetRunning() const{ return pRunning; }
	
	/** \brief Start engine. */
	void StartEngine();
	
	/** \brief Stop engine. */
	void StopEngine();
	/*@}*/
	
	
	
private:
	void pConfigModules();
	
	void pCreateMainRenderWindow();
	void pDestroyMainRenderWindow();
	deLoadableModule *GetBestModuleForType( deModuleSystem::eModuleTypes moduleType ) const;
};

#endif
