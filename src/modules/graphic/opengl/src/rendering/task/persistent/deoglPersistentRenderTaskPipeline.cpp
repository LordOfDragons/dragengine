/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglPersistentRenderTask.h"
#include "deoglPersistentRenderTaskPool.h"
#include "deoglPersistentRenderTaskPipeline.h"
#include "deoglPersistentRenderTaskTexture.h"
#include "../../../texture/texunitsconfig/deoglTexUnitsConfig.h"

#include <dragengine/common/exceptions.h>



// Class deoglPersistentRenderTaskPipeline
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPersistentRenderTaskPipeline::deoglPersistentRenderTaskPipeline( deoglPersistentRenderTaskPool &pool ) :
pPool( pool ),
pLLTask( this ),
pParentTask( NULL ),
pParamBlock( NULL ),
pSPBInstanceIndexBase( -1 ),
pDrawIDOffset( -1 ){
}

deoglPersistentRenderTaskPipeline::~deoglPersistentRenderTaskPipeline(){
	Clear();
}



// Management
///////////////

int deoglPersistentRenderTaskPipeline::GetTotalPointCount() const{
	decPointerLinkedList::cListEntry *iter = pTextures.GetRoot();
	int pointCount = 0;
	
	while( iter ){
		pointCount += ( ( deoglPersistentRenderTaskTexture* )iter->GetOwner() )->GetTotalPointCount();
		iter = iter->GetNext();
	}
	
	return pointCount;
}

int deoglPersistentRenderTaskPipeline::GetTotalVAOCount() const{
	decPointerLinkedList::cListEntry *iter = pTextures.GetRoot();
	int vaoCount = 0;
	
	while( iter ){
		vaoCount += ( ( deoglPersistentRenderTaskTexture* )iter->GetOwner() )->GetVAOCount();
		iter = iter->GetNext();
	}
	
	return vaoCount;
}

int deoglPersistentRenderTaskPipeline::GetTotalInstanceCount() const{
	decPointerLinkedList::cListEntry *iter = pTextures.GetRoot();
	int instanceCount = 0;
	
	while( iter ){
		instanceCount += ( ( deoglPersistentRenderTaskTexture* )iter->GetOwner() )->GetTotalInstanceCount();
		iter = iter->GetNext();
	}
	
	return instanceCount;
}

int deoglPersistentRenderTaskPipeline::GetTotalSubInstanceCount() const{
	decPointerLinkedList::cListEntry *iter = pTextures.GetRoot();
	int subInstanceCount = 0;
	
	while( iter ){
		subInstanceCount += ( ( deoglPersistentRenderTaskTexture* )iter->GetOwner() )->GetTotalSubInstanceCount();
		iter = iter->GetNext();
	}
	
	return subInstanceCount;
}

void deoglPersistentRenderTaskPipeline::SetParentTask( deoglPersistentRenderTask *task ){
	pParentTask = task;
}



void deoglPersistentRenderTaskPipeline::SetPipeline( const deoglPipeline *pipeline ){
	pPipeline = pipeline;
}

void deoglPersistentRenderTaskPipeline::SetParameterBlock( deoglSPBlockUBO *block ){
	pParamBlock = block;
}

void deoglPersistentRenderTaskPipeline::SetSPBInstanceIndexBase( int parameter ){
	pSPBInstanceIndexBase = parameter;
}

void deoglPersistentRenderTaskPipeline::SetDrawIDOffset( int parameter ){
	pDrawIDOffset = parameter;
}



int deoglPersistentRenderTaskPipeline::GetTextureCount() const{
	return pTextures.GetCount();
}

decPointerLinkedList::cListEntry *deoglPersistentRenderTaskPipeline::GetRootTexture() const{
	return pTextures.GetRoot();
}

deoglPersistentRenderTaskTexture * deoglPersistentRenderTaskPipeline::GetTextureWith(
const deoglTexUnitsConfig *tuc ) const{
	if( ! tuc ){
		DETHROW( deeInvalidParam );
	}
	
	deoglPersistentRenderTaskTexture *texture;
	return pTexturesMap.GetAt( tuc, tuc->GetUniqueKey(), ( void** )&texture ) ? texture : NULL;
}

deoglPersistentRenderTaskTexture *deoglPersistentRenderTaskPipeline::AddTexture(
const deoglTexUnitsConfig *tuc ){
	if( ! tuc ){
		DETHROW( deeInvalidParam );
	}
	
	// commented out in the name of performance
// 	if( pPipelinesMap.Has( tuc, tuc->GetUniqueKey() ) ){
// 		DETHROW( deeInvalidParam );
// 	}
	
	deoglPersistentRenderTaskTexture * const texture = pPool.GetTexture();
	pTextures.Add( &texture->GetLLTexture() );
	texture->SetParentPipeline( this );
	texture->SetTUC( tuc );
	pTexturesMap.SetAt( tuc, tuc->GetUniqueKey(), texture );
	return texture;
}

void deoglPersistentRenderTaskPipeline::RemoveTexture( deoglPersistentRenderTaskTexture *texture ){
	if( ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	pTexturesMap.Remove( texture->GetTUC(), texture->GetTUC()->GetUniqueKey() );
	pTextures.Remove( &texture->GetLLTexture() );
	pPool.ReturnTexture( texture );
}

void deoglPersistentRenderTaskPipeline::RemoveAllTextures(){
	decPointerLinkedList::cListEntry *iter = pTextures.GetRoot();
	while( iter ){
		pPool.ReturnTexture( ( deoglPersistentRenderTaskTexture* )iter->GetOwner() );
		iter = iter->GetNext();
	}
	pTextures.RemoveAll();
	pTexturesMap.RemoveAll();
}



void deoglPersistentRenderTaskPipeline::Clear(){
	RemoveAllTextures();
	pParamBlock = NULL;
	pSPBInstanceIndexBase = -1;
	pDrawIDOffset = -1;
}

void deoglPersistentRenderTaskPipeline::RemoveFromParentIfEmpty(){
	if( pTextures.GetCount() == 0 ){
		pParentTask->RemovePipeline( this );
	}
}
