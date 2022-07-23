/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

deoglRenderWorldInfo::deoglRenderWorldInfo( deoglRenderThread &renderThread ) :
deoglRenderBase( renderThread )
{
	const decColor colorText( 1.0f, 1.0f, 1.0f, 1.0f );
	const decColor colorBg( 0.0f, 0.0f, 0.25f, 0.75f );
	const decColor colorBgSub( 0.05f, 0.05f, 0.05f, 0.75f );
	//const decColor colorBgSub2( 0.1f, 0.1f, 0.1f, 0.75f );
	const decColor colorBgSpecial( 0.05f, 0.025f, 0.05f, 0.75f );
	
	try{
		infoWorld.TakeOver( new deoglDebugInformation( "World", colorText, colorBg ) );
		
		infoPassMasked.TakeOver( new deoglDebugInformation( "Masked", colorText, colorBgSub ) );
		infoWorld->GetChildren().Add( infoPassMasked );
		
		infoGITraceRays.TakeOver( new deoglDebugInformation( "GI Trace Rays", colorText, colorBgSub ) );
		infoWorld->GetChildren().Add( infoGITraceRays );
		
		infoSolidGeometry.TakeOver( new deoglDebugInformation( "Solid Geometry", colorText, colorBgSub ) );
		infoWorld->GetChildren().Add( infoSolidGeometry );
		
		infoReflection.TakeOver( new deoglDebugInformation( "Reflection", colorText, colorBgSub ) );
		infoWorld->GetChildren().Add( infoReflection );
		
		infoSSR.TakeOver( new deoglDebugInformation( "SSR", colorText, colorBgSub ) );
		infoWorld->GetChildren().Add( infoSSR );
		
		infoLuminancePrepare.TakeOver( new deoglDebugInformation( "Lum Prepare", colorText, colorBgSub ) );
		infoWorld->GetChildren().Add( infoLuminancePrepare );
		
		infoSolidGeometryLights.TakeOver( new deoglDebugInformation( "Solid Lights", colorText, colorBgSub ) );
		infoWorld->GetChildren().Add( infoSolidGeometryLights );
		
		infoTransparent.TakeOver( new deoglDebugInformation( "Transparent", colorText, colorBgSub ) );
		infoWorld->GetChildren().Add( infoTransparent );
		
		infoToneMapping.TakeOver( new deoglDebugInformation( "Tone Mapping", colorText, colorBgSub ) );
		infoWorld->GetChildren().Add( infoToneMapping );
		
		infoDebugDrawers.TakeOver( new deoglDebugInformation( "Debug Drawers", colorText, colorBgSub ) );
		infoWorld->GetChildren().Add( infoDebugDrawers );
		
		infoPostProcessing.TakeOver( new deoglDebugInformation( "Post Processing", colorText, colorBgSub ) );
		infoWorld->GetChildren().Add( infoPostProcessing );
		
		infoDeveloperMode.TakeOver( new deoglDebugInformation( "Developer Mode", colorText, colorBgSpecial ) );
		infoWorld->GetChildren().Add( infoDeveloperMode );
		
		infoDeveloperModeTemp.TakeOver( new deoglDebugInformation( "", colorText, colorBgSpecial ) );
		
		
		
		infoSolidGeometryDetails.TakeOver( new deoglDebugInformation( "Solid Geometry", colorText, colorBg ) );
		
		infoSolidGeometryDepthTask.TakeOver( new deoglDebugInformation( "Depth Task", colorText, colorBgSub ) );
		infoSolidGeometryDetails->GetChildren().Add( infoSolidGeometryDepthTask );
		
		infoSolidGeometryDepthRender.TakeOver( new deoglDebugInformation( "Depth Render", colorText, colorBgSub ) );
		infoSolidGeometryDetails->GetChildren().Add( infoSolidGeometryDepthRender );
		
		infoSolidGeometryOcclusion.TakeOver( new deoglDebugInformation( "Occlusion", colorText, colorBgSub ) );
		infoSolidGeometryDetails->GetChildren().Add( infoSolidGeometryOcclusion );
		
		infoSolidGeometryTranspCounter.TakeOver( new deoglDebugInformation( "Transp Counter", colorText, colorBgSub ) );
		infoSolidGeometryDetails->GetChildren().Add( infoSolidGeometryTranspCounter );
		
		infoSolidGeometrySky.TakeOver( new deoglDebugInformation( "Sky", colorText, colorBgSub ) );
		infoSolidGeometryDetails->GetChildren().Add( infoSolidGeometrySky );
		
		infoSolidGeometryTask.TakeOver( new deoglDebugInformation( "Task", colorText, colorBgSub ) );
		infoSolidGeometryDetails->GetChildren().Add( infoSolidGeometryTask );
		
		infoSolidGeometryRender.TakeOver( new deoglDebugInformation( "Render", colorText, colorBgSub ) );
		infoSolidGeometryDetails->GetChildren().Add( infoSolidGeometryRender );
		
		infoSolidGeometryDecals.TakeOver( new deoglDebugInformation( "Decals", colorText, colorBgSub ) );
		infoSolidGeometryDetails->GetChildren().Add( infoSolidGeometryDecals );
		
		infoSolidGeometryDownsampleDepth.TakeOver( new deoglDebugInformation( "Downsample Depth", colorText, colorBgSub ) );
		infoSolidGeometryDetails->GetChildren().Add( infoSolidGeometryDownsampleDepth );
		
		infoSolidGeometrySSAO.TakeOver( new deoglDebugInformation( "SSAO", colorText, colorBgSub ) );
		infoSolidGeometryDetails->GetChildren().Add( infoSolidGeometrySSAO );
		
		
		
		infoTransparentDetails.TakeOver( new deoglDebugInformation( "Transparency", colorText, colorBg ) );
		
		infoTransparentClear.TakeOver( new deoglDebugInformation( "Clear", colorText, colorBgSub ) );
		infoTransparentDetails->GetChildren().Add( infoTransparentClear );
		
		infoTransparentDepthTask.TakeOver( new deoglDebugInformation( "Depth Task", colorText, colorBgSub ) );
		infoTransparentDetails->GetChildren().Add( infoTransparentDepthTask );
		
		infoTransparentDepthRender.TakeOver( new deoglDebugInformation( "Depth Render", colorText, colorBgSub ) );
		infoTransparentDetails->GetChildren().Add( infoTransparentDepthRender );
		
		infoTransparentCopyDepth.TakeOver( new deoglDebugInformation( "Copy Depth", colorText, colorBgSub ) );
		infoTransparentDetails->GetChildren().Add( infoTransparentCopyDepth );
		
		infoTransparentTask.TakeOver( new deoglDebugInformation( "Task", colorText, colorBgSub ) );
		infoTransparentDetails->GetChildren().Add( infoTransparentTask );
		
		infoTransparentRender.TakeOver( new deoglDebugInformation( "Render", colorText, colorBgSub ) );
		infoTransparentDetails->GetChildren().Add( infoTransparentRender );
		
		infoTransparentSSR.TakeOver( new deoglDebugInformation( "SSR", colorText, colorBgSub ) );
		infoTransparentDetails->GetChildren().Add( infoTransparentSSR );
		
		infoTransparentLights.TakeOver( new deoglDebugInformation( "Lights", colorText, colorBgSub ) );
		infoTransparentDetails->GetChildren().Add( infoTransparentLights );
		
		infoTransparentVolumetric.TakeOver( new deoglDebugInformation( "Volumetric", colorText, colorBgSub ) );
		infoTransparentDetails->GetChildren().Add( infoTransparentVolumetric );
		
	}catch( const deException & ){
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
	list.Add( infoWorld );
	list.Add( infoSolidGeometryDetails );
	list.Add( infoTransparentDetails );
}

void deoglRenderWorldInfo::DevModeDebugInfoChanged(){
	const int details = GetRenderThread().GetDebug().GetDeveloperMode().GetDebugInfoDetails();
	
	infoWorld->SetVisible( ( details & deoglDeveloperMode::edimWorld ) == deoglDeveloperMode::edimWorld );
	infoSolidGeometryDetails->SetVisible(
		( details & ( deoglDeveloperMode::edimWorld | deoglDeveloperMode::edimSolidGeometry ) )
		== ( deoglDeveloperMode::edimWorld | deoglDeveloperMode::edimSolidGeometry ) );
	infoTransparentDetails->SetVisible(
		( details & ( deoglDeveloperMode::edimWorld | deoglDeveloperMode::edimSolidGeometry ) )
		== ( deoglDeveloperMode::edimWorld | deoglDeveloperMode::edimSolidGeometry ) );
	
	//infoSolidGeometryTask->SetVisible( false );
}



// Private Functions
//////////////////////

void deoglRenderWorldInfo::pCleanUp(){
	deoglDebugInformationList &dilist = GetRenderThread().GetDebug().GetDebugInformationList();
	dilist.RemoveIfPresent( infoWorld );
	dilist.RemoveIfPresent( infoSolidGeometryDetails );
	dilist.RemoveIfPresent( infoTransparentDetails );
}
