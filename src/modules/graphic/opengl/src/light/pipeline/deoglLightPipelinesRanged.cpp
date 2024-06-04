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

#include "deoglLightPipelinesRanged.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../light/deoglRLight.h"

#include <dragengine/common/exceptions.h>


// Class deoglLightPipelinesRanged
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglLightPipelinesRanged::deoglLightPipelinesRanged( const deoglRLight &light ) :
deoglLightPipelines( light.GetRenderThread() ),
pLight( light ){
}

deoglLightPipelinesRanged::~deoglLightPipelinesRanged(){
}



// Protected Functions
/////////////////////////

void deoglLightPipelinesRanged::pSetBaseNoShadow( deoglLightShaderConfig &shaconf ){
	pSetNonGI( shaconf );
	shaconf.SetSubSurface( pLight.GetRenderThread().GetConfiguration().GetSSSSSEnable() );
}

void deoglLightPipelinesRanged::pSetBaseSolid1( deoglLightShaderConfig &shaconf ){
	pSetNonGI( shaconf );
	shaconf.SetSubSurface( pLight.GetRenderThread().GetConfiguration().GetSSSSSEnable() );
	shaconf.SetTextureShadow1Solid( true );
	shaconf.SetTextureShadow1Ambient( true );
}

void deoglLightPipelinesRanged::pSetBaseSolid1Transp1( deoglLightShaderConfig &shaconf ){
	pSetNonGI( shaconf );
	shaconf.SetSubSurface( pLight.GetRenderThread().GetConfiguration().GetSSSSSEnable() );
	shaconf.SetTextureShadow1Solid( true );
	shaconf.SetTextureShadow1Transparent( true );
	shaconf.SetTextureShadow1Ambient( true );
}

void deoglLightPipelinesRanged::pSetBaseSolid2( deoglLightShaderConfig &shaconf ){
	pSetNonGI( shaconf );
	shaconf.SetSubSurface( pLight.GetRenderThread().GetConfiguration().GetSSSSSEnable() );
	shaconf.SetTextureShadow1Solid( true );
	shaconf.SetTextureShadow2Solid( true );
	shaconf.SetTextureShadow1Ambient( true );
	shaconf.SetTextureShadow2Ambient( true );
}

void deoglLightPipelinesRanged::pSetBaseSolid2Transp1( deoglLightShaderConfig &shaconf ){
	pSetNonGI( shaconf );
	shaconf.SetSubSurface( pLight.GetRenderThread().GetConfiguration().GetSSSSSEnable() );
	shaconf.SetTextureShadow1Solid( true );
	shaconf.SetTextureShadow1Transparent( true );
	shaconf.SetTextureShadow2Solid( true );
	shaconf.SetTextureShadow1Ambient( true );
	shaconf.SetTextureShadow2Ambient( true );
}

void deoglLightPipelinesRanged::pSetBaseSolid2Transp2( deoglLightShaderConfig &shaconf ){
	pSetNonGI( shaconf );
	shaconf.SetSubSurface( pLight.GetRenderThread().GetConfiguration().GetSSSSSEnable() );
	shaconf.SetTextureShadow1Solid( true );
	shaconf.SetTextureShadow1Transparent( true );
	shaconf.SetTextureShadow2Solid( true );
	shaconf.SetTextureShadow2Transparent( true );
	shaconf.SetTextureShadow1Ambient( true );
	shaconf.SetTextureShadow2Ambient( true );
}

void deoglLightPipelinesRanged::pSetBaseLumSolid1( deoglLightShaderConfig &shaconf ){
	pSetNonGI( shaconf );
	shaconf.SetTextureShadow1Solid( true );
	shaconf.SetTextureShadow1Ambient( true );
}

void deoglLightPipelinesRanged::pSetBaseLumSolid2( deoglLightShaderConfig &shaconf ){
	pSetNonGI( shaconf );
	shaconf.SetTextureShadow1Solid( true );
	shaconf.SetTextureShadow2Solid( true );
	shaconf.SetTextureShadow1Ambient( true );
	shaconf.SetTextureShadow2Ambient( true );
}

void deoglLightPipelinesRanged::pSetBaseGIRayNoShadow( deoglLightShaderConfig &shaconf ){
	pSetGI( shaconf );
}

void deoglLightPipelinesRanged::pSetBaseGIRaySolid1( deoglLightShaderConfig &shaconf ){
	pSetGI( shaconf );
	shaconf.SetTextureShadow1Solid( true );
}

void deoglLightPipelinesRanged::pSetBaseGIRaySolid2( deoglLightShaderConfig &shaconf ){
	pSetGI( shaconf );
	shaconf.SetTextureShadow1Solid( true );
	shaconf.SetTextureShadow2Solid( true );
}



void deoglLightPipelinesRanged::pSetGI( deoglLightShaderConfig &shaconf ){
	shaconf.SetGIRay( true );
	shaconf.SetFullScreenQuad( true );
	shaconf.SetMaterialNormalModeDec( deoglLightShaderConfig::emnmFloat );
}

void deoglLightPipelinesRanged::pSetNonGI( deoglLightShaderConfig &shaconf ){
	shaconf.SetMaterialNormalModeDec( deoglLightShaderConfig::emnmFloat );
}
