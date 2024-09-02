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
pTransparency( 1.0f ){
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
	
	if( enabled ){
		pEnsureCreated();
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
		pLayer = XR_NULL_HANDLE;
	}
	
	if( pPassthrough != XR_NULL_HANDLE ){
		OXR_CHECK( instance.xrDestroyPassthroughFB( pPassthrough ) );
		pPassthrough = XR_NULL_HANDLE;
	}
}

void deoxrPassthrough::pUpdateLayerStyle(){
	pLayerStyle.textureOpacityFactor = pTransparency;
	
	OXR_CHECK( pSession.GetSystem().GetInstance().xrPassthroughLayerSetStyleFB( pLayer, &pLayerStyle ) );
}

void deoxrPassthrough::pEnsureCreated(){
	if( pPassthrough != XR_NULL_HANDLE ){
		return;
	}
	
	deoxrInstance &instance = pSession.GetSystem().GetInstance();
	
	try{
		// create passthrough. flags can not be 0 so we have to specify at least
		// XR_PASSTHROUGH_IS_RUNNING_AT_CREATION_BIT_FB or XR_PASSTHROUGH_LAYER_DEPTH_BIT_FB.
		// but since we have no depth layer yet we are forced to use
		// XR_PASSTHROUGH_IS_RUNNING_AT_CREATION_BIT_FB to not get into troubles
		XrPassthroughCreateInfoFB createInfoPass;
		memset( &createInfoPass, 0, sizeof( createInfoPass ) );
		createInfoPass.type = XR_TYPE_PASSTHROUGH_CREATE_INFO_FB;
		createInfoPass.flags = XR_PASSTHROUGH_IS_RUNNING_AT_CREATION_BIT_FB;
		
		OXR_CHECK( instance.xrCreatePassthroughFB( pSession.GetSession(), &createInfoPass, &pPassthrough ) );
		
		// create layer
		XrPassthroughLayerCreateInfoFB createInfoLayer;
		memset( &createInfoLayer, 0, sizeof( createInfoLayer ) );
		createInfoLayer.type = XR_TYPE_PASSTHROUGH_LAYER_CREATE_INFO_FB;
		createInfoLayer.passthrough = pPassthrough;
		createInfoLayer.purpose = XR_PASSTHROUGH_LAYER_PURPOSE_RECONSTRUCTION_FB;
		
		OXR_CHECK( instance.xrCreatePassthroughLayerFB( pSession.GetSession(), &createInfoLayer, &pLayer ) );
		
		// init layer style structs
		memset( &pLayerStyle, 0, sizeof( pLayerStyle ) );
		pLayerStyle.type = XR_TYPE_PASSTHROUGH_STYLE_FB;
		pLayerStyle.textureOpacityFactor = pTransparency;
		
		// init composite layer struct
		memset( &pCompositeLayer, 0, sizeof( XrCompositionLayerPassthroughFB ) );
		pCompositeLayer.type = XR_TYPE_COMPOSITION_LAYER_PASSTHROUGH_FB;
		pCompositeLayer.layerHandle = pLayer;
		
		// pause passthrough since it gets automatically activated
		OXR_CHECK( instance.xrPassthroughPauseFB( pPassthrough ) );
		OXR_CHECK( instance.xrPassthroughLayerPauseFB( pLayer ) );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}
