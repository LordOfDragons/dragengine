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

#include "deoxrPassthrough.h"
#include "deoxrSystem.h"
#include "deoxrInstance.h"
#include "deVROpenXR.h"

#include <dragengine/common/exceptions.h>


// Class deoxrPassthrough
///////////////////////////

// Constructor, destructor
////////////////////////////

deoxrPassthrough::deoxrPassthrough(deoxrSession &session) :
pSession(session),
pEnabled(false),
pTransparency(1.0f),
pPassthroughFB(XR_NULL_HANDLE),
pLayerFB(XR_NULL_HANDLE),
pLayerStyleFB{XR_TYPE_PASSTHROUGH_STYLE_FB},
pCompositeLayerFB{XR_TYPE_COMPOSITION_LAYER_PASSTHROUGH_FB},
pEnvBlendMode(XR_ENVIRONMENT_BLEND_MODE_OPAQUE),
pCompositionLayerFlags(0){
}

deoxrPassthrough::~deoxrPassthrough(){
	pCleanUp();
}


// Management
///////////////

void deoxrPassthrough::SetEnabled(bool enabled){
	if(enabled == pEnabled){
		return;
	}
	
	if(enabled){
		pEnsureCreated();
	}
	
	pEnabled = enabled;
	
	pEnablePassthroughFB();
	pUpdateEnvBlendMode();
}

void deoxrPassthrough::SetTransparency(float transparency){
	if(fabsf(transparency - pTransparency) < 0.001f){
		return;
	}
	
	pTransparency = transparency;
	
	pUpdateLayerStyleFB();
	pUpdateEnvBlendMode();
}

bool deoxrPassthrough::ShowPassthroughLayerFB() const{
	return pLayerFB != XR_NULL_HANDLE && pEnabled && pTransparency > 0.001f;
}


// Private Functions
//////////////////////

void deoxrPassthrough::pCleanUp(){
	deoxrInstance &instance = pSession.GetSystem().GetInstance();
	
	if(pLayerFB != XR_NULL_HANDLE){
		OXR_CHECK(instance.xrDestroyPassthroughLayerFB(pLayerFB));
		pLayerFB = XR_NULL_HANDLE;
	}
	
	if(pPassthroughFB != XR_NULL_HANDLE){
		OXR_CHECK(instance.xrDestroyPassthroughFB(pPassthroughFB));
		pPassthroughFB = XR_NULL_HANDLE;
	}
}

void deoxrPassthrough::pEnsureCreated(){
	deoxrInstance &instance = pSession.GetSystem().GetInstance();
	if(instance.SupportsExtension(deoxrInstance::extFBPassthrough)){
		pEnsureCreateFB();
	}
}

void deoxrPassthrough::pEnsureCreateFB(){
	if(pPassthroughFB != XR_NULL_HANDLE){
		return;
	}
	
	deoxrInstance &instance = pSession.GetSystem().GetInstance();
	
	try{
		// create passthrough. flags can not be 0 so we have to specify at least
		// XR_PASSTHROUGH_IS_RUNNING_AT_CREATION_BIT_FB or XR_PASSTHROUGH_LAYER_DEPTH_BIT_FB.
		// but since we have no depth layer yet we are forced to use
		// XR_PASSTHROUGH_IS_RUNNING_AT_CREATION_BIT_FB to not get into troubles
		XrPassthroughCreateInfoFB createInfoPass{XR_TYPE_PASSTHROUGH_CREATE_INFO_FB};
		createInfoPass.flags = XR_PASSTHROUGH_IS_RUNNING_AT_CREATION_BIT_FB;
		
		OXR_CHECK(instance.xrCreatePassthroughFB(pSession.GetSession(), &createInfoPass, &pPassthroughFB));
		
		// create layer
		XrPassthroughLayerCreateInfoFB createInfoLayer{XR_TYPE_PASSTHROUGH_LAYER_CREATE_INFO_FB};
		createInfoLayer.passthrough = pPassthroughFB;
		createInfoLayer.purpose = XR_PASSTHROUGH_LAYER_PURPOSE_RECONSTRUCTION_FB;
		
		OXR_CHECK(instance.xrCreatePassthroughLayerFB(pSession.GetSession(), &createInfoLayer, &pLayerFB));
		
		// init layer style structs
		pLayerStyleFB.textureOpacityFactor = pTransparency;
		
		// init composite layer struct
		pCompositeLayerFB.layerHandle = pLayerFB;
		
		// pause passthrough since it gets automatically activated
		OXR_CHECK(instance.xrPassthroughPauseFB(pPassthroughFB));
		OXR_CHECK(instance.xrPassthroughLayerPauseFB(pLayerFB));
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	
	pEnvBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE;
	pCompositionLayerFlags = 0;
}

void deoxrPassthrough::pUpdateLayerStyleFB(){
	pLayerStyleFB.textureOpacityFactor = pTransparency;
	
	if(pLayerFB != XR_NULL_HANDLE){
		OXR_CHECK(pSession.GetSystem().GetInstance().xrPassthroughLayerSetStyleFB(pLayerFB, &pLayerStyleFB));
	}
}

void deoxrPassthrough::pEnablePassthroughFB(){
	if(pPassthroughFB == XR_NULL_HANDLE){
		return;
	}
	
	deoxrInstance &instance = pSession.GetSystem().GetInstance();
	
	if(pEnabled){
		OXR_CHECK(instance.xrPassthroughStartFB(pPassthroughFB));
		OXR_CHECK(instance.xrPassthroughLayerResumeFB(pLayerFB));
		
	}else{
		OXR_CHECK(instance.xrPassthroughPauseFB(pPassthroughFB));
		OXR_CHECK(instance.xrPassthroughLayerPauseFB(pLayerFB));
	}
}

void deoxrPassthrough::pUpdateEnvBlendMode(){
	if(!pSession.GetSystem().GetSupportsEnvBlendModeAlphaBlend()
	|| pPassthroughFB != XR_NULL_HANDLE){
		return;
	}
	
	if(pEnabled && pTransparency > 0.5f){
		pEnvBlendMode = XR_ENVIRONMENT_BLEND_MODE_ALPHA_BLEND;
		pCompositionLayerFlags = XR_COMPOSITION_LAYER_BLEND_TEXTURE_SOURCE_ALPHA_BIT;
		
	}else{
		pEnvBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE;
		pCompositionLayerFlags = 0;
	}
}
