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

#ifndef _DEOGLRENDERTASKPIPELINE_H_
#define _DEOGLRENDERTASKPIPELINE_H_

#include <dragengine/common/collection/decPointerList.h>

class deoglPipeline;
class deoglRenderTaskSharedTexture;
class deoglRenderTaskTexture;



/**
 * Render Task Pipeline.
 */
class deoglRenderTaskPipeline{
private:
	const deoglPipeline *pPipeline;
	
	decPointerList pTextures;
	int pTextureCount;
	
	deoglRenderTaskTexture **pHasTexture;
	int pHasTextureCount;
	int pHasTextureSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render task pipeline. */
	deoglRenderTaskPipeline();
	
	/** Clean up render task pipeline. */
	~deoglRenderTaskPipeline();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Reset pipeline. */
	void Reset();
	
	/** Total amount of points in this pipeline. */
	int GetTotalPointCount() const;
	
	/** Total amount of vaos in this pipeline. */
	int GetTotalVAOCount() const;
	
	/** Total amount of instances in this pipeline. */
	int GetTotalInstanceCount() const;
	
	/** Total amount of subinstances in this pipeline. */
	int GetTotalSubInstanceCount() const;
	
	/** Pipeline. */
	inline const deoglPipeline *GetPipeline() const{ return pPipeline; }
	
	/** Set pipeline. */
	void SetPipeline(const deoglPipeline *pipeline);
	
	/** Number of render task textures. */
	inline int GetTextureCount() const{ return pTextureCount; }
	
	/** Render task texture at index. */
	deoglRenderTaskTexture *GetTextureAt(int index) const;
	
	/** Add render task texture. */
	deoglRenderTaskTexture *AddTexture(const deoglRenderTaskSharedTexture *texture);
	/*@}*/
};

#endif
