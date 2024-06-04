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

#include "seUMappedRemove.h"
#include "../../skin/texture/seTexture.h"

#include <dragengine/common/exceptions.h>



// Class seUMappedRemove
///////////////////////////

// Constructor, destructor
////////////////////////////

seUMappedRemove::seUMappedRemove( seMapped *mapped ) :
pMapped( mapped ),
pSkin( mapped ? mapped->GetSkin() : nullptr )
{
	DEASSERT_NOTNULL( pMapped )
	DEASSERT_NOTNULL( pSkin )
	
	SetShortInfo( "Remove Mapped" );
	
	int i, j, k;
	
	const seTextureList &textureList = pSkin->GetTextureList();
	const int textureCount = textureList.GetCount();
	for( i=0; i<textureCount; i++ ){
		seTexture * const texture = textureList.GetAt( i );
		
		const sePropertyList &propertyList = texture->GetPropertyList();
		const int propertyCount = propertyList.GetCount();
		
		for( j=0; j<propertyCount; j++ ){
			seProperty * const property = propertyList.GetAt( j );
			
			for( k=0; k<4; k++ ){
				if( property->GetMappedComponent( k ) == mapped ){
					pDependencies.Add( sDependency::Ref::New( new sDependency( property, k ) ) );
				}
			}
		}
	}
}

seUMappedRemove::~seUMappedRemove(){
}



// Management
///////////////

int seUMappedRemove::GetDependencyCount() const{
	return pDependencies.GetCount();
}

void seUMappedRemove::Undo(){
	pSkin->AddMapped( pMapped );
	
	const int count = pDependencies.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const sDependency &dependency = *( ( sDependency* )pDependencies.GetAt( i ) );
		
		if( dependency.mappedComponent != -1 ){
			dependency.property->SetMappedComponent( dependency.mappedComponent, pMapped );
		}
	}
}

void seUMappedRemove::Redo(){
	const int count = pDependencies.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const sDependency &dependency = *( ( sDependency* )pDependencies.GetAt( i ) );
		
		if( dependency.mappedComponent != -1 ){
			dependency.property->SetMappedComponent( dependency.mappedComponent, nullptr );
		}
	}
	
	pSkin->RemoveMapped( pMapped );
}
