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

#include "deoglPersistentRenderTaskShader.h"
#include "deoglPersistentRenderTaskTexture.h"
#include "../../../texture/texunitsconfig/deoglTexUnitsConfig.h"

#include <dragengine/common/exceptions.h>



// Class deoglPersistentRenderTaskShader
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPersistentRenderTaskShader::deoglPersistentRenderTaskShader( deoglShaderProgram *shader ) :
pShader( shader ),
pParamBlock( NULL ),
pSPBInstanceIndexBase( -1 )
{
	if( ! shader ){
		DETHROW( deeInvalidParam );
	}
}

deoglPersistentRenderTaskShader::~deoglPersistentRenderTaskShader(){
	RemoveAllTextures();
}



// Management
///////////////

int deoglPersistentRenderTaskShader::GetTotalPointCount() const{
	const int count = pTextures.GetCount();
	int pointCount = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		pointCount += ( ( deoglPersistentRenderTaskTexture* )pTextures.GetAt( i ) )->GetTotalPointCount();
	}
	
	return pointCount;
}

int deoglPersistentRenderTaskShader::GetTotalVAOCount() const{
	const int count = pTextures.GetCount();
	int vaoCount = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		vaoCount += ( ( deoglPersistentRenderTaskTexture* )pTextures.GetAt( i ) )->GetVAOCount();
	}
	
	return vaoCount;
}

int deoglPersistentRenderTaskShader::GetTotalInstanceCount() const{
	const int count = pTextures.GetCount();
	int instanceCount = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		instanceCount += ( ( deoglPersistentRenderTaskTexture* )pTextures.GetAt( i ) )->GetTotalInstanceCount();
	}
	
	return instanceCount;
}

int deoglPersistentRenderTaskShader::GetTotalSubInstanceCount() const{
	const int count = pTextures.GetCount();
	int subInstanceCount = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		subInstanceCount += ( ( deoglPersistentRenderTaskTexture* )pTextures.GetAt( i ) )->GetTotalSubInstanceCount();
	}
	
	return subInstanceCount;
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

deoglPersistentRenderTaskTexture * deoglPersistentRenderTaskShader::GetTextureAt( int index ) const{
	return ( deoglPersistentRenderTaskTexture* )pTextures.GetAt( index );
}

deoglPersistentRenderTaskTexture * deoglPersistentRenderTaskShader::GetTextureWith( deoglTexUnitsConfig *tuc ) const{
	if( ! tuc ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pTextures.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglPersistentRenderTaskTexture * const texture = ( deoglPersistentRenderTaskTexture* )pTextures.GetAt( i );
		if( texture->GetTUC() == tuc ){
			return texture;
		}
	}
	
	return NULL;
}

deoglPersistentRenderTaskTexture *deoglPersistentRenderTaskShader::AddTexture( deoglTexUnitsConfig *tuc ){
	if( ! tuc ){
		DETHROW( deeInvalidParam );
	}
	
	deoglPersistentRenderTaskTexture * const texture = new deoglPersistentRenderTaskTexture( tuc );
	pTextures.Add( texture );
	return texture;
}

void deoglPersistentRenderTaskShader::RemoveAllTextures(){
	const int count = pTextures.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		delete ( deoglPersistentRenderTaskTexture* )pTextures.GetAt( i );
	}
	pTextures.RemoveAll();
}



void deoglPersistentRenderTaskShader::RemoveOwnedBy( void *owner ){
	int i, count = pTextures.GetCount();
	for( i=0; i<count; i++ ){
		deoglPersistentRenderTaskTexture * const texture = ( deoglPersistentRenderTaskTexture* )pTextures.GetAt( i );
		
		texture->RemoveOwnedBy( owner );
		
		if( texture->GetVAOCount() > 0 ){
			continue;
		}
		
		if( i < count - 1 ){
			pTextures.SetAt( i, pTextures.GetAt( count - 1 ) );
		}
		pTextures.RemoveFrom( count - 1 );
		count--;
		i--;
		
		delete texture;
	}
}
