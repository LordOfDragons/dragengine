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

deoxrSwapchain::deoxrSwapchain( deoxrSession &session ) :
pSession( session ),
pSwapchain( XR_NULL_HANDLE ),
pImages( nullptr ),
pImageCount( 0 )
{
	try{
		XrSwapchainCreateInfo createInfo;
		memset( &createInfo, 0, sizeof( createInfo ) );
		createInfo.type = XR_TYPE_SWAPCHAIN_CREATE_INFO;
		createInfo.createFlags = 0;
		createInfo.usageFlags = XR_SWAPCHAIN_USAGE_TRANSFER_DST_BIT;
		createInfo.sampleCount = 1;
		createInfo.width = 1668;
		createInfo.height = 1856;
		createInfo.faceCount = 1;
		createInfo.arraySize = 1;
		createInfo.mipCount = 1;
		
		switch( session.GetGraphicApi() ){
		case deoxrSession::egaOpenGL:
			createInfo.format = 0x881a; // GL_RGBA16F;
			break;
			
		default:
			DETHROW_INFO( deeInvalidParam, "no graphic api" );
		}
		
		OXR_CHECK( session.GetSystem().GetInstance().GetOxr(),
			session.GetSystem().GetInstance().xrCreateSwapchain(
				session.GetSession(), &createInfo, &pSwapchain ) );
		
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

void deoxrSwapchain::AcquireImage(){
	deoxrInstance &instance = pSession.GetSystem().GetInstance();
	
	// acquire image
	OXR_CHECK( instance.GetOxr(), instance.xrAcquireSwapchainImage( pSwapchain, nullptr, &pAcquiredImage ) );
	
	// wait for image
	XrSwapchainImageWaitInfo waitInfo;
	memset( &waitInfo, 0, sizeof( waitInfo ) );
	waitInfo.type = XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO;
	waitInfo.timeout = XR_INFINITE_DURATION;
	
	OXR_CHECK( instance.GetOxr(), instance.xrWaitSwapchainImage( pSwapchain, &waitInfo ) );
}

void deoxrSwapchain::ReleaseImage(){
	deoxrInstance &instance = pSession.GetSystem().GetInstance();
	
	OXR_CHECK( instance.GetOxr(), instance.xrReleaseSwapchainImage( pSwapchain, nullptr ) );
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
	OXR_CHECK( instance.GetOxr(), instance.xrEnumerateSwapchainImages(
		pSwapchain, 0, &count, nullptr ) );
	
	if( count == 0 ){
		return;
	}
	
	pImages = new sImage[ count ];
	
	switch( pSession.GetGraphicApi() ){
	case deoxrSession::egaOpenGL:{
		XrSwapchainImageOpenGLKHR *images = new XrSwapchainImageOpenGLKHR[ count ];
		try{
			for( i=0; i<count; i++ ){
				images[ i ].type = XR_TYPE_SWAPCHAIN_IMAGE_OPENGL_KHR;
			}
			
			OXR_CHECK( instance.GetOxr(), instance.xrEnumerateSwapchainImages(
				pSwapchain, count, &count, ( XrSwapchainImageBaseHeader* )images ) );
			
			for( i=0; i<count; i++ ){
				pImages[ i ].openglImage = images[ i ].image;
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
