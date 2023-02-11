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
pRenderThread( renderThread ){
}

deoglRenderTaskSharedPool::~deoglRenderTaskSharedPool(){
}



// Management
///////////////

deoglRenderTaskSharedTexture *deoglRenderTaskSharedPool::GetTexture(){
	deoglRenderTaskSharedTexture *texture;
	
	const int index = pFreeTextures.GetCount() - 1;
	if( index > -1 ){
		texture = ( deoglRenderTaskSharedTexture* )pFreeTextures.GetAt( index );
		pFreeTextures.RemoveFrom( index );
		
	}else{
		texture = new deoglRenderTaskSharedTexture( *this, pTextures.GetCount() );
		pTextures.Add( deoglRenderTaskSharedTexture::Ref::New( texture ) );
		
		if( pTextures.GetCount() % 200 == 0 ){
			pRenderThread.GetLogger().LogInfoFormat(
				"RenderTaskSharedPool: Reached %d Textures", pTextures.GetCount() );
		}
	}
	
	return texture;
}

deoglRenderTaskSharedVAO *deoglRenderTaskSharedPool::GetVAO(){
	deoglRenderTaskSharedVAO *vao;
	
	const int index = pFreeVAOs.GetCount() - 1;
	if( index > -1 ){
		vao = ( deoglRenderTaskSharedVAO* )pFreeVAOs.GetAt( index );
		pFreeVAOs.RemoveFrom( index );
		
	}else{
		vao = new deoglRenderTaskSharedVAO( *this, pVAOs.GetCount() );
		pVAOs.Add( deoglRenderTaskSharedVAO::Ref::New( vao ) );
		
		if( pVAOs.GetCount() % 100 == 0 ){
			pRenderThread.GetLogger().LogInfoFormat(
				"RenderTaskSharedPool: Reached %d VAOs", pVAOs.GetCount() );
		}
	}
	
	return vao;
}

deoglRenderTaskSharedInstance *deoglRenderTaskSharedPool::GetInstance(){
	deoglRenderTaskSharedInstance *instance;
	
	const int index = pFreeInstances.GetCount() - 1;
	if( index > -1 ){
		instance = ( deoglRenderTaskSharedInstance* )pFreeInstances.GetAt( index );
		pFreeInstances.RemoveFrom( index );
		
	}else{
		instance = new deoglRenderTaskSharedInstance( *this, pInstances.GetCount() );
		pInstances.Add( deoglRenderTaskSharedInstance::Ref::New( instance ) );
		
		if( pInstances.GetCount() % 500 == 0 ){
			pRenderThread.GetLogger().LogInfoFormat(
				"RenderTaskSharedPool: Reached %d Instances", pInstances.GetCount() );
		}
	}
	
	return instance;
}

int deoglRenderTaskSharedPool::AssignSkinTexture( deoglSkinTexture *skinTexture ){
	int slot;
	
	int index = pFreeSkinTextures.GetCount() - 1;
	if( index > -1 ){
		slot = pFreeSkinTextures.GetAt( index );
		pFreeSkinTextures.RemoveFrom( index );
		
	}else{
		slot = pSkinTextures.GetCount();
		pSkinTextures.Add( skinTexture );
		
		if( pSkinTextures.GetCount() % 100 == 0 ){
			pRenderThread.GetLogger().LogInfoFormat(
				"RenderTaskSharedPool: Reached %d Skin Textures", pSkinTextures.GetCount() );
		}
	}
	
	return slot;
}



deoglRenderTaskSharedTexture &deoglRenderTaskSharedPool::GetTextureAt( int index ) const{
	return *( deoglRenderTaskSharedTexture* )pTextures.GetAt( index );
}

deoglRenderTaskSharedVAO &deoglRenderTaskSharedPool::GetVAOAt( int index ) const{
	return *( deoglRenderTaskSharedVAO* )pVAOs.GetAt( index );
}

deoglRenderTaskSharedInstance &deoglRenderTaskSharedPool::GetInstanceAt( int index ) const{
	return *( deoglRenderTaskSharedInstance* )pInstances.GetAt( index );
}

deoglSkinTexture *deoglRenderTaskSharedPool::GetSkinTextureAt( int index ) const{
	return ( deoglSkinTexture* )pSkinTextures.GetAt( index );
}

int deoglRenderTaskSharedPool::GetSkinTextureCount() const{
	return pSkinTextures.GetCount();
}



void deoglRenderTaskSharedPool::ReturnTexture( deoglRenderTaskSharedTexture *texture ){
	DEASSERT_NOTNULL( texture )
	
	texture->Clear();
	pFreeTextures.Add( texture );
}

void deoglRenderTaskSharedPool::ReturnVAO( deoglRenderTaskSharedVAO *vao ){
	DEASSERT_NOTNULL( vao )
	
	vao->Clear();
	pFreeVAOs.Add( vao );
}

void deoglRenderTaskSharedPool::ReturnInstance( deoglRenderTaskSharedInstance *instance ){
	DEASSERT_NOTNULL( instance )
	
	instance->Clear();
	pFreeInstances.Add( instance );
}

void deoglRenderTaskSharedPool::ReturnSkinTexture( int slot ){
	pSkinTextures.SetAt( slot, nullptr );
	pFreeSkinTextures.Add( slot );
}
