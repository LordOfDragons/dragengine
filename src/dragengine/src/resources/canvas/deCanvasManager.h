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

#ifndef _DECANVASMANAGER_H_
#define _DECANVASMANAGER_H_

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deCanvas;
class deCanvasImage;
class deCanvasPaint;
class deCanvasCanvasView;
class deCanvasRenderWorld;
class deCanvasText;
class deCanvasVideoPlayer;
class deCanvasView;
class deEngine;


/**
 * \brief Canvas resource manager
 */
class DE_DLL_EXPORT deCanvasManager : public deResourceManager{
private:
	deResourceList pCanvas;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create canvas resource manager. */
	deCanvasManager( deEngine *engine );
	
	/** \brief Clean up canvas resource manager and reports leaking resources. */
	virtual ~deCanvasManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of canvas. */
	int GetCanvasCount() const;
	
	/** \brief Root canvas resource for iteration purpose. */
	deCanvas *GetRootCanvas() const;
	
	/** \brief Create image canvas. */
	deCanvasImage *CreateCanvasImage();
	
	/** \brief Create paint canvas. */
	deCanvasPaint *CreateCanvasPaint();
	
	/** \brief Create canvas view canvas. */
	deCanvasCanvasView *CreateCanvasCanvasView();
	
	/** \brief Create render world canvas. */
	deCanvasRenderWorld *CreateCanvasRenderWorld();
	
	/** \brief Create text canvas. */
	deCanvasText *CreateCanvasText();
	
	/** \brief Create video player canvas. */
	deCanvasVideoPlayer *CreateCanvasVideoPlayer();
	
	/** \brief Create view canvas. */
	deCanvasView *CreateCanvasView();
	
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
