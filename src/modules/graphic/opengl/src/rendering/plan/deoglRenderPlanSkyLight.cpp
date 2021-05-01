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

#include "deoglRenderPlanSkyLight.h"
#include "deoglRenderPlan.h"
#include "parallel/deoglRPTSkyLightFindContent.h"
#include "parallel/deoglRPTSkyLightGIPrepare.h"
#include "../light/deoglRenderGI.h"
#include "../light/deoglRenderLight.h"
#include "../light/deoglRLSVisitorCollectElements.h"
#include "../../deGraphicOpenGl.h"
#include "../../collidelist/deoglCollideListComponent.h"
#include "../../component/deoglRComponent.h"
#include "../../debug/debugSnapshot.h"
#include "../../debug/deoglDebugInformation.h"
#include "../../gi/deoglGIState.h"
#include "../../model/deoglRModel.h"
#include "../../occlusiontest/deoglOcclusionTest.h"
#include "../../occlusiontest/deoglOcclusionTestPool.h"
#include "../../rendering/deoglRenderCanvas.h"
#include "../../rendering/task/persistent/deoglPersistentRenderTaskOwner.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../sky/deoglRSkyInstance.h"
#include "../../sky/deoglRSkyInstanceLayer.h"
#include "../../utils/collision/deoglCollisionBox.h"
#include "../../world/deoglRWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/parallel/deParallelProcessing.h>



// Class deoglRenderPlanSkyLight::cComponentChangeListener
////////////////////////////////////////////////////////////

deoglRenderPlanSkyLight::cGIComponentChangeListener::cGIComponentChangeListener( deoglRenderPlanSkyLight &plan ) :
pPlan( plan ){
}

void deoglRenderPlanSkyLight::cGIComponentChangeListener::ComponentDestroyed( deoglRComponent &component ){
	pPlan.GIComponentDestroyed( component );
}

void deoglRenderPlanSkyLight::cGIComponentChangeListener::TUCChanged( deoglRComponent &component ){
	pPlan.GIComponentChangedTUC( component );
}



// Class deoglRenderPlanSkyLight
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderPlanSkyLight::deoglRenderPlanSkyLight( deoglRenderPlan &plan ) :
pPlan( plan ),
pSky( NULL ),
pLayer( NULL ),
pOcclusionTest( NULL ),
pPlanned( false ),
pUseLight( true ),
pUseShadow( false ),
pShadowLayerCount( 0 ),
pGIRenderTask( plan.GetRenderThread().GetPersistentRenderTaskPool() ),
pGIRenderTaskAdd( plan.GetRenderThread(), pGIRenderTask ),
pGIRenderTaskUpdateMarker( false ),
pTaskFindContent( NULL ),
pTaskGIPrepare( NULL )
{
	pGIRenderTaskAdd.SetSkinShaderType( deoglSkinTexture::estComponentShadowOrthogonal );
	pGIRenderTaskAdd.SetSolid( true );
	pGIRenderTaskAdd.SetNoShadowNone( true );
	pGIRenderTaskAdd.SetForceDoubleSided( true );
	//pGIRenderTaskAdd.SetFilterHoles( true );
	//pGIRenderTaskAdd.SetWithHoles( false );
	
	pGIComponentChangeListener.TakeOver( new cGIComponentChangeListener( *this ) );
}

deoglRenderPlanSkyLight::~deoglRenderPlanSkyLight(){
	Clear();
}



// Management
///////////////

void deoglRenderPlanSkyLight::SetOcclusionTest( deoglOcclusionTest *occlusionTest ){
	if( occlusionTest == pOcclusionTest ){
		return;
	}
	
	if( pOcclusionTest ){
		pPlan.GetRenderThread().GetOcclusionTestPool().Return( pOcclusionTest );
	}
	
	pOcclusionTest = occlusionTest;
}

void deoglRenderPlanSkyLight::SetFrustumBoxExtend( const decVector &minExtend, const decVector &maxExtend ){
	pFrustumBoxMinExtend = minExtend;
	pFrustumBoxMaxExtend = maxExtend;
}

