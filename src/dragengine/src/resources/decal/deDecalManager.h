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

#ifndef _DEDECALMANAGER_H_
#define _DEDECALMANAGER_H_

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deDecal;


/**
 * \brief Decal Resourse Manager.
 *
 */
class DE_DLL_EXPORT deDecalManager : public deResourceManager{
private:
	deResourceList pDecals;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new decal resource manager linked to the given engine. */
	deDecalManager( deEngine *engine );
	
	/** \brief Clean up decal resource manager and reports leaking resources. */
	virtual ~deDecalManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of decals. */
	int GetDecalCount() const;
	
	/** \brief Root decal resource for iteration purpose. */
	deDecal *GetRootDecal() const;
	
	/** \brief Create new and empty decal. */
	deDecal *CreateDecal();
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	void SystemGraphicLoad();
	void SystemGraphicUnload();
	void SystemPhysicsLoad();
	void SystemPhysicsUnload();
	void SystemAudioLoad();
	void SystemAudioUnload();
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
