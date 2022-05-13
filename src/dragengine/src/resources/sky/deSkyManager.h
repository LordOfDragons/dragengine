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

#ifndef _DESKYMANAGER_H_
#define _DESKYMANAGER_H_ 

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deSky;


/**
 * \brief Sky Resource Manager.
 *
 */
class DE_DLL_EXPORT deSkyManager : public deResourceManager{
private:
	deResourceList pSkies;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new scene sky resource manager linked to the given engine. */
	deSkyManager( deEngine *engine );
	
	/** \brief Clean up scene sky resource manager and reports leaking resources. */
	~deSkyManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of scene skies. */
	int GetSkyCount() const;
	
	/** \brief Root sky resource for iteration purpose. */
	deSky *GetRootSky() const;
	
	/** \brief Create new scene sky. */
	deSky *CreateSky();
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	void SystemGraphicLoad();
	void SystemGraphicUnload();
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * Those functions are only for resource objects and should never be
	 * called directly from an application.
	 */
	/*@{*/
	void RemoveResource( deResource *resource );
	/*@}*/
};

#endif