void deoglRenderPlanSkyLight::ClearPlanned(){
	pPlanned = false;
}

deoglRenderPlanSkyLight::sShadowLayer &deoglRenderPlanSkyLight::GetShadowLayerAt( int index ){
	if( index < 0 || index >= pShadowLayerCount ){
		DETHROW( deeInvalidParam );
	}
	return pShadowLayers[ index ];
}

const deoglRenderPlanSkyLight::sShadowLayer &deoglRenderPlanSkyLight::GetShadowLayerAt( int index ) const{
	if( index < 0 || index >= pShadowLayerCount ){
		DETHROW( deeInvalidParam );
	}
	return pShadowLayers[ index ];
}

void deoglRenderPlanSkyLight::SetGIRenderTaskUpdateMarker( bool updateMarker ){
	pGIRenderTaskUpdateMarker = updateMarker;
}

void deoglRenderPlanSkyLight::GIComponentDestroyed( deoglRComponent &component ){
	deoglPersistentRenderTaskOwner * const owner = pGIRenderTask.GetOwnerWith( &component, component.GetUniqueKey() );
	if( owner ){
		pGIRenderTask.RemoveOwnedBy( *owner );
		pGIRenderTask.RemoveOwner( owner );
	}
}

void deoglRenderPlanSkyLight::GIComponentChangedTUC( deoglRComponent &component ){
	component.RemoveListener( ( cGIComponentChangeListener* )( deObject* )pGIComponentChangeListener );
	
	deoglPersistentRenderTaskOwner * const owner = pGIRenderTask.GetOwnerWith( &component, component.GetUniqueKey() );
	if( owner ){
		pGIRenderTask.RemoveOwnedBy( *owner );
		pGIRenderTask.RemoveOwner( owner );
	}
}



void deoglRenderPlanSkyLight::Clear(){
	cGIComponentChangeListener * const giccl = ( cGIComponentChangeListener* )( deObject* )pGIComponentChangeListener;
	decPointerLinkedList::cListEntry *iterOwner = pGIRenderTask.GetRootOwner();
	while( iterOwner ){
		const deoglPersistentRenderTaskOwner &owner = *( ( deoglPersistentRenderTaskOwner* )iterOwner->GetOwner() );
		if( owner.GetComponent() ){
			owner.GetComponent()->RemoveListener( giccl );
		}
		iterOwner = iterOwner->GetNext();
	}
	pGIRenderTask.Clear();
	pGICollideList.Clear();
	
	SetOcclusionTest( NULL );
	pCollideList.Clear();
	pUseLight = false;
	pUseShadow = false;
	
	pSky = NULL;
	pLayer = NULL;
}

void deoglRenderPlanSkyLight::SetLayer( deoglRSkyInstance *sky, deoglRSkyInstanceLayer *layer ){
	pSky = sky;
	pLayer = layer;
}

#define DO_SPECIAL_TIMING 1
#ifdef DO_SPECIAL_TIMING
#include <dragengine/common/utils/decTimer.h>
#define INIT_SPECIAL_TIMING decTimer sttimer;
#define SPECIAL_TIMER_PRINT(w) pPlan.GetRenderThread().GetLogger().LogInfoFormat("RenderPlanSkyLight.Prepare: " w "=%dys", (int)(sttimer.GetElapsedTime()*1e6f));
#else
#define INIT_SPECIAL_TIMING
#define SPECIAL_TIMER_PRINT(w)
#endif

void deoglRenderPlanSkyLight::Plan(){
	pPlanned = true;
	
	pDetermineShadowParameters();
	
	if( ! pUseShadow ){
		return;
	}
	
	pCalcShadowLayerParams();
	
	if( pPlan.GetUpdateGIState() ){
		pGICalcShadowLayerParams();
	}
}

