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

#include "deoglSkinState.h"
#include "deoglSkinStateRenderable.h"
#include "deoglSkinStateMapped.h"
#include "deoglSkinStateBone.h"
#include "deoglSkinStateCalculated.h"
#include "deoglSkinStateConstructed.h"
#include "../deoglRSkin.h"
#include "../deoglSkinRenderable.h"
#include "../dynamic/deoglRDynamicSkin.h"
#include "../dynamic/renderables/render/deoglRDSRenderable.h"
#include "../../billboard/deoglRBillboard.h"
#include "../../component/deoglRComponent.h"
#include "../../component/deoglRComponentTexture.h"
#include "../../decal/deoglRDecal.h"
#include "../../light/deoglRLight.h"
#include "../../rendering/plan/deoglRenderPlan.h"
#include "../../rendering/plan/deoglRenderPlanMasked.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../video/deoglRVideoPlayer.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class deoglSkinState
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinState::deoglSkinState( deoglRenderThread &renderThread ) :
pRenderThread( renderThread )
{
	(void)pRenderThread; // silence compiler warning
	pSharedInit();
}

deoglSkinState::deoglSkinState( deoglRenderThread &renderThread, deoglRComponent &component, int texture ) :
pRenderThread( renderThread )
{
	pSharedInit();
	pOwnerComponent = &component;
	pOwnerComponentTexture = texture;
}

deoglSkinState::deoglSkinState( deoglRenderThread &renderThread, deoglRBillboard &billboard ) :
pRenderThread( renderThread )
{
	pSharedInit();
	pOwnerBillboard = &billboard;
}

deoglSkinState::deoglSkinState( deoglRenderThread &renderThread, deoglRDecal &decal ) :
pRenderThread( renderThread )
{
	pSharedInit();
	pOwnerDecal = &decal;
}

deoglSkinState::deoglSkinState( deoglRenderThread &renderThread, deoglRLight &light ) :
pRenderThread( renderThread )
{
	pSharedInit();
	pOwnerLight = &light;
}

deoglSkinState::~deoglSkinState(){
	SetVideoPlayerCount( 0 );
	
	RemoveAllRenderables();
	if( pRenderables ){
		delete [] pRenderables;
	}
	
	SetCalculatedPropertyCount( 0 );
	SetConstructedPropertyCount( 0 );
	SetMappedCount( 0 );
	SetBoneCount( 0 );
}



// Management
///////////////

deoglRSkin *deoglSkinState::GetOwnerSkin() const{
	if( pOwnerComponent ){
		if( pOwnerComponentTexture != -1 ){
			return pOwnerComponent->GetTextureAt( pOwnerComponentTexture ).GetSkin();
			
		}else{
			return pOwnerComponent->GetSkin();
		}
		
	}else if( pOwnerBillboard ){
		return pOwnerBillboard->GetSkin();
		
	}else if( pOwnerDecal ){
		return pOwnerDecal->GetSkin();
		
	}else if( pOwnerLight ){
		return pOwnerLight->GetLightSkin();
		
	}else{
		return NULL;
	}
}

deoglRDynamicSkin *deoglSkinState::GetOwnerDynamicSkin() const{
	if( pOwnerComponent ){
		if( pOwnerComponentTexture != -1 ){
			deoglRDynamicSkin * const dynamicSkin =
				pOwnerComponent->GetTextureAt( pOwnerComponentTexture ).GetDynamicSkin();
			return dynamicSkin ? dynamicSkin : pOwnerComponent->GetDynamicSkin();
			
		}else{
			return pOwnerComponent->GetDynamicSkin();
		}
		
	}else if( pOwnerBillboard ){
		return pOwnerBillboard->GetDynamicSkin();
		
	}else if( pOwnerDecal ){
		return pOwnerDecal->GetUseDynamicSkin();
		
	}else if( pOwnerLight ){
		return pOwnerLight->GetDynamicSkin();
		
	}else{
		return NULL;
	}
}



void deoglSkinState::SetTime( float time ){
	pTime = time;
}

void deoglSkinState::AdvanceTime( float timeStep ){
	pTime += timeStep;
}



int deoglSkinState::GetVideoPlayerCount() const{
	return pVideoPlayers.GetCount();
}

void deoglSkinState::SetVideoPlayerCount( int count ){
	if( count == pVideoPlayers.GetCount() ){
		return;
	}
	
	pVideoPlayers.RemoveAll();
	
	int i;
	for( i=0; i<count; i++ ){
		pVideoPlayers.Add( NULL );
	}
}

deoglRVideoPlayer *deoglSkinState::GetVideoPlayerAt( int index ) const{
	return ( deoglRVideoPlayer* )pVideoPlayers.GetAt( index );
}

void deoglSkinState::SetVideoPlayerAt( int index, deoglRVideoPlayer *videoPlayer ){
	pVideoPlayers.SetAt( index, videoPlayer );
}



