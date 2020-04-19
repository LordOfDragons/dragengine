/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEENGINECONTROLLER_H_
#define _IGDEENGINECONTROLLER_H_

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
class igdeEngineController{
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
	
	/** \brief Updates the engine using the given game definition. */
	void UpdateEngine( const igdeGameProject &gameProject,
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
};

#endif
