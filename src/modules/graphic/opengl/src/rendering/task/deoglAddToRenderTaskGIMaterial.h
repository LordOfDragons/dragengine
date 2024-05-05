/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
