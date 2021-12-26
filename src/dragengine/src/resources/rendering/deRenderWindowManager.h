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

#ifndef _DERENDERWINDOWMANAGER_H_
#define _DERENDERWINDOWMANAGER_H_ 

#include "../../dragengine_configuration.h"

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deRenderWindow;


/**
 * \brief Render Window Resource Manager.
 */
class DE_DLL_EXPORT deRenderWindowManager : public deResourceManager{
private:
	deResourceList pRenderWindows;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render window resource manager. */
	deRenderWindowManager( deEngine *engine );
	
	/** \brief Clean up render window resource manager and reports leaking resources. */
	virtual ~deRenderWindowManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of render windows. */
	int GetRenderWindowCount() const;
	
	/** \brief Root render window resource. */
	deRenderWindow *GetRootRenderWindow() const;
	
	/** \brief Create top level render window. */
	deRenderWindow *CreateRenderWindow();
	
	/** \brief Create render window inside a host window. */
	#ifdef ANDROID
	deRenderWindow *CreateRenderWindowInside( void *window );
	#endif
	#ifdef OS_BEOS
	deRenderWindow *CreateRenderWindowInside( BWindow *window );
	#endif
	#if defined OS_UNIX && defined HAS_LIB_X11
	deRenderWindow *CreateRenderWindowInside( Window window );
	#endif
	#ifdef OS_W32
	deRenderWindow *CreateRenderWindowInside( HWND window );
	#endif
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/** \brief Graphic System Peers of all stored resources have to be created. */
	virtual void SystemGraphicLoad();
	
	/** \brief Graphic System Peers of all stored resources have to be freed. */
	virtual void SystemGraphicUnload();
	/*@}*/
	
	
	
	/** \name Resource only Functions */
	/*@{*/
	/**
	 * \brief Remove resource.
	 * 
	 * For use by deRenderWindow only.
	 */
	void RemoveResource( deResource *resource );
	/*@}*/
};

#endif
