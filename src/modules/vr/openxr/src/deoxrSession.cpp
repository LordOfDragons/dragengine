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

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>

#ifdef OS_UNIX
	#include <dragengine/app/deOSUnix.h>
#elif defined OS_W32
	#include <dragengine/app/deOSWindows.h>
#endif


// class deoxrSession
//////////////////////

deoxrSession::deoxrSession( deoxrSystem &system ) :
pSystem( system ),
pSession( 0 )
{
	try{
		XrSessionCreateInfo createInfo;
		memset( &createInfo, 0, sizeof( createInfo ) );
		createInfo.type = XR_TYPE_SESSION_CREATE_INFO;
		createInfo.systemId = system.GetSystemId();
		
		const void **next = &createInfo.next;
		
		#ifdef OS_UNIX
			XrGraphicsBindingOpenGLXlibKHR graphics;
			memset( &graphics, 0, sizeof( graphics ) );
			graphics.type = XR_TYPE_GRAPHICS_BINDING_OPENGL_XLIB_KHR;
			graphics.xDisplay = 0; // glXGetCurrentDisplay()
			graphics.visualid = 0; // glXGetVisualFromFBConfig(Display*, GLXFBConfig);
			graphics.glxFBConfig = 0;
			graphics.glxDrawable = 0; // glXGetCurrentDrawable()
			graphics.glxContext = 0; // glXGetCurrentContext()
			
		#elif defined OS_W32
			XrGraphicsBindingOpenGLWin32KHR graphics;
			memset( &graphics, 0, sizeof( graphics ) );
			graphics.type = XR_TYPE_GRAPHICS_BINDING_OPENGL_WIN32_KHR;
			graphics.hDC = NULL;
			graphics.hGLRC = NULL;
			
		#else
			#error not supported
		#endif
		
		*next = &graphics;
		next = &graphics.next;
		
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



// Private Functions
//////////////////////

void deoxrSession::pCleanUp(){
	if( pSession ){
		pSystem.GetInstance().xrDestroySession( pSession );
		pSession = 0;
	}
}
