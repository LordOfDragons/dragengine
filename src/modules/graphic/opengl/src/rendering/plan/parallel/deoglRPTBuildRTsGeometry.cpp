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
pElapsedTime( 0.0f ){
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
		pSolid();
		pSolidTerrain();
		pSolidOutline();
		pSolidDecals();
		
	}catch( const deException &e ){
		pPlan.GetPlan().GetRenderThread().GetLogger().LogException( e );
		pSemaphore.Wait();
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

void deoglRPTBuildRTsGeometry::pSolid(){
	const deoglCollideList &collideList = pPlan.GetPlan().GetCollideList();
	deoglRenderTask &renderTask = pPlan.GetSolidGeometryTask();
	deoglAddToRenderTask addToRenderTask( pPlan.GetPlan().GetRenderThread(), renderTask );
	
	renderTask.Clear();
	
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoRendered( true );
	addToRenderTask.SetNoNotReflected( pPlan.GetPlan().GetNoReflections() );
	
	// components without decals
	addToRenderTask.SetFilterDecal( true );
	addToRenderTask.SetDecal( false );
	addToRenderTask.SetSkinShaderType( deoglSkinTexture::estComponentGeometry );
	addToRenderTask.AddComponents( collideList );
	addToRenderTask.SetFilterDecal( false );
	
	// billboards
	addToRenderTask.SetSkinShaderType( deoglSkinTexture::estBillboardGeometry );
	addToRenderTask.AddBillboards( collideList );
	
	// prop fields
	addToRenderTask.SetSkinShaderType( deoglSkinTexture::estPropFieldGeometry );
	addToRenderTask.AddPropFields( collideList, false );
	
	addToRenderTask.SetSkinShaderType( deoglSkinTexture::estPropFieldImposterGeometry );
	addToRenderTask.AddPropFields( collideList, true );
	
	// particles
	if( pPlan.GetPlan().GetRenderThread().GetChoices().GetRealTransparentParticles() ){
		addToRenderTask.SetSkinShaderType( deoglSkinTexture::estParticleGeometry );
		addToRenderTask.SetSkinShaderTypeRibbon( deoglSkinTexture::estParticleRibbonGeometry );
		addToRenderTask.SetSkinShaderTypeBeam( deoglSkinTexture::estParticleBeamGeometry );
		addToRenderTask.AddParticles( collideList );
	}
}

void deoglRPTBuildRTsGeometry::pSolidTerrain(){
	const deoglCollideList &collideList = pPlan.GetPlan().GetCollideList();
	if( collideList.GetHTSectorCount() == 0 ){
		return;
	}
	
	{ // pass 1
	deoglRenderTask &renderTask = pPlan.GetSolidGeometryHeight1Task();
	deoglAddToRenderTask addToRenderTask( pPlan.GetPlan().GetRenderThread(), renderTask );
	
	renderTask.Clear();
	
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoRendered( true );
	addToRenderTask.SetNoNotReflected( pPlan.GetPlan().GetNoReflections() );
	addToRenderTask.SetSkinShaderType( deoglSkinTexture::estHeightMapGeometry );
	addToRenderTask.AddHeightTerrains( collideList, true );
	}
	
	{ // pass 2
	deoglRenderTask &renderTask = pPlan.GetSolidGeometryHeight2Task();
	deoglAddToRenderTask addToRenderTask( pPlan.GetPlan().GetRenderThread(), renderTask );
	
	renderTask.Clear();
	
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoRendered( true );
	addToRenderTask.SetNoNotReflected( pPlan.GetPlan().GetNoReflections() );
	addToRenderTask.SetSkinShaderType( deoglSkinTexture::estHeightMapGeometry );
	addToRenderTask.AddHeightTerrains( collideList, false );
	}
}

void deoglRPTBuildRTsGeometry::pSolidOutline(){
	const deoglCollideList &collideList = pPlan.GetPlan().GetCollideList();
	deoglRenderTask &renderTask = pPlan.GetSolidGeometryOutlineTask();
	deoglAddToRenderTask addToRenderTask( pPlan.GetPlan().GetRenderThread(), renderTask );
	
	renderTask.Clear();
	
	addToRenderTask.SetOutline( true );
	addToRenderTask.SetFilterDecal( true );
	addToRenderTask.SetDecal( false );
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoRendered( true );
	addToRenderTask.SetNoNotReflected( pPlan.GetPlan().GetNoReflections() );
	
	addToRenderTask.SetSkinShaderType( deoglSkinTexture::estOutlineGeometry );
	addToRenderTask.AddComponents( collideList );
}

void deoglRPTBuildRTsGeometry::pSolidDecals(){
	const deoglCollideList &collideList = pPlan.GetPlan().GetCollideList();
	deoglRenderTask &renderTask = pPlan.GetSolidDecalsTask();
	deoglAddToRenderTask addToRenderTask( pPlan.GetPlan().GetRenderThread(), renderTask );
	
	renderTask.Clear();
	
	addToRenderTask.SetNoRendered( true );
	addToRenderTask.SetNoNotReflected( pPlan.GetPlan().GetNoReflections() );
	addToRenderTask.SetSkinShaderType( deoglSkinTexture::estDecalGeometry );
	
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
