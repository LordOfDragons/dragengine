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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglRCamera.h"
#include "deoglRWorld.h"
#include "deoglWorldOctree.h"
#include "../deoglBasics.h"
#include "../configuration/deoglConfiguration.h"
#include "../effects/render/deoglREffect.h"
#include "../rendering/plan/deoglRenderPlan.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../vr/deoglVR.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/curve/decCurveBezierEvaluator.h>



// Class deoglRCamera
///////////////////////

// Constructor, destructor
////////////////////////////

deoglRCamera::deoglRCamera(deoglRenderThread &renderThread) :
pRenderThread(renderThread),

pParentWorld(NULL),

pTextureToneMapParams(nullptr),
pElapsedToneMapAdaption(0.0f),
pForceToneMapAdaption(true),

pEnableHDRR(true),
pExposure(1.0f),
pLowestIntensity(0.0f),
pHighestIntensity(1.0f),
pAdaptionTime(1.0f),

pEnableGI(false),

pWhiteIntensity(1.0f), // 1.5f
pBloomIntensity(1.5f), // 2.0f
pBloomStrength(1.0f),
pBloomBlend(1.0f),
pBloomSize(0.25f),

pToneMapCurveResolution(1024),
pTextureToneMapCurve(nullptr),
pDirtyToneMapCurve(true),

pPlan(NULL),

pInitTexture(true),

pLastAverageLuminance(0.0f),
pDirtyLastAverageLuminance(true),

