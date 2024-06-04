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
