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

#ifndef _DEBILLBOARDMANAGER_H_
#define _DEBILLBOARDMANAGER_H_ 

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deBillboard;


/**
 * \brief Billboard Resource Manager.
 */
class DE_DLL_EXPORT deBillboardManager : public deResourceManager{
private:
	deResourceList pBillboards;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new billboard resource manager. */
	deBillboardManager( deEngine *engine );
	
	/** \brief Clean up billboard resource manager. */
	virtual ~deBillboardManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of billboards. */
	int GetBillboardCount() const;
	
	/** \brief Root billboard for iteration purpose. */
	deBillboard *GetRootBillboard() const;
	
	/** \brief Create new billboard. */
	deBillboard *CreateBillboard();
	
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