void deoglRenderPlanSkyLight::StartFindContent(){
	if( ! pLayer || ! pUseShadow ){
		return;
	}
	
	if( pTaskFindContent || pTaskGIPrepare ){
		DETHROW( deeInvalidParam );
	}
	
	deParallelProcessing &pp = pPlan.GetRenderThread().GetOgl().GetGameEngine()->GetParallelProcessing();
	SetOcclusionTest( pPlan.GetRenderThread().GetOcclusionTestPool().Get() );
	
	pTaskFindContent = new deoglRPTSkyLightFindContent( *this );
	pp.AddTaskAsync( pTaskFindContent );
	
	if( pPlan.GetUpdateGIState() ){
		pTaskGIPrepare = new deoglRPTSkyLightGIPrepare( *this );
		pp.AddTaskAsync( pTaskGIPrepare );
	}
}

void deoglRenderPlanSkyLight::StartOcclusionTests(){
}

void deoglRenderPlanSkyLight::Prepare(){
// 	if( ! pUseShadow ){
// 		return;
// 	}
	
	// TODO start render occlusion test. this is not so time consuming (<0.5ms) but still
	//      reading back the results can be delayed
}

void deoglRenderPlanSkyLight::FinishPrepare(){
	pWaitFinishedFindContent();
	pWaitFinishedGIPrepare();
	
// 	if( pPlan.GetUpdateGIState() ){
// 		pPlan.GetRenderThread().GetRenderers().GetCanvas().ResetDebugInfoTimerGI( pPlan );
// 		pPlan.GetRenderThread().GetRenderers().GetCanvas().SampleDebugInfoPlanPrepareGISkyShadowRenderTask( pPlan );
// 	}
}

void deoglRenderPlanSkyLight::CleanUp(){
	pWaitFinishedFindContent();
	pWaitFinishedGIPrepare();
	
	pGICollideList.Clear();
	pCollideList.Clear();
	pUseLight = false;
	pUseShadow = false;
	SetOcclusionTest( NULL );
}



// Private Functions
//////////////////////

void deoglRenderPlanSkyLight::pDetermineShadowParameters(){
	const deoglConfiguration &config = pPlan.GetRenderThread().GetConfiguration();
	
	pUseLight = true;
	pUseShadow = config.GetDebugUseShadow();
	pShadowLayerCount = 4;
	
	// if array-texture support is missing no shadows for the time being
	if( ! pglFramebufferTexture ){
		pUseShadow = false;
	}
	
	// if there is no direct light scrap the shadows
	if( ! pLayer->GetHasLightDirect() ){
		pUseShadow = false;
	}
	
	// determine if shadow casting has to be used
	/*
	if( plan.GetFBOTarget() ){
		pUseShadow = false;
	}
	*/
	
	// if the total light contribution is a lot smaller than the total ambient only light
	// contribution of the entire sky this layer can be skipped to improve performance.
	// for example in a sky with a sun and moon light the sun typically has 16 intensity
	// and 4 ambient intensity for a total of 20 intensity. the moon light has typically
	// 0.009 intensity and 0.001 ambient intensity for a total of 0.01 intensity. during
	// the day the total moon light (0.01) is a lot less than the sun ambient light (4).
	// while it is possible the sun light is totally blocked and the moon light might
	// be visible this is barely every a use case worth wasting time. in the outlined
	// case the moon light is 0.25% . in general while rendering an intensity difference
	// of 1% can be barely noticed. but most of the time one light dominates so right
	// now a threshold of 10% is used. if the need arises for changing this value
	// a module parameter can be introduced
	if( pLayer->GetTotalLightIntensity() < pSky->GetTotalSkyAmbientIntensity() * 0.1f ){
		pUseLight = false;
		pUseShadow = false;
	}
	
	// temporary hack to deal with slow shadow casting for the video capturing
// 	if( pLayer->GetLightIntensity() < 0.1f ){
// 		pUseShadow = false;
// 	}
}

