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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglRenderTaskSharedPool.h"
#include "deoglRenderTaskSharedShader.h"
#include "deoglRenderTaskSharedTexture.h"
#include "deoglRenderTaskSharedVAO.h"
#include "deoglRenderTaskSharedInstance.h"
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderTaskSharedPool
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTaskSharedPool::deoglRenderTaskSharedPool( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pNextIndexShader( 0 ),
pNextIndexTexture( 0 ),
pNextIndexVAO( 0 ),
pNextIndexInstance( 0 ){
}

deoglRenderTaskSharedPool::~deoglRenderTaskSharedPool(){
	int i, count = pInstances.GetCount();
	for( i=0; i<count; i++ ){
		delete ( deoglRenderTaskSharedInstance* )pInstances.GetAt( i );
	}
	
	count = pVAOs.GetCount();
	for( i=0; i<count; i++ ){
		delete ( deoglRenderTaskSharedVAO* )pVAOs.GetAt( i );
	}
	
	count = pTextures.GetCount();
	for( i=0; i<count; i++ ){
		delete ( deoglRenderTaskSharedTexture* )pTextures.GetAt( i );
	}
	
	count = pShaders.GetCount();
	for( i=0; i<count; i++ ){
		delete ( deoglRenderTaskSharedShader* )pShaders.GetAt( i );
	}
}



// Management
///////////////

deoglRenderTaskSharedShader *deoglRenderTaskSharedPool::GetShader(){
	deoglRenderTaskSharedShader *shader;
	
	const int index = pShaders.GetCount() - 1;
	if( index > -1 ){
		shader = ( deoglRenderTaskSharedShader* )pShaders.GetAt( index );
		pShaders.RemoveFrom( index );
		
	}else{
		shader = new deoglRenderTaskSharedShader( *this, pNextIndexShader++ );
		
		if( pNextIndexShader % 100 == 0 ){
			pRenderThread.GetLogger().LogInfoFormat( "RenderTaskSharedPool: Reached %d Shaders", pNextIndexShader );
		}
	}
	
	return shader;
}

deoglRenderTaskSharedTexture *deoglRenderTaskSharedPool::GetTexture(){
	deoglRenderTaskSharedTexture *texture;
	
	const int index = pTextures.GetCount() - 1;
	if( index > -1 ){
		texture = ( deoglRenderTaskSharedTexture* )pTextures.GetAt( index );
		pTextures.RemoveFrom( index );
		
	}else{
		texture = new deoglRenderTaskSharedTexture( *this, pNextIndexTexture++ );
		
		if( pNextIndexTexture % 200 == 0 ){
			pRenderThread.GetLogger().LogInfoFormat( "RenderTaskSharedPool: Reached %d Textures", pNextIndexTexture );
		}
	}
	
	return texture;
}

deoglRenderTaskSharedVAO *deoglRenderTaskSharedPool::GetVAO(){
	deoglRenderTaskSharedVAO *vao;
	
	const int index = pVAOs.GetCount() - 1;
	if( index > -1 ){
		vao = ( deoglRenderTaskSharedVAO* )pVAOs.GetAt( index );
		pVAOs.RemoveFrom( index );
		
	}else{
		vao = new deoglRenderTaskSharedVAO( *this, pNextIndexVAO++ );
		
		if( pNextIndexVAO % 100 == 0 ){
			pRenderThread.GetLogger().LogInfoFormat( "RenderTaskSharedPool: Reached %d VAOs", pNextIndexVAO );
		}
	}
	
	return vao;
}

deoglRenderTaskSharedInstance *deoglRenderTaskSharedPool::GetInstance(){
	deoglRenderTaskSharedInstance *instance;
	
	const int index = pInstances.GetCount() - 1;
	if( index > -1 ){
		instance = ( deoglRenderTaskSharedInstance* )pInstances.GetAt( index );
		pInstances.RemoveFrom( index );
		
	}else{
		instance = new deoglRenderTaskSharedInstance( *this, pNextIndexInstance++ );
		
		if( pNextIndexInstance % 500 == 0 ){
			pRenderThread.GetLogger().LogInfoFormat( "RenderTaskSharedPool: Reached %d Instances", pNextIndexInstance );
		}
	}
	
	return instance;
}



void deoglRenderTaskSharedPool::ReturnShader( deoglRenderTaskSharedShader *shader ){
	DEASSERT_NOTNULL( shader )
	
	shader->Clear();
	pShaders.Add( shader );
}

void deoglRenderTaskSharedPool::ReturnTexture( deoglRenderTaskSharedTexture *texture ){
	DEASSERT_NOTNULL( texture )
	
	texture->Clear();
	pTextures.Add( texture );
}

void deoglRenderTaskSharedPool::ReturnVAO( deoglRenderTaskSharedVAO *vao ){
	DEASSERT_NOTNULL( vao )
	
	vao->Clear();
	pVAOs.Add( vao );
}

void deoglRenderTaskSharedPool::ReturnInstance( deoglRenderTaskSharedInstance *instance ){
	DEASSERT_NOTNULL( instance )
	
	instance->Clear();
	pInstances.Add( instance );
}
