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

#ifndef _DECAPTURECANVASMANAGER_H_
#define _DECAPTURECANVASMANAGER_H_

#include "../../deResourceManager.h"
#include "../../deResourceList.h"

class deCaptureCanvas;
class deEngine;


/**
 * \brief Capture canvas resource manager
 */
class deCaptureCanvasManager : public deResourceManager{
private:
	deResourceList pCaptureCanvas;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create capture canvas resource manager. */
	deCaptureCanvasManager( deEngine *engine );
	
	/** \brief Clean up capture canvas resource manager and reports leaking resources. */
	virtual ~deCaptureCanvasManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of capture canvas. */
	int GetCaptureCanvasCount() const;
	
	/** \brief Root capture canvas resource for iteration purpose. */
	deCaptureCanvas *GetRootCaptureCanvas() const;
	
	/** \brief Create capture canvas. */
	deCaptureCanvas *CreateCaptureCanvas();
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/** \brief Graphic system peers of all stored resources have to be created. */
	virtual void SystemGraphicLoad();
	
	/** \brief Graphic system peers of all stored resources have to be freed. */
	virtual void SystemGraphicUnload();
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * 
	 * Those functions are only for resource objects and should never be
	 * called directly from an application.
	 */
	/*@{*/
	void RemoveResource( deResource *resource );
	/*@}*/
};

#endif