void deoglRenderPlanSkyLight::pCalcShadowLayerParams(){
	const deoglConfiguration &config = pPlan.GetRenderThread().GetConfiguration();
	const float smOffsetScale = config.GetDistShadowScale();
	//const float smOffsetBias = config.GetDistShadowBias() / ( float )( ( 1 << 24 ) - 1 ); // config.GetShadowMapOffsetBias();
	//const float smOffsetBias = 0.001f; //config.GetDistShadowBias() / ( float )( ( 1 << 16 ) - 1 ); // config.GetShadowMapOffsetBias();
	const float smOffsetBias = config.GetDistShadowBias();
	
//	const deSkyLayer &engSkyLayer = *pLayer->GetLayer();
	const decDMatrix &matCamInv = pPlan.GetInverseCameraMatrix();
	float znear = pPlan.GetCameraImageDistance();
	float zfar = pPlan.GetCameraViewDistance();
		//zfar = 20.0f; // NOTE for better performance cut this value down
	float fov = pPlan.GetCameraFov();
	float fovRatio = pPlan.GetCameraFovRatio();
//	float aspectRatio = plan.GetAspectRatio();
	float factor;
	int p;
	
	float fx, xnear = tanf( fov * 0.5f ); // * znear, dropped since we calc x'=z'*(xnear/znear)
	float fy, ynear = tanf( fov * 0.5f * fovRatio ); // * znear, dropped since we calc y'=z'*(ynear/znear)
	decVector frustumCorners[ 8 ];
	int c;
	
	// get light properties
	//const decMatrix matLig( decMatrix::CreateRotation( 0.0f, PI, 0.0f ) * decMatrix::CreateFromQuaternion( engSkyLayer.GetOrientation() ) );
	const decMatrix matLig( decMatrix::CreateRotation( 0.0f, PI, 0.0f ) * pLayer->GetMatrix() );
	const decMatrix matLigInv( matLig.Invert() );
	const decMatrix matCL( matCamInv.GetRotationMatrix().ToMatrix() * matLigInv );
	
	// calculate the frustum distances
	/*
	frustumNear[ 0 ] = znear;
	frustumNear[ 1 ] = znear + 10.0f;
	frustumNear[ 2 ] = znear + 100.0f;
	frustumFar[ 0 ] = frustumNear[ 1 ] * 1.005f;
	frustumFar[ 1 ] = frustumNear[ 2 ] * 1.005f;
	frustumFar[ 2 ] = zfar;
	
	passCount = 3;
	*/
	
	
	// nvidia:
	// z(i) = near * pow( far / near, i / N ) => has problems
	// z(i) = lambda*near*pow(far/near,i/N) + (1-lambda)*( near + (i/N)*(far-near) )
	//      better version with lambda strength of correction
	
	factor = 0.01f * 1024.0f / ( 2.0f * tanf( fov * 0.5f ) ); // precision * baseShadowSize / ( 2 * tan( fov / 2 ) )
	pShadowLayers[ 0 ].frustumNear = znear;
	pShadowLayers[ 1 ].frustumNear = znear + factor; // roughly 5.12
	pShadowLayers[ 2 ].frustumNear = znear + factor * pShadowLayers[ 1 ].frustumNear; // roughly 26.21
	pShadowLayers[ 3 ].frustumNear = znear + factor * pShadowLayers[ 2 ].frustumNear; // roughly 134.21
	pShadowLayers[ 0 ].frustumFar = pShadowLayers[ 1 ].frustumNear * 1.005f;
	pShadowLayers[ 1 ].frustumFar = pShadowLayers[ 2 ].frustumNear * 1.005f;
	pShadowLayers[ 2 ].frustumFar = pShadowLayers[ 3 ].frustumNear * 1.005f;
	pShadowLayers[ 3 ].frustumFar = zfar;
	pShadowLayers[ 0 ].layerBorder = pShadowLayers[ 1 ].frustumNear * 1.0025f;
	pShadowLayers[ 1 ].layerBorder = pShadowLayers[ 2 ].frustumNear * 1.0025f;
	pShadowLayers[ 2 ].layerBorder = pShadowLayers[ 3 ].frustumNear * 1.0025f;
	pShadowLayers[ 3 ].layerBorder = zfar;
	
	float lambda = 0.96f;//0.5f; // from config
	
	for( p=0; p<pShadowLayerCount; p++ ){
		if( p == pShadowLayerCount - 1 ){
			pShadowLayers[ p ].layerBorder = zfar;
			pShadowLayers[ p ].layerBorder = zfar;
			
		}else{
			factor = ( float )( p + 1 ) / ( float )pShadowLayerCount;
			pShadowLayers[ p ].frustumFar = lambda * znear * powf( zfar / znear, factor )
				+ ( 1.0f - lambda ) * ( znear + factor * ( zfar - znear ) );
			pShadowLayers[ p ].layerBorder = pShadowLayers[ p ].frustumFar * 1.0025f;
		}
		
		if( p == 0 ){
			pShadowLayers[ p ].frustumNear = znear * 1.005f;
			
		}else{
			pShadowLayers[ p ].frustumNear = pShadowLayers[ p - 1 ].frustumFar;
		}
	}
	
	//const float baseDistFactor = 1.0f / ( pShadowLayers[ 0 ].frustumFar - pShadowLayers[ 0 ].frustumNear );
	pShadowLayers[ 0 ].zscale = smOffsetScale;
	pShadowLayers[ 0 ].zoffset = smOffsetBias;
	
	//const float distScale1 = baseDistFactor * ( pShadowLayers[ 1 ].frustumFar - pShadowLayers[ 1 ].frustumNear );
	pShadowLayers[ 1 ].zscale = smOffsetScale;
	pShadowLayers[ 1 ].zoffset = smOffsetBias;
	
	//const float distScale2 = baseDistFactor * ( pShadowLayers[ 2 ].frustumFar - pShadowLayers[ 2 ].frustumNear );
	pShadowLayers[ 2 ].zscale = smOffsetScale;
	pShadowLayers[ 2 ].zoffset = smOffsetBias;
	
	//const float distScale3 = baseDistFactor * ( pShadowLayers[ 3 ].frustumFar - pShadowLayers[ 3 ].frustumNear );
	pShadowLayers[ 3 ].zscale = smOffsetScale;
	pShadowLayers[ 3 ].zoffset = smOffsetBias;
	
	/*
	float / * lambda = 0.5f, * / N = ( float )layerCount;
	ogl.LogInfoFormat( "RenderSkyLight: znear=%f zfar=%f", znear, zfar );
	for( p=0; p<layerCount; p++ ){
		ogl.LogInfoFormat( "layer %i: near=%f far=%f new=%f", p, pShadowLayers[ p ].frustumNear, pShadowLayers[ p ].frustumFar,
			lambda * znear * powf( zfar / znear, ( float )p / N ) + ( 1.0f - lambda ) * ( znear + ( ( float )p / N ) * ( zfar - znear ) ) );
	}
	*/
	
	/*
	ogl.LogInfoFormat( "RenderSkyLight: factor=%f", factor );
	for( p=0; p<layerCount; p++ ){
		ogl.LogInfoFormat( "pass %i: znear=%f zfar=%f", p+1, pShadowLayers[ p ].frustumNear, pShadowLayers[ p ].frustumFar );
	}
	*/
	
	// calculate layer parameters
	for( p=0; p<pShadowLayerCount; p++ ){
		sShadowLayer &sl = pShadowLayers[ p ];
		
		// calculate the 8 corners of the frustum used to create the shadow map
		//frustumNear = znear;
		//frustumFar = znear + 10.0f; // a 10 meter slice close to the player
		fx = xnear * sl.frustumNear;
		fy = ynear * sl.frustumNear;
		matCL.Transform( frustumCorners[ 0 ], -fx,  fy, sl.frustumNear );
		matCL.Transform( frustumCorners[ 1 ],  fx,  fy, sl.frustumNear );
		matCL.Transform( frustumCorners[ 2 ],  fx, -fy, sl.frustumNear );
		matCL.Transform( frustumCorners[ 3 ], -fx, -fy, sl.frustumNear );
		
		fx = xnear * sl.frustumFar;
		fy = ynear * sl.frustumFar;
		matCL.Transform( frustumCorners[ 4 ], -fx,  fy, sl.frustumFar );
		matCL.Transform( frustumCorners[ 5 ],  fx,  fy, sl.frustumFar );
		matCL.Transform( frustumCorners[ 6 ],  fx, -fy, sl.frustumFar );
		matCL.Transform( frustumCorners[ 7 ], -fx, -fy, sl.frustumFar );
		
		// calculate the smallest oriented box fitting around the frustum corners
		sl.minExtend = frustumCorners[ 0 ];
		sl.maxExtend = frustumCorners[ 0 ];
		
		for( c=1; c<8; c++ ){
			if( frustumCorners[ c ].x < sl.minExtend.x ){
				sl.minExtend.x = frustumCorners[ c ].x;
			}
			if( frustumCorners[ c ].y < sl.minExtend.y ){
				sl.minExtend.y = frustumCorners[ c ].y;
			}
			if( frustumCorners[ c ].z < sl.minExtend.z ){
				sl.minExtend.z = frustumCorners[ c ].z;
			}
			if( frustumCorners[ c ].x > sl.maxExtend.x ){
				sl.maxExtend.x = frustumCorners[ c ].x;
			}
			if( frustumCorners[ c ].y > sl.maxExtend.y ){
				sl.maxExtend.y = frustumCorners[ c ].y;
			}
			if( frustumCorners[ c ].z > sl.maxExtend.z ){
				sl.maxExtend.z = frustumCorners[ c ].z;
			}
		}
		
		if( pPlan.GetRenderThread().GetConfiguration().GetDebugSnapshot() == edbgsnapLightSkySplits ){
			const float zf = sl.frustumFar;
			const float zn = sl.frustumNear;
			const float xn = xnear * zn;
			const float yn = ynear * zn;
			const float xf = xnear * zf;
			const float yf = ynear * zf;
			const decDVector &pos = pPlan.GetCameraPosition();
			const decQuaternion &rot = pPlan.GetInverseCameraMatrix().ToQuaternion();
			
			printf( "\n# frustm for split %i\n", p );
			printf( "scale = 0.01\n" );
			printf( "meshFrustum = bpy.data.meshes.new( 'frustum%i' )\n", p );
			printf( "objFrustum = bpy.data.objects.new( 'frustum%i', meshFrustum )\n", p );
			printf( "bpy.context.scene.objects.link( objFrustum )\n" );
			printf( "bpy.context.scene.objects.active = objFrustum\n" );
			printf( "objFrustum.select = True\n" );
			printf( "vertices = []\n" );
			printf( "vertices.append( [%g,%g,%g] )\n", -xn, yn, zn );
			printf( "vertices.append( [%g,%g,%g] )\n", xn, yn, zn );
			printf( "vertices.append( [%g,%g,%g] )\n", xn, -yn, zn );
			printf( "vertices.append( [%g,%g,%g] )\n", -xn, -yn, zn );
			printf( "vertices.append( [%g,%g,%g] )\n", -xf, yf, zf );
			printf( "vertices.append( [%g,%g,%g] )\n", xf, yf, zf );
			printf( "vertices.append( [%g,%g,%g] )\n", xf, -yf, zf );
			printf( "vertices.append( [%g,%g,%g] )\n", -xf, -yf, zf );
			printf( "vertices = [ cposs( v, scale ) for v in vertices ]\n" );
			printf( "faces = []\n" );
			printf( "faces.append( [%i,%i,%i,%i] )\n", 1, 5, 6, 2 );
			printf( "faces.append( [%i,%i,%i,%i] )\n", 4, 0, 3, 7 );
			printf( "faces.append( [%i,%i,%i,%i] )\n", 4, 5, 1, 0 );
			printf( "faces.append( [%i,%i,%i,%i] )\n", 3, 2, 6, 7 );
			printf( "faces.append( [%i,%i,%i,%i] )\n", 5, 4, 7, 6 );
			printf( "faces.append( [%i,%i,%i,%i] )\n", 0, 1, 2, 3 );
			printf( "faces = [ f[::-1] for f in faces ]\n" );
			printf( "meshFrustum.from_pydata( vertices, [], faces )\n" );
			printf( "meshFrustum.update()\n" );
			printf( "objFrustum.location = cposs( [%g,%g,%g], scale )\n", pos.x, pos.y, pos.z );
			printf( "objFrustum.rotation_mode = 'QUATERNION'\n" );
			printf( "objFrustum.rotation_quaternion = cquat( [%g,%g,%g,%g] )\n", rot.x, rot.y, rot.z, rot.w );
		}
		//ogl.LogInfoFormat( "RenderSkyLight: box min=(%f,%f,%f) max=(%f,%f,%f)", minExtend.x, minExtend.y, minExtend.z,
		//	maxExtend.x, maxExtend.y, maxExtend.z );
	}
}

