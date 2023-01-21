/* 
 * Drag[en]gine OpenXR
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#include <stdlib.h>
#include <string.h>

#include "deVROpenXR.h"
#include "deoxrBasics.h"
#include "deoxrInstance.h"
#include "deoxrSession.h"
#include "deoxrSystem.h"
#include "deoxrUtils.h"
#include "action/deoxrActionSet.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicModule.h>

#ifdef OS_UNIX
	#include <dragengine/app/deOSUnix.h>
#elif defined OS_W32
	#include <dragengine/app/deOSWindows.h>
#endif


// class deoxrSession
//////////////////////

deoxrSession::deoxrSession( deoxrSystem &system ) :
pSystem( system ),
pGraphicApi( egaHeadless ),
pSession( 0 ),
pRunning( false ),
pPredictedDisplayTime( 0 ),
pPredictedDisplayPeriod( 0 ),
pShouldRender( false ),
pFrameRunning( false ),
pSwapchainFormats( nullptr ),
pSwapchainFormatCount( 0 ),
pIsGACOpenGL( false ),
#ifdef OS_UNIX
	pGACOpenGLDisplay( nullptr ),
	pGACOpenGLDrawable( 0 ),
	pGACOpenGLContext( nullptr )
#elif defined OS_W32
	pGACOpenGLHDC( NULL ),
	pGACOpenGLContext( NULL )
#endif
{
	const deoxrInstance &instance = system.GetInstance();
	deVROpenXR &oxr = instance.GetOxr();
	
	// initial values based on a vive hmd. graphic modules should not query the perspective
	// value before BeginFrame() has been called but in case this happens these values are
	// used to not be too far away from the actual values
	// 
	// NOTE OpenXR flips up/down and uses angles not projection matrix values
	pLeftEyeFov.angleLeft = atanf( -1.39863f );
	pLeftEyeFov.angleRight = atanf( 1.24906f );
	pLeftEyeFov.angleUp = atanf( 1.47526f );
	pLeftEyeFov.angleDown = atanf( -1.46793f );
	
	pRightEyeFov.angleLeft = atanf( -1.24382f );
	pRightEyeFov.angleRight = atanf( 1.39166f );
	pRightEyeFov.angleUp = atanf( 1.47029f );
	pRightEyeFov.angleDown = atanf( -1.45786f );
	
	try{
		// query graphic api connection parameters
		deBaseGraphicModule::sGraphicApiConnection gacon;
		oxr.GetGameEngine()->GetGraphicSystem()->GetActiveModule()->GetGraphicApiConnection( gacon );
		
		#ifdef OS_BEOS
			#error Unsupported
		#elif defined OS_UNIX
			pGACOpenGLDisplay = ( Display* )gacon.opengl.display;
			pGACOpenGLDrawable = ( GLXDrawable )gacon.opengl.glxDrawable;
			pGACOpenGLContext = ( GLXContext )gacon.opengl.glxContext;
		#elif defined OS_W32
			pGACOpenGLHDC = ( HDC )gacon.opengl.hDC;
			pGACOpenGLContext = ( HGLRC )gacon.opengl.hGLRC;
		#endif
		
		// create session info struct depending on what the graphic module supports
		XrSessionCreateInfo createInfo;
		memset( &createInfo, 0, sizeof( createInfo ) );
		createInfo.type = XR_TYPE_SESSION_CREATE_INFO;
		createInfo.systemId = system.GetSystemId();
		
		const void *graphicBinding = nullptr;
		
		// opengl
		#ifdef OS_UNIX
		XrGraphicsBindingOpenGLXlibKHR gbopengl;
		#elif defined OS_W32
		XrGraphicsBindingOpenGLWin32KHR gbopengl;
		#endif
		
		if( instance.SupportsExtension( deoxrInstance::extKHROpenglEnable ) ){
			oxr.LogInfo( "Create Session: Testing OpenGL Support" );
			// openxr specification requires this call to be done although the result is not used
			XrGraphicsRequirementsOpenGLKHR requirements;
			memset( &requirements, 0, sizeof( requirements ) );
			requirements.type = XR_TYPE_GRAPHICS_REQUIREMENTS_OPENGL_KHR;
			
			instance.xrGetOpenGLGraphicsRequirementsKHR(
				instance.GetInstance(), system.GetSystemId(), &requirements );
			
			// add opengl connection struct
			#ifdef OS_UNIX
				if( gacon.opengl.display && gacon.opengl.display
				&& gacon.opengl.glxFBConfig && gacon.opengl.glxDrawable ){
					oxr.GetGraphicApiOpenGL().Load();
					
					memset( &gbopengl, 0, sizeof( gbopengl ) );
					gbopengl.type = XR_TYPE_GRAPHICS_BINDING_OPENGL_XLIB_KHR;
					gbopengl.xDisplay = pGACOpenGLDisplay;
					gbopengl.visualid = gacon.opengl.visualid;
					gbopengl.glxFBConfig = ( GLXFBConfig )gacon.opengl.glxFBConfig;
					gbopengl.glxDrawable = pGACOpenGLDrawable;
					gbopengl.glxContext = pGACOpenGLContext;
					
					pGraphicApi = egaOpenGL;
					graphicBinding = &gbopengl;
					pIsGACOpenGL = true;
					oxr.LogInfo( "Create Session: Using OpenGL on Xlib" );
				}
				
			#elif defined OS_W32
				if( gacon.opengl.hDC && gacon.opengl.hGLRC ){
					oxr.GetGraphicApiOpenGL().Load();
					
					memset( &gbopengl, 0, sizeof( gbopengl ) );
					gbopengl.type = XR_TYPE_GRAPHICS_BINDING_OPENGL_WIN32_KHR;
					gbopengl.hDC = ( HDC )gacon.opengl.hDC;
					gbopengl.hGLRC = ( HGLRC )gacon.opengl.hGLRC;
					
					pGraphicApi = egaOpenGL;
					graphicBinding = &gbopengl;
					pIsGACOpenGL = true;
					oxr.LogInfo( "Create Session: Using OpenGL on Windows" );
				}
			#endif
		}
		
		if( ! graphicBinding ){
			DETHROW_INFO( deeInvalidAction, "no supported graphic api" );
		}
		
		createInfo.next = graphicBinding;
		
		// create session
		OXR_CHECK( instance.xrCreateSession( instance.GetInstance(), &createInfo, &pSession ) );
		
		// create spaces
		pSpaceStage.TakeOver( new deoxrSpace( *this, XR_REFERENCE_SPACE_TYPE_STAGE ) );
		pSpaceView.TakeOver( new deoxrSpace( *this, XR_REFERENCE_SPACE_TYPE_VIEW ) );
		
		// enumerate swapchain formats
		pEnumSwapchainFormats();
		
		// create swap chains
		pSwapchainLeftEye.TakeOver( new deoxrSwapchain( *this, pSystem.GetLeftEyeViewSize() ) );
		pSwapchainRightEye.TakeOver( new deoxrSwapchain( *this, pSystem.GetRightEyeViewSize() ) );
		
		if( pIsGACOpenGL ){
			// WARNING SteamVR messes with the current context state causing all future OpenGL
			//         calls to fail. not sure why SteamVR unsets the current context but it
			//         breaks everything. i dont know if the spec would actually requires
			//         runtimes to restore current context if they change it but to work
			//         around such annoying runtimes the current context is restored to the
			//         parameters provided by the graphic module
			RestoreOpenGLCurrent();
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoxrSession::~deoxrSession(){
	pCleanUp();
}



// Management
///////////////

void deoxrSession::Begin(){
	if( pRunning ){
		return;
	}
	
	const deoxrInstance &instance = pSystem.GetInstance();
	instance.GetOxr().LogInfoFormat( "Begin Session" );
	
	XrSessionBeginInfo beginInfo;
	memset( &beginInfo, 0, sizeof( beginInfo ) );
	beginInfo.type = XR_TYPE_SESSION_BEGIN_INFO;
	beginInfo.primaryViewConfigurationType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;
	
	OXR_CHECK( instance.xrBeginSession( pSession, &beginInfo ) );
	
	pRunning = true;
}

void deoxrSession::End(){
	if( ! pRunning ){
		return;
	}
	
	const deoxrInstance &instance = pSystem.GetInstance();
	instance.GetOxr().LogInfoFormat( "End Session" );
	
	EndFrame();
	pAttachedActionSet = nullptr;
	
	if( ! instance.GetOxr().GetPreventDeletion() ){
		// prevent deletion of graphic api resources that are typically linked to another
		// thread. this will cause memory leaks but better leak than crash if the runtime
		// is buggy or not very resiliant (like SteamVR for example)
		OXR_CHECK( instance.xrEndSession( pSession ) );
	}
	
	pRunning = false;
	pPredictedDisplayTime = 0;
	pPredictedDisplayPeriod = 0;
	pShouldRender = false;
}

void deoxrSession::ForceEnd(){
	if( ! pRunning ){
		return;
	}
	
	const deoxrInstance &instance = pSystem.GetInstance();
	instance.GetOxr().LogInfoFormat( "Force End Session" );
	ForceEndFrame();
	pAttachedActionSet = nullptr;
	if( ! instance.GetOxr().GetPreventDeletion() ){
		instance.xrEndSession( pSession );
	}
	pRunning = false;
	pPredictedDisplayTime = 0;
	pPredictedDisplayPeriod = 0;
	pShouldRender = false;
}

void deoxrSession::AttachActionSet( deoxrActionSet *actionSet ){
	if( ! actionSet ){
		DETHROW_INFO( deeNullPointer, "actionSet" );
	}
	
	const deoxrInstance &instance = pSystem.GetInstance();
	instance.GetOxr().LogInfoFormat( "Attach Action Set: %s", actionSet->GetLocalizedName().GetString() );
	
	XrSessionActionSetsAttachInfo attachInfo;
	memset( &attachInfo, 0, sizeof( attachInfo ) );
	attachInfo.type = XR_TYPE_SESSION_ACTION_SETS_ATTACH_INFO;
	
	XrActionSet actionSets[ 1 ] = { actionSet->GetActionSet() };
	attachInfo.countActionSets = 1;
	attachInfo.actionSets = actionSets;
	
	OXR_CHECK( instance.xrAttachSessionActionSets( pSession, &attachInfo ) );
	
	pAttachedActionSet = actionSet;
}

void deoxrSession::WaitFrame(){
	if( ! pRunning || pFrameRunning ){
		return;
	}
	
	const deoxrInstance &instance = pSystem.GetInstance();
	
	// synchronization
	XrFrameState state;
	memset( &state, 0, sizeof( state ) );
	state.type = XR_TYPE_FRAME_STATE;
	
	OXR_CHECK( instance.xrWaitFrame( pSession, nullptr, &state ) );
	
	pPredictedDisplayTime = state.predictedDisplayTime;
	pPredictedDisplayPeriod = state.predictedDisplayPeriod;
	pShouldRender = state.shouldRender;
}

void deoxrSession::BeginFrame(){
	if( ! pRunning || pFrameRunning ){
		return;
	}
	
	const deoxrInstance &instance = pSystem.GetInstance();
	
	// begin frame
	OXR_CHECK( instance.xrBeginFrame( pSession, nullptr ) );
	pFrameRunning = true;
	
	// locate views
	XrViewState viewState;
	memset( &viewState, 0, sizeof( viewState ) );
	viewState.type = XR_TYPE_VIEW_STATE;
	
	XrView views[ 2 ];
	memset( &views, 0, sizeof( views ) );
	views[ 0 ].type = XR_TYPE_VIEW;
	views[ 1 ].type = XR_TYPE_VIEW;
	
	XrViewLocateInfo viewLocateInfo;
	memset( &viewLocateInfo, 0, sizeof( viewLocateInfo ) );
	viewLocateInfo.type = XR_TYPE_VIEW_LOCATE_INFO;
	viewLocateInfo.displayTime = pPredictedDisplayTime;
	viewLocateInfo.space = pSpaceStage->GetSpace();
	viewLocateInfo.viewConfigurationType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;
	
	uint32_t viewCount;
	OXR_CHECK( instance.xrLocateViews( pSession, &viewLocateInfo, &viewState, 2, &viewCount, views ) );
	
	pLeftEyePose = views[ 0 ].pose;
	pLeftEyeFov = views[ 0 ].fov;
	
	pRightEyePose = views[ 1 ].pose;
	pRightEyeFov = views[ 1 ].fov;
	
	// if hidden mesh are supported but not existing create and update them. this is done
	// only once unless an event is received the images changed. theoretically if the fov
	// changes the mask could change too but this is nigh impossible to ever happen
	if( instance.SupportsExtension( deoxrInstance::extKHRVisibilityMask ) ){
		if( ! pLeftEyeHiddenMesh ){
			pLeftEyeHiddenMesh.TakeOver( new deoxrHiddenMesh( *this,
				XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO, 0 ) );
			UpdateLeftEyeHiddenMesh();
		}
		
		if( ! pRightEyeHiddenMesh ){
			pRightEyeHiddenMesh.TakeOver( new deoxrHiddenMesh( *this,
				XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO, 1 ) );
			UpdateRightEyeHiddenMesh();
		}
	}
	
	// locate hmd
	if( pPredictedDisplayTime > 0 ){
		pSpaceView->LocateSpace( pSpaceStage, pPredictedDisplayTime,
			pHeadPosition, pHeadOrientation, pHeadLinearVelocity, pHeadAngularVelocity );
		
		/*
		instance.GetOxr().LogInfoFormat( "Locate HMD: pos=(%g,%g,%g) rot=(%g,%g,%g) lv=(%g,%g,%g) av=(%g,%g,%g)",
			pHeadMatrix.GetPosition().x, pHeadMatrix.GetPosition().y, pHeadMatrix.GetPosition().z,
			pHeadMatrix.GetEulerAngles().x * RAD2DEG, pHeadMatrix.GetEulerAngles().y * RAD2DEG,
				pHeadMatrix.GetEulerAngles().z * RAD2DEG,
			pHeadLinearVelocity.x, pHeadLinearVelocity.y, pHeadLinearVelocity.z,
			pHeadAngularVelocity.x * RAD2DEG, pHeadAngularVelocity.y * RAD2DEG,
				pHeadAngularVelocity.z * RAD2DEG );
		*/
		
		// calculate eye matrices transforming from camera space to eye space. we have to do
		// this calculation since OpenXR does not provide this information. this happens
		// though once each frame update so not a performance problem
		const decMatrix headMatrix( decMatrix::CreateWorld( pHeadPosition, pHeadOrientation ) );
		pLeftEyeMatrix = headMatrix.QuickMultiply( deoxrUtils::Convert( pLeftEyePose ).QuickInvert() );
		pRightEyeMatrix = headMatrix.QuickMultiply( deoxrUtils::Convert( pRightEyePose ).QuickInvert() );
		
		/*
		instance.GetOxr().LogInfoFormat( "- Left Eye: pos=(%g,%g,%g) rot=(%g,%g,%g)",
			pLeftEyeMatrix.GetPosition().x, pLeftEyeMatrix.GetPosition().y, pLeftEyeMatrix.GetPosition().z,
			pLeftEyeMatrix.GetEulerAngles().x * RAD2DEG, pLeftEyeMatrix.GetEulerAngles().y * RAD2DEG,
				pLeftEyeMatrix.GetEulerAngles().z * RAD2DEG );
		
		instance.GetOxr().LogInfoFormat( "- Right Eye: pos=(%g,%g,%g) rot=(%g,%g,%g)",
			pRightEyeMatrix.GetPosition().x, pRightEyeMatrix.GetPosition().y, pRightEyeMatrix.GetPosition().z,
			pRightEyeMatrix.GetEulerAngles().x * RAD2DEG, pRightEyeMatrix.GetEulerAngles().y * RAD2DEG,
				pRightEyeMatrix.GetEulerAngles().z * RAD2DEG );
		*/
	}
}

