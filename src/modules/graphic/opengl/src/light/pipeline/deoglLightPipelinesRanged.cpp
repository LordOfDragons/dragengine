/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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
