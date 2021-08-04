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
#include <string.h>

#include "deoglRenderPlan.h"
#include "deoglRenderPlanLight.h"
#include "../../collidelist/deoglCollideListLight.h"
#include "../../light/deoglRLight.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../shadow/deoglShadowCaster.h"
#include "../../shadow/deoglSCAmbient.h"
#include "../../shadow/deoglSCSolid.h"
#include "../../shadow/deoglSCTransparent.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderPlanLight
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderPlanLight::deoglRenderPlanLight( deoglRenderPlan &plan ) :
pPlan( plan ),
pLight( NULL ),

pDistance( 0.0f ),
pReductionFactor( 1 ),

pShadowSizeStatic( 0 ),
pTranspShadowSizeStatic( 0 ),
pAmbientShadowSizeStatic( 0 ),

pShadowSizeDynamic( 0 ),
pTranspShadowSizeDynamic( 0 ),
pAmbientShadowSizeDynamic( 0 ){
}

deoglRenderPlanLight::~deoglRenderPlanLight(){
}



// Management
///////////////

void deoglRenderPlanLight::SetLight( deoglCollideListLight *light ){
	pLight = light;
}

void deoglRenderPlanLight::Init(){
	const deoglRLight &light = *pLight->GetLight();
	
	// calculate distance of light to camera
	pPosition = light.GetMatrix().GetPosition();
	pDistance = ( float )( pPosition - pPlan.GetCameraPosition() ).Length();
}

