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

#include "deoxrInstance.h"
#include "deoxrSession.h"
#include "deoxrSystem.h"
#include "deVROpenXR.h"
#include "deoxrBasics.h"

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
pRunning( false )
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
	
	OXR_CHECK( instance.GetOxr(), instance.xrEndSession( pSession ) );
	
	pRunning = false;
}



// Private Functions
//////////////////////

void deoxrSession::pCleanUp(){
	if( pRunning ){
		const deoxrInstance &instance = pSystem.GetInstance();
		instance.GetOxr().LogInfoFormat( "Exit Session due to shuting down" );
		
		OXR_CHECK( instance.GetOxr(), instance.xrRequestExitSession( pSession ) );
		pRunning = false;
	}
	
	if( pSession ){
		pSystem.GetInstance().xrDestroySession( pSession );
		pSession = 0;
	}
}
