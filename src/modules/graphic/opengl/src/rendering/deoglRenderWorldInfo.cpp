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
deoglRenderBase( renderThread ),

infoWorld( NULL ),
infoPrepare( NULL ),
infoPassMasked( NULL ),
infoSolidGeometry( NULL ),
infoReflection( NULL ),
infoSSR( NULL ),
infoGI( NULL ),
infoSolidGeometryLights( NULL ),
infoTransparent( NULL ),
infoToneMapping( NULL ),
infoDebugDrawers( NULL ),
infoPostProcessing( NULL ),

infoSolidGeometryDetails( NULL ),
infoSolidGeometryClear( NULL ),
infoSolidGeometryDepthTask( NULL ),
infoSolidGeometryDepthRender( NULL ),
infoSolidGeometryOcclusion( NULL ),
infoSolidGeometryTranspCounter( NULL ),
infoSolidGeometrySky( NULL ),
infoSolidGeometryTask( NULL ),
infoSolidGeometryRender( NULL ),
infoSolidGeometryDecals( NULL ),
infoSolidGeometryDownsampleDepth( NULL ),
infoSolidGeometrySSAO( NULL ),

infoTransparentDetails( NULL ),
infoTransparentClear( NULL ),
infoTransparentDepthTask( NULL ),
infoTransparentDepthRender( NULL ),
infoTransparentCopyDepth( NULL ),
infoTransparentTask( NULL ),
infoTransparentRender( NULL ),
infoTransparentSSR( NULL ),
infoTransparentLights( NULL ),
infoTransparentVolumetric( NULL )
{
	const decColor colorText( 1.0f, 1.0f, 1.0f, 1.0f );
	const decColor colorBg( 0.0f, 0.0f, 0.0f, 0.75f );
	const decColor colorBgSub( 0.05f, 0.05f, 0.05f, 0.75f );
	//const decColor colorBgSub2( 0.1f, 0.1f, 0.1f, 0.75f );
	
	try{
		infoWorld = new deoglDebugInformation( "World", colorText, colorBg );
		
		infoPrepare = new deoglDebugInformation( "Prepare", colorText, colorBgSub );
		infoWorld->GetChildren().Add( infoPrepare );
		
		infoPassMasked = new deoglDebugInformation( "Masked", colorText, colorBgSub );
		infoWorld->GetChildren().Add( infoPassMasked );
		
		infoSolidGeometry = new deoglDebugInformation( "Solid Geometry", colorText, colorBgSub );
		infoWorld->GetChildren().Add( infoSolidGeometry );
		
		infoReflection = new deoglDebugInformation( "Reflection", colorText, colorBgSub );
		infoWorld->GetChildren().Add( infoReflection );
		
		infoSSR = new deoglDebugInformation( "SSR", colorText, colorBgSub );
		infoWorld->GetChildren().Add( infoSSR );
		
		infoGI = new deoglDebugInformation( "GI", colorText, colorBgSub );
		infoWorld->GetChildren().Add( infoGI );
		
		infoSolidGeometryLights = new deoglDebugInformation( "Solid Lights", colorText, colorBgSub );
		infoWorld->GetChildren().Add( infoSolidGeometryLights );
		
		infoTransparent = new deoglDebugInformation( "Transparent", colorText, colorBgSub );
		infoWorld->GetChildren().Add( infoTransparent );
		
		infoToneMapping = new deoglDebugInformation( "Tone Mapping", colorText, colorBgSub );
		infoWorld->GetChildren().Add( infoToneMapping );
		
		infoDebugDrawers = new deoglDebugInformation( "Debug Drawers", colorText, colorBgSub );
		infoWorld->GetChildren().Add( infoDebugDrawers );
		
		infoPostProcessing = new deoglDebugInformation( "Post Processing", colorText, colorBgSub );
		infoWorld->GetChildren().Add( infoPostProcessing );
		
		
		
		infoSolidGeometryDetails = new deoglDebugInformation( "Solid Geometry", colorText, colorBg );
		
		infoSolidGeometryClear = new deoglDebugInformation( "Clear", colorText, colorBgSub );
		infoSolidGeometryDetails->GetChildren().Add( infoSolidGeometryClear );
		
		infoSolidGeometryDepthTask = new deoglDebugInformation( "Depth Task", colorText, colorBgSub );
		infoSolidGeometryDetails->GetChildren().Add( infoSolidGeometryDepthTask );
		
		infoSolidGeometryDepthRender = new deoglDebugInformation( "Depth Render", colorText, colorBgSub );
		infoSolidGeometryDetails->GetChildren().Add( infoSolidGeometryDepthRender );
		
		infoSolidGeometryOcclusion = new deoglDebugInformation( "Occlusion", colorText, colorBgSub );
		infoSolidGeometryDetails->GetChildren().Add( infoSolidGeometryOcclusion );
		
		infoSolidGeometryTranspCounter = new deoglDebugInformation( "Transp Counter", colorText, colorBgSub );
		infoSolidGeometryDetails->GetChildren().Add( infoSolidGeometryTranspCounter );
		
		infoSolidGeometrySky = new deoglDebugInformation( "Sky", colorText, colorBgSub );
		infoSolidGeometryDetails->GetChildren().Add( infoSolidGeometrySky );
		
		infoSolidGeometryTask = new deoglDebugInformation( "Task", colorText, colorBgSub );
		infoSolidGeometryDetails->GetChildren().Add( infoSolidGeometryTask );
		
		infoSolidGeometryRender = new deoglDebugInformation( "Render", colorText, colorBgSub );
		infoSolidGeometryDetails->GetChildren().Add( infoSolidGeometryRender );
		
		infoSolidGeometryDecals = new deoglDebugInformation( "Decals", colorText, colorBgSub );
		infoSolidGeometryDetails->GetChildren().Add( infoSolidGeometryDecals );
		
		infoSolidGeometryDownsampleDepth = new deoglDebugInformation( "Downsample Depth", colorText, colorBgSub );
		infoSolidGeometryDetails->GetChildren().Add( infoSolidGeometryDownsampleDepth );
		
		infoSolidGeometrySSAO = new deoglDebugInformation( "SSAO", colorText, colorBgSub );
		infoSolidGeometryDetails->GetChildren().Add( infoSolidGeometrySSAO );
		
		
		
		infoTransparentDetails = new deoglDebugInformation( "Transparency", colorText, colorBg );
		
		infoTransparentClear = new deoglDebugInformation( "Clear", colorText, colorBgSub );
		infoTransparentDetails->GetChildren().Add( infoTransparentClear );
		
		infoTransparentDepthTask = new deoglDebugInformation( "Depth Task", colorText, colorBgSub );
		infoTransparentDetails->GetChildren().Add( infoTransparentDepthTask );
		
		infoTransparentDepthRender = new deoglDebugInformation( "Depth Render", colorText, colorBgSub );
		infoTransparentDetails->GetChildren().Add( infoTransparentDepthRender );
		
		infoTransparentCopyDepth = new deoglDebugInformation( "Copy Depth", colorText, colorBgSub );
		infoTransparentDetails->GetChildren().Add( infoTransparentCopyDepth );
		
		infoTransparentTask = new deoglDebugInformation( "Task", colorText, colorBgSub );
		infoTransparentDetails->GetChildren().Add( infoTransparentTask );
		
		infoTransparentRender = new deoglDebugInformation( "Render", colorText, colorBgSub );
		infoTransparentDetails->GetChildren().Add( infoTransparentRender );
		
		infoTransparentSSR = new deoglDebugInformation( "SSR", colorText, colorBgSub );
		infoTransparentDetails->GetChildren().Add( infoTransparentSSR );
		
		infoTransparentLights = new deoglDebugInformation( "Lights", colorText, colorBgSub );
		infoTransparentDetails->GetChildren().Add( infoTransparentLights );
		
		infoTransparentVolumetric = new deoglDebugInformation( "Volumetric", colorText, colorBgSub );
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
	infoPrepare->Clear();
	infoPassMasked->Clear();
	infoSolidGeometry->Clear();
	infoReflection->Clear();
	infoSSR->Clear();
	//infoGI->Clear(); // not done here since this happens during prepare
	infoSolidGeometryLights->Clear();
	infoTransparent->Clear();
	infoToneMapping->Clear();
	infoDebugDrawers->Clear();
	infoPostProcessing->Clear();
	
	infoSolidGeometryClear->Clear();
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
	if( infoWorld ){
		GetRenderThread().GetDebug().GetDebugInformationList()
			.RemoveIfPresent( infoWorld );
		infoWorld->FreeReference();
	}
	if( infoPrepare ){
		infoPrepare->FreeReference();
	}
	if( infoPassMasked ){
		infoPassMasked->FreeReference();
	}
	if( infoSolidGeometry ){
		infoSolidGeometry->FreeReference();
	}
	if( infoReflection ){
		infoReflection->FreeReference();
	}
	if( infoSSR ){
		infoSSR->FreeReference();
	}
	if( infoGI ){
		infoGI->FreeReference();
	}
	if( infoSolidGeometryLights ){
		infoSolidGeometryLights->FreeReference();
	}
	if( infoTransparent ){
		infoTransparent->FreeReference();
	}
	if( infoToneMapping ){
		infoToneMapping->FreeReference();
	}
	if( infoDebugDrawers ){
		infoDebugDrawers->FreeReference();
	}
	if( infoPostProcessing ){
		infoPostProcessing->FreeReference();
	}
	
	if( infoSolidGeometryDetails ){
		GetRenderThread().GetDebug().GetDebugInformationList()
			.RemoveIfPresent( infoSolidGeometryDetails );
		infoSolidGeometryDetails->FreeReference();
	}
	if( infoSolidGeometryClear ){
		infoSolidGeometryClear->FreeReference();
	}
	if( infoSolidGeometryDepthTask ){
		infoSolidGeometryDepthTask->FreeReference();
	}
	if( infoSolidGeometryDepthRender ){
		infoSolidGeometryDepthRender->FreeReference();
	}
	if( infoSolidGeometryOcclusion ){
		infoSolidGeometryOcclusion->FreeReference();
	}
	if( infoSolidGeometryTranspCounter ){
		infoSolidGeometryTranspCounter->FreeReference();
	}
	if( infoSolidGeometrySky ){
		infoSolidGeometrySky->FreeReference();
	}
	if( infoSolidGeometryTask ){
		infoSolidGeometryTask->FreeReference();
	}
	if( infoSolidGeometryRender ){
		infoSolidGeometryRender->FreeReference();
	}
	if( infoSolidGeometryDecals ){
		infoSolidGeometryDecals->FreeReference();
	}
	if( infoSolidGeometryDownsampleDepth ){
		infoSolidGeometryDownsampleDepth->FreeReference();
	}
	if( infoSolidGeometrySSAO ){
		infoSolidGeometrySSAO->FreeReference();
	}
	
	if( infoTransparentDetails ){
		GetRenderThread().GetDebug().GetDebugInformationList()
			.RemoveIfPresent( infoTransparentDetails );
		infoTransparentDetails->FreeReference();
	}
	if( infoTransparentClear ){
		infoTransparentClear->FreeReference();
	}
	if( infoTransparentDepthTask ){
		infoTransparentDepthTask->FreeReference();
	}
	if( infoTransparentDepthRender ){
		infoTransparentDepthRender->FreeReference();
	}
	if( infoTransparentCopyDepth ){
		infoTransparentCopyDepth->FreeReference();
	}
	if( infoTransparentTask ){
		infoTransparentTask->FreeReference();
	}
	if( infoTransparentRender ){
		infoTransparentRender->FreeReference();
	}
	if( infoTransparentSSR ){
		infoTransparentSSR->FreeReference();
	}
	if( infoTransparentLights ){
		infoTransparentLights->FreeReference();
	}
	if( infoTransparentVolumetric ){
		infoTransparentVolumetric->FreeReference();
	}
}
