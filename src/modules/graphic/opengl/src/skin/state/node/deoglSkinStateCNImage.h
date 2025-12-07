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

#ifndef _DEOGLSKINSTATECNIMAGE_H_
#define _DEOGLSKINSTATECNIMAGE_H_

#include "deoglSkinStateConstructedNode.h"
#include "../../../texture/deoglRImage.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/image/deImage.h>

class deSkinPropertyNodeImage;


/**
 * Skin state constructed node.
 */
class deoglSkinStateCNImage : public deoglSkinStateConstructedNode{
public:
	typedef deTObjectReference<deoglSkinStateCNImage> Ref;
	
	
private:
	const deImage::Ref pSyncImage;
	const deoglRImage::Ref pImage;
	const decPoint pRepeat;
	
	decTexMatrix2 pTCTransform;
	decVector2 pTCClampMin;
	decVector2 pTCClampMax;
	bool pDirtyTCTransform;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin state constructed node. */
	deoglSkinStateCNImage(deSkinPropertyNodeImage &node);
	
	/** Create skin state constructed node. */
	deoglSkinStateCNImage(const deoglSkinStateCNImage &node);
	
protected:
	/** Clean up skin state mapped. */
	~deoglSkinStateCNImage() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Image. */
	inline const deoglRImage::Ref &GetImage() const{ return pImage; }
	
	/** Repeat. */
	inline const decPoint &GetRepeat() const{ return pRepeat; }
	
	/** TC Transform. */
	inline const decTexMatrix2 &GetTCTransform() const{ return pTCTransform; }
	
	/** Texture coordinates clamp minimum. */
	inline const decVector2 &GetTCClampMinimum() const{ return pTCClampMin; }
	
	/** Texture coordinates clamp maximum. */
	inline const decVector2 &GetTCClampMaximum() const{ return pTCClampMax; }
	
	
	
	/**
	 * Update.
	 * \warning Called from main thread.
	 */
	void Update(deoglSkinState &state) override;
	
	/** Prepare for render. */
	void PrepareForRender(deoglSkinState &state) override;
	
	/** Render. */
	void Render(deoglSkinState &state, const deoglRenderCanvasContext &context) override;
	
	/** Create copy. */
	deoglSkinStateConstructedNode::Ref Copy() const override;
	/*@}*/
	
	
	
private:
	void pUpdateTCTransform();
};

#endif
