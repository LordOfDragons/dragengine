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

#include "deGraphicOpenGl.h"

#include "billboard/deoglBillboard.h"

#include "configuration/deoglConfiguration.h"

#include "font/deoglFont.h"

#include "world/deoglWorld.h"
#include "world/deoglCamera.h"

#include "light/deoglLight.h"

#include "collidelist/deoglCollideListManager.h"

#include "commands/deoglCommandExecuter.h"

#include "component/deoglComponent.h"

#include "decal/deoglDecal.h"

#include "debugdrawer/deoglDebugDrawer.h"

#include "effects/deoglEffectColorMatrix.h"
#include "effects/deoglEffectDistortImage.h"
#include "effects/deoglEffectFilterKernel.h"
#include "effects/deoglEffect.h"
#include "effects/deoglEffectOverlayImage.h"

#include "model/deoglModel.h"

#include "occlusiontest/mesh/deoglOcclusionMesh.h"

#include "parameters/deoglParameter.h"
#include "parameters/deoglParameterList.h"
#include "parameters/deoglPLogLevel.h"
#include "parameters/ao/deoglPAOSelfShadowEnable.h"
#include "parameters/ao/deoglPAOSelfShadowSmoothAngle.h"
#include "parameters/debug/deoglPDebugContext.h"
#include "parameters/debug/deoglPDebugNoCulling.h"
#include "parameters/debug/deoglPDebugShowCB.h"
#include "parameters/debug/deoglPDebugUseShadow.h"
#include "parameters/debug/deoglPQuickDebug.h"
#include "parameters/debug/deoglPShowLightCB.h"
#include "parameters/debug/deoglPOcclusionReduction.h"
#include "parameters/debug/deoglPOccTestMode.h"
#include "parameters/debug/deoglPWireframeMode.h"
#include "parameters/defren/deoglPDefRenEncDepth.h"
#include "parameters/defren/deoglPDefRenSizeLimit.h"
#include "parameters/defren/deoglPDefRenUsePOTs.h"
#include "parameters/defren/deoglPHDRRMaximumIntensity.h"
#include "parameters/defren/deoglPRenderDownScale.h"
#include "parameters/defren/deoglPTranspLayerLimit.h"
#include "parameters/defren/deoglPAsyncRenderSkipSyncTimeRatio.h"
#include "parameters/defren/deoglPFrameRateLimit.h"
#include "parameters/light/deoglPLightCutOffIntensity.h"
#include "parameters/lod/deoglPLODMaxErrorPerLevel.h"
#include "parameters/lod/deoglPLODMaxPixelError.h"
#include "parameters/norRoughCorr/deoglPNorRougCorrStrength.h"
#include "parameters/postprocessing/deoglPBrightness.h"
#include "parameters/postprocessing/deoglPContrast.h"
#include "parameters/postprocessing/deoglPGammaCorrection.h"
#include "parameters/ssr/deoglPSSRCoverageEdgeSize.h"
#include "parameters/ssr/deoglPSSRCoveragePowerEdge.h"
#include "parameters/ssr/deoglPSSRCoveragePowerRayLength.h"
#include "parameters/ssr/deoglPSSREnable.h"
#include "parameters/ssr/deoglPSSRMaxRayLength.h"
#include "parameters/ssr/deoglPSSRReduction.h"
#include "parameters/ssr/deoglPSSRStepCount.h"
#include "parameters/ssao/deoglPSSAOEdgeBlurThreshold.h"
#include "parameters/ssao/deoglPSSAOEnable.h"
#include "parameters/ssao/deoglPSSAOMipMapBase.h"
#include "parameters/ssao/deoglPSSAORadius.h"
#include "parameters/ssao/deoglPSSAORadiusLimit.h"
#include "parameters/ssao/deoglPSSAOSelfOcclusionAngle.h"
#include "parameters/ssao/deoglPSSAOTapCount.h"
#include "parameters/ssao/deoglPSSAOTurnCount.h"
#include "parameters/shadow/deoglPShadowCubePCFSize.h"
#include "parameters/shadow/deoglPShadowCubeSize.h"
#include "parameters/shadow/deoglPShadowMapOffsetBias.h"
#include "parameters/shadow/deoglPShadowMapOffsetScale.h"
#include "parameters/shadow/deoglPShadowMapSize.h"

