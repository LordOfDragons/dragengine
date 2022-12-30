/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include <stdio.h>
#include <string.h>

#include "deoglRPTBuildRTsGeometry.h"
#include "../deoglRenderPlan.h"
#include "../deoglRenderPlanTasks.h"
#include "../deoglRenderPlanMasked.h"
#include "../../../deGraphicOpenGl.h"
#include "../../../rendering/task/deoglAddToRenderTask.h"
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../renderthread/deoglRTChoices.h"
#include "../../../renderthread/deoglRTLogger.h"
#include "../../../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decTimer.h>


// Class deoglRPTBuildRTsGeometry
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglRPTBuildRTsGeometry::deoglRPTBuildRTsGeometry( deoglRenderPlanTasks &plan, const deoglRenderPlanMasked *mask ) :
deParallelTask( &plan.GetPlan().GetRenderThread().GetOgl() ),
pPlan( plan ),
pMask( mask ),
pElapsedTime( 0.0f )
{
	(void)pMask; // silence compiler warning
	SetMarkFinishedAfterRun( true );
}

deoglRPTBuildRTsGeometry::~deoglRPTBuildRTsGeometry(){
}



// Management
///////////////

void deoglRPTBuildRTsGeometry::Run(){
	if( IsCancelled() ){
		return;
	}
	
	decTimer timer;
	try{
		pInitPipelineModifier();
		
		pSolid( false );
		pSolidTerrain( false );
		pSolidOutline( false );
		pSolidDecals( false );
		
		pSolid( true );
		pSolidTerrain( true );
		pSolidOutline( true );
		pSolidDecals( true );
		
	}catch( const deException &e ){
		pPlan.GetPlan().GetRenderThread().GetLogger().LogException( e );
		pSemaphore.Signal();
		throw;
	}
	
	pElapsedTime = timer.GetElapsedTime();
	pSemaphore.Signal();
}

void deoglRPTBuildRTsGeometry::Finished(){
	pSemaphore.Signal(); // in case cancelled before run finished
}

decString deoglRPTBuildRTsGeometry::GetDebugName() const{
	return "RPTBuildRTsGeometry";
}



// Private Functions
//////////////////////

void deoglRPTBuildRTsGeometry::pInitPipelineModifier(){
	pPipelineModifier = 0;
	if( pPlan.GetPlan().GetFlipCulling() ){
		pPipelineModifier |= deoglSkinTexturePipelines::emFlipCullFace;
	}
	if( pPlan.GetPlan().GetRenderStereo() ){
		pPipelineModifier |= deoglSkinTexturePipelines::emStereo;
	}
}

void deoglRPTBuildRTsGeometry::pSolid( bool xray ){
	deoglRenderTask &renderTask = xray ? pPlan.GetSolidGeometryXRayTask() : pPlan.GetSolidGeometryTask();
	const deoglCollideList &collideList = pPlan.GetPlan().GetCollideList();
	deoglAddToRenderTask addToRenderTask( pPlan.GetPlan().GetRenderThread(), renderTask );
	
	renderTask.Clear();
	renderTask.SetRenderVSStereo( pPlan.GetPlan().GetRenderStereo()
		&& pPlan.GetPlan().GetRenderThread().GetChoices().GetRenderStereoVSLayer() );
	
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoRendered( true );
	addToRenderTask.SetNoNotReflected( pPlan.GetPlan().GetNoReflections() );
	if( xray ){
		addToRenderTask.SetFilterXRay( true );
		addToRenderTask.SetXRay( true );
	}
	addToRenderTask.SetSkinPipelineType( deoglSkinTexturePipelines::etGeometry );
	addToRenderTask.SetSkinPipelineModifier( pPipelineModifier );
	
	// components without decals
	addToRenderTask.SetFilterDecal( true );
	addToRenderTask.SetDecal( false );
	addToRenderTask.AddComponents( collideList );
	addToRenderTask.SetFilterDecal( false );
	
	addToRenderTask.AddBillboards( collideList );
	
	addToRenderTask.AddPropFields( collideList, false );
	addToRenderTask.AddPropFields( collideList, true );
	
	if( pPlan.GetPlan().GetRenderThread().GetChoices().GetRealTransparentParticles() ){
		addToRenderTask.AddParticles( collideList );
	}
}

