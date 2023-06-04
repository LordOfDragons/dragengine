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

#ifndef _DEOGLSKINTEXTUREPIPELINESLIST_H_
#define _DEOGLSKINTEXTUREPIPELINESLIST_H_

#include "deoglSkinTexturePipelines.h"

class deoglShaderLoadingTimeout;


/**
 * Skin texture pipelines list.
 */
class deoglSkinTexturePipelinesList{
public:
	enum ePipelineTypes{
		eptComponent,
		eptBillboard,
		eptDecal,
		eptPropField,
		eptPropFieldImposter,
		eptHeightMap1,
		eptHeightMap2,
		eptParticle,
		eptParticleRibbon,
		eptParticleBeam,
		eptOutline
	};
	
	static const int PipelineTypesCount = eptOutline + 1;
	
	
	
private:
	const deoglSkinTexture &pTexture;
	
	deoglSkinTexturePipelines::Ref pPipelines[ PipelineTypesCount ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin texture pipeline. */
	deoglSkinTexturePipelinesList( const deoglSkinTexture &texture );
	
	/** Clean up skin texture pipeline. */
	~deoglSkinTexturePipelinesList();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Get pipelines. */
	deoglSkinTexturePipelines &GetAt( ePipelineTypes type ) const;
	
	/** Prepare all pipelines. */
	void Prepare( deoglShaderLoadingTimeout &timeout );
	/*@}*/
};

#endif