void deoglSkinState::PrepareRenderables( deoglRSkin *skin, deoglRDynamicSkin *dynamicSkin,
const deoglRenderPlanMasked *renderPlanMask ){
	const int videoPlayerCount = pVideoPlayers.GetCount();
	int i;
	for( i=0; i< videoPlayerCount; i++ ){
		deoglRVideoPlayer * const videoPlayer = ( deoglRVideoPlayer* )pVideoPlayers.GetAt( i );
		if( videoPlayer ){
			videoPlayer->UpdateTexture();
		}
	}
	
	for( i=0; i<pRenderableCount; i++ ){
		deoglSkinStateRenderable &skinStateRenderable = *pRenderables[ i ];
		skinStateRenderable.Clear();
		
		const int hostIndex = skinStateRenderable.GetHostRenderable();
		if( hostIndex != -1 && dynamicSkin ){
			dynamicSkin->GetRenderableAt( hostIndex )->PrepareForRender( renderPlanMask );
		}
	}
}

void deoglSkinState::RenderRenderables( deoglRSkin *skin, deoglRDynamicSkin *dynamicSkin,
const deoglRenderPlanMasked *renderPlanMask ){
	int i;
	for( i=0; i<pRenderableCount; i++ ){
		deoglSkinStateRenderable &skinStateRenderable = *pRenderables[ i ];
		skinStateRenderable.Clear();
		
		const int hostIndex = skinStateRenderable.GetHostRenderable();
		if( hostIndex != -1 && dynamicSkin ){
			dynamicSkin->GetRenderableAt( hostIndex )->Render( renderPlanMask );
		}
	}
}

void deoglSkinState::AddRenderPlans( deoglRenderPlan &plan ){
	// HACK: preventing a dead loop. has to be made better
	if( plan.GetLevel() > 0 ){
		return;
	}
	
	// TODO: currently only the mirror related plans are properly added.
	// renderable related plans are wrapped by a dirty-skin test and can be
	// skipped the second time.
	// required to verify if this is an issue or not ( and acting accordingly ).
	int i;
	for( i=0; i<pRenderableCount; i++ ){
		pRenderables[ i ]->AddRenderPlans( plan );
	}
}



// Renderables
////////////////

deoglSkinStateRenderable *deoglSkinState::GetRenderableAt( int index ) const{
	if( index < 0 || index >= pRenderableCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pRenderables[ index ];
}

deoglSkinStateRenderable *deoglSkinState::AddRenderable(){
	deoglSkinStateRenderable * const renderable = new deoglSkinStateRenderable( *this, pRenderableCount );
	
	if( pRenderableCount == pRenderableSize ){
		int newSize = pRenderableSize * 3 / 2 + 1;
		deoglSkinStateRenderable **newArray = new deoglSkinStateRenderable*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pRenderables ){
			memcpy( newArray, pRenderables, sizeof( deoglSkinStateRenderable* ) * pRenderableSize );
			delete [] pRenderables;
		}
		pRenderables = newArray;
		pRenderableSize = newSize;
	}
	
	pRenderables[ pRenderableCount ] = renderable;
	pRenderableCount++;
	
	return renderable;
}

void deoglSkinState::AddRenderables( deoglRSkin &skin, deoglRDynamicSkin &dynamicSkin ){
	const int count = skin.GetRenderableCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglSkinStateRenderable * const renderable = AddRenderable();
		renderable->SetHostRenderable( dynamicSkin.IndexOfRenderableNamed( skin.GetRenderableAt( i ).GetName() ) );
	}
}

void deoglSkinState::RemoveAllRenderables(){
	while( pRenderableCount > 0 ){
		pRenderableCount--;
		delete pRenderables[ pRenderableCount ];
	}
}



// Mapped
///////////

deoglSkinStateMapped &deoglSkinState::GetMappedAt( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pMappedCount )
	
	return pMapped[ index ];
}

void deoglSkinState::SetMappedCount( int count ){
	DEASSERT_TRUE( count >= 0 )
	
	if( pMapped ){
		delete [] pMapped;
		pMapped = nullptr;
		pMappedCount = 0;
	}
	
	if( count > 0 ){
		pMapped = new deoglSkinStateMapped[ count ];
		pMappedCount = count;
	}
}



// Calculated properties
//////////////////////////

deoglSkinStateCalculated &deoglSkinState::GetCalculatedPropertyAt( int index ) const{
	if( index < 0 || index >= pCalculatedPropertyCount ){
		DETHROW( deeInvalidParam );
	}
	return pCalculatedProperties[ index ];
}

void deoglSkinState::SetCalculatedPropertyCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pCalculatedProperties ){
		delete [] pCalculatedProperties;
		pCalculatedProperties = NULL;
		pCalculatedPropertyCount = 0;
	}
	
	if( count > 0 ){
		pCalculatedProperties = new deoglSkinStateCalculated[ count ];
		pCalculatedPropertyCount = count;
	}
}



