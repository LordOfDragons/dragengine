/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLDYNAMICSKINLISTENER_H_
#define _DEOGLDYNAMICSKINLISTENER_H_

class deoglDynamicSkin;
class deoglDSRenderable;


/**
 * Dynamic skin listener.
 * \note Called from main thread during synchronization.
 */
class deoglDynamicSkinListener{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create dynamic skin listener. */
	deoglDynamicSkinListener();
	
	/** Clean up dynamic skin listener. */
	virtual ~deoglDynamicSkinListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/**
	 * Dynamic skin has been destroyed. Owner has to drop weak reference to dynamic skin.
	 */
	virtual void DynamicSkinDestroyed();
	
	/** Rendeable added, removed or renamed. */
	virtual void DynamicSkinRenderablesChanged();
	
	/** Renderable changed. Typically requires texture configuration update. */
	virtual void DynamicSkinRenderableChanged( deoglDSRenderable &renderable );
	
	/** Rendeable content requires sync. */
	virtual void DynamicSkinRenderableRequiresSync( deoglDSRenderable &renderable );
	/*@}*/
};

#endif
