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

#ifndef _DEOGLSKINTEXTUREPIPELINEMAP_H_
#define _DEOGLSKINTEXTUREPIPELINEMAP_H_

#include "deoglSkinTexturePipelines.h"
#include "deoglSkinTexturePipelinesList.h"


/**
 * Skin texture pipeline map.
 */
class deoglSkinTexturePipelineMap{
public:
	struct sEntry{
		deoglSkinTexturePipelinesList::ePipelineTypes list;
		deoglSkinTexturePipelines::eTypes type;
		int modifiers;
	};
	
	
	
private:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin texture pipeline map. */
	deoglSkinTexturePipelineMap();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Map pipeline. */
	int Map( deoglSkinTexturePipelinesList::ePipelineTypes list,
		deoglSkinTexturePipelines::eTypes type,
		deoglSkinTexturePipelines::eModifiers modifier ); 
	/*@}*/
	
	
	
private:
	int pMap[ deoglSkinTexturePipelinesList::PipelineTypesCount ]
		[ deoglSkinTexturePipelines::TypeCount ]
		[ deoglSkinTexturePipelines::ModifiersPerType ];
	
	sEntry pMapInv[ 749 ];
	
	void pInitMap();
	
	void pInitGeometry( int &index, deoglSkinTexturePipelinesList::ePipelineTypes list );
	void pInitGeometryDepthTest( int &index, deoglSkinTexturePipelinesList::ePipelineTypes list );
	void pInitMask( int &index, deoglSkinTexturePipelinesList::ePipelineTypes list );
	void pInitGIMaterial( int &index, deoglSkinTexturePipelinesList::ePipelineTypes list );
	void pInitAllDepth( int &index, deoglSkinTexturePipelinesList::ePipelineTypes list );
	void pInitAllCounter( int &index, deoglSkinTexturePipelinesList::ePipelineTypes list );
	void pInitAllShadow( int &index, deoglSkinTexturePipelinesList::ePipelineTypes list );
	void pInitAllGeometryDepthCounterShadow( int &index, deoglSkinTexturePipelinesList::ePipelineTypes list );
	
	void pSetModified( int &index, deoglSkinTexturePipelinesList::ePipelineTypes list,
		deoglSkinTexturePipelines::eTypes type, int modifierMask );
};

#endif
