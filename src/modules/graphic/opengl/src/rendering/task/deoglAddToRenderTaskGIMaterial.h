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

#ifndef _DEOGLADDTORENDERTASKGIMATERIAL_H_
#define _DEOGLADDTORENDERTASKGIMATERIAL_H_

#include "../../deoglBasics.h"
#include "../../skin/deoglSkinTexture.h"

#include <dragengine/common/math/decMath.h>

class deoglRComponentLOD;
class deoglRenderTask;
class deoglRenderTaskTexture;
class deoglRenderThread;
class deoglTexUnitsConfig;


/**
 * Add elements to render task.
 * 
 * Provides support for filtering the elements using texture properties.
 */
class deoglAddToRenderTaskGIMaterial{
private:
	deoglRenderThread &pRenderThread;
	
	deoglRenderTask &pRenderTask;
	deoglSkinTexturePipelines::eTypes pSkinPipelineType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create new add to render task. */
	deoglAddToRenderTaskGIMaterial( deoglRenderThread &renderThread, deoglRenderTask &renderTask );
	
	/** Clean up add to render task . */
	~deoglAddToRenderTaskGIMaterial();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Pipeline type. */
	inline deoglSkinTexturePipelines::eTypes GetSkinPipelineType() const{ return pSkinPipelineType; }
	
	/** Set pipeline type. */
	void SetSkinPipelineType( deoglSkinTexturePipelines::eTypes type );
	
	
	
	/** Reset render task parameters. */
	void Reset();
	
	
	
	/**
	 * Add component texture.
	 * 
	 * Component is supposed to be updated, and model, skin and parent world exist.
	 */
	deoglRenderTaskTexture *AddComponentTexture( deoglRComponentLOD &component, int texture );
	/*@}*/
	
	
	
private:
	bool pFilterReject( const deoglSkinTexture *skinTexture ) const;
	
	deoglRenderTaskTexture *pGetTaskTexture( deoglSkinTexture *skinTexture, deoglTexUnitsConfig *tuc );
};

#endif
