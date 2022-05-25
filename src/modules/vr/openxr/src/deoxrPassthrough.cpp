/* 
 * Drag[en]gine OpenXR VR Module
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

#include "deoxrPassthrough.h"
#include "deoxrSystem.h"
#include "deoxrInstance.h"
#include "deVROpenXR.h"

#include <dragengine/common/exceptions.h>



// Class deoxrPassthrough
///////////////////////////

// Constructor, destructor
////////////////////////////

deoxrPassthrough::deoxrPassthrough( deoxrSession &session ) :
pSession( session ),
pPassthrough( XR_NULL_HANDLE ),
pLayer( XR_NULL_HANDLE ),
pEnabled( false ),
pTransparency( 1.0f )
{
	deoxrInstance &instance = session.GetSystem().GetInstance();
	
	try{
		// create passthrough
		XrPassthroughCreateInfoFB createInfoPass;
		memset( &createInfoPass, 0, sizeof( createInfoPass ) );
		createInfoPass.type = XR_TYPE_PASSTHROUGH_CREATE_INFO_FB;
		
		OXR_CHECK( instance.xrCreatePassthroughFB( session.GetSession(), &createInfoPass, &pPassthrough ) );
		
		// create layer
		XrPassthroughLayerCreateInfoFB createInfoLayer;
		memset( &createInfoLayer, 0, sizeof( createInfoLayer ) );
		createInfoLayer.type = XR_TYPE_PASSTHROUGH_LAYER_CREATE_INFO_FB;
		createInfoLayer.passthrough = pPassthrough;
		createInfoLayer.purpose = XR_PASSTHROUGH_LAYER_PURPOSE_RECONSTRUCTION_FB;
		
		OXR_CHECK( instance.xrCreatePassthroughLayerFB( session.GetSession(), &createInfoLayer, &pLayer ) );
		
		// init layer style structs
		memset( &pLayerStyle, 0, sizeof( pLayerStyle ) );
		pLayerStyle.type = XR_TYPE_PASSTHROUGH_STYLE_FB;
		pLayerStyle.textureOpacityFactor = pTransparency;
		
		// init composite layer struct
		memset( &pCompositeLayer, 0, sizeof( XrCompositionLayerPassthroughFB ) );
		pCompositeLayer.type = XR_TYPE_COMPOSITION_LAYER_PASSTHROUGH_FB;
		pCompositeLayer.layerHandle = pLayer;
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoxrPassthrough::~deoxrPassthrough(){
	pCleanUp();
}



// Management
///////////////

void deoxrPassthrough::SetEnabled( bool enabled ){
	if( enabled == pEnabled ){
		return;
	}
	
	pEnabled = enabled;
	
	deoxrInstance &instance = pSession.GetSystem().GetInstance();
	
	if( enabled ){
		OXR_CHECK( instance.xrPassthroughStartFB( pPassthrough ) );
		OXR_CHECK( instance.xrPassthroughLayerResumeFB( pLayer ) );
		
	}else{
		OXR_CHECK( instance.xrPassthroughPauseFB( pPassthrough ) );
		OXR_CHECK( instance.xrPassthroughLayerPauseFB( pLayer ) );
	}
}

void deoxrPassthrough::SetTransparency( float transparency ){
	if( fabsf( transparency - pTransparency ) < 0.001f ){
		return;
	}
	
	pTransparency = transparency;
	pUpdateLayerStyle();
}



// Private Functions
//////////////////////

void deoxrPassthrough::pCleanUp(){
	deoxrInstance &instance = pSession.GetSystem().GetInstance();
	
	if( pLayer != XR_NULL_HANDLE ){
		OXR_CHECK( instance.xrDestroyPassthroughLayerFB( pLayer ) );
	}
	
	if( pPassthrough != XR_NULL_HANDLE ){
		OXR_CHECK( instance.xrDestroyPassthroughFB( pPassthrough ) );
	}
}

void deoxrPassthrough::pUpdateLayerStyle(){
	pLayerStyle.textureOpacityFactor = pTransparency;
	
	OXR_CHECK( pSession.GetSystem().GetInstance().xrPassthroughLayerSetStyleFB( pLayer, &pLayerStyle ) );
}