void deoxrSession::EndFrame(){
	if( ! pRunning || ! pFrameRunning ){
		return;
	}
	
	const deoxrInstance &instance = pSystem.GetInstance();
	
	// end frame. views have to be submitted even if not rendered to otherwise runtimes
	// like SteamVR can crash entire OpenGL
	XrFrameEndInfo endInfo;
	memset( &endInfo, 0, sizeof( endInfo ) );
	
	endInfo.type = XR_TYPE_FRAME_END_INFO;
	endInfo.environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE;
	endInfo.displayTime = pPredictedDisplayTime;
	
	XrCompositionLayerProjectionView views[ 2 ];
	memset( &views, 0, sizeof( views ) );
	
	views[ 0 ].type = XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW;
	views[ 0 ].subImage.swapchain = pSwapchainLeftEye->GetSwapchain();
	views[ 0 ].subImage.imageRect.extent.width = pSwapchainLeftEye->GetSize().x;
	views[ 0 ].subImage.imageRect.extent.height = pSwapchainLeftEye->GetSize().y;
	views[ 0 ].subImage.imageRect.offset.x = 0;
	views[ 0 ].subImage.imageRect.offset.y = 0;
	views[ 0 ].pose = pLeftEyePose;
	views[ 0 ].fov = pLeftEyeFov;
	
	views[ 1 ].type = XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW;
	views[ 1 ].subImage.swapchain = pSwapchainRightEye->GetSwapchain();
	views[ 1 ].subImage.imageRect.extent.width = pSwapchainRightEye->GetSize().x;
	views[ 1 ].subImage.imageRect.extent.height = pSwapchainRightEye->GetSize().y;
	views[ 1 ].subImage.imageRect.offset.x = 0;
	views[ 1 ].subImage.imageRect.offset.y = 0;
	views[ 1 ].pose = pRightEyePose;
	views[ 1 ].fov = pRightEyeFov;
	
	const XrCompositionLayerBaseHeader *layers[ 2 ];
	int layerCount = 0;
	
	const deoxrPassthrough * const passthrough = instance.GetOxr().GetPassthrough();
	if( passthrough && passthrough->GetEnabled() && passthrough->GetTransparency() > 0.001f ){
		layers[ layerCount++ ] = ( const XrCompositionLayerBaseHeader* )&passthrough->GetCompositeLayer();
	}
	
	XrCompositionLayerProjection layerProjection;
	memset( &layerProjection, 0, sizeof( layerProjection ) );
	
	layerProjection.type = XR_TYPE_COMPOSITION_LAYER_PROJECTION;
// 	layerProjection.layerFlags = XR_COMPOSITION_LAYER_BLEND_TEXTURE_SOURCE_ALPHA_BIT;
	layerProjection.space = pSpaceStage->GetSpace();
	layerProjection.viewCount = 2;
	layerProjection.views = views;
	layers[ layerCount++ ] = ( const XrCompositionLayerBaseHeader* )&layerProjection;
	
	endInfo.layerCount = layerCount;
	endInfo.layers = layers;
	
	OXR_CHECK( instance.xrEndFrame( pSession, &endInfo ) );
	pFrameRunning = false;
	
	if( pIsGACOpenGL ){
		// WARNING SteamVR messes with the current context state causing all future OpenGL
		//         calls to fail. not sure why SteamVR unsets the current context but it
		//         breaks everything. i dont know if the spec would actually requires
		//         runtimes to restore current context if they change it but to work
		//         around such annoying runtimes the current context is restored to the
		//         parameters provided by the graphic module
		RestoreOpenGLCurrent();
	}
}

