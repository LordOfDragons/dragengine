/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deoglOcclusionMap.h"
#include "deoglOcclusionMapPool.h"

#include <dragengine/common/exceptions.h>



// Class deoglOcclusionMapPool
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglOcclusionMapPool::deoglOcclusionMapPool( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ){
}

deoglOcclusionMapPool::~deoglOcclusionMapPool(){
	int count = pOcclusionMaps.GetCount();
	while( count > 0 ){
		delete ( deoglOcclusionMap* )pOcclusionMaps.GetAt( --count );
	}
}



// Management
///////////////

deoglOcclusionMap *deoglOcclusionMapPool::Get( int width, int height, int layerCount ){
	if( width < 1 || height < 1 || layerCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pOcclusionMaps.GetCount();
	int i;
	
	for( i=count-1; i>=0; i-- ){
		deoglOcclusionMap * const occlusionMap = ( deoglOcclusionMap* )pOcclusionMaps.GetAt( i );
		if( occlusionMap->GetWidth() == width
		&& occlusionMap->GetHeight() == height
		&& occlusionMap->GetLayerCount() == layerCount ){
			pOcclusionMaps.RemoveFrom( i );
			return occlusionMap;
		}
	}
	
	return new deoglOcclusionMap( pRenderThread, width, height, layerCount );
}

void deoglOcclusionMapPool::Return( deoglOcclusionMap *occlusionMap ){
	if( ! occlusionMap ){
		DETHROW( deeInvalidParam );
	}
	
	pOcclusionMaps.Add( occlusionMap );
}
