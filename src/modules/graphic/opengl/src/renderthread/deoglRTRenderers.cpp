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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglRTRenderers.h"
#include "../rendering/debug/deoglRenderDebugDrawer.h"
#include "../rendering/debug/deoglRenderDebug.h"
#include "../rendering/deoglRenderCanvas.h"
#include "../rendering/deoglRenderConstructed.h"
#include "../rendering/deoglRenderCompute.h"
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
pCanvas( nullptr ),
pConstructed( nullptr ),
pCompute( nullptr ),
pDebugDrawer( nullptr ),
pDebug( nullptr ),
pDepthPass( nullptr ),
pDevMode( nullptr ),
pGeometry( nullptr ),
pGeometryPass( nullptr ),
pLight( nullptr ),
pOcclusion( nullptr ),
pParticles( nullptr ),
pReflection( nullptr ),
pSky( nullptr ),
pToneMap( nullptr ),
pTransparencyCounter( nullptr ),
pTransparentPasses( nullptr ),
pVR( nullptr ),
pWorld( nullptr )
{
	try{
		pCanvas = new deoglRenderCanvas( renderThread );
		pConstructed = new deoglRenderConstructed( renderThread );
		pCompute = new deoglRenderCompute( renderThread );
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
		pConstructed->AddTopLevelDebugInfo();
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
	pConstructed->DevModeDebugInfoChanged();
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
	if( pCompute ){
		delete pCompute;
	}
	if( pConstructed ){
		delete pConstructed;
	}
	if( pCanvas ){
		delete pCanvas;
	}
}
