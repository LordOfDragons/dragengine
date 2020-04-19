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

#ifndef _DEOGLRRTFRAMEBUFFER_H_
#define _DEOGLRRTFRAMEBUFFER_H_

#include "../deoglBasics.h"

class deoglFramebuffer;
class deoglFramebufferManager;
class deoglRenderThread;



/**
 * \brief Render thread framebuffer related objects.
 */
class deoglRTFramebuffer{
private:
	deoglFramebufferManager *pManager;
	
	deoglFramebuffer *pPrimary;
	deoglFramebuffer *pActive;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render thread framebuffer related object. */
	deoglRTFramebuffer( deoglRenderThread &renderThread );
	
	/** \brief Clean up render thread framebuffer related object. */
	virtual ~deoglRTFramebuffer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Framebuffer manager. */
	inline deoglFramebufferManager &GetManager() const{ return *pManager; }
	
	/** \brief Primary framebuffer. */
	inline deoglFramebuffer *GetPrimary() const{ return pPrimary; }
	
	/** \brief Active framebuffer. */
	inline deoglFramebuffer *GetActive() const{ return pActive; }
	
	/** \brief Activate framebuffer or \em NULL to activate the primary framebuffer. */
	void Activate( deoglFramebuffer *framebuffer );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