void deoglRPTBuildRTsGeometry::pSolidTerrain( bool xray ){
	const deoglCollideList &collideList = pPlan.GetPlan().GetCollideList();
	if( collideList.GetHTSectorCount() == 0 ){
		return;
	}
	
	{ // pass 1
	deoglRenderTask &renderTask = xray ? pPlan.GetSolidGeometryHeight1XRayTask() : pPlan.GetSolidGeometryHeight1Task();
	deoglAddToRenderTask addToRenderTask( pPlan.GetPlan().GetRenderThread(), renderTask );
	
	renderTask.Clear();
	renderTask.SetRenderVSStereo( pPlan.GetPlan().GetRenderStereo()
		&& pPlan.GetPlan().GetRenderThread().GetChoices().GetRenderStereoVSLayer() );
	
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoRendered( true );
	addToRenderTask.SetNoNotReflected( pPlan.GetPlan().GetNoReflections() );
	if( xray ){
		addToRenderTask.SetFilterXRay( true );
		addToRenderTask.SetXRay( true );
	}
	addToRenderTask.SetSkinPipelineType( deoglSkinTexturePipelines::etGeometry );
	addToRenderTask.SetSkinPipelineModifier( pPipelineModifier );
	
	addToRenderTask.AddHeightTerrains( collideList, true );
	}
	
	{ // pass 2
	deoglRenderTask &renderTask = xray ? pPlan.GetSolidGeometryHeight2XRayTask() : pPlan.GetSolidGeometryHeight2Task();
	deoglAddToRenderTask addToRenderTask( pPlan.GetPlan().GetRenderThread(), renderTask );
	
	renderTask.Clear();
	renderTask.SetRenderVSStereo( pPlan.GetPlan().GetRenderStereo()
		&& pPlan.GetPlan().GetRenderThread().GetChoices().GetRenderStereoVSLayer() );
	
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoRendered( true );
	addToRenderTask.SetNoNotReflected( pPlan.GetPlan().GetNoReflections() );
	if( xray ){
		addToRenderTask.SetFilterXRay( true );
		addToRenderTask.SetXRay( true );
	}
	addToRenderTask.SetSkinPipelineType( deoglSkinTexturePipelines::etGeometry );
	addToRenderTask.SetSkinPipelineModifier( pPipelineModifier );
	
	addToRenderTask.AddHeightTerrains( collideList, false );
	}
}

void deoglRPTBuildRTsGeometry::pSolidOutline( bool xray ){
	deoglRenderTask &renderTask = xray ? pPlan.GetSolidGeometryOutlineXRayTask() : pPlan.GetSolidGeometryOutlineTask();
	const deoglCollideList &collideList = pPlan.GetPlan().GetCollideList();
	deoglAddToRenderTask addToRenderTask( pPlan.GetPlan().GetRenderThread(), renderTask );
	
	renderTask.Clear();
	renderTask.SetRenderVSStereo( pPlan.GetPlan().GetRenderStereo()
		&& pPlan.GetPlan().GetRenderThread().GetChoices().GetRenderStereoVSLayer() );
	
	addToRenderTask.SetOutline( true );
	addToRenderTask.SetFilterDecal( true );
	addToRenderTask.SetDecal( false );
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoRendered( true );
	addToRenderTask.SetNoNotReflected( pPlan.GetPlan().GetNoReflections() );
	if( xray ){
		addToRenderTask.SetFilterXRay( true );
		addToRenderTask.SetXRay( true );
	}
	addToRenderTask.SetSkinPipelineType( deoglSkinTexturePipelines::etGeometry );
	addToRenderTask.SetSkinPipelineModifier( pPipelineModifier );
	
	addToRenderTask.AddComponents( collideList );
}

void deoglRPTBuildRTsGeometry::pSolidDecals( bool xray ){
	deoglRenderTask &renderTask = xray ? pPlan.GetSolidDecalsXRayTask() : pPlan.GetSolidDecalsTask();
	const deoglCollideList &collideList = pPlan.GetPlan().GetCollideList();
	deoglAddToRenderTask addToRenderTask( pPlan.GetPlan().GetRenderThread(), renderTask );
	
	renderTask.Clear();
	renderTask.SetRenderVSStereo( pPlan.GetPlan().GetRenderStereo()
		&& pPlan.GetPlan().GetRenderThread().GetChoices().GetRenderStereoVSLayer() );
	
	addToRenderTask.SetNoRendered( true );
	addToRenderTask.SetNoNotReflected( pPlan.GetPlan().GetNoReflections() );
	if( xray ){
		addToRenderTask.SetFilterXRay( true );
		addToRenderTask.SetXRay( true );
	}
	addToRenderTask.SetSkinPipelineType( deoglSkinTexturePipelines::etGeometry );
	addToRenderTask.SetSkinPipelineModifier( pPipelineModifier );
	
	// component model decals
	addToRenderTask.SetFilterDecal( true );
	addToRenderTask.SetDecal( true );
	
	addToRenderTask.SetSolid( true );
	addToRenderTask.AddComponents( collideList );
	
	addToRenderTask.SetSolid( false );
	addToRenderTask.AddComponents( collideList );
	addToRenderTask.SetFilterDecal( false );
	
	// component attached decals
	addToRenderTask.SetSolid( true );
	addToRenderTask.AddDecals( collideList );
}
