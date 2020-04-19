/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglRDynamicSkin.h"
#include "renderables/render/deoglRDSRenderable.h"
#include "../../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>



// Class deoglRDynamicSkin
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRDynamicSkin::deoglRDynamicSkin( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pUpdateNumber( 0 )
{
	LEAK_CHECK_CREATE( renderThread, DynamicSkin );
}

deoglRDynamicSkin::~deoglRDynamicSkin(){
	LEAK_CHECK_FREE( pRenderThread, DynamicSkin );
}



// Management
///////////////

int deoglRDynamicSkin::GetRenderableCount() const{
	return pRenderables.GetCount();
}

deoglRDSRenderable *deoglRDynamicSkin::GetRenderableAt( int index ) const{
	return ( deoglRDSRenderable* )pRenderables.GetAt( index );
}

deoglRDSRenderable *deoglRDynamicSkin::GetRenderableNamed( const char *name ) const{
	const int count = pRenderables.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglRDSRenderable * const renderable = ( deoglRDSRenderable* )pRenderables.GetAt( i );
		if( renderable->GetName() == name ){
			return renderable;
		}
	}
	
	return NULL;
}

int deoglRDynamicSkin::IndexOfRenderableNamed( const char *name ) const{
	const int count = pRenderables.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( deoglRDSRenderable* )pRenderables.GetAt( i ) )->GetName() == name ){
			return i;
		}
	}
	
	return -1;
}

void deoglRDynamicSkin::AddRenderable( deoglRDSRenderable *renderable ){
	pRenderables.Add( renderable );
	TextureConfigurationChanged();
}

void deoglRDynamicSkin::RemoveAllRenderables(){
	pRenderables.RemoveAll();
	TextureConfigurationChanged();
}

void deoglRDynamicSkin::TextureConfigurationChanged(){
	pUpdateNumber++;
}

int deoglRDynamicSkin::Update(){
	return pUpdateNumber;
}
