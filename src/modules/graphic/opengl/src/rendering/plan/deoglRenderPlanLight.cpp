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
pReductionFactorStatic( 1 ),
pReductionFactorDynamic( 1 ),

pShadowSizeStatic( 0 ),
pTranspShadowSizeStatic( 0 ),
pAmbientShadowSizeStatic( 0 ),

pShadowSizeDynamic( 0 ),
pTranspShadowSizeDynamic( 0 ),
pAmbientShadowSizeDynamic( 0 ),

pGIShadowSizeDynamic( 0 ),

pUseShadow( false ),
pUseShadowTemporary( false ),
pUseAmbient( false ),
pRefilterShadows( false ){
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
	pDetermineUseShadow();
	pDetermineUseAmbient();
	pDetermineRefilterShadows();
	
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
	
	pShadowSizeDynamic = pShadowSizeStatic;
	
	pGIShadowSizeDynamic = pShadowSizeStatic;
	
	// calculate reduction factors
	pCalcReductionFactorStatic();
	pCalcReductionFactorDynamic();
	
	// for temporary shadow casting reduce shadow map size. this is done to avoid requesting
	// and keeping large shared textures. rendering time is not affected much by size
	int giReductionFactorDynamic = decMath::max( 2, pReductionFactorDynamic );
	
	if( pUseShadowTemporary ){
		pReductionFactorStatic += 2;
		pReductionFactorDynamic += 2;
		
		giReductionFactorDynamic += 2;
	}
	
	// reduce shadow map size
	if( pReductionFactorStatic > 0 ){
		pShadowSizeStatic = decMath::max( pShadowSizeStatic >> pReductionFactorStatic, minSize );
	}
	
	if( pReductionFactorDynamic > 0 ){
		pShadowSizeDynamic = decMath::max( pShadowSizeDynamic >> pReductionFactorDynamic, minSize );
	}
	
	pGIShadowSizeDynamic = decMath::max( pShadowSizeDynamic >> giReductionFactorDynamic, minSize );
	
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
	
	// manipulate cached shadows only if not using temporary shadows
	if( ! pUseShadowTemporary ){
		// update next frame sizes. this keeps the largest size across an entire frame
		deoglShadowCaster &shadowCaster = *pLight->GetLight()->GetShadowCaster();
		deoglSCTransparent &sctransp = shadowCaster.GetTransparent();
		deoglSCAmbient &scambient = shadowCaster.GetAmbient();
		deoglSCSolid &scsolid = shadowCaster.GetSolid();
		
		scsolid.SetLargestNextSizeStatic( pShadowSizeStatic );
		scsolid.SetLargestNextSizeDynamic( pShadowSizeDynamic );
		
		sctransp.SetLargestNextSizeStatic( pTranspShadowSizeStatic );
		sctransp.SetLargestNextSizeDynamic( pTranspShadowSizeDynamic );
		
		scambient.SetLargestNextSizeStatic( pAmbientShadowSizeStatic );
		scambient.SetLargestNextSizeDynamic( pAmbientShadowSizeDynamic );
		
		// clamp sizes to last frame and next frame sizes to avoid resizing textures.
		// using the last frame size avoids resizing if the first rendered camera requires
		// a smaller size than a later one. using the next frame size avoids resizing if
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
		
		pAmbientShadowSizeStatic = decMath::max( pAmbientShadowSizeStatic,
			scambient.GetLastSizeStatic(), scambient.GetNextSizeStatic() );
		
		pAmbientShadowSizeDynamic = decMath::max( pAmbientShadowSizeDynamic,
			scambient.GetLastSizeDynamic(), scambient.GetNextSizeDynamic() );
	}
	
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

void deoglRenderPlanLight::pCalcReductionFactorStatic(){
	// use distance to scale down shadow maps. this is quite arbitrary but has the goal to
	// avoid lots of lights using high resolution shadow maps if they are seen from far away
	const deoglRLight &light = *pLight->GetLight();
	
	pReductionFactorStatic = 0;
	
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
	pReductionFactorStatic = decMath::clamp( ( int )log2f( multiple ), 0, 8 );
	
	// NOTE maybe this can be improved by using the light clamp box if present to reduce
	//      the range. this would allow for higher reduction for lights
}

void deoglRenderPlanLight::pCalcReductionFactorDynamic(){
	// two possible solutions.
	// 
	// the first one is to use a reduction factor 1 higher than static. this causes all
	// dynamic shadows to be half the size of their static sibligns. while reducing memory
	// consuumption a lot and render time a bit the quality difference is noticable
	// especially in smaller resolutions.
	// 
	// the second solution is to use the same calculation as pCalcReductionFactorStatic()
	// but reducing light range by 50%. this causes close up dynamic shadows to retain
	// the good quality level with farther away dynamic shadows reducing resolution quicker
	// than the static shadows. this reduces memory consumption faster and has a similar
	// effect without sacrificing quality up close
	
	// version 1
// 	pReductionFactorDynamic = pReductionFactorStatic + 1;
	
	// version 2
	pReductionFactorDynamic = 0;
	
// 	const float rangeReduction = 1.0f; // consumption 4x compared to version 1
	const float rangeReduction = 0.5f; // consumption ~2.6x compared to version 1
// 	const float rangeReduction = 0.4f; // consumption ~2.4x compared to version 1
// 	const float rangeReduction = 0.25f; // consumption ~2x compared to version 1
	
	const float range = decMath::max( pLight->GetLight()->GetRange(), 1.0f ) * rangeReduction;
	if( pDistance <= range ){
		return;
	}
	
	const float multiple = ( pDistance / range ) - 0.5f;
	pReductionFactorDynamic = decMath::clamp( ( int )log2f( multiple ), 0, 8 );
}

void deoglRenderPlanLight::pDetermineUseShadow(){
	pUseShadow = pPlan.GetRenderThread().GetConfiguration().GetShadowQuality() != deoglConfiguration::esqOff;
	pUseShadowTemporary = false;
	
	// no shadows if the light wishes so
	if( ! pLight->GetLight()->GetCastShadows() ){
		pUseShadow = false;
	}
	
	// it can also happen that the light has nothing to cast shadows with
	// so in this case shadow casting is disabled too as it is faster
	const deoglShadowCaster::eShadowTypes shadowType = pLight->GetLight()->GetShadowCaster()->GetShadowType();
	if( shadowType == deoglShadowCaster::estNoShadows ){
		pUseShadow = false;
	}
}

void deoglRenderPlanLight::pDetermineUseAmbient(){
	pUseAmbient = ! pPlan.GetNoAmbientLight();
	
	// disable ambient if GI is used. for rendering uses faster shader. for boundary
	// calculation uses only depth map. ambient is only required if GI is disabled
	// since then lights have no way to do ambient lighting otherwise
	if( pPlan.GetRenderGIState() ){
		pUseAmbient = false;
	}
}

void deoglRenderPlanLight::pDetermineRefilterShadows(){
	// if layer mask restriction is used dynamic only shadows have to be used to filter properly.
	// the logic is this. lights filter scene elements to be included in their shadow maps by
	// matching the element "layer mask" against the "shadow layer mask". if the camera restricts
	// the layer mask this filtering stays correct if all bits of the "shadow layer mask" are
	// covered by the bits of the "camery layer mask".
	// 
	// as a side note it would be also possible for this rule to not apply if not all bits of
	// the "shadow layer mask" match the "camera layer mask". this requires or combining all
	// layer masks of all filtered scene elements. if this combined layer mask does match in
	// all bits the "camera layer mask" then this would be enough to still fullfil the
	// requirement to use the static shadow maps.
	// TODO check if this special filter check should be added or not
	pRefilterShadows = pPlan.GetUseLayerMask()
		&& ! pLight->GetLight()->StaticMatchesCamera( pPlan.GetLayerMask() );
	
	if( pRefilterShadows ){
		// if refiltering is used temporary shadow maps have to use used to avoid rebuilding
		// cached shadow maps slowing rendering down and producing hitches
		pUseShadowTemporary = true;
	}
}
