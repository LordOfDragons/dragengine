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

#include "igdeGDCComponent.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCComponent
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCComponent::igdeGDCComponent() :
pLightShadowIgnore( false )
{
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
pAnimatorPath( component.pAnimatorPath ),
pOccMeshPath( component.pOccMeshPath ),
pAudioModelPath( component.pAudioModelPath ),
pPlaybackController( component.pPlaybackController ),
pAnimationPath( component.pAnimationPath ),
pMove( component.pMove ),
pDoNotScale( component.pDoNotScale ),
pStatic( component.pStatic ),
pRenderEnvMap( component.pRenderEnvMap ),
pAffectsAudio( component.pAffectsAudio ),
pPartialHide( component.pPartialHide ),
pAttachTarget( component.pAttachTarget ),
pLightShadowIgnore( component.pLightShadowIgnore ),
pColliderResponseType( component.pColliderResponseType ),
pPosition( component.pPosition ),
pOrientation( component.pOrientation ),
pBoneName( component.pBoneName )
{
	int i;
	for( i=0; i<=epMove; i++ ){
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
	pAnimatorPath = path;
}

void igdeGDCComponent::SetAnimationPath( const char *path ){
	pAnimationPath = path;
}

void igdeGDCComponent::SetMove( const char *move ){
	pMove = move;
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

void igdeGDCComponent::SetLightShadowIgnore( bool lightShadowIgnore ){
	pLightShadowIgnore = lightShadowIgnore;
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
	for( j=0; j<=epMove; j++ ){
		if( pPropertyNames[ j ] == name ){
			return true;
		}
	}
	
	return false;
}
