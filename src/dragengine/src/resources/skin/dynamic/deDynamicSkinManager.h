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

#ifndef _DEDYNAMICSKINMANAGER_H_
#define _DEDYNAMICSKINMANAGER_H_

#include "../../deResourceManager.h"
#include "../../deResourceList.h"

class deDynamicSkin;


/**
 * \brief Dynamic Skin Resource Manager.
 *
 * Manages dynamic skin resources.
 */
class deDynamicSkinManager : public deResourceManager{
private:
	deResourceList pSkins;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new manager. */
	deDynamicSkinManager( deEngine *engine );
	
	/** \brief Clean up manager. */
	virtual ~deDynamicSkinManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of dynamic skins. */
	int GetDynamicSkinCount() const;
	
	/** \brief Root dynamic skin resource for iteration purpose. */
	deDynamicSkin *GetRootDynamicSkin() const;
	
	/** \brief Create new dynamic skin. */
	deDynamicSkin *CreateDynamicSkin();
	
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
