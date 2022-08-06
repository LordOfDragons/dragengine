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
		pSolid();
		pSolidOutline();
		
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

void deoglRPTBuildRTsDepth::pSolid(){
	const deoglCollideList &collideList = pPlan.GetPlan().GetCollideList();
	deoglRenderTask &renderTask = pPlan.GetSolidDepthTask();
	deoglAddToRenderTask addToRenderTask( pPlan.GetPlan().GetRenderThread(), renderTask );
	const bool renderStereo = pPlan.GetPlan().GetRenderStereo();
	
	renderTask.Clear();
	renderTask.SetRenderVSStereo( pPlan.GetPlan().GetRenderStereo()
		&& pPlan.GetPlan().GetRenderThread().GetChoices().GetRenderStereoVSLayer() );
	
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoNotReflected( pPlan.GetPlan().GetNoReflections() );
	addToRenderTask.SetNoRendered( pMask );
	
	// components
	if( pMask && pMask->GetUseClipPlane() ){
		addToRenderTask.SetSkinShaderType( renderStereo
			? deoglSkinTexture::estStereoComponentDepthClipPlane
			: deoglSkinTexture::estComponentDepthClipPlane );
		
	}else{
		addToRenderTask.SetSkinShaderType( renderStereo
			? deoglSkinTexture::estStereoComponentDepth
			: deoglSkinTexture::estComponentDepth );
	}
	addToRenderTask.AddComponents( collideList );
	
	// billboards
	if( pMask && pMask->GetUseClipPlane() ){
		addToRenderTask.SetSkinShaderType( renderStereo
			? deoglSkinTexture::estStereoBillboardDepthClipPlane
			: deoglSkinTexture::estBillboardDepthClipPlane );
		
	}else{
		addToRenderTask.SetSkinShaderType( renderStereo
			? deoglSkinTexture::estStereoBillboardDepth
			: deoglSkinTexture::estBillboardDepth );
	}
	addToRenderTask.AddBillboards( collideList );
	
	// prop fields
	deoglSkinTexture::eShaderTypes propFieldShaderType1, propFieldShaderType2;
	
	if( pMask && pMask->GetUseClipPlane() ){
		propFieldShaderType1 = renderStereo
			? deoglSkinTexture::estStereoPropFieldDepthClipPlane
			: deoglSkinTexture::estPropFieldDepthClipPlane;
		propFieldShaderType2 = renderStereo
			? deoglSkinTexture::estStereoPropFieldImposterDepthClipPlane
			: deoglSkinTexture::estPropFieldImposterDepthClipPlane;
		
	}else{
		propFieldShaderType1 = renderStereo
			? deoglSkinTexture::estStereoPropFieldDepth
			: deoglSkinTexture::estPropFieldDepth;
		propFieldShaderType2 = renderStereo
			? deoglSkinTexture::estStereoPropFieldImposterDepth
			: deoglSkinTexture::estPropFieldImposterDepth;
	}
	
	addToRenderTask.SetSkinShaderType( propFieldShaderType1 );
	addToRenderTask.AddPropFields( collideList, false );
	
	addToRenderTask.SetSkinShaderType( propFieldShaderType2 );
	addToRenderTask.AddPropFields( collideList, true );
	
	// height terrains
	if( pMask && pMask->GetUseClipPlane() ){
		addToRenderTask.SetSkinShaderType( renderStereo
			? deoglSkinTexture::estStereoHeightMapDepthClipPlane
			: deoglSkinTexture::estHeightMapDepthClipPlane );
		
	}else{
		addToRenderTask.SetSkinShaderType( renderStereo
			? deoglSkinTexture::estStereoHeightMapDepth
			: deoglSkinTexture::estHeightMapDepth );
	}
	addToRenderTask.AddHeightTerrains( collideList, true );
	
	// particles
	if( pPlan.GetPlan().GetRenderThread().GetChoices().GetRealTransparentParticles() ){
		if( pMask && pMask->GetUseClipPlane() ){
			addToRenderTask.SetSkinShaderType( renderStereo
				? deoglSkinTexture::estStereoParticleDepthClipPlane
				: deoglSkinTexture::estParticleDepthClipPlane );
			addToRenderTask.SetSkinShaderTypeRibbon( renderStereo
				? deoglSkinTexture::estStereoParticleRibbonDepthClipPlane
				: deoglSkinTexture::estParticleRibbonDepthClipPlane );
			addToRenderTask.SetSkinShaderTypeBeam( renderStereo
				? deoglSkinTexture::estStereoParticleBeamDepthClipPlane
				: deoglSkinTexture::estParticleBeamDepthClipPlane );
			
		}else{
			addToRenderTask.SetSkinShaderType( renderStereo
				? deoglSkinTexture::estStereoParticleDepth
				: deoglSkinTexture::estParticleDepth );
			addToRenderTask.SetSkinShaderTypeRibbon( renderStereo
				? deoglSkinTexture::estStereoParticleRibbonDepth
				: deoglSkinTexture::estParticleRibbonDepth );
			addToRenderTask.SetSkinShaderTypeBeam( renderStereo
				? deoglSkinTexture::estStereoParticleBeamDepth
				: deoglSkinTexture::estParticleBeamDepth );
		}
		addToRenderTask.AddParticles( collideList );
	}
}

void deoglRPTBuildRTsDepth::pSolidOutline(){
	const deoglCollideList &collideList = pPlan.GetPlan().GetCollideList();
	deoglRenderTask &renderTask = pPlan.GetSolidDepthOutlineTask();
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
	
	if( pMask && pMask->GetUseClipPlane() ){
		addToRenderTask.SetSkinShaderType( pPlan.GetPlan().GetRenderStereo()
			? deoglSkinTexture::estStereoOutlineDepthClipPlane
			: deoglSkinTexture::estOutlineDepthClipPlane );
		
	}else{
		addToRenderTask.SetSkinShaderType( pPlan.GetPlan().GetRenderStereo()
			? deoglSkinTexture::estStereoOutlineDepth
			: deoglSkinTexture::estOutlineDepth );
	}
	addToRenderTask.AddComponents( collideList );
}
