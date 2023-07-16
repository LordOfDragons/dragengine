/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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
	deoglSkinStateCNImage( deSkinPropertyNodeImage &node );
	
	/** Create skin state constructed node. */
	deoglSkinStateCNImage( const deoglSkinStateCNImage &node );
	
protected:
	/** Clean up skin state mapped. */
	virtual ~deoglSkinStateCNImage() override;
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
	virtual void Update( deoglSkinState &state ) override;
	
	/** Prepare for render. */
	virtual void PrepareForRender( deoglSkinState &state ) override;
	
	/** Render. */
	virtual void Render( deoglSkinState &state, const deoglRenderCanvasContext &context ) override;
	
	/** Create copy. */
	virtual deoglSkinStateConstructedNode::Ref Copy() const override;
	/*@}*/
	
	
	
private:
	void pUpdateTCTransform();
};

#endif