// Constructed properties
///////////////////////////

deoglSkinStateConstructed &deoglSkinState::GetConstructedPropertyAt( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pConstructedPropertyCount )
	return pConstructedProperties[ index ];
}

void deoglSkinState::SetConstructedPropertyCount( int count ){
	DEASSERT_TRUE( count >= 0 )
	
	if( pConstructedProperties ){
		delete [] pConstructedProperties;
		pConstructedProperties = nullptr;
		pConstructedPropertyCount = 0;
	}
	
	if( count > 0 ){
		pConstructedProperties = new deoglSkinStateConstructed[ count ];
		pConstructedPropertyCount = count;
	}
}

void deoglSkinState::PrepareConstructedProperties(){
	int i;
	for( i=0; i<pConstructedPropertyCount; i++ ){
		pConstructedProperties[ i ].PrepareForRender( *this );
	}
}



// Bones
///////////

deoglSkinStateBone &deoglSkinState::GetBoneAt( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pBoneCount )
	
	return pBones[ index ];
}

void deoglSkinState::SetBoneCount( int count ){
	DEASSERT_TRUE( count >= 0 )
	
	if( pBones ){
		delete [] pBones;
		pBones = nullptr;
		pBoneCount = 0;
	}
	
	if( count > 0 ){
		pBones = new deoglSkinStateBone[ count ];
		pBoneCount = count;
	}
}



void deoglSkinState::InitAll(){
	deoglRSkin * const skin = GetOwnerSkin();
	int i;
	
	if( ! skin ){
		SetMappedCount( 0 );
		SetBoneCount( 0 );
		SetCalculatedPropertyCount( 0 );
		SetConstructedPropertyCount( 0 );
		return;
	}
	
	SetMappedCount( skin->GetMappedCount() );
	for( i=0; i<pMappedCount; i++ ){
		pMapped[ i ].SetMapped( skin->GetMappedAt( i ) );
		pMapped[ i ].SetComponent( pOwnerComponent );
	}
	
	SetBoneCount( skin->GetBoneCount() );
	for( i=0; i<pBoneCount; i++ ){
		pBones[ i ].SetSkinBone( skin->GetBoneAt( i ) );
	}
	
	SetCalculatedPropertyCount( skin->GetCalculatedPropertyCount() );
	for( i=0; i<pCalculatedPropertyCount; i++ ){
		pCalculatedProperties[ i ].SetProperty( skin->GetCalculatedPropertyAt( i ) );
	}
	
	SetConstructedPropertyCount( skin->GetConstructedPropertyCount() );
	for( i=0; i<pConstructedPropertyCount; i++ ){
		pConstructedProperties[ i ].SetProperty( skin->GetConstructedPropertyAt( i ) );
	}
}

void deoglSkinState::UpdateAll(){
	int i;
	for( i=0; i<pMappedCount; i++ ){
		pMapped[ i ].Update( *this );
	}
	for( i=0; i<pCalculatedPropertyCount; i++ ){
		pCalculatedProperties[ i ].Update( *this );
	}
	for( i=0; i<pConstructedPropertyCount; i++ ){
		pConstructedProperties[ i ].Update( *this );
	}
}

void deoglSkinState::MapBonesAll( const deComponent &component ){
	int i;
	for( i=0; i<pMappedCount; i++ ){
		pMapped[ i ].MapBone( component );
	}
	for( i=0; i<pBoneCount; i++ ){
		pBones[ i ].MapBone( component );
	}
}

void deoglSkinState::UpdateBonesAll( const deComponent &component ){
	int i;
	for( i=0; i<pMappedCount; i++ ){
		pMapped[ i ].UpdateBone( component );
	}
	for( i=0; i<pBoneCount; i++ ){
		pBones[ i ].UpdateBone( component );
	}
}



void deoglSkinState::SetVariationSeed( const decPoint &seed ){
	pVariationSeed = seed;
}



// Private Functions
//////////////////////

void deoglSkinState::pSharedInit(){
	pOwnerComponent = NULL;
	pOwnerComponentTexture = 0;
	pOwnerBillboard = NULL;
	pOwnerDecal = NULL;
	pOwnerLight = NULL;
	
	pTime = 0.0f;
	
	pRenderables = NULL;
	pRenderableCount = 0;
	pRenderableSize = 0;
	
	pMapped = nullptr;
	pMappedCount = 0;
	
	pCalculatedProperties = NULL;
	pCalculatedPropertyCount = 0;
	
	pConstructedProperties = nullptr;
	pConstructedPropertyCount = 0;
	
	pBones = nullptr;
	pBoneCount = 0;
	
	pVariationSeed.x = ( int )( ( ( float )rand() / ( float )RAND_MAX ) * 100.0f );
	pVariationSeed.y = ( int )( ( ( float )rand() / ( float )RAND_MAX ) * 100.0f );
}
