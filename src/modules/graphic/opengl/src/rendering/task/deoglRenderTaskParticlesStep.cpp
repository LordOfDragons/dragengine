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

#include "deoglRenderTaskParticlesStep.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderTaskParticlesStep
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTaskParticlesStep::deoglRenderTaskParticlesStep() :
pInstances( nullptr ),
pInstanceCount( 0 ),
pInstanceSize( 0 )
{
	Reset();
}

deoglRenderTaskParticlesStep::~deoglRenderTaskParticlesStep(){
	if( pInstances ){
		delete [] pInstances;
	}
}



// Management
///////////////

void deoglRenderTaskParticlesStep::Reset(){
	pPipeline = nullptr;
	pParamBlockTexture = nullptr;
	pParamBlockInstance = nullptr;
	pTUC = nullptr;
	
	pSkin = nullptr;
	pDynamicSkin = nullptr;
	pTexture = 0;
	
	pFirstIndex = 0;
	pIndexCount = 0;
	pPrimitiveType = GL_POINTS;
	
	RemoveAllInstances();
}



void deoglRenderTaskParticlesStep::SetPipeline( const deoglPipeline *pipeline ){
	pPipeline = pipeline;
}

void deoglRenderTaskParticlesStep::SetParameterBlockTexture( const deoglShaderParameterBlock *block ){
	pParamBlockTexture = block;
}

void deoglRenderTaskParticlesStep::SetParameterBlockInstance( const deoglShaderParameterBlock *block ){
	pParamBlockInstance = block;
}

void deoglRenderTaskParticlesStep::SetTUC( const deoglTexUnitsConfig *tuc ){
	pTUC = tuc;
}



void deoglRenderTaskParticlesStep::SetVAO( const deoglVAO *vao ){
	pVAO = vao;
}

void deoglRenderTaskParticlesStep::SetSkin( const deoglRSkin *skin ){
	pSkin = skin;
}

void deoglRenderTaskParticlesStep::SetDynamicSkin( const deoglRDynamicSkin *dynamicSkin ){
	pDynamicSkin = dynamicSkin;
}

void deoglRenderTaskParticlesStep::SetTexture( int texture ){
	pTexture = texture;
}



void deoglRenderTaskParticlesStep::SetFirstIndex( int firstIndex ){
	pFirstIndex = firstIndex;
}

void deoglRenderTaskParticlesStep::SetIndexCount( int indexCount ){
	pIndexCount = indexCount;
}

void deoglRenderTaskParticlesStep::IncrementIndexCount( int amount ){
	pIndexCount += amount;
}

void deoglRenderTaskParticlesStep::SetPrimitiveType( GLenum primitiveType ){
	pPrimitiveType = primitiveType;
}



// Instances
//////////////

deoglRenderTaskParticlesStep::sInstance &deoglRenderTaskParticlesStep::GetInstanceAt( int index ) const{
	if( index < 0 || index >= pInstanceCount ) DETHROW( deeInvalidParam );
	
	return pInstances[ index ];
}

deoglRenderTaskParticlesStep::sInstance &deoglRenderTaskParticlesStep::AddInstance(){
	if( pInstanceCount == pInstanceSize ){
		int newSize = pInstanceSize + 10;
		sInstance *newArray = new sInstance[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pInstances ){
			memcpy( newArray, pInstances, sizeof( sInstance ) * pInstanceSize );
			delete [] pInstances;
		}
		pInstances = newArray;
		pInstanceSize = newSize;
	}
	
	pInstanceCount++;
	return pInstances[ pInstanceCount - 1 ];
}

void deoglRenderTaskParticlesStep::RemoveAllInstances(){
	pInstanceCount = 0;
}