void deoxrSession::ForceEndFrame(){
	if( ! pRunning || ! pFrameRunning ){
		return;
	}
	
	XrFrameEndInfo endInfo;
	memset( &endInfo, 0, sizeof( endInfo ) );
	endInfo.type = XR_TYPE_FRAME_END_INFO;
	endInfo.environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE;
	endInfo.displayTime = pPredictedDisplayTime;
	endInfo.layerCount = 0;
	pSystem.GetInstance().xrEndFrame( pSession, &endInfo );
	pFrameRunning = false;
}

void deoxrSession::SyncActions(){
	if( ! pRunning || ! pAttachedActionSet ){
		return;
	}
	
	const deoxrInstance &instance = pSystem.GetInstance();
	
	XrActionsSyncInfo syncInfo;
	memset( &syncInfo, 0, sizeof( syncInfo ) );
	
	syncInfo.type = XR_TYPE_ACTIONS_SYNC_INFO;
	
	XrActiveActionSet activeActionSets[ 1 ];
	activeActionSets[ 0 ].actionSet = pAttachedActionSet->GetActionSet();
	activeActionSets[ 0 ].subactionPath = XR_NULL_PATH;
	
	syncInfo.countActiveActionSets = 1;
	syncInfo.activeActionSets = activeActionSets;
	
	OXR_CHECK( instance.xrSyncActions( pSession, &syncInfo ) );
}