#include "particle/deoglParticleEmitter.h"
#include "particle/deoglParticleEmitterInstance.h"

#include "propfield/deoglPropField.h"

#include "rendering/cache/deoglRenderCache.h"

#include "window/deoglRenderWindow.h"

#include "sensor/deoglLumimeter.h"

#include "skin/deoglSkin.h"
#include "skin/dynamic/deoglDynamicSkin.h"

#include "sky/deoglSky.h"
#include "sky/deoglSkyInstance.h"

#include "smoke/deoglSmokeEmitter.h"

#include "terrain/heightmap/deoglHeightTerrain.h"

#include "texture/deoglImage.h"

#include "video/deoglVideo.h"
#include "video/deoglVideoPlayer.h"
#include "envmap/deoglEnvMapProbe.h"
#include "deoglCaches.h"
#include "canvas/deoglCanvasImage.h"
#include "canvas/deoglCanvasPaint.h"
#include "canvas/deoglCanvasRenderWorld.h"
#include "canvas/deoglCanvasText.h"
#include "canvas/deoglCanvasVideoPlayer.h"
#include "canvas/deoglCanvasView.h"
#include "canvas/deoglCanvasCanvasView.h"
#include "canvas/capture/deoglCaptureCanvas.h"
#include "renderthread/deoglRenderThread.h"
#include "renderthread/deoglRTContext.h"
#include "configuration/deoglLSConfiguration.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/resources/canvas/deCanvasVisitorIdentify.h>
#include <dragengine/resources/canvas/deCanvas.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/effect/deEffect.h>
#include <dragengine/resources/effect/deEffectColorMatrix.h>
#include <dragengine/resources/effect/deEffectDistortImage.h>
#include <dragengine/resources/effect/deEffectFilterKernel.h>
#include <dragengine/resources/effect/deEffectOverlayImage.h>
#include <dragengine/resources/effect/deEffectVisitorIdentify.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyInstance.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/systems/modules/deModuleParameter.h>


// export definition
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *OpenGLCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif


// entry function
// used to create a opengl graphic module.
// has to be named CreateModule returning deBaseModule.
// returns NULL on error.
/////////////////////////////////////////////////////////
deBaseModule *OpenGLCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	try{
		module = new deGraphicOpenGl( *loadableModule );
	}catch( const deException &e ){
		e.PrintError();
		return NULL;
	}
	return module;
}



// Class deGraphicOpenGl
//////////////////////////

// Constructor, destructor
////////////////////////////

deGraphicOpenGl::deGraphicOpenGl( deLoadableModule &loadableModule ) :
deBaseGraphicModule( loadableModule ),

pCommandExecuter( *this ),

pRenderWindowList( *this ),
pCaptureCanvasList( *this ),

pRenderThread( NULL ),
pCaches( NULL ),
pDebugOverlay( *this )
{
	pCreateParameters();
	pLoadConfig();
}

deGraphicOpenGl::~deGraphicOpenGl(){
	CleanUp();
}



// Management
///////////////

bool deGraphicOpenGl::Init( deRenderWindow *renderWindow ){
	if( ! renderWindow ){
		return false;
	}
	
	if( pConfiguration.GetDoLogInfo() ){
		LogInfo( "Init" );
	}
	
	try{
		pCaches = new deoglCaches( *this );
		
		pRenderThread = new deoglRenderThread( *this ); // make this a permanently existing object just with Init/CleanUp
		pRenderThread->Init( renderWindow );
		
	}catch( const deException &e ){
		e.PrintError();
		return false;
	}
	
	return true;
}

void deGraphicOpenGl::CleanUp(){
	if( pConfiguration.GetDoLogInfo() ){
		LogInfo( "CleanUp" );
	}
	
	if( pRenderThread ){
		pRenderThread->CleanUp();
		delete pRenderThread;
		pRenderThread = NULL;
	}
	
	deoglLSConfiguration saveConfig( *this );
	saveConfig.SaveConfig( pConfiguration );
	
	if( pCaches ){
		delete pCaches;
		pCaches = NULL;
	}
	
	LogInfo( "Done CleanUp" );
}

