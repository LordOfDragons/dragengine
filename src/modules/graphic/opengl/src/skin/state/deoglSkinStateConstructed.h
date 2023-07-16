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

#ifndef _DEOGLSKINSTATECONSTRUCTED_H_
#define _DEOGLSKINSTATECONSTRUCTED_H_

#include "node/deoglSkinStateCNGroup.h"

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglTexture;
class deoglSkinState;
class deoglSkinConstructedProperty;


/**
 * Constructed property state.
 */
class deoglSkinStateConstructed{
private:
	deoglTexture *pTexture;
	deoglSkinConstructedProperty *pProperty;
	deoglSkinStateCNGroup::Ref pContent;
	
	
	
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
	/** Texture. */
	inline deoglTexture *GetTexture() const{ return pTexture; }
	
	/** Set texture. */
	void SetTexture( deoglTexture *texture );
	
	/** Skin constructed property. */
	deoglSkinConstructedProperty *GetProperty() const{ return pProperty; }
	
	/** Set skin constructed property. */
	void SetProperty( deoglSkinConstructedProperty *property );
	
	/** Content. */
	inline deoglSkinStateCNGroup &GetContent(){ return pContent; }
	inline const deoglSkinStateCNGroup &GetContent() const{ return pContent; }
	
	/** Update. */
	void Update( deoglSkinState &skinState );
	/*@}*/
};

#endif
