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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "deoglRenderDevMode.h"
#include "deoglRenderWorldInfo.h"
#include "../debug/deoglDebugInformation.h"
#include "../devmode/deoglDeveloperMode.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTDebug.h"
#include "../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderWorldInfo
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderWorldInfo::deoglRenderWorldInfo(deoglRenderThread &renderThread) :
deoglRenderBase(renderThread)
{
	const decColor colorText(1.0f, 1.0f, 1.0f, 1.0f);
	const decColor colorBg(0.0f, 0.0f, 0.25f, 0.75f);
	const decColor colorBgSub(0.05f, 0.05f, 0.05f, 0.75f);
	//const decColor colorBgSub2( 0.1f, 0.1f, 0.1f, 0.75f );
	const decColor colorBgSpecial(0.05f, 0.025f, 0.05f, 0.75f);
	
	try{
		infoWorld = deoglDebugInformation::Ref::New("World", colorText, colorBg);
		
		infoPassMasked = deoglDebugInformation::Ref::New("Masked", colorText, colorBgSub);
		infoWorld->GetChildren().Add(infoPassMasked);
		
		infoGITraceRays = deoglDebugInformation::Ref::New("GI Trace Rays", colorText, colorBgSub);
		infoWorld->GetChildren().Add(infoGITraceRays);
		
		infoSolidGeometry = deoglDebugInformation::Ref::New("Solid Geometry", colorText, colorBgSub);
		infoWorld->GetChildren().Add(infoSolidGeometry);
		
		infoReflection = deoglDebugInformation::Ref::New("Reflection", colorText, colorBgSub);
		infoWorld->GetChildren().Add(infoReflection);
		
		infoSSR = deoglDebugInformation::Ref::New("SSR", colorText, colorBgSub);
		infoWorld->GetChildren().Add(infoSSR);
		
		infoLuminancePrepare = deoglDebugInformation::Ref::New("Lum Prepare", colorText, colorBgSub);
		infoWorld->GetChildren().Add(infoLuminancePrepare);
		
		infoSolidGeometryLights = deoglDebugInformation::Ref::New("Solid Lights", colorText, colorBgSub);
		infoWorld->GetChildren().Add(infoSolidGeometryLights);
		
		infoTransparent = deoglDebugInformation::Ref::New("Transparent", colorText, colorBgSub);
		infoWorld->GetChildren().Add(infoTransparent);
		
		infoToneMapping = deoglDebugInformation::Ref::New("Tone Mapping", colorText, colorBgSub);
		infoWorld->GetChildren().Add(infoToneMapping);
		
		infoDebugDrawers = deoglDebugInformation::Ref::New("Debug Drawers", colorText, colorBgSub);
		infoWorld->GetChildren().Add(infoDebugDrawers);
		
		infoPostProcessing = deoglDebugInformation::Ref::New("Post Processing", colorText, colorBgSub);
		infoWorld->GetChildren().Add(infoPostProcessing);
		
		infoDeveloperMode = deoglDebugInformation::Ref::New("Developer Mode", colorText, colorBgSpecial);
		infoWorld->GetChildren().Add(infoDeveloperMode);
		
		infoDeveloperModeTemp = deoglDebugInformation::Ref::New("", colorText, colorBgSpecial);
		
		
		
		infoSolidGeometryDetails = deoglDebugInformation::Ref::New("Solid Geometry", colorText, colorBg);
		
		infoSolidGeometryDepthTask = deoglDebugInformation::Ref::New("Depth Task", colorText, colorBgSub);
		infoSolidGeometryDetails->GetChildren().Add(infoSolidGeometryDepthTask);
		
		infoSolidGeometryDepthRender = deoglDebugInformation::Ref::New("Depth Render", colorText, colorBgSub);
		infoSolidGeometryDetails->GetChildren().Add(infoSolidGeometryDepthRender);
		
		infoSolidGeometryOcclusion = deoglDebugInformation::Ref::New("Occlusion", colorText, colorBgSub);
		infoSolidGeometryDetails->GetChildren().Add(infoSolidGeometryOcclusion);
		
		infoSolidGeometryTranspCounter = deoglDebugInformation::Ref::New("Transp Counter", colorText, colorBgSub);
		infoSolidGeometryDetails->GetChildren().Add(infoSolidGeometryTranspCounter);
		
		infoSolidGeometrySky = deoglDebugInformation::Ref::New("Sky", colorText, colorBgSub);
		infoSolidGeometryDetails->GetChildren().Add(infoSolidGeometrySky);
		
		infoSolidGeometryTask = deoglDebugInformation::Ref::New("Task", colorText, colorBgSub);
		infoSolidGeometryDetails->GetChildren().Add(infoSolidGeometryTask);
		
		infoSolidGeometryRender = deoglDebugInformation::Ref::New("Render", colorText, colorBgSub);
		infoSolidGeometryDetails->GetChildren().Add(infoSolidGeometryRender);
		
		infoSolidGeometryDecals = deoglDebugInformation::Ref::New("Decals", colorText, colorBgSub);
		infoSolidGeometryDetails->GetChildren().Add(infoSolidGeometryDecals);
		
		infoSolidGeometryDownsampleDepth = deoglDebugInformation::Ref::New("Downsample Depth", colorText, colorBgSub);
		infoSolidGeometryDetails->GetChildren().Add(infoSolidGeometryDownsampleDepth);
		
		infoSolidGeometrySSAO = deoglDebugInformation::Ref::New("SSAO", colorText, colorBgSub);
		infoSolidGeometryDetails->GetChildren().Add(infoSolidGeometrySSAO);
		
		
		
		infoTransparentDetails = deoglDebugInformation::Ref::New("Transparency", colorText, colorBg);
		
		infoTransparentClear = deoglDebugInformation::Ref::New("Clear", colorText, colorBgSub);
		infoTransparentDetails->GetChildren().Add(infoTransparentClear);
		
		infoTransparentDepthTask = deoglDebugInformation::Ref::New("Depth Task", colorText, colorBgSub);
		infoTransparentDetails->GetChildren().Add(infoTransparentDepthTask);
		
		infoTransparentDepthRender = deoglDebugInformation::Ref::New("Depth Render", colorText, colorBgSub);
		infoTransparentDetails->GetChildren().Add(infoTransparentDepthRender);
		
		infoTransparentCopyDepth = deoglDebugInformation::Ref::New("Copy Depth", colorText, colorBgSub);
		infoTransparentDetails->GetChildren().Add(infoTransparentCopyDepth);
		
		infoTransparentTask = deoglDebugInformation::Ref::New("Task", colorText, colorBgSub);
		infoTransparentDetails->GetChildren().Add(infoTransparentTask);
		
		infoTransparentRender = deoglDebugInformation::Ref::New("Render", colorText, colorBgSub);
		infoTransparentDetails->GetChildren().Add(infoTransparentRender);
		
		infoTransparentSSR = deoglDebugInformation::Ref::New("SSR", colorText, colorBgSub);
		infoTransparentDetails->GetChildren().Add(infoTransparentSSR);
		
		infoTransparentLights = deoglDebugInformation::Ref::New("Lights", colorText, colorBgSub);
		infoTransparentDetails->GetChildren().Add(infoTransparentLights);
		
		infoTransparentVolumetric = deoglDebugInformation::Ref::New("Volumetric", colorText, colorBgSub);
		infoTransparentDetails->GetChildren().Add(infoTransparentVolumetric);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglRenderWorldInfo::~deoglRenderWorldInfo(){
	pCleanUp();
}



// Debug
//////////

void deoglRenderWorldInfo::ClearAll(){
	infoWorld->Clear();
	infoPassMasked->Clear();
	infoGITraceRays->Clear();
	infoLuminancePrepare->Clear();
	infoSolidGeometry->Clear();
	infoReflection->Clear();
	infoSSR->Clear();
	//infoGI->Clear(); // not done here since this happens during prepare
	infoSolidGeometryLights->Clear();
	infoTransparent->Clear();
	infoToneMapping->Clear();
	infoDebugDrawers->Clear();
	infoPostProcessing->Clear();
	//infoDeveloperMode, infoDeveloperModeTemp => special
	
	infoSolidGeometryDepthTask->Clear();
	infoSolidGeometryDepthRender->Clear();
	infoSolidGeometryOcclusion->Clear();
	infoSolidGeometryTranspCounter->Clear();
	infoSolidGeometrySky->Clear();
	infoSolidGeometryTask->Clear();
	infoSolidGeometryRender->Clear();
	infoSolidGeometryDecals->Clear();
	infoSolidGeometryDownsampleDepth->Clear();
	infoSolidGeometrySSAO->Clear();
	
	infoTransparentClear->Clear();
	infoTransparentDepthTask->Clear();
	infoTransparentDepthRender->Clear();
	infoTransparentCopyDepth->Clear();
	infoTransparentTask->Clear();
	infoTransparentRender->Clear();
	infoTransparentSSR->Clear();
	infoTransparentLights->Clear();
	infoTransparentVolumetric->Clear();
}

void deoglRenderWorldInfo::AddTopLevelDebugInfo(){
	deoglDebugInformationList &list = GetRenderThread().GetDebug().GetDebugInformationList();
	list.Add(infoWorld);
	list.Add(infoSolidGeometryDetails);
	list.Add(infoTransparentDetails);
}

void deoglRenderWorldInfo::DevModeDebugInfoChanged(){
	const int details = GetRenderThread().GetDebug().GetDeveloperMode().GetDebugInfoDetails();
	
	infoWorld->SetVisible((details & deoglDeveloperMode::edimWorld) == deoglDeveloperMode::edimWorld);
	infoSolidGeometryDetails->SetVisible(
		(details & (deoglDeveloperMode::edimWorld | deoglDeveloperMode::edimSolidGeometry))
		== (deoglDeveloperMode::edimWorld | deoglDeveloperMode::edimSolidGeometry));
	infoTransparentDetails->SetVisible(
		(details & (deoglDeveloperMode::edimWorld | deoglDeveloperMode::edimSolidGeometry))
		== (deoglDeveloperMode::edimWorld | deoglDeveloperMode::edimSolidGeometry));
	
	//infoSolidGeometryTask->SetVisible( false );
}



// Private Functions
//////////////////////

void deoglRenderWorldInfo::pCleanUp(){
	deoglDebugInformationList &dilist = GetRenderThread().GetDebug().GetDebugInformationList();
	dilist.RemoveIfPresent(infoWorld);
	dilist.RemoveIfPresent(infoSolidGeometryDetails);
	dilist.RemoveIfPresent(infoTransparentDetails);
}