void deoglRenderPlanLight::PlanShadowCasting(){
	// the static shadow maps are used across multiple frames and uses the best resolution.
	// if memory becomes low static shadow maps can be removed from memory and reduced in size.
	// if reduced in size the shadow maps have to be dropped to force rebuilding them.
	// 
	// the dynamic shadow maps are used only during solid geometry rendering and global
	// illumination. although they can be forgotten later especially due to global illumination
	// the shadow mapper can not be used. instead they are stored like static shadow maps
	// but removed from memory if not used for a couple of frames.
	// 
	// transparent shadow maps are used for transparent render passes only. these shadow maps
	// work similar to static ones in that they are keep over multiple render passes (all
	// transparent render passes) then they are forgotten. they are produced from the dynamic
	// shadow maps by down sampling to save time
	const deoglRLight &light = *pLight->GetLight();
	const int minSize = 16;
	
	// static shadow map size
	switch( light.GetLightType() ){
	case deLight::eltSpot:
	case deLight::eltProjector:
		pShadowSizeStatic = pPlan.GetShadowMapSize();
		break;
		
	case deLight::eltPoint:
		pShadowSizeStatic = pPlan.GetShadowCubeSize();
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
	
	// calculate reduction factor
	pCalcReductionFactor();
	
	// reduce static shadow map size
	if( pReductionFactor > 0 ){
		pShadowSizeStatic = decMath::max( pShadowSizeStatic >> pReductionFactor, minSize );
	}
	
	// reduce size for dynamic shadow maps
	pShadowSizeDynamic = decMath::max( pShadowSizeStatic >> 1, minSize );
	
	// reduce size for transparent shadow maps (or use static sizes?)
	pTranspShadowSizeStatic = decMath::max( pShadowSizeStatic >> 1, minSize );
	pTranspShadowSizeDynamic = decMath::max( pShadowSizeDynamic >> 1, minSize );
	
	// temporary hack. calculating the static point shadow map at higher resolution
	// is currently a problem and causes noticeable stutter. reducing the static
	// point shadow map size until this problem is fixed
	/*
	switch( light.GetLightType() ){
	case deLight::eltPoint:
	case deLight::eltSemiPoint:
	case deLight::eltAmbient:
		sizeSolidStatic = decMath::max( pShadowCubeSize >> 1, minSize );
		break;
		
	default:
		break;
	}
	*/
	
	// ambient is same size as base size due to boundary box calculation
	pAmbientShadowSizeStatic = pShadowSizeStatic;
	pAmbientShadowSizeDynamic = pShadowSizeDynamic;
	
	// update next frame sizes. this keeps the largest size across an entire frame
	deoglShadowCaster &shadowCaster = *pLight->GetLight()->GetShadowCaster();
	deoglSCSolid &scsolid = shadowCaster.GetSolid();
	deoglSCTransparent &sctransp = shadowCaster.GetTransparent();
	
	scsolid.SetLargestNextSizeStatic( pShadowSizeStatic );
	scsolid.SetLargestNextSizeDynamic( pShadowSizeDynamic );
	
	sctransp.SetLargestNextSizeStatic( pTranspShadowSizeStatic );
	sctransp.SetLargestNextSizeDynamic( pTranspShadowSizeDynamic );
	
	// clamp sizes to last frame and next frame sizes to avoid resizing textures.
	// using the last frame size avoids resizing if the first rendered camera requires
	// a smaller size than a later one. using the next frame size avoids resiting if
	// the first rendered camera requires a larger size than a later one.
// 	pPlan.GetRenderThread().GetLogger().LogInfoFormat("PlanShadowCasting: %p %d %d %d",
// 		pLight->GetLight(), pShadowSizeDynamic, scsolid.GetLastSizeDynamic(), scsolid.GetNextSizeDynamic() );
	
	pShadowSizeStatic = decMath::max( pShadowSizeStatic,
		scsolid.GetLastSizeStatic(), scsolid.GetNextSizeStatic() );
	
	pShadowSizeDynamic = decMath::max( pShadowSizeDynamic,
		scsolid.GetLastSizeDynamic(), scsolid.GetNextSizeDynamic() );
	
	pTranspShadowSizeStatic = decMath::max( pTranspShadowSizeStatic,
		sctransp.GetLastSizeStatic(), sctransp.GetNextSizeStatic() );
	
	pTranspShadowSizeDynamic = decMath::max( pTranspShadowSizeDynamic,
		sctransp.GetLastSizeDynamic(), sctransp.GetNextSizeDynamic() );
	
	// log values used
#if 0
	static const char * const lightTypeNames[] = { "point", "spot", "projector" };
	pPlan.GetRenderThread().GetLogger().LogInfoFormat(
		"RenderPlanLight: %s (%g,%g,%g): r=%g d=%.1f r=%d size=%d",
		lightTypeNames[ light.GetLightType() ], pPosition.x, pPosition.y, pPosition.z,
		light.GetRange(), pDistance, pReductionFactor, pSolidShadowSizeStatic );
#endif
}



void deoglRenderPlanLight::SetShadowSizeStatic( int size ){
	pShadowSizeStatic = size;
}

void deoglRenderPlanLight::SetTranspShadowSizeStatic( int size ){
	pTranspShadowSizeStatic = size;
}

void deoglRenderPlanLight::SetAmbientShadowSizeStatic( int size ){
	pAmbientShadowSizeStatic = size;
}

void deoglRenderPlanLight::SetShadowSizeDynamic( int size ){
	pShadowSizeDynamic = size;
}

void deoglRenderPlanLight::SetTranspShadowSizeDynamic( int size ){
	pTranspShadowSizeDynamic = size;
}

void deoglRenderPlanLight::SetAmbientShadowSizeDynamic( int size ){
	pAmbientShadowSizeDynamic = size;
}



// Private Functions
//////////////////////

void deoglRenderPlanLight::pCalcReductionFactor(){
	// use distance to scale down shadow maps. this is quite arbitrary but has the goal to
	// avoid lots of lights using high resolution shadow maps if they are seen from far away
	const deoglRLight &light = *pLight->GetLight();
	
	pReductionFactor = 0;
	
	// up to the light range the scaling has to be highest quality
	const float range = decMath::max( light.GetRange(), 1.0f );
	if( pDistance <= range ){
		return;
	}
	
	// for each doubling of the distance reduce the shadow map size by factor 2. the highest
	// supported shadow map size is 4096. the minimum shadow map size is 16. start with a
	// reduction factor of 1 at range. this yields a maximum reduction factor of
	// 8 = log2(4096) - log2(16). clamping is used since reduction factor is used as shift
	// 
	// to be more on the safe side the multiple is reduced by 0.5 . this way the first
	// reduction step happens at 1.5 multiple and the next at 2.5
	const float multiple = ( pDistance / range ) - 0.5f;
	pReductionFactor = decMath::clamp( ( int )log2f( multiple ), 0, 8 );
	
	// NOTE maybe this can be improved by using the light clamp box if present to reduce
	//      the range. this would allow for higher reduction for lights
}