void deGraphicOpenGl::InputOverlayCanvasChanged(){
}

#ifdef ANDROID
/** \brief Application window has been created. */
void deGraphicOpenGl::InitAppWindow(){
	if( pConfiguration.GetDoLogInfo() ){
		LogInfo( "InitAppWindow" );
	}
	if( pRenderThread ){
		pRenderThread->InitAppWindow();
	}
}

/** \brief Application window has been closed. */
void deGraphicOpenGl::TerminateAppWindow(){
	if( pConfiguration.GetDoLogInfo() ){
		LogInfo( "TerminateAppWindow" );
	}
	if( pRenderThread ){
		pRenderThread->TerminateAppWindow();
	}
}
#endif



// Frame Management
/////////////////////

// #include <dragengine/common/utils/decTimer.h>
void deGraphicOpenGl::RenderWindows(){
// 	decTimer timer;
	// wait for rendering to finish. if done asynchronously uses time history to judge if
	// rendering is finished soon enough to wait for this event or to skip synchronization
	// and running another game frame update
// 	LogInfoFormat( "RenderWindows() %d", __LINE__ );
	if( ! pRenderThread->MainThreadWaitFinishRendering() ){
// 		LogInfoFormat( "RenderWindows() %d", __LINE__ );
		return; // enough time left to run another game frame update
	}
// 		LogInfoFormat( "RenderWindows: MainThreadWaitFinishRendering = %d ys", (int)(timer.GetElapsedTime() * 1e6f) );
	
#ifdef OS_ANDROID
	pRenderThread->DebugMemoryUsage( "deGraphicOpenGl::RenderWindows ENTER" );
#endif
	// finalize asynchronously loaded resources
// 	LogInfoFormat( "RenderWindows() %d", __LINE__ );
	pRenderThread->FinalizeAsyncResLoading();
// 		LogInfoFormat( "RenderWindows: FinalizeAsyncResLoading = %d ys", (int)(timer.GetElapsedTime() * 1e6f) );
	
	// synchronize capture canvas. this creates images from pixel buffers for finished
	// captures and prepares new captures if present
// 	LogInfoFormat( "RenderWindows() %d", __LINE__ );
	pCaptureCanvasList.SyncToRender();
// 		LogInfoFormat( "RenderWindows: CaptureCanvasList.Sync = %d ys", (int)(timer.GetElapsedTime() * 1e6f) );
	
	// synchronize windows. this is done in the main thread and is a synchronization point
// 	LogInfoFormat( "RenderWindows() %d", __LINE__ );
	pRenderWindowList.SyncToRender(); 
// 		LogInfoFormat( "RenderWindows: RenderWindowList.Sync = %d ys", (int)(timer.GetElapsedTime() * 1e6f) );
	
	// synchronize overlay canvas view if present
	deCanvasView * const inputOverlayCanvas = GetGameEngine()->GetGraphicSystem()->GetInputOverlayCanvas();
	if( inputOverlayCanvas ){
		deoglCanvas &oglCanvas = *( ( deoglCanvas* )inputOverlayCanvas->GetPeerGraphic() );
		oglCanvas.SyncToRender();
		pRenderThread->SetCanvasInputOverlay( oglCanvas.GetRCanvas() );
	}
	
	pDebugOverlay.PrepareOverlay( *GetGameEngine()->GetGraphicSystem()->GetDebugOverlayCanvas() );
// 	LogInfoFormat( "RenderWindows() %d", __LINE__ );
	
	// synchronize render thread and trigger next render cycle
	pRenderThread->Synchronize();
// 	LogInfoFormat( "RenderWindows() %d", __LINE__ );
// 		LogInfoFormat( "RenderWindows: RenderThread.Sync = %d ys", (int)(timer.GetElapsedTime() * 1e6f) );
#ifdef OS_ANDROID
	pRenderThread->DebugMemoryUsage( "deGraphicOpenGl::RenderWindows EXIT" );
#endif
}



