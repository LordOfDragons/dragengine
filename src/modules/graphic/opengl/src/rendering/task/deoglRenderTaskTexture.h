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

#ifndef _DEOGLRENDERTASKTEXTURE_H_
#define _DEOGLRENDERTASKTEXTURE_H_

#include "../../deoglBasics.h"

#include <dragengine/common/collection/decPointerList.h>

class deoglRenderTaskSharedTexture;
class deoglRenderTaskSharedVAO;
class deoglRenderTaskVAO;


/**
 * Render Task Texture.
 */
class deoglRenderTaskTexture{
private:
	const deoglRenderTaskSharedTexture *pTexture;
	
	decPointerList pVAOs;
	int pVAOCount;
	
	deoglRenderTaskVAO **pHasVAO;
	int pHasVAOCount;
	int pHasVAOSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render task shader. */
	deoglRenderTaskTexture();
	
	/** Clean up render task shader. */
	~deoglRenderTaskTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Reset. */
	void Reset();
	
	/** Total count of points. */
	int GetTotalPointCount() const;
	
	/** Total count of instances. */
	int GetTotalInstanceCount() const;
	
	/** Total count of subinstances. */
	int GetTotalSubInstanceCount() const;
	
	
	
	/** Shared texture. */
	inline const deoglRenderTaskSharedTexture *GetTexture() const{ return pTexture; }
	
	/** Set shared texture. */
	void SetTexture( const deoglRenderTaskSharedTexture *texture );
	
	
	
	/** Count of render task vaos. */
	inline int GetVAOCount() const{ return pVAOCount; }
	
	/** Render task vao at index. */
	deoglRenderTaskVAO *GetVAOAt( int index ) const;
	
	/** Add render task vao. */
	deoglRenderTaskVAO *AddVAO( const deoglRenderTaskSharedVAO *vao );
	/*@}*/
};

#endif
