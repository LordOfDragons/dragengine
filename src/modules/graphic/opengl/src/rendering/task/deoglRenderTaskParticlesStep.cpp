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

#include "deoglRenderTaskParticlesStep.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderTaskParticlesStep
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTaskParticlesStep::deoglRenderTaskParticlesStep(){
	pInstances = NULL;
	pInstanceCount = 0;
	pInstanceSize = 0;
	
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
	pShader = NULL;
	pParamBlockTexture = NULL;
	pParamBlockInstance = NULL;
	pTUC = NULL;
	
	pSkin = NULL;
	pDynamicSkin = NULL;
	pTexture = 0;
	
	pFirstIndex = 0;
	pIndexCount = 0;
	pPrimitiveType = GL_POINTS;
	
	RemoveAllInstances();
}



void deoglRenderTaskParticlesStep::SetShader( deoglShaderProgram *shader ){
	pShader = shader;
}

void deoglRenderTaskParticlesStep::SetParameterBlockTexture( deoglShaderParameterBlock *block ){
	pParamBlockTexture = block;
}

void deoglRenderTaskParticlesStep::SetParameterBlockInstance( deoglShaderParameterBlock *block ){
	pParamBlockInstance = block;
}

void deoglRenderTaskParticlesStep::SetTUC( deoglTexUnitsConfig *tuc ){
	pTUC = tuc;
}



void deoglRenderTaskParticlesStep::SetVAO( deoglVAO *vao ){
	pVAO = vao;
}

void deoglRenderTaskParticlesStep::SetSkin( deoglRSkin *skin ){
	pSkin = skin;
}

void deoglRenderTaskParticlesStep::SetDynamicSkin( deoglRDynamicSkin *dynamicSkin ){
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