deBaseGraphicBillboard *deGraphicOpenGl::CreateBillboard( deBillboard *billboard ){
	return new deoglBillboard( *this, *billboard );
}

deBaseGraphicCanvas *deGraphicOpenGl::CreateCanvas( deCanvas *canvas ){
	deCanvasVisitorIdentify identify;
	canvas->Visit( identify );
	
	switch( identify.GetType() ){
	case deCanvasVisitorIdentify::ectImage:
		return new deoglCanvasImage( *this, identify.CastToImage() );
		
	case deCanvasVisitorIdentify::ectPaint:
		return new deoglCanvasPaint( *this, identify.CastToPaint() );
		
	case deCanvasVisitorIdentify::ectCanvasView:
		return new deoglCanvasCanvasView( *this, identify.CastToCanvasView() );
		
	case deCanvasVisitorIdentify::ectRenderWorld:
		return new deoglCanvasRenderWorld( *this, identify.CastToRenderWorld() );
		
	case deCanvasVisitorIdentify::ectText:
		return new deoglCanvasText( *this, identify.CastToText() );
		
	case deCanvasVisitorIdentify::ectVideoPlayer:
		return new deoglCanvasVideoPlayer( *this, identify.CastToVideoPlayer() );
		
	case deCanvasVisitorIdentify::ectView:
		return new deoglCanvasView( *this, identify.CastToView() );
		
	default:
		DETHROW( deeInvalidParam );
	}
}

deBaseGraphicCaptureCanvas *deGraphicOpenGl::CreateCaptureCanvas( deCaptureCanvas *captureCanvas ){
	return new deoglCaptureCanvas( *this, *captureCanvas );
}

deBaseGraphicCamera *deGraphicOpenGl::CreateCamera( deCamera *camera ){
	return new deoglCamera( *this, *camera );
}

deBaseGraphicComponent *deGraphicOpenGl::CreateComponent( deComponent *component ){
	return new deoglComponent( *this, *component );
}

deBaseGraphicDebugDrawer *deGraphicOpenGl::CreateDebugDrawer( deDebugDrawer *debugDrawer ){
	return new deoglDebugDrawer( *this, *debugDrawer );
}

deBaseGraphicDecal *deGraphicOpenGl::CreateDecal( deDecal *decal ){
	return new deoglDecal( *this, *decal );
}

deBaseGraphicDynamicSkin *deGraphicOpenGl::CreateDynamicSkin( deDynamicSkin *dynamicSkin ){
	return new deoglDynamicSkin( *this, *dynamicSkin );
}

deBaseGraphicEffect *deGraphicOpenGl::CreateEffect( deEffect *effect ){
	deEffectVisitorIdentify identify;
	
	effect->Visit( identify );
	
	switch( identify.GetType() ){
	case deEffectVisitorIdentify::eetColorMatrix:
		return new deoglEffectColorMatrix( *this, identify.CastToColorMatrix() );
		
	case deEffectVisitorIdentify::eetDistortImage:
		return new deoglEffectDistortImage( *this, identify.CastToDistortImage() );
		
	case deEffectVisitorIdentify::eetFilterKernel:
		return new deoglEffectFilterKernel( *this, identify.CastToFilterKernel() );
		
	case deEffectVisitorIdentify::eetOverlayImage:
		return new deoglEffectOverlayImage( *this, identify.CastToOverlayImage() );
		
	default:
		DETHROW( deeInvalidParam );
	}
}

deBaseGraphicEnvMapProbe *deGraphicOpenGl::CreateEnvMapProbe( deEnvMapProbe *envMapProbe ){
	return new deoglEnvMapProbe( *this, *envMapProbe );
}

deBaseGraphicFont *deGraphicOpenGl::CreateFont( deFont *font ){
	return new deoglFont( *this, *font );
}

deBaseGraphicHeightTerrain *deGraphicOpenGl::CreateHeightTerrain( deHeightTerrain *heightTerrain ){
	return new deoglHeightTerrain( *this, *heightTerrain );
}

deBaseGraphicImage *deGraphicOpenGl::CreateImage(deImage *image){
	return new deoglImage( *this, *image );
}

