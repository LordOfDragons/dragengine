/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLPRELOADER_H_
#define _DEOGLPRELOADER_H_

#include <dragengine/common/collection/decPointerSet.h>

class deoglRenderThread;



/**
 * @brief Preload OpenGL objects.
 * This class supports preloading various objects so they can be used for rendering. This includes
 * objects which have to be prepared before the first rendering takes place to avoid stalling the
 * gpu driver. This is different from rendering specific preparation routines which can be done
 * during rendering without stalling the gpu driver. One type of objects included in this class
 * are resources created during the frame update loop. The opengl objects linked to these resources
 * can not be created safely and efficiently outside the frame rendering call. Hence the creation
 * of opengl objects is delayed until right before rendering as then the opengl context is in a
 * safe state and creation calls do not stall the gpu driver.
 */
class deoglPreloader{
private:
	deoglRenderThread &pRenderThread;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new preloader. */
	deoglPreloader( deoglRenderThread &renderThread );
	/** Cleans up the preloader. */
	~deoglPreloader();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Preload all objects. */
	void PreloadAll();
	/*@}*/
};

#endif
