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
pSession( 0 ),
pRunning( false ),
pFrameRunning( false )
{
	const deoxrInstance &instance = system.GetInstance();
	deVROpenXR &oxr = instance.GetOxr();
	
	try{
		// query graphic api connection parameters
		deBaseGraphicModule::sGraphicApiConnection gacon;
		oxr.GetGameEngine()->GetGraphicSystem()->GetActiveModule()->GetGraphicApiConnection( gacon );
		
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
					memset( &gbopengl, 0, sizeof( gbopengl ) );
					gbopengl.type = XR_TYPE_GRAPHICS_BINDING_OPENGL_XLIB_KHR;
					gbopengl.xDisplay = ( Display* )gacon.opengl.display;
					gbopengl.visualid = gacon.opengl.visualid;
					gbopengl.glxFBConfig = ( GLXFBConfig )gacon.opengl.glxFBConfig;
					gbopengl.glxDrawable = ( GLXDrawable )gacon.opengl.glxDrawable;
					gbopengl.glxContext = ( GLXContext )gacon.opengl.glxContext;
					graphicBinding = &gbopengl;
					oxr.LogInfo( "Create Session: Using OpenGL on Xlib" );
				}
				
			#elif defined OS_W32
				if( gacon.opengl.hDC && gacon.opengl.hGLRC ){
					memset( &gbopengl, 0, sizeof( gbopengl ) );
					gbopengl.type = XR_TYPE_GRAPHICS_BINDING_OPENGL_WIN32_KHR;
					gbopengl.hDC = ( HDC )gacon.opengl.hDC;
					gbopengl.hGLRC = ( HGLRC )gacon.opengl.hGLRC;
					graphicBinding = &gbopengl;
					oxr.LogInfo( "Create Session: Using OpenGL on Windows" );
				}
			#endif
		}
		
		if( ! graphicBinding ){
			DETHROW_INFO( deeInvalidAction, "no supported graphic api" );
		}
		
		createInfo.next = graphicBinding;
		
		OXR_CHECK( system.GetInstance().GetOxr(), system.GetInstance().xrCreateSession(
			system.GetInstance().GetInstance(), &createInfo, &pSession ) );
		
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
	beginInfo.primaryViewConfigurationType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;
	
	OXR_CHECK( instance.GetOxr(), instance.xrBeginSession( pSession, &beginInfo ) );
	
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
	
	OXR_CHECK( instance.GetOxr(), instance.xrEndSession( pSession ) );
	
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
	
	OXR_CHECK( instance.GetOxr(), instance.xrAttachSessionActionSets( pSession, &attachInfo ) );
	
	pAttachedActionSet = actionSet;
}

void deoxrSession::BeginFrame(){
	if( ! pRunning || pFrameRunning ){
		return;
	}
	
	const deoxrInstance &instance = pSystem.GetInstance();
	
	// synchronization
	XrFrameState state;
	memset( &state, 0, sizeof( state ) );
	state.type = XR_TYPE_FRAME_STATE;
	
	OXR_CHECK( instance.GetOxr(), instance.xrWaitFrame( pSession, nullptr, &state ) );
	
	pPredictedDisplayTime = state.predictedDisplayTime;
	pPredictedDisplayPeriod = state.predictedDisplayPeriod;
	pShouldRender = state.shouldRender;
	
	// begin frame
	OXR_CHECK( instance.GetOxr(), instance.xrBeginFrame( pSession, nullptr ) );
}

void deoxrSession::EndFrame(){
	if( ! pRunning || ! pFrameRunning ){
		return;
	}
	
	const deoxrInstance &instance = pSystem.GetInstance();
	
	// end frame
	XrFrameEndInfo endInfo;
	memset( &endInfo, 0, sizeof( endInfo ) );
	
	endInfo.type = XR_TYPE_FRAME_END_INFO;
	endInfo.environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE;
	
	if( pShouldRender ){
		endInfo.layerCount = 0;
		
	}else{
		endInfo.layerCount = 0;
	}
	
	OXR_CHECK( instance.GetOxr(), instance.xrEndFrame( pSession, nullptr ) );
}

void deoxrSession::SyncActions(){
	if( ! pRunning || ! pAttachedActionSet ){
		return;
	}
	
	const deoxrInstance &instance = pSystem.GetInstance();
	
	XrActionsSyncInfo syncInfo;
	memset( &syncInfo, 0, sizeof( syncInfo ) );
	
	syncInfo.type = XR_TYPE_ACTIONS_SYNC_INFO;
	
	XrActiveActionSet activeActionSets[ 1 ] = {
		{ pAttachedActionSet->GetActionSet(), XR_NULL_PATH }
	};
	syncInfo.countActiveActionSets = 1;
	syncInfo.activeActionSets = activeActionSets;
	
	OXR_CHECK( instance.GetOxr(), instance.xrSyncActions( pSession, nullptr ) );
}



// Private Functions
//////////////////////

void deoxrSession::pCleanUp(){
	if( pRunning ){
		const deoxrInstance &instance = pSystem.GetInstance();
		instance.GetOxr().LogInfoFormat( "Exit Session due to shuting down" );
		
		OXR_CHECK( instance.GetOxr(), instance.xrRequestExitSession( pSession ) );
		pRunning = false;
		pPredictedDisplayTime = 0;
		pPredictedDisplayPeriod = 0;
		pShouldRender = false;
	}
	
	pAttachedActionSet = nullptr;
	
	if( pSession ){
		pSystem.GetInstance().xrDestroySession( pSession );
		pSession = 0;
	}
}