pVR(nullptr)
{
	try{
		// create render plan
		pPlan = new deoglRenderPlan(renderThread);
		pPlan->SetCamera(this);
		
		// create tone mapping parameters texture
		pTextureToneMapParams = new deoglTexture(renderThread);
		pTextureToneMapParams->SetSize(1, 1);
		pTextureToneMapParams->SetFBOFormat(4, renderThread.GetConfiguration().GetUseHDRR());
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	LEAK_CHECK_CREATE(renderThread, Camera);
}

deoglRCamera::~deoglRCamera(){
	LEAK_CHECK_FREE(pRenderThread, Camera);
	pCleanUp();
}



// Management
///////////////

void deoglRCamera::SetParentWorld(deoglRWorld *parentWorld){
	if(parentWorld == pParentWorld){
		return;
	}
	
	pPlan->SetWorld(NULL); // has to come first since SetWorld accesses previous world
	
	
	pParentWorld = parentWorld;
	
	if(parentWorld){
		parentWorld->AddReference();
	}
	
	pPlan->SetWorld(parentWorld);
}



void deoglRCamera::SetPosition(const decDVector &position){
	pPosition = position;
}



void deoglRCamera::SetCameraMatrices(const decDMatrix &matrix){
	pCameraMatrix = matrix;
	pInverseCameraMatrix = pCameraMatrix.QuickInvert();
}



void deoglRCamera::SetToneMapParamsTexture(deoglTexture *texture){
	pTextureToneMapParams = texture;
	pDirtyLastAverageLuminance = true;
}

void deoglRCamera::SetElapsedToneMapAdaption(float elapsed){
	pElapsedToneMapAdaption = elapsed;
}

void deoglRCamera::SetForceToneMapAdaption(bool forceAdaption){
	pForceToneMapAdaption = forceAdaption;
	pDirtyLastAverageLuminance |= forceAdaption;
}

void deoglRCamera::ResetElapsedToneMapAdaption(){
	pElapsedToneMapAdaption = 0.0f;
}

bool deoglRCamera::UseCustomToneMapCurve() const{
	return pToneMapCurve.GetPointCount() > 0;
}



void deoglRCamera::SetEnableHDRR(bool enable){
	pEnableHDRR = enable;
}

void deoglRCamera::SetExposure(float exposure){
	pExposure = decMath::max(exposure, 0.0f);
}

void deoglRCamera::SetLowestIntensity(float lowestIntensity){
	pLowestIntensity = decMath::max(lowestIntensity, 0.0f);
}

void deoglRCamera::SetHighestIntensity(float highestIntensity){
	pHighestIntensity = decMath::max(highestIntensity, 0.0f);
}

void deoglRCamera::SetAdaptionTime(float adaptionTime){
	pAdaptionTime = decMath::max(adaptionTime, 0.0f);
}



void deoglRCamera::SetEnableGI(bool enable){
	pEnableGI = enable;
	pPlan->SetUseGIState(enable);
}



void deoglRCamera::SetWhiteIntensity(float intensity){
	intensity = decMath::max(intensity, 0.01f);
	if(fabsf(intensity - pWhiteIntensity) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pWhiteIntensity = intensity;
	pDirtyToneMapCurve = true;
}

void deoglRCamera::SetBloomIntensity(float intensity){
	pBloomIntensity = decMath::max(intensity, 0.0f);
}

void deoglRCamera::SetBloomStrength(float strength){
	pBloomStrength = decMath::max(strength, 0.0f);
}

void deoglRCamera::SetBloomBlend(float blend){
	pBloomBlend = decMath::clamp(blend, 0.0f, 1.0f);
}

void deoglRCamera::SetBloomSize(float size){
	pBloomSize = decMath::clamp(size, 0.0f, 1.0f);
}

void deoglRCamera::SetToneMapCurve(const decCurveBezier &curve){
	if(curve == pToneMapCurve){
		return;
	}
	
	pToneMapCurve = curve;
	pDirtyToneMapCurve = true;
}



void deoglRCamera::EnableVR(bool enable){
	if(enable){
		if(!pVR){
			pVR = new deoglVR(*this);
		}
		
		pRenderThread.SetVRCamera(this);
		
	}else{
		if(pRenderThread.GetVRCamera() == this){
			pRenderThread.SetVRCamera(nullptr);
		}
		
		if(pVR){
			delete pVR;
			pVR = nullptr;
		}
	}
}



float deoglRCamera::GetLastAverageLuminance(){
	if(pDirtyLastAverageLuminance){
		pDirtyLastAverageLuminance = false;
		
		if(pInitTexture || pForceToneMapAdaption){
			pLastAverageLuminance = pHighestIntensity * pRenderThread.GetConfiguration().GetHDRRSceneKey();
			
		}else{
			const deoglPixelBuffer::Ref pbToneMapParams(deoglPixelBuffer::Ref::New(
				new deoglPixelBuffer(deoglPixelBuffer::epfFloat4, 1, 1, 1)));
			pTextureToneMapParams->GetPixels(pbToneMapParams);
			pLastAverageLuminance = pbToneMapParams->GetPointerFloat4()->r;
		}
	}
	
	return pLastAverageLuminance;
}



int deoglRCamera::GetEffectCount() const{
	return pEffects.GetCount();
}

deoglREffect &deoglRCamera::GetEffectAt(int index) const{
	return *(deoglREffect*)pEffects.GetAt(index);
}

void deoglRCamera::AddEffect(deoglREffect *effect){
	pEffects.Add(effect);
}

void deoglRCamera::RemoveAllEffects(){
	pEffects.RemoveAll();
}



void deoglRCamera::Update(float elapsed){
	pElapsedToneMapAdaption += elapsed;
	pPlan->GetDirectEnvMapFader().Update(elapsed);
}

void deoglRCamera::PrepareForRender(){
	if(pInitTexture){
		const deoglPixelBuffer::Ref pbToneMapParams(deoglPixelBuffer::Ref::New(
			new deoglPixelBuffer(deoglPixelBuffer::epfFloat4, 1, 1, 1)));
		deoglPixelBuffer::sFloat4 &dataToneMapParams = *pbToneMapParams->GetPointerFloat4();
		dataToneMapParams.r = pRenderThread.GetConfiguration().GetHDRRSceneKey(); // averageLuminance
		dataToneMapParams.g = 0.0f; // scaleLum
		dataToneMapParams.b = 0.0f; // lwhite
		dataToneMapParams.a = 0.0f; // brightPassThreshold
		pTextureToneMapParams->SetPixels(pbToneMapParams);
		pInitTexture = false;
		pForceToneMapAdaption = true;
		pDirtyLastAverageLuminance = true;
	}
	
	if(UseCustomToneMapCurve()){
		pPrepareToneMapCurveTexture();
	}
	
	const int effectCount = pEffects.GetCount();
	int i;
	for(i=0; i<effectCount; i++){
		((deoglREffect*)pEffects.GetAt(i))->PrepareForRender();
	}
}



// Private Functions
//////////////////////

void deoglRCamera::pCleanUp(){
	EnableVR(false);
	SetParentWorld(nullptr);
	
	RemoveAllEffects();
	
	delete pPlan;
	
	if(pTextureToneMapCurve){
		delete pTextureToneMapCurve;
	}
	if(pTextureToneMapParams){
		delete pTextureToneMapParams;
	}
}

void deoglRCamera::pPrepareToneMapCurveTexture(){
	if(!pDirtyToneMapCurve){
		return;
	}
	pDirtyToneMapCurve = false;
	
	if(!pTextureToneMapCurve){
		pTextureToneMapCurve = new deoglTexture(pRenderThread);
		pTextureToneMapCurve->SetSize(pToneMapCurveResolution, 1);
		pTextureToneMapCurve->SetMapingFormat(1, true, false);
	}
	
	const deoglPixelBuffer::Ref pbuf(deoglPixelBuffer::Ref::New(
		new deoglPixelBuffer(deoglPixelBuffer::epfFloat1, pToneMapCurveResolution, 1, 1)));
	deoglPixelBuffer::sFloat1 * const pixels = pbuf->GetPointerFloat1();
	
	int i;
	
	if(pToneMapCurve.GetPointCount() > 0){
		const float factor = pWhiteIntensity / (float)(pToneMapCurveResolution - 1);
		decCurveBezierEvaluator evaluator(pToneMapCurve);
		for(i=0; i<pToneMapCurveResolution; i++){
			pixels[i].r = (GLfloat)(evaluator.EvaluateAt(factor * (float)i));
		}
		
	}else{
		const float factor = 1.0f / (float)(pToneMapCurveResolution - 1);
		for(i=0; i<pToneMapCurveResolution; i++){
			pixels[i].r = (GLfloat)(factor * (float)i);
		}
	}
	
	pTextureToneMapCurve->SetPixels(pbuf);
}
