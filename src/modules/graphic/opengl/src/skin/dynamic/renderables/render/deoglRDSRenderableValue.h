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

#ifndef _DEOGLRRDSRENDERABLEVALUE_H_
#define _DEOGLRRDSRENDERABLEVALUE_H_

#include <dragengine/deObject.h>

#include "deoglRDSRenderable.h"



/**
 * Render dynamic skin value renderable.
 */
class deoglRDSRenderableValue : public deoglRDSRenderable{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglRDSRenderableValue> Ref;
	
	
private:
	deoglTexture *pTexture;
	float pValue;
	bool pDirty;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render dynamic skin value renderable. */
	deoglRDSRenderableValue(deoglRDynamicSkin &dynamicSkin);
	
	/** Clean up render dynamic skin value renderable. */
	~deoglRDSRenderableValue() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Set value. */
	void SetValue(float value);
	
	/** Prepare for render. */
	void PrepareForRender(const deoglRenderPlanMasked *renderPlanMask) override;
	
	/**
	 * Get value if support or default value.
	 * \details Default implementation returns default value.
	 */
	float GetRenderValue(float defaultValue) override;
	
	/**
	 * Get color if support or default color.
	 * \details Default implementation returns default value.
	 */
	virtual decColor GetRenderColor(const decColor &defaultColor);
	
	/**
	 * Get texture to use for rendering or \em NULL if not applicable.
	 * \details Default implementation returns \em NULL.
	 */
	deoglTexture *GetRenderTexture() override;
	/*@}*/
};

#endif
