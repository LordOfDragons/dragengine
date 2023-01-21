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

#include "deoglRPTBuildRTsDepth.h"
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


// Class deoglRPTBuildRTsDepth
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRPTBuildRTsDepth::deoglRPTBuildRTsDepth( deoglRenderPlanTasks &plan, const deoglRenderPlanMasked *mask ) :
deParallelTask( &plan.GetPlan().GetRenderThread().GetOgl() ),
pPlan( plan ),
pMask( mask ),
pElapsedTime( 0.0f )
{
	SetMarkFinishedAfterRun( true );
}

deoglRPTBuildRTsDepth::~deoglRPTBuildRTsDepth(){
}



// Management
///////////////

void deoglRPTBuildRTsDepth::Run(){
	if( IsCancelled() ){
		return;
	}
	
	decTimer timer;
	try{
		pSolid( false );
		pSolidOutline( false );
		
		pSolid( true );
		pSolidOutline( true );
		
	}catch( const deException &e ){
		pPlan.GetPlan().GetRenderThread().GetLogger().LogException( e );
		pSemaphore.Signal();
		throw;
	}
	
	pElapsedTime = timer.GetElapsedTime();
	pSemaphore.Signal();
}

void deoglRPTBuildRTsDepth::Finished(){
	pSemaphore.Signal(); // in case cancelled before run finished
}

decString deoglRPTBuildRTsDepth::GetDebugName() const{
	return "RPTBuildRTsDepth";
}



// Private Functions
//////////////////////
 
void deoglRPTBuildRTsDepth::pSolid( bool xray ){
	deoglRenderTask &renderTask = xray ? pPlan.GetSolidDepthXRayTask() : pPlan.GetSolidDepthTask();
	const deoglCollideList &collideList = pPlan.GetPlan().GetCollideList();
	deoglAddToRenderTask addToRenderTask( pPlan.GetPlan().GetRenderThread(), renderTask );
	
	renderTask.Clear();
	renderTask.SetRenderVSStereo( pPlan.GetPlan().GetRenderStereo()
		&& pPlan.GetPlan().GetRenderThread().GetChoices().GetRenderStereoVSLayer() );
	
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoNotReflected( pPlan.GetPlan().GetNoReflections() );
	addToRenderTask.SetNoRendered( pMask );
	if( xray ){
		addToRenderTask.SetFilterXRay( true );
		addToRenderTask.SetXRay( true );
	}
	
	if( pMask && pMask->GetUseClipPlane() ){
		addToRenderTask.SetSkinPipelineType( deoglSkinTexturePipelines::etDepthClipPlane );
		
	}else{
		addToRenderTask.SetSkinPipelineType( deoglSkinTexturePipelines::etDepth );
	}
	
	int pipelineModifier = 0;
	if( pPlan.GetPlan().GetFlipCulling() ){
		pipelineModifier |= deoglSkinTexturePipelines::emFlipCullFace;
	}
	if( pPlan.GetPlan().GetRenderStereo() ){
		pipelineModifier |= deoglSkinTexturePipelines::emStereo;
	}
	addToRenderTask.SetSkinPipelineModifier( pipelineModifier );
	
	addToRenderTask.AddComponents( collideList );
	
	addToRenderTask.AddBillboards( collideList );
	
	addToRenderTask.AddPropFields( collideList, false );
	addToRenderTask.AddPropFields( collideList, true );
	
	addToRenderTask.AddHeightTerrains( collideList, true );
	
	if( pPlan.GetPlan().GetRenderThread().GetChoices().GetRealTransparentParticles() ){
		addToRenderTask.AddParticles( collideList );
	}
}

void deoglRPTBuildRTsDepth::pSolidOutline( bool xray ){
	deoglRenderTask &renderTask = xray ? pPlan.GetSolidDepthOutlineXRayTask() : pPlan.GetSolidDepthOutlineTask();
	const deoglCollideList &collideList = pPlan.GetPlan().GetCollideList();
	deoglAddToRenderTask addToRenderTask( pPlan.GetPlan().GetRenderThread(), renderTask );
	
	renderTask.Clear();
	renderTask.SetRenderVSStereo( pPlan.GetPlan().GetRenderStereo()
		&& pPlan.GetPlan().GetRenderThread().GetChoices().GetRenderStereoVSLayer() );
	
	addToRenderTask.SetOutline( true );
	addToRenderTask.SetFilterDecal( true );
	addToRenderTask.SetDecal( false );
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoNotReflected( pPlan.GetPlan().GetNoReflections() );
	addToRenderTask.SetNoRendered( pMask );
	if( xray ){
		addToRenderTask.SetFilterXRay( true );
		addToRenderTask.SetXRay( true );
	}
	
	if( pMask && pMask->GetUseClipPlane() ){
		addToRenderTask.SetSkinPipelineType( deoglSkinTexturePipelines::etDepthClipPlane );
		
	}else{
		addToRenderTask.SetSkinPipelineType( deoglSkinTexturePipelines::etDepth );
	}
	
	int pipelineModifier = 0;
	if( pPlan.GetPlan().GetFlipCulling() ){
		pipelineModifier |= deoglSkinTexturePipelines::emFlipCullFace;
	}
	if( pPlan.GetPlan().GetRenderStereo() ){
		pipelineModifier |= deoglSkinTexturePipelines::emStereo;
	}
	addToRenderTask.SetSkinPipelineModifier( pipelineModifier );
	
	if( pMask && pMask->GetUseClipPlane() ){
		addToRenderTask.SetSkinPipelineType( deoglSkinTexturePipelines::etDepthClipPlane );
		
	}else{
		addToRenderTask.SetSkinPipelineType( deoglSkinTexturePipelines::etDepth );
	}
	
	addToRenderTask.AddComponents( collideList );
}
