/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deDynamicSkin.h"
#include "deDynamicSkinManager.h"
#include "renderables/deDSRenderable.h"

#include "../../../systems/modules/graphic/deBaseGraphicDynamicSkin.h"
#include "../../../common/exceptions.h"



// Class deDynamicSkin
////////////////////////

// Constructor, destructor
////////////////////////////

deDynamicSkin::deDynamicSkin( deDynamicSkinManager *manager ) : deResource( manager ){
	pRenderables = NULL;
	pRenderableCount = 0;
	pRenderableSize = 0;
	
	pPeerGraphic = NULL;
}

deDynamicSkin::~deDynamicSkin(){
	if( pPeerGraphic ){
		delete pPeerGraphic;
		pPeerGraphic = NULL;
	}
	
	RemoveAllRenderables();
	if( pRenderables ) delete [] pRenderables;
}



// Management
///////////////

deDSRenderable *deDynamicSkin::GetRenderableAt( int index ) const{
	if( index < 0 || index >= pRenderableCount ) DETHROW( deeInvalidParam );
	
	return pRenderables[ index ];
}

deDSRenderable *deDynamicSkin::GetRenderableNamed( const char *name ) const{
	int index = IndexOfRenderableNamed( name );
	
	if( index == -1 ){
		return NULL;
		
	}else{
		return pRenderables[ index ];
	}
}

bool deDynamicSkin::HasRenderable( deDSRenderable *renderable ) const{
	return IndexOfRenderable( renderable ) != -1;
}

bool deDynamicSkin::HasRenderableNamed( const char *name ) const{
	return IndexOfRenderableNamed( name ) != -1;
}

int deDynamicSkin::IndexOfRenderable( deDSRenderable *renderable ) const{
	if( ! renderable ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pRenderableCount; i++ ){
		if( renderable == pRenderables[ i ] ){
			return i;
		}
	}
	
	return -1;
}

int deDynamicSkin::IndexOfRenderableNamed( const char *name ) const{
	if( ! name ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pRenderableCount; i++ ){
		if( strcmp( name, pRenderables[ i ]->GetName() ) == 0 ){
			return i;
		}
	}
	
	return -1;
}

void deDynamicSkin::AddRenderable( deDSRenderable *renderable ){
	if( ! renderable || HasRenderableNamed( renderable->GetName() ) ) DETHROW( deeInvalidParam );
	
	if( pRenderableCount == pRenderableSize ){
		int newSize = pRenderableSize * 3 / 2 + 1;
		deDSRenderable **newArray = new deDSRenderable*[ newSize ];
		if( pRenderables ){
			memcpy( newArray, pRenderables, sizeof( deDSRenderable* ) * pRenderableSize );
			delete [] pRenderables;
		}
		pRenderables = newArray;
		pRenderableSize = newSize;
	}
	
	pRenderables[ pRenderableCount ] = renderable;
	pRenderableCount++;
	
	if( pPeerGraphic ){
		pPeerGraphic->RenderableAdded( pRenderableCount - 1, renderable );
	}
}

void deDynamicSkin::RemoveRenderable( deDSRenderable *renderable ){
	int i, index = IndexOfRenderable( renderable );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	if( pPeerGraphic ){
		pPeerGraphic->RenderableRemoved( index, renderable );
	}
	
	for( i=index+1; i<pRenderableCount; i++ ){
		pRenderables[ i - 1 ] = pRenderables[ i ];
	}
	pRenderableCount--;
	
	delete renderable;
}

void deDynamicSkin::RemoveRenderableNamed( const char *name ){
	int i, index = IndexOfRenderableNamed( name );
	
	if( index != -1 ){
		if( pPeerGraphic ){
			pPeerGraphic->RenderableRemoved( index, pRenderables[ index ] );
		}
		
		delete pRenderables[ index ];
		
		for( i=index+1; i<pRenderableCount; i++ ){
			pRenderables[ i - 1 ] = pRenderables[ i ];
		}
		pRenderableCount--;
	}
}

void deDynamicSkin::RemoveAllRenderables(){
	if( pRenderableCount > 0 ){
		if( pPeerGraphic ){
			pPeerGraphic->AllRenderablesRemoved();
		}
		
		while( pRenderableCount > 0 ){
			pRenderableCount--;
			delete pRenderables[ pRenderableCount ];
		}
	}
}



void deDynamicSkin::NotifyRenderableChanged( int index ){
	if( index < 0 || index >= pRenderableCount ) DETHROW( deeInvalidParam );
	
	if( pPeerGraphic ){
		pPeerGraphic->RenderableChanged( index, pRenderables[ index ] );
	}
}




// System Peers
/////////////////

void deDynamicSkin::SetPeerGraphic( deBaseGraphicDynamicSkin *peer ){
	if( pPeerGraphic ) delete pPeerGraphic;
	pPeerGraphic = peer;
}