void deoxrSession::UpdateLeftEyeHiddenMesh(){
	pLeftEyeHiddenMesh->SetFov( pLeftEyeFov );
	pLeftEyeHiddenMesh->UpdateModel();
}

void deoxrSession::UpdateRightEyeHiddenMesh(){
	pRightEyeHiddenMesh->SetFov( pRightEyeFov );
	pRightEyeHiddenMesh->UpdateModel();
}

void deoxrSession::RestoreOpenGLCurrent(){
	#ifdef OS_UNIX
		pSystem.GetInstance().GetOxr().GetGraphicApiOpenGL().MakeCurrent(
			pGACOpenGLDisplay, pGACOpenGLDrawable, pGACOpenGLContext );
		
	#elif defined OS_W32
		pSystem.GetInstance().GetOxr().GetGraphicApiOpenGL().MakeCurrent(
			pGACOpenGLHDC, pGACOpenGLContext );
	#endif
}



// Private Functions
//////////////////////

void deoxrSession::pCleanUp(){
	if( pRunning ){
		const deoxrInstance &instance = pSystem.GetInstance();
		instance.GetOxr().LogInfoFormat( "Exit Session due to shuting down" );
		
		OXR_CHECK( instance.xrRequestExitSession( pSession ) );
		pRunning = false;
		pPredictedDisplayTime = 0;
		pPredictedDisplayPeriod = 0;
		pShouldRender = false;
		
		// wait until ready to exit
		/*
		instance.GetOxr().LogInfoFormat( "Waiting for exit request to be acknowledged" );
		pSystem.GetInstance().GetOxr().WaitUntilReadyExit();
		
		instance.GetOxr().LogInfoFormat( "Exit request acknowledged" );
		*/
		End();
	}
	
	pAttachedActionSet = nullptr;
	
	pSwapchainLeftEye = nullptr;
	pSwapchainRightEye = nullptr;
	   pSpaceStage = nullptr;
	
	if( pSession ){
		pSystem.GetInstance().xrDestroySession( pSession );
		pSession = XR_NULL_HANDLE;
	}
	
	if( pSwapchainFormats ){
		delete [] pSwapchainFormats;
		pSwapchainFormats = nullptr;
	}
}

