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

class deSkinPropertyNodeImage;


/**
 * Skin state constructed node.
 */
class deoglSkinStateCNImage : public deoglSkinStateConstructedNode{
public:
	typedef deTObjectReference<deoglSkinStateCNImage> Ref;
	
	
	
private:
	const deoglRImage::Ref pImage;
	const decPoint pRepeat;
	
	
	
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
	
	
	
	/** Create copy. */
	virtual deoglSkinStateConstructedNode::Ref Copy() const override;
	/*@}*/
};

#endif
