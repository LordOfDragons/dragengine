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
	
	renderTask.Clear();
	
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoNotReflected( pPlan.GetPlan().GetNoReflections() );
	addToRenderTask.SetNoRendered( pMask );
	
	// components
	if( pMask && pMask->GetUseClipPlane() ){
		addToRenderTask.SetSkinShaderType( deoglSkinTexture::estComponentDepthClipPlane );
		
	}else{
		addToRenderTask.SetSkinShaderType( deoglSkinTexture::estComponentDepth );
	}
	addToRenderTask.AddComponents( collideList );
	
	// billboards
	if( pMask && pMask->GetUseClipPlane() ){
		addToRenderTask.SetSkinShaderType( deoglSkinTexture::estBillboardDepthClipPlane );
		
	}else{
		addToRenderTask.SetSkinShaderType( deoglSkinTexture::estBillboardDepth );
	}
	addToRenderTask.AddBillboards( collideList );
	
	// prop fields
	deoglSkinTexture::eShaderTypes propFieldShaderType1, propFieldShaderType2;
	
	if( pMask && pMask->GetUseClipPlane() ){
		propFieldShaderType1 = deoglSkinTexture::estPropFieldDepthClipPlane;
		propFieldShaderType2 = deoglSkinTexture::estPropFieldImposterDepthClipPlane;
		
	}else{
		propFieldShaderType1 = deoglSkinTexture::estPropFieldDepth;
		propFieldShaderType2 = deoglSkinTexture::estPropFieldImposterDepth;
	}
	
	addToRenderTask.SetSkinShaderType( propFieldShaderType1 );
	addToRenderTask.AddPropFields( collideList, false );
	
	addToRenderTask.SetSkinShaderType( propFieldShaderType2 );
	addToRenderTask.AddPropFields( collideList, true );
	
	// height terrains
	if( pMask && pMask->GetUseClipPlane() ){
		addToRenderTask.SetSkinShaderType( deoglSkinTexture::estHeightMapDepthClipPlane );
		
	}else{
		addToRenderTask.SetSkinShaderType( deoglSkinTexture::estHeightMapDepth );
	}
	addToRenderTask.AddHeightTerrains( collideList, true );
	
	// particles
	if( pPlan.GetPlan().GetRenderThread().GetChoices().GetRealTransparentParticles() ){
		if( pMask && pMask->GetUseClipPlane() ){
			addToRenderTask.SetSkinShaderType(
				deoglSkinTexture::estParticleDepthClipPlane );
			addToRenderTask.SetSkinShaderTypeRibbon(
				deoglSkinTexture::estParticleRibbonDepthClipPlane );
			addToRenderTask.SetSkinShaderTypeBeam(
				deoglSkinTexture::estParticleBeamDepthClipPlane );
			
		}else{
			addToRenderTask.SetSkinShaderType( deoglSkinTexture::estParticleDepth );
			addToRenderTask.SetSkinShaderTypeRibbon( deoglSkinTexture::estParticleRibbonDepth );
			addToRenderTask.SetSkinShaderTypeBeam( deoglSkinTexture::estParticleBeamDepth );
		}
		addToRenderTask.AddParticles( collideList );
	}
}

void deoglRPTBuildRTsDepth::pSolidOutline(){
	const deoglCollideList &collideList = pPlan.GetPlan().GetCollideList();
	deoglRenderTask &renderTask = pPlan.GetSolidDepthOutlineTask();
	deoglAddToRenderTask addToRenderTask( pPlan.GetPlan().GetRenderThread(), renderTask );
	
	renderTask.Clear();
	
	addToRenderTask.SetOutline( true );
	addToRenderTask.SetFilterDecal( true );
	addToRenderTask.SetDecal( false );
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoNotReflected( pPlan.GetPlan().GetNoReflections() );
	addToRenderTask.SetNoRendered( pMask );
	
	if( pMask && pMask->GetUseClipPlane() ){
		addToRenderTask.SetSkinShaderType( deoglSkinTexture::estOutlineDepthClipPlane );
		
	}else{
		addToRenderTask.SetSkinShaderType( deoglSkinTexture::estOutlineDepth );
	}
	addToRenderTask.AddComponents( collideList );
}