deBaseGraphicLight *deGraphicOpenGl::CreateLight( deLight *light ){
	return new deoglLight( *this, *light );
}

deBaseGraphicLumimeter *deGraphicOpenGl::CreateLumimeter( deLumimeter *lumimeter ){
	return new deoglLumimeter( *this, *lumimeter );
}

deBaseGraphicModel *deGraphicOpenGl::CreateModel( deModel *model ){
	return new deoglModel( *this, *model );
}

deBaseGraphicOcclusionMesh *deGraphicOpenGl::CreateOcclusionMesh( deOcclusionMesh *occmesh ){
	return new deoglOcclusionMesh( *this, *occmesh );
}

deBaseGraphicParticleEmitter *deGraphicOpenGl::CreateParticleEmitter( deParticleEmitter *emitter ){
	return new deoglParticleEmitter( *this, *emitter );
}

deBaseGraphicParticleEmitterInstance *deGraphicOpenGl::CreateParticleEmitterInstance( deParticleEmitterInstance *instance ){ 
	return new deoglParticleEmitterInstance( *this, *instance );
}

deBaseGraphicPropField *deGraphicOpenGl::CreatePropField( dePropField *propField ){
	return new deoglPropField( *this, *propField );
}

deBaseGraphicRenderWindow *deGraphicOpenGl::CreateRenderWindow( deRenderWindow *renderWindow ){
	return new deoglRenderWindow( *this, *renderWindow );
}

deBaseGraphicSkin *deGraphicOpenGl::CreateSkin( deSkin *skin ){
	return new deoglSkin( *this, *skin );
}

deBaseGraphicSky *deGraphicOpenGl::CreateSky( deSky *sky ){
	return new deoglSky( *this, *sky );
}

deBaseGraphicSkyInstance *deGraphicOpenGl::CreateSkyInstance( deSkyInstance *instance ){
	return new deoglSkyInstance( *this, *instance );
}

deBaseGraphicSmokeEmitter *deGraphicOpenGl::CreateSmokeEmitter( deSmokeEmitter *smokeEmitter ){
	return new deoglSmokeEmitter( this, smokeEmitter );
}

deBaseGraphicVideo *deGraphicOpenGl::CreateVideo( deVideo *video ){
	return new deoglVideo( *this, *video );
}

deBaseGraphicVideoPlayer *deGraphicOpenGl::CreateVideoPlayer( deVideoPlayer *videoPlayer ){
	return new deoglVideoPlayer( *this, *videoPlayer );
}

deBaseGraphicWorld *deGraphicOpenGl::CreateWorld( deWorld *world ){
	return new deoglWorld( *this, *world );
}



// Parameters
///////////////

int deGraphicOpenGl::GetParameterCount() const{
	return pParameters.GetParameterCount();
}

void deGraphicOpenGl::GetParameterInfo( int index, deModuleParameter &info ) const{
	info = pParameters.GetParameterAt( index );
}

int deGraphicOpenGl::IndexOfParameterNamed( const char *name ) const{
	return pParameters.IndexOfParameterNamed( name );
}

decString deGraphicOpenGl::GetParameterValue( const char *name ) const{
	return pParameters.GetParameterNamed( name ).GetParameterValue();
}

void deGraphicOpenGl::SetParameterValue( const char *name, const char *value ){
	pParameters.GetParameterNamed( name ).SetParameterValue( value );
}



void deGraphicOpenGl::PauseParallelProcessingUpdate(){
	if( ! pRenderThread ){
		return;
	}
	
	pRenderThread->FinalizeAsyncResLoading();
}

void deGraphicOpenGl::SendCommand( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	pCommandExecuter.ExecuteCommand( command, answer );
}



// configuration
//////////////////

bool deGraphicOpenGl::HasRenderThread() const{
	return pRenderThread != NULL;
}



// Private Functions
//////////////////////

void deGraphicOpenGl::pLoadConfig(){
	deoglLSConfiguration loadConfig( *this );
	loadConfig.LoadConfig( pConfiguration );
	
	#ifdef ANDROID
	pConfiguration.SetLogLevel( deoglConfiguration::ellDebug );
	#endif
}