void deoxrSession::pEnumSwapchainFormats(){
	const deoxrInstance &instance = pSystem.GetInstance();
	instance.GetOxr().LogInfoFormat( "Enumerate Swapchain Formats:" );
	
	uint32_t count;
	OXR_CHECK( instance.xrEnumerateSwapchainFormats( pSession, 0, &count, nullptr ) );
	if( count == 0 ){
		return;
	}
	
	pSwapchainFormats = new int64_t[ count ];
	OXR_CHECK( instance.xrEnumerateSwapchainFormats( pSession, count, &count, pSwapchainFormats ) );
	
	uint32_t i;
	for( i=0; i<count; i++ ){
		const int format = ( int )pSwapchainFormats[ i ];
		instance.GetOxr().LogInfoFormat( "- %d (0x%x)", format, format );
	}
	
	/*
	Enumerate Swapchain Formats:
	- 32859 (0x805b) => GL_RGBA16_EXT
	- 34842 (0x881a) => GL_RGBA16F
	- 34843 (0x881b) => GL_RGB16F
	- 35905 (0x8c41) => GL_SRGB8_EXT
	- 35907 (0x8c43) => GL_SRGB8_ALPHA8_EXT
	- 33189 (0x81a5) => GL_DEPTH_COMPONENT16_SGIX
	- 33190 (0x81a6) => GL_DEPTH_COMPONENT24_SGIX
	- 33191 (0x81a7) => GL_DEPTH_COMPONENT32_SGIX
	*/
}
