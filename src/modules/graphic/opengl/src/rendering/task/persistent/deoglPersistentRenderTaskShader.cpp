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

#include "deoglPersistentRenderTaskPool.h"
#include "deoglPersistentRenderTaskShader.h"
#include "deoglPersistentRenderTaskTexture.h"
#include "../../../texture/texunitsconfig/deoglTexUnitsConfig.h"

#include <dragengine/common/exceptions.h>



// Class deoglPersistentRenderTaskShader
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPersistentRenderTaskShader::deoglPersistentRenderTaskShader( deoglPersistentRenderTaskPool &pool ) :
pPool( pool ),
pLLTask( this ),

pParamBlock( NULL ),
pSPBInstanceIndexBase( -1 ){
}

deoglPersistentRenderTaskShader::~deoglPersistentRenderTaskShader(){
	Clear();
}



// Management
///////////////

int deoglPersistentRenderTaskShader::GetTotalPointCount() const{
	decPointerLinkedList::cListEntry *iter = pTextures.GetRoot();
	int pointCount = 0;
	
	while( iter ){
		pointCount += ( ( deoglPersistentRenderTaskTexture* )iter->GetOwner() )->GetTotalPointCount();
		iter = iter->GetNext();
	}
	
	return pointCount;
}

int deoglPersistentRenderTaskShader::GetTotalVAOCount() const{
	decPointerLinkedList::cListEntry *iter = pTextures.GetRoot();
	int vaoCount = 0;
	
	while( iter ){
		vaoCount += ( ( deoglPersistentRenderTaskTexture* )iter->GetOwner() )->GetVAOCount();
		iter = iter->GetNext();
	}
	
	return vaoCount;
}

int deoglPersistentRenderTaskShader::GetTotalInstanceCount() const{
	decPointerLinkedList::cListEntry *iter = pTextures.GetRoot();
	int instanceCount = 0;
	
	while( iter ){
		instanceCount += ( ( deoglPersistentRenderTaskTexture* )iter->GetOwner() )->GetTotalInstanceCount();
		iter = iter->GetNext();
	}
	
	return instanceCount;
}

int deoglPersistentRenderTaskShader::GetTotalSubInstanceCount() const{
	decPointerLinkedList::cListEntry *iter = pTextures.GetRoot();
	int subInstanceCount = 0;
	
	while( iter ){
		subInstanceCount += ( ( deoglPersistentRenderTaskTexture* )iter->GetOwner() )->GetTotalSubInstanceCount();
		iter = iter->GetNext();
	}
	
	return subInstanceCount;
}



void deoglPersistentRenderTaskShader::SetShader( deoglShaderProgram *shader ){
	pShader = shader;
}

void deoglPersistentRenderTaskShader::SetParameterBlock( deoglSPBlockUBO *block ){
	pParamBlock = block;
}

void deoglPersistentRenderTaskShader::SetSPBInstanceIndexBase( int parameter ){
	pSPBInstanceIndexBase = parameter;
}



int deoglPersistentRenderTaskShader::GetTextureCount() const{
	return pTextures.GetCount();
}

decPointerLinkedList::cListEntry *deoglPersistentRenderTaskShader::GetRootTexture() const{
	return pTextures.GetRoot();
}

deoglPersistentRenderTaskTexture * deoglPersistentRenderTaskShader::GetTextureWith( deoglTexUnitsConfig *tuc ) const{
	if( ! tuc ){
		DETHROW( deeInvalidParam );
	}
	
	decPointerLinkedList::cListEntry *iterLeft = pTextures.GetRoot();
	int left = 0, right = pTextures.GetCount() - 1;
	
	while( left <= right ){
		const int middle = left + ( right - left ) / 2;
		
		decPointerLinkedList::cListEntry *iterMiddle = iterLeft;
		int mover = left;
		while( mover++ < middle ){
			iterMiddle = iterMiddle->GetNext();
		}
		
		deoglPersistentRenderTaskTexture * const texture = ( deoglPersistentRenderTaskTexture* )iterMiddle->GetOwner();
		
		if( texture->GetTUC() == tuc ){
			return texture;
			
		}else if( texture->GetTUC() < tuc ){
			left = middle + 1;
			iterLeft = iterMiddle->GetNext();
			
		}else{
			right = middle - 1;
		}
	}
	
	return NULL;
}

deoglPersistentRenderTaskTexture *deoglPersistentRenderTaskShader::AddTexture( deoglTexUnitsConfig *tuc ){
	if( ! tuc ){
		DETHROW( deeInvalidParam );
	}
	
	decPointerLinkedList::cListEntry *iterLeft = pTextures.GetRoot();
	int left = 0, right = pTextures.GetCount() - 1;
	
	while( left <= right ){
		const int middle = left + ( right - left ) / 2;
		
		decPointerLinkedList::cListEntry *iterMiddle = iterLeft;
		int mover = left;
		while( mover++ < middle ){
			iterMiddle = iterMiddle->GetNext();
		}
		
		if( ( ( deoglPersistentRenderTaskTexture* )iterMiddle->GetOwner() )->GetTUC() <= tuc ){
			left = middle + 1;
			iterLeft = iterMiddle->GetNext();
			
		}else{
			right = middle - 1;
		}
	}
	
	deoglPersistentRenderTaskTexture * const texture = pPool.GetTexture();
	
	if( iterLeft ){
		pTextures.InsertBefore( &texture->GetLLShader(),
			&( ( deoglPersistentRenderTaskTexture* )iterLeft->GetOwner() )->GetLLShader() );
		
	}else{
		pTextures.Add( &texture->GetLLShader() );
	}
	
	texture->SetParentShader( this );
	texture->SetTUC( tuc );
	return texture;
}

void deoglPersistentRenderTaskShader::RemoveTexture( deoglPersistentRenderTaskTexture *texture ){
	if( ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	pTextures.Remove( &texture->GetLLShader() );
	pPool.ReturnTexture( texture );
}

void deoglPersistentRenderTaskShader::RemoveAllTextures(){
	decPointerLinkedList::cListEntry *iter = pTextures.GetRoot();
	while( iter ){
		pPool.ReturnTexture( ( deoglPersistentRenderTaskTexture* )iter->GetOwner() );
		iter = iter->GetNext();
	}
	pTextures.RemoveAll();
}



void deoglPersistentRenderTaskShader::Clear(){
	RemoveAllTextures();
	pParamBlock = NULL;
	pSPBInstanceIndexBase = -1;
}
