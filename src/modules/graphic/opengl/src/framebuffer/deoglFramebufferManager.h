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

// include only once
#ifndef _DEOGLFRAMEBUFFERMANAGER_H_
#define _DEOGLFRAMEBUFFERMANAGER_H_

// predefinitions
class deoglRenderThread;
class deoglFramebuffer;



/**
 * @brief Framebuffer manager.
 * Manages framebuffers. Hands out framebuffers of a given size creating
 * or reusing framebuffers to keep one framebuffer for each resolution
 * asked for. This avoids slowdown due to changing the resolution of
 * framebuffer attachments. On some systems the opposite is true though.
 * If the use only one FBO flag is set only one FBO is created and handed
 * out.
 */
class deoglFramebufferManager{
private:
	deoglRenderThread &pRenderThread;
	
	deoglFramebuffer **pFBOs;
	int pFBOCount;
	int pFBOSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new framebuffer manager. */
	deoglFramebufferManager( deoglRenderThread &renderThread );
	/** Cleans up the framebuffer manager. */
	~deoglFramebufferManager();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of framebuffers. */
	inline int GetFBOCount() const{ return pFBOCount; }
	/** Retrieves the framebuffer for reading only at the given location. */
	const deoglFramebuffer *GetFBOAt( int index ) const;
	
	/** Retrieves the framebuffer for the given resolution. */
	deoglFramebuffer *GetFBOWithResolution( int width, int height );
	/*@}*/
};

// end of include only once
#endif
