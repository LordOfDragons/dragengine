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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglRTRenderers.h"
#include "../rendering/debug/deoglRenderDebugDrawer.h"
#include "../rendering/debug/deoglRenderDebug.h"
#include "../rendering/deoglRenderCanvas.h"
#include "../rendering/deoglRenderDepthPass.h"
#include "../rendering/deoglRenderDevMode.h"
#include "../rendering/deoglRenderGeometry.h"
#include "../rendering/deoglRenderGeometryPass.h"
#include "../rendering/deoglRenderOcclusion.h"
#include "../rendering/deoglRenderParticles.h"
#include "../rendering/deoglRenderReflection.h"
#include "../rendering/deoglRenderSky.h"
#include "../rendering/deoglRenderToneMap.h"
#include "../rendering/deoglRenderTranspCounting.h"
#include "../rendering/deoglRenderTransparentPasses.h"
#include "../rendering/deoglRenderVR.h"
#include "../rendering/deoglRenderWorld.h"
#include "../rendering/light/deoglRenderLight.h"

#include <dragengine/common/exceptions.h>



// Class deoglRTRenderers
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTRenderers::deoglRTRenderers( deoglRenderThread &renderThread ) :
pCanvas( NULL ),
pDebugDrawer( NULL ),
pDebug( NULL ),
pDepthPass( NULL ),
pDevMode( NULL ),
pGeometry( NULL ),
pGeometryPass( NULL ),
pLight( NULL ),
pOcclusion( NULL ),
pParticles( NULL ),
pReflection( NULL ),
pSky( NULL ),
pToneMap( NULL ),
pTransparencyCounter( NULL ),
pVR( nullptr ),
pWorld( NULL )
{
	try{
		pCanvas = new deoglRenderCanvas( renderThread );
		pWorld = new deoglRenderWorld( renderThread );
		pDepthPass = new deoglRenderDepthPass( renderThread );
		pTransparencyCounter = new deoglRenderTranspCounting( renderThread );
		pSky = new deoglRenderSky( renderThread );
		pGeometry = new deoglRenderGeometry( renderThread );
		pGeometryPass = new deoglRenderGeometryPass( renderThread );
		pTransparentPasses = new deoglRenderTransparentPasses( renderThread );
		pParticles = new deoglRenderParticles( renderThread );
		pOcclusion = new deoglRenderOcclusion( renderThread );
		pVR = new deoglRenderVR( renderThread );
		
		pLight = new deoglRenderLight( renderThread, *this );
			// depends on pOcclusion to be initialized
		
		pDebug = new deoglRenderDebug( renderThread );
		pDebugDrawer = new deoglRenderDebugDrawer( renderThread );
		pDevMode = new deoglRenderDevMode( renderThread );
		pReflection = new deoglRenderReflection( renderThread );
		pToneMap = new deoglRenderToneMap( renderThread );
		
		pCanvas->AddTopLevelDebugInfo();
		pWorld->AddTopLevelDebugInfo();
		pLight->AddTopLevelDebugInfo();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRTRenderers::~deoglRTRenderers(){
	pCleanUp();
}



// Management
///////////////

void deoglRTRenderers::DevModeDebugInfoChanged(){
	pCanvas->DevModeDebugInfoChanged();
	pDebugDrawer->DevModeDebugInfoChanged();
	pDebug->DevModeDebugInfoChanged();
	pDepthPass->DevModeDebugInfoChanged();
	pDevMode->DevModeDebugInfoChanged();
	pGeometry->DevModeDebugInfoChanged();
	pGeometryPass->DevModeDebugInfoChanged();
	pLight->DevModeDebugInfoChanged();
	pOcclusion->DevModeDebugInfoChanged();
	pParticles->DevModeDebugInfoChanged();
	pReflection->DevModeDebugInfoChanged();
	pSky->DevModeDebugInfoChanged();
	pToneMap->DevModeDebugInfoChanged();
	pTransparentPasses->DevModeDebugInfoChanged();
	pTransparencyCounter->DevModeDebugInfoChanged();
	pVR->DevModeDebugInfoChanged();
	pWorld->DevModeDebugInfoChanged();
}



// Private Functions
//////////////////////

void deoglRTRenderers::pCleanUp(){
	if( pWorld ){
		delete pWorld;
	}
	if( pVR ){
		delete pVR;
	}
	if( pTransparencyCounter ){
		delete pTransparencyCounter;
	}
	if( pTransparentPasses ){
		delete pTransparentPasses;
	}
	if( pToneMap ){
		delete pToneMap;
	}
	if( pReflection ){
		delete pReflection;
	}
	if( pDevMode ){
		delete pDevMode;
	}
	if( pDepthPass ){
		delete pDepthPass;
	}
	if( pDebugDrawer ){
		delete pDebugDrawer;
	}
	if( pDebug ){
		delete pDebug;
	}
	if( pLight ){
		delete pLight;
	}
	if( pOcclusion ){
		delete pOcclusion;
	}
	if( pParticles ){
		delete pParticles;
	}
	if( pGeometryPass ){
		delete pGeometryPass;
	}
	if( pGeometry ){
		delete pGeometry;
	}
	if( pSky ){
		delete pSky;
	}
	if( pCanvas ){
		delete pCanvas;
	}
}