void deGraphicOpenGl::pCreateParameters() {
	pParameters.AddParameter( new deoglPLogLevel( *this ) );
	pParameters.AddParameter( new deoglPFrameRateLimit( *this ) );
	pParameters.AddParameter( new deoglPAsyncRenderSkipSyncTimeRatio( *this ) );
	
	pParameters.AddParameter( new deoglPGammaCorrection( *this ) );
	pParameters.AddParameter( new deoglPContrast( *this ) );
	pParameters.AddParameter( new deoglPBrightness( *this ) );
	pParameters.AddParameter( new deoglPRenderDownScale( *this ) );
	
	pParameters.AddParameter( new deoglPLODMaxPixelError( *this ) );
	pParameters.AddParameter( new deoglPLODMaxErrorPerLevel( *this ) );
	
	pParameters.AddParameter( new deoglPNorRougCorrStrength( *this ) );
	
	pParameters.AddParameter( new deoglPSSREnable( *this ) );
	pParameters.AddParameter( new deoglPSSRStepCount( *this ) );
	pParameters.AddParameter( new deoglPSSRMaxRayLength( *this ) );
	pParameters.AddParameter( new deoglPSSRReduction( *this ) );
	pParameters.AddParameter( new deoglPSSRCoverageEdgeSize( *this ) );
	pParameters.AddParameter( new deoglPSSRCoveragePowerEdge( *this ) );
	pParameters.AddParameter( new deoglPSSRCoveragePowerRayLength( *this ) );
	
	pParameters.AddParameter( new deoglPAOSelfShadowEnable( *this ) );
	pParameters.AddParameter( new deoglPAOSelfShadowSmoothAngle( *this ) );
	
	pParameters.AddParameter( new deoglPSSAOEnable( *this ) );
	pParameters.AddParameter( new deoglPSSAOTapCount( *this ) );
	pParameters.AddParameter( new deoglPSSAORadius( *this ) );
	pParameters.AddParameter( new deoglPSSAOSelfOcclusionAngle( *this ) );
	pParameters.AddParameter( new deoglPSSAOEdgeBlurThreshold( *this ) );
	pParameters.AddParameter( new deoglPSSAORadiusLimit( *this ) );
	pParameters.AddParameter( new deoglPSSAOMipMapBase( *this ) );
	pParameters.AddParameter( new deoglPSSAOTurnCount( *this ) );
	
	pParameters.AddParameter( new deoglPLightCutOffIntensity( *this ) );
	pParameters.AddParameter( new deoglPShadowMapSize( *this ) );
	pParameters.AddParameter( new deoglPShadowMapOffsetScale( *this ) );
	pParameters.AddParameter( new deoglPShadowMapOffsetBias( *this ) );
	pParameters.AddParameter( new deoglPShadowCubeSize( *this ) );
	pParameters.AddParameter( new deoglPShadowCubePCFSize( *this ) );
	
	pParameters.AddParameter( new deoglPHDRRMaximumIntensity( *this ) );
	pParameters.AddParameter( new deoglPDefRenEncDepth( *this ) );
	pParameters.AddParameter( new deoglPDefRenUsePOTs( *this ) );
	pParameters.AddParameter( new deoglPDefRenSizeLimit( *this ) );
	pParameters.AddParameter( new deoglPTranspLayerLimit( *this ) );
	
#ifdef WITH_DEBUG
	pParameters.AddParameter( new deoglPDebugContext( *this ) );
	pParameters.AddParameter( new deoglPDebugNoCulling( *this ) );
	pParameters.AddParameter( new deoglPDebugShowCB( *this ) );
	pParameters.AddParameter( new deoglPDebugUseShadow( *this ) );
	pParameters.AddParameter( new deoglPOcclusionReduction( *this ) );
	pParameters.AddParameter( new deoglPOccTestMode( *this ) );
	pParameters.AddParameter( new deoglPQuickDebug( *this ) );
	pParameters.AddParameter( new deoglPShowLightCB( *this ) );
	pParameters.AddParameter( new deoglPWireframeMode( *this ) );
#endif
}
