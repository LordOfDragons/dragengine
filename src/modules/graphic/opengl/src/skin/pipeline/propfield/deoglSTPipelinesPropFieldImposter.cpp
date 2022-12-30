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

#include "deoglSTPipelinesPropFieldImposter.h"
#include "../../deoglSkinTexture.h"
#include "../../shader/deoglSkinShaderManager.h"
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../renderthread/deoglRTChoices.h"
#include "../../../renderthread/deoglRTShader.h"

#include <dragengine/common/exceptions.h>



// Class deoglSTPipelinesPropFieldImposter
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSTPipelinesPropFieldImposter::deoglSTPipelinesPropFieldImposter( const deoglSkinTexture &texture ) :
deoglSTPipelinesPropField( texture ){
}

deoglSTPipelinesPropFieldImposter::~deoglSTPipelinesPropFieldImposter(){
}



// Management
///////////////

const char *deoglSTPipelinesPropFieldImposter::GetDebugName() const{
	return "deoglSTPipelinesPropFieldImposter";
}



// Protected Functions
////////////////////////

void deoglSTPipelinesPropFieldImposter::pPreparePipelines( const ChannelInfo &cinfo ){
	deoglSkinShaderConfig baseShaderConfig;
	baseShaderConfig.Reset();
	baseShaderConfig.SetGeometryMode( deoglSkinShaderConfig::egmPropField );
	baseShaderConfig.SetBillboard( true );
	
	pPrepareGeometry( baseShaderConfig, cinfo );
	// pPrepareGeometryDepthTest( baseShaderConfig, cinfo );
	pPrepareAllDepth( baseShaderConfig, cinfo );
	pPrepareAllCounter( baseShaderConfig, cinfo );
	// pPrepareMask( baseShaderConfig, cinfo );
	pPrepareAllShadow( baseShaderConfig, cinfo );
	// pPrepareEnvMap( baseShaderConfig, cinfo );
	// pPrepareLuminance( baseShaderConfig, cinfo );
	// pPrepareGIMaterial( baseShaderConfig, cinfo );
}

void deoglSTPipelinesPropFieldImposter::pSetBase( deoglSkinShaderConfig &config ){
	deoglSTPipelinesPropField::pSetBase( config );
	
	config.SetBillboard( true );
}
