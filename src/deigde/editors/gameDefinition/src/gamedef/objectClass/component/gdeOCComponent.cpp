/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeOCComponent.h"
#include "gdeOCComponentTexture.h"

#include <dragengine/common/exceptions.h>



// Class gdeOCComponent
/////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCComponent::gdeOCComponent() :
pDoNotScale( false ),
pStatic( true ),
pRenderEnvMap( true ),
pAffectsAudio( true ),
pPartialHide( false ),
pAttachTarget( true ),
pLightShadowIgnore( false ),
pColliderResponseType( deCollider::ertStatic ),
pActiveTexture( NULL ){
}

gdeOCComponent::gdeOCComponent( const gdeOCComponent &component ) :
pModelPath( component.pModelPath ),
pSkinPath( component.pSkinPath ),
pRigPath( component.pRigPath ),
pAnimPath( component.pAnimPath ),
pOccMeshPath( component.pOccMeshPath ),
pAudioModelPath( component.pAudioModelPath ),
pPlaybackController( component.pPlaybackController ),
pDoNotScale( component.pDoNotScale ),
pStatic( component.pStatic ),
pRenderEnvMap( component.pRenderEnvMap ),
pAffectsAudio( component.pAffectsAudio ),
pPartialHide( component.pPartialHide ),
pAttachTarget( component.pAttachTarget ),
pLightShadowIgnore( component.pLightShadowIgnore ),
pColliderResponseType( component.pColliderResponseType ),
pPosition( component.pPosition ),
pRotation( component.pRotation ),
pBoneName( component.pBoneName ),
pActiveTexture( NULL )
{
	const int textureCount = component.pTextures.GetCount();
	gdeOCComponentTexture *texture = NULL;
	int i;
	
	try{
		for( i=0; i<textureCount; i++ ){
			texture = new gdeOCComponentTexture( *component.pTextures.GetAt( i ) );
			pTextures.Add( texture );
			texture->FreeReference();
			texture = NULL;
		}
		
		for( i=0; i<=epAttachRotation; i++ ){
			pPropertyNames[ i ] = component.pPropertyNames[ i ];
		}
		
	}catch( const deException & ){
		if( texture ){
			texture->FreeReference();
		}
		throw;
	}
}

gdeOCComponent::~gdeOCComponent(){
	if( pActiveTexture ){
		pActiveTexture->FreeReference();
	}
}



// Management
///////////////

void gdeOCComponent::SetModelPath( const char *path ){
	pModelPath = path;
}

void gdeOCComponent::SetSkinPath( const char *path ){
	pSkinPath = path;
}

void gdeOCComponent::SetRigPath( const char *path ){
	pRigPath = path;
}

void gdeOCComponent::SetAnimatorPath( const char *path ){
	pAnimPath = path;
}

void gdeOCComponent::SetOcclusionMeshPath( const char *path ){
	pOccMeshPath = path;
}

void gdeOCComponent::SetAudioModelPath( const char *path ){
	pAudioModelPath = path;
}

void gdeOCComponent::SetPlaybackController( const char *controller ){
	pPlaybackController = controller;
}

void gdeOCComponent::SetDoNotScale( bool doNotScale ){
	pDoNotScale = doNotScale;
}

void gdeOCComponent::SetStatic( bool bStatic ){
	pStatic = bStatic;
}

void gdeOCComponent::SetPartialHide( bool partiallyHidden ){
	pPartialHide = partiallyHidden;
}

void gdeOCComponent::SetAttachTarget( bool attachTarget ){
	pAttachTarget = attachTarget;
}

void gdeOCComponent::SetColliderResponseType( deCollider::eResponseType responseType ){
	pColliderResponseType = responseType;
}

void gdeOCComponent::SetRenderEnvMap( bool renderEnvMap ){
	pRenderEnvMap = renderEnvMap;
}

void gdeOCComponent::SetAffectsAudio( bool affectsAudio ){
	pAffectsAudio = affectsAudio;
}

void gdeOCComponent::SetLightShadowIgnore( bool lightShadowIgnore ){
	pLightShadowIgnore = lightShadowIgnore;
}

void gdeOCComponent::SetPosition( const decVector &position ){
	pPosition = position;
}

void gdeOCComponent::SetRotation( const decVector &orientation ){
	pRotation = orientation;
}

void gdeOCComponent::SetBoneName( const char *boneName ){
	pBoneName = boneName;
}



bool gdeOCComponent::IsPropertySet( eProperties property ) const{
	return ! pPropertyNames[ property ].IsEmpty();
}

const decString &gdeOCComponent::GetPropertyName( eProperties property ) const{
	return pPropertyNames[ property ];
}

void gdeOCComponent::SetPropertyName( eProperties property, const char *name ){
	pPropertyNames[ property ] = name;
}

bool gdeOCComponent::HasPropertyWithName( const char *name ) const{
	int i;
	for( i=0; i<=epAttachRotation; i++ ){
		if( pPropertyNames[ i ] == name ){
			return true;
		}
	}
	
	return false;
}



void gdeOCComponent::SetActiveTexture( gdeOCComponentTexture *texture ){
	if( texture == pActiveTexture ){
		return;
	}
	
	if( pActiveTexture ){
		pActiveTexture->FreeReference();
	}
	
	pActiveTexture = texture;
	
	if( texture ){
		texture->AddReference();
	}
}
