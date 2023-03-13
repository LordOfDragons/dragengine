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

#include "deoxrSwapchain.h"
#include "deoxrSession.h"
#include "deVROpenXR.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class deoxrSwapchain
/////////////////////////

deoxrSwapchain::deoxrSwapchain( deoxrSession &session, const decPoint &size ) :
pSession( session ),
pSize( size ),
pSwapchain( XR_NULL_HANDLE ),
pImages( nullptr ),
pImageCount( 0 )
{
	deoxrInstance &instance = session.GetSystem().GetInstance();
	deVROpenXR &oxr = instance.GetOxr();
	
	try{
		XrSwapchainCreateInfo createInfo;
		memset( &createInfo, 0, sizeof( createInfo ) );
		createInfo.type = XR_TYPE_SWAPCHAIN_CREATE_INFO;
		createInfo.createFlags = 0;
		createInfo.usageFlags = XR_SWAPCHAIN_USAGE_TRANSFER_DST_BIT;
		createInfo.sampleCount = 1;
		createInfo.width = size.x;
		createInfo.height = size.y;
		createInfo.faceCount = 1;
		createInfo.arraySize = 1;
		createInfo.mipCount = 1;
		
		switch( session.GetGraphicApi() ){
		case deoxrSession::egaOpenGL:
			if( session.HasSwapchainFormat( deoxrSession::escfGlRgba16f ) ){
				// seen on SteamVR Windows. OculusVR does not support this
				oxr.LogInfo( "Using Swapchain format GL_RGBA16F" );
				createInfo.format = deoxrSession::escfGlRgba16f; // GL_RGBA16F
				
			}else if( session.HasSwapchainFormat( deoxrSession::escfGlRgba16Ext ) ){
				oxr.LogInfo( "Using Swapchain format GL_RGBA16_EXT" );
				createInfo.format = deoxrSession::escfGlRgba16Ext; // GL_RGBA16_EXT
				
			}else if( session.HasSwapchainFormat( deoxrSession::escfGlRgb16f ) ){
				// seen on OculusVR Windows
				oxr.LogInfo( "Using Swapchain format GL_RGB16F" );
				createInfo.format = deoxrSession::escfGlRgb16f; // GL_RGB16F
				
			}else if( session.HasSwapchainFormat( deoxrSession::escfGlR11fG11fB10f ) ){
				// seen on OculusVR Windows
				oxr.LogInfo( "Using Swapchain format GL_R11F_G11F_B10F" );
				createInfo.format = deoxrSession::escfGlR11fG11fB10f; // GL_R11F_G11F_B10F
				
			}else if( session.HasSwapchainFormat( deoxrSession::escfGlSrgb8Alpha8Ext ) ){
				// seen on SteamVR Linux and OculusVR Windows
				oxr.LogInfo( "Using Swapchain format GL_SRGB8_ALPHA8_EXT" );
				createInfo.format = deoxrSession::escfGlSrgb8Alpha8Ext; // GL_SRGB8_ALPHA8_EXT
				
			}else if( session.HasSwapchainFormat( deoxrSession::escfGlSrgb8Ext ) ){
				// seen on SteamVR Linux and OculusVR Windows
				oxr.LogInfo( "Using Swapchain format GL_SRGB8_EXT" );
				createInfo.format = deoxrSession::escfGlSrgb8Ext; // GL_SRGB8_EXT
				
			}else{
				DETHROW_INFO( deeInvalidParam, "no supported Swapchain format found" );
			}
			break;
			
		default:
			DETHROW_INFO( deeInvalidParam, "no graphic api" );
		}
		
		OXR_CHECK( instance.xrCreateSwapchain( session.GetSession(), &createInfo, &pSwapchain ) );
		
		pGetImages();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoxrSwapchain::~deoxrSwapchain(){
	pCleanUp();
}



// Management
///////////////

const deoxrSwapchain::sImage &deoxrSwapchain::GetImageAt( int index ) const{
	if( index < 0 || index >= pImageCount ){
		DETHROW( deeInvalidParam );
	}
	return pImages[ index ];
}

void deoxrSwapchain::AcquireImage(){
	deoxrInstance &instance = pSession.GetSystem().GetInstance();
	
	// acquire image
	OXR_CHECK( instance.xrAcquireSwapchainImage( pSwapchain, nullptr, &pAcquiredImage ) );
	
	// wait for image
	XrSwapchainImageWaitInfo waitInfo;
	memset( &waitInfo, 0, sizeof( waitInfo ) );
	waitInfo.type = XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO;
	waitInfo.timeout = XR_INFINITE_DURATION;
	
	OXR_CHECK( instance.xrWaitSwapchainImage( pSwapchain, &waitInfo ) );
	
	if( pSession.GetIsGACOpenGL() ){
		// WARNING SteamVR messes with the current context state causing all future OpenGL
		//         calls to fail. not sure why SteamVR unsets the current context but it
		//         breaks everything. i dont know if the spec would actually requires
		//         runtimes to restore current context if they change it but to work
		//         around such annoying runtimes the current context is restored to the
		//         parameters provided by the graphic module
		pSession.RestoreOpenGLCurrent();
	}
}

void deoxrSwapchain::ReleaseImage(){
	deoxrInstance &instance = pSession.GetSystem().GetInstance();
	
	OXR_CHECK( instance.xrReleaseSwapchainImage( pSwapchain, nullptr ) );
	
	if( pSession.GetIsGACOpenGL() ){
		// WARNING SteamVR messes with the current context state causing all future OpenGL
		//         calls to fail. not sure why SteamVR unsets the current context but it
		//         breaks everything. i dont know if the spec would actually requires
		//         runtimes to restore current context if they change it but to work
		//         around such annoying runtimes the current context is restored to the
		//         parameters provided by the graphic module
		pSession.RestoreOpenGLCurrent();
	}
}



// Private Functions
//////////////////////

void deoxrSwapchain::pCleanUp(){
	if( pImages ){
		delete [] pImages;
	}
	
	if( pSwapchain ){
		pSession.GetSystem().GetInstance().xrDestroySwapchain( pSwapchain );
		pSwapchain = XR_NULL_HANDLE;
	}
}

void deoxrSwapchain::pGetImages(){
	deoxrInstance &instance = pSession.GetSystem().GetInstance();
	
	uint32_t count, i;
	OXR_CHECK( instance.xrEnumerateSwapchainImages( pSwapchain, 0, &count, nullptr ) );
	
	if( count == 0 ){
		return;
	}
	
	pImages = new sImage[ count ];
	
	switch( pSession.GetGraphicApi() ){
	case deoxrSession::egaOpenGL:{
		XrSwapchainImageOpenGLKHR *images = new XrSwapchainImageOpenGLKHR[ count ];
		memset( images, 0, sizeof( XrSwapchainImageOpenGLKHR ) * count );
		
		try{
			for( i=0; i<count; i++ ){
				images[ i ].type = XR_TYPE_SWAPCHAIN_IMAGE_OPENGL_KHR;
			}
			
			OXR_CHECK( instance.xrEnumerateSwapchainImages( pSwapchain,
				count, &count, ( XrSwapchainImageBaseHeader* )images ) );
			
			for( i=0; i<count; i++ ){
				pImages[ i ].openglImage = images[ i ].image;
// 				pImages[ i ].openglFbo.TakeOver( new deoxrGraphicApiOpenGL::Framebuffer(
// 					oxr.GetGraphicApiOpenGL(), images[ i ].image ) );
			}
			pImageCount = ( int )count;
			
		}catch( const deException & ){
			delete [] images;
		}
		}break;
		
	default:
		DETHROW_INFO( deeInvalidParam, "invalid graphic api" );
	}
}
