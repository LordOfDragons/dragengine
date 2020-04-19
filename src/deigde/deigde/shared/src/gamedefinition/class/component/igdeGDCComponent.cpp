/* 
 * Drag[en]gine IGDE
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

#include "igdeGDCComponent.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCComponent
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCComponent::igdeGDCComponent(){
	pDoNotScale = false;
	pStatic = true;
	pPartialHide = false;
	pRenderEnvMap = true;
	pAffectsAudio = true;
	pAttachTarget = true;
	pColliderResponseType = deCollider::ertStatic;
}

igdeGDCComponent::igdeGDCComponent( const igdeGDCComponent &component ) :
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
pColliderResponseType( component.pColliderResponseType ),
pPosition( component.pPosition ),
pOrientation( component.pOrientation ),
pBoneName( component.pBoneName )
{
	int i;
	for( i=0; i<=epAttachRotation; i++ ){
		pPropertyNames[ i ] = component.pPropertyNames[ i ];
	}
	
	pTextureList.SetToDeepCopyFrom( component.GetTextureList() );
}

igdeGDCComponent::~igdeGDCComponent(){
}



// Management
///////////////

void igdeGDCComponent::SetModelPath( const char *path ){
	pModelPath = path;
}

void igdeGDCComponent::SetSkinPath( const char *path ){
	pSkinPath = path;
}

void igdeGDCComponent::SetRigPath( const char *path ){
	pRigPath = path;
}

void igdeGDCComponent::SetAnimatorPath( const char *path ){
	pAnimPath = path;
}

void igdeGDCComponent::SetOcclusionMeshPath( const char *path ){
	pOccMeshPath = path;
}

void igdeGDCComponent::SetAudioModelPath( const char *path ){
	pAudioModelPath = path;
}

void igdeGDCComponent::SetPlaybackController( const char *controller ){
	pPlaybackController = controller;
}

void igdeGDCComponent::SetDoNotScale( bool doNotScale ){
	pDoNotScale = doNotScale;
}

void igdeGDCComponent::SetStatic( bool bStatic ){
	pStatic = bStatic;
}

void igdeGDCComponent::SetPartialHide( bool partiallyHidden ){
	pPartialHide = partiallyHidden;
}

void igdeGDCComponent::SetAttachTarget( bool attachTarget ){
	pAttachTarget = attachTarget;
}

void igdeGDCComponent::SetColliderResponseType( deCollider::eResponseType responseType ){
	pColliderResponseType = responseType;
}

void igdeGDCComponent::SetRenderEnvMap( bool renderEnvMap ){
	pRenderEnvMap = renderEnvMap;
}

void igdeGDCComponent::SetAffectsAudio( bool affectsAudio ){
	pAffectsAudio = affectsAudio;
}

void igdeGDCComponent::SetPosition( const decDVector &position ){
	pPosition = position;
}

void igdeGDCComponent::SetOrientation( const decQuaternion &orientation ){
	pOrientation = orientation;
}

void igdeGDCComponent::SetBoneName( const char *boneName ){
	pBoneName = boneName;
}



bool igdeGDCComponent::IsPropertySet( int property ) const{
	return ! pPropertyNames[ property ].IsEmpty();
}

const decString &igdeGDCComponent::GetPropertyName( int property ) const{
	return pPropertyNames[ property ];
}

void igdeGDCComponent::SetPropertyName( int property, const char *name ){
	pPropertyNames[ property ] = name;
}

bool igdeGDCComponent::HasPropertyWithName( const char *name ) const{
	int j;
	for( j=0; j<=epAttachRotation; j++ ){
		if( pPropertyNames[ j ] == name ){
			return true;
		}
	}
	
	return false;
}