void deoglRenderPlanSkyLight::pWaitFinishedFindContent(){
	if( ! pTaskFindContent ){
		return;
	}
	
// 	pPlan.GetRenderThread().GetLogger().LogInfoFormat( "RenderPlanSkyLight(%p, %p) WaitFinishedFindContent(%p)", this, &pPlan, pTaskFindContent );
	pTaskFindContent->GetSemaphore().Wait();
	pTaskFindContent->FreeReference();
	pTaskFindContent = NULL;
}

void deoglRenderPlanSkyLight::pWaitFinishedGIPrepare(){
	if( ! pTaskGIPrepare ){
		return;
	}
	
	pTaskGIPrepare->GetSemaphore().Wait();
	
	const int countAdded = pTaskGIPrepare->GetCountAdded();
	const int countRemoved = pTaskGIPrepare->GetCountRemoved();
	const int elapsedAdded = pTaskGIPrepare->GetElapsedAdded();
	const int elapsedRemoved = pTaskGIPrepare->GetElapsedRemoved();
	
	pTaskGIPrepare->FreeReference();
	pTaskGIPrepare = NULL;
	
	if( countAdded == 0 && countRemoved == 0 ){
		return;
	}
	
		decTimer timer;
	// this call does modify a shader parameter block and can thus not be parallel
	pGIRenderTask.PrepareForRender( pPlan.GetRenderThread() );
	
	pPlan.GetRenderThread().GetLogger().LogInfoFormat(
		"GIUpdateRenderTask: owners=%d shaders=%d textures=%d vaos=%d instances=%d subinstances=%d (+%d -%d) [%dys; +%dys -%dys]",
		pGIRenderTask.GetOwnerCount(), pGIRenderTask.GetShaderCount(),
		pGIRenderTask.GetTotalTextureCount(), pGIRenderTask.GetTotalVAOCount(),
		pGIRenderTask.GetTotalInstanceCount(), pGIRenderTask.GetTotalSubInstanceCount(),
		countAdded, countRemoved, ( int )( timer.GetElapsedTime() * 1e6f ), elapsedAdded, elapsedRemoved );
	
}

void deoglRenderPlanSkyLight::pGICalcShadowLayerParams(){
	const deoglGIState * const giState = pPlan.GetRenderGIState();
	if( ! giState ){
		return;
	}
	
	const deoglConfiguration &config = pPlan.GetRenderThread().GetConfiguration();
	const decMatrix matLig( decMatrix::CreateRotation( 0.0f, PI, 0.0f ) * pLayer->GetMatrix() );
	
	pGIShadowLayer.frustumNear = 0.0f;
	pGIShadowLayer.frustumFar = 1.0f;
	pGIShadowLayer.layerBorder = 1.0f;
	pGIShadowLayer.zscale = config.GetDistShadowScale();
	pGIShadowLayer.zoffset = config.GetDistShadowBias();
	
	deoglCollisionBox colBoxGI( decVector(), giState->GetDetectionBox(), matLig.Invert().ToQuaternion() );
	
	deoglCollisionBox enclosingBox;
	colBoxGI.GetEnclosingBox( &enclosingBox );
	
	pGIShadowLayer.minExtend = enclosingBox.GetCenter() - enclosingBox.GetHalfSize();
	pGIShadowLayer.maxExtend = enclosingBox.GetCenter() + enclosingBox.GetHalfSize();
}
