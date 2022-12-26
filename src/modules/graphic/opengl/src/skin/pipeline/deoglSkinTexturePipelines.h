/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLSKINTEXTUREPIPELINES_H_
#define _DEOGLSKINTEXTUREPIPELINES_H_

#include "deoglSkinTexturePipeline.h"
#include "../../pipeline/deoglPipeline.h"

class deoglSkinTexture;


/**
 * Geometry specific skin texture pipelines.
 */
class deoglSkinTexturePipelines{
public:
	/** Type. */
	enum eTypes{
		etGeometry,
		etDepth,
		etCounter,
		etShadowProjection,
		etShadowOrthogonal,
		etShadowOrthogonalCascaded,
		etShadowDistance,
		etShadowDistanceCube,
		etEnvMap,
		etLuminance,
		etGIMaterial
	};
	
	/** Modifier. */
	enum eModifiers{
		emClipPlane = 0x1,
		emReversed = 0x2,
		emStereo = 0x4,
		emDepthTest = 0x8
	};
	
	
	
private:
	const static int TypeCount = etGIMaterial + 1;
	const static int ModifiersPerType = emDepthTest << 1;
	
	const deoglSkinTexture &pTexture;
	
	deoglSkinTexturePipeline::Ref pPipelines[ TypeCount ][ ModifiersPerType ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin texture pipeline. */
	deoglSkinTexturePipelines( const deoglSkinTexture &texture );
	
	/** Clean up skin texture pipeline. */
	virtual ~deoglSkinTexturePipelines();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Texture. */
	inline const deoglSkinTexture &GetTexture() const{ return pTexture; }
	
	/** Pipeline with type and modifiers or nullptr. */
	const deoglSkinTexturePipeline *GetWith( eTypes type, int modifiers ) const;
	
	/** Prepare pipelines. */
	void Prepare();
	/*@}*/
};

#endif
