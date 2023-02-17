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
#include "../framebuffer/deoglFramebuffer.h"
#include "../framebuffer/deoglFramebufferManager.h"

class deoglFramebuffer;
class deoglFramebufferManager;
class deoglRenderThread;



/**
 * Render thread framebuffer related objects.
 */
class deoglRTFramebuffer{
private:
	deoglFramebufferManager pManager;
	
	deoglFramebuffer *pActive;
	
	deoglFramebuffer pPrimary;
	
	deoglFramebuffer pEnvMap;
	deoglFramebuffer pEnvMapMaterial;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render thread framebuffer related object. */
	deoglRTFramebuffer( deoglRenderThread &renderThread );
	
	/** Clean up render thread framebuffer related object. */
	virtual ~deoglRTFramebuffer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Framebuffer manager. */
	inline deoglFramebufferManager &GetManager(){ return pManager; }
	inline const deoglFramebufferManager &GetManager() const{ return pManager; }
	
	/** Primary framebuffer. */
	inline deoglFramebuffer &GetPrimary(){ return pPrimary; }
	inline const deoglFramebuffer &GetPrimary() const{ return pPrimary; }
	
	/** Active framebuffer. */
	inline deoglFramebuffer *GetActive() const{ return pActive; }
	
	/** Activate framebuffer or \em NULL to activate the primary framebuffer. */
	void Activate( deoglFramebuffer *framebuffer );
	
	/** Environment map framebuffer. */
	inline deoglFramebuffer &GetEnvMap(){ return pEnvMap; }
	inline const deoglFramebuffer &GetEnvMap() const{ return pEnvMap; }
	
	/** Environment material map framebuffer. */
	inline deoglFramebuffer &GetEnvMapMaterial(){ return pEnvMapMaterial; }
	inline const deoglFramebuffer &GetEnvMapMaterial() const{ return pEnvMapMaterial; }
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
