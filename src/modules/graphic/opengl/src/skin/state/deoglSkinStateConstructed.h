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

#ifndef _DEOGLSKINSTATECONSTRUCTED_H_
#define _DEOGLSKINSTATECONSTRUCTED_H_

#include "node/deoglSkinStateCNGroup.h"
#include "../../target/deoglRenderTarget.h"

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglSkinState;
class deoglSkinConstructedProperty;


/**
 * Constructed property state.
 */
class deoglSkinStateConstructed{
private:
	deoglRenderTarget::Ref pRenderTarget;
	deoglSkinConstructedProperty *pProperty;
	deoglSkinStateCNGroup::Ref pContent;
	bool pDirty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin state constructed. */
	deoglSkinStateConstructed();
	
	/** Clean up skin state constructed. */
	~deoglSkinStateConstructed();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render target. */
	inline const deoglRenderTarget::Ref &GetRenderTarget() const{ return pRenderTarget; }
	
	/** Skin constructed property. */
	deoglSkinConstructedProperty *GetProperty() const{ return pProperty; }
	
	/** Set skin constructed property. */
	void SetProperty(deoglSkinConstructedProperty *property);
	
	/** Content. */
	inline deoglSkinStateCNGroup &GetContent(){ return pContent; }
	inline const deoglSkinStateCNGroup &GetContent() const{ return pContent; }
	
	/**
	 * Update.
	 * \warning Called from main thread.
	 */
	void Update(deoglSkinState &skinState);
	
	/** Prepare for render. */
	void PrepareForRender(deoglSkinState &skinState);
	/*@}*/
	
	
	
private:
	void pPrepareRenderTarget(deoglSkinState &skinState);
};

#endif
