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

#ifndef _DEOGLRENDERTASKSHAREDPOOL_H_
#define _DEOGLRENDERTASKSHAREDPOOL_H_

#include <dragengine/common/collection/decPointerList.h>

class deoglRenderThread;
class deoglRenderTaskSharedInstance;
class deoglRenderTaskSharedTexture;
class deoglRenderTaskSharedVAO;


/**
 * GLobal pool for render task shared resources.
 */
class deoglRenderTaskSharedPool{
private:
	deoglRenderThread &pRenderThread;
	
	int pNextIndexTexture;
	int pNextIndexVAO;
	int pNextIndexInstance;
	
	decPointerList pTextures;
	decPointerList pVAOs;
	decPointerList pInstances;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render task shared pool. */
	deoglRenderTaskSharedPool( deoglRenderThread &renderThread );
	
	/** Clean up render task shared pool. */
	~deoglRenderTaskSharedPool();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Get texture. */
	deoglRenderTaskSharedTexture *GetTexture();
	
	/** Get VAO. */
	deoglRenderTaskSharedVAO *GetVAO();
	
	/** Get instance. */
	deoglRenderTaskSharedInstance *GetInstance();
	
	
	
	/** Return texture. */
	void ReturnTexture( deoglRenderTaskSharedTexture *texture );
	
	/** Return VAO. */
	void ReturnVAO( deoglRenderTaskSharedVAO *vao );
	
	/** Return instance. */
	void ReturnInstance( deoglRenderTaskSharedInstance *instance );
	/*@}*/
};

#endif
