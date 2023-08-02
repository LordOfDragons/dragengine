/* 
 * Drag[en]gine IGDE Skin Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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
