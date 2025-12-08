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
		infoWorld.TakeOverWith("World", colorText, colorBg);
		
		infoPassMasked.TakeOverWith("Masked", colorText, colorBgSub);
		infoWorld->GetChildren().Add(infoPassMasked);
		
		infoGITraceRays.TakeOverWith("GI Trace Rays", colorText, colorBgSub);
		infoWorld->GetChildren().Add(infoGITraceRays);
		
		infoSolidGeometry.TakeOverWith("Solid Geometry", colorText, colorBgSub);
		infoWorld->GetChildren().Add(infoSolidGeometry);
		
		infoReflection.TakeOverWith("Reflection", colorText, colorBgSub);
		infoWorld->GetChildren().Add(infoReflection);
		
		infoSSR.TakeOverWith("SSR", colorText, colorBgSub);
		infoWorld->GetChildren().Add(infoSSR);
		
		infoLuminancePrepare.TakeOverWith("Lum Prepare", colorText, colorBgSub);
		infoWorld->GetChildren().Add(infoLuminancePrepare);
		
		infoSolidGeometryLights.TakeOverWith("Solid Lights", colorText, colorBgSub);
		infoWorld->GetChildren().Add(infoSolidGeometryLights);
		
		infoTransparent.TakeOverWith("Transparent", colorText, colorBgSub);
		infoWorld->GetChildren().Add(infoTransparent);
		
		infoToneMapping.TakeOverWith("Tone Mapping", colorText, colorBgSub);
		infoWorld->GetChildren().Add(infoToneMapping);
		
		infoDebugDrawers.TakeOverWith("Debug Drawers", colorText, colorBgSub);
		infoWorld->GetChildren().Add(infoDebugDrawers);
		
		infoPostProcessing.TakeOverWith("Post Processing", colorText, colorBgSub);
		infoWorld->GetChildren().Add(infoPostProcessing);
		
		infoDeveloperMode.TakeOverWith("Developer Mode", colorText, colorBgSpecial);
		infoWorld->GetChildren().Add(infoDeveloperMode);
		
		infoDeveloperModeTemp.TakeOverWith("", colorText, colorBgSpecial);
		
		
		
		infoSolidGeometryDetails.TakeOverWith("Solid Geometry", colorText, colorBg);
		
		infoSolidGeometryDepthTask.TakeOverWith("Depth Task", colorText, colorBgSub);
		infoSolidGeometryDetails->GetChildren().Add(infoSolidGeometryDepthTask);
		
		infoSolidGeometryDepthRender.TakeOverWith("Depth Render", colorText, colorBgSub);
		infoSolidGeometryDetails->GetChildren().Add(infoSolidGeometryDepthRender);
		
		infoSolidGeometryOcclusion.TakeOverWith("Occlusion", colorText, colorBgSub);
		infoSolidGeometryDetails->GetChildren().Add(infoSolidGeometryOcclusion);
		
		infoSolidGeometryTranspCounter.TakeOverWith("Transp Counter", colorText, colorBgSub);
		infoSolidGeometryDetails->GetChildren().Add(infoSolidGeometryTranspCounter);
		
		infoSolidGeometrySky.TakeOverWith("Sky", colorText, colorBgSub);
		infoSolidGeometryDetails->GetChildren().Add(infoSolidGeometrySky);
		
		infoSolidGeometryTask.TakeOverWith("Task", colorText, colorBgSub);
		infoSolidGeometryDetails->GetChildren().Add(infoSolidGeometryTask);
		
		infoSolidGeometryRender.TakeOverWith("Render", colorText, colorBgSub);
		infoSolidGeometryDetails->GetChildren().Add(infoSolidGeometryRender);
		
		infoSolidGeometryDecals.TakeOverWith("Decals", colorText, colorBgSub);
		infoSolidGeometryDetails->GetChildren().Add(infoSolidGeometryDecals);
		
		infoSolidGeometryDownsampleDepth.TakeOverWith("Downsample Depth", colorText, colorBgSub);
		infoSolidGeometryDetails->GetChildren().Add(infoSolidGeometryDownsampleDepth);
		
		infoSolidGeometrySSAO.TakeOverWith("SSAO", colorText, colorBgSub);
		infoSolidGeometryDetails->GetChildren().Add(infoSolidGeometrySSAO);
		
		
		
		infoTransparentDetails.TakeOverWith("Transparency", colorText, colorBg);
		
		infoTransparentClear.TakeOverWith("Clear", colorText, colorBgSub);
		infoTransparentDetails->GetChildren().Add(infoTransparentClear);
		
		infoTransparentDepthTask.TakeOverWith("Depth Task", colorText, colorBgSub);
		infoTransparentDetails->GetChildren().Add(infoTransparentDepthTask);
		
		infoTransparentDepthRender.TakeOverWith("Depth Render", colorText, colorBgSub);
		infoTransparentDetails->GetChildren().Add(infoTransparentDepthRender);
		
		infoTransparentCopyDepth.TakeOverWith("Copy Depth", colorText, colorBgSub);
		infoTransparentDetails->GetChildren().Add(infoTransparentCopyDepth);
		
		infoTransparentTask.TakeOverWith("Task", colorText, colorBgSub);
		infoTransparentDetails->GetChildren().Add(infoTransparentTask);
		
		infoTransparentRender.TakeOverWith("Render", colorText, colorBgSub);
		infoTransparentDetails->GetChildren().Add(infoTransparentRender);
		
		infoTransparentSSR.TakeOverWith("SSR", colorText, colorBgSub);
		infoTransparentDetails->GetChildren().Add(infoTransparentSSR);
		
		infoTransparentLights.TakeOverWith("Lights", colorText, colorBgSub);
		infoTransparentDetails->GetChildren().Add(infoTransparentLights);
		
		infoTransparentVolumetric.TakeOverWith("Volumetric", colorText, colorBgSub);
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
