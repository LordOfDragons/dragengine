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

#include "deoglSTPipelinesPropField.h"
#include "../../deoglSkinTexture.h"
#include "../../shader/deoglSkinShaderManager.h"
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../renderthread/deoglRTChoices.h"
#include "../../../renderthread/deoglRTShader.h"

#include <dragengine/common/exceptions.h>



// Class deoglSTPipelinesPropField
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSTPipelinesPropField::deoglSTPipelinesPropField( const deoglSkinTexture &texture ) :
deoglSkinTexturePipelines( texture ){
}

deoglSTPipelinesPropField::~deoglSTPipelinesPropField(){
}



// Management
///////////////

const char *deoglSTPipelinesPropField::GetDebugName() const{
	return "deoglSTPipelinesPropField";
}



// Protected Functions
////////////////////////

void deoglSTPipelinesPropField::pPreparePipelines( const ChannelInfo &cinfo,
deoglShaderLoadingTimeout &timeout ){
	deoglSkinShaderConfig baseShaderConfig;
	baseShaderConfig.SetGeometryMode( deoglSkinShaderConfig::egmPropField );
	
	pPrepareGeometry( baseShaderConfig, cinfo, timeout );
	// pPrepareGeometryDepthTest( baseShaderConfig, cinfo, timeout );
	pPrepareAllDepth( baseShaderConfig, cinfo, timeout );
	pPrepareAllCounter( baseShaderConfig, cinfo, timeout );
	// pPrepareMask( baseShaderConfig, cinfo, timeout );
	pPrepareAllShadow( baseShaderConfig, cinfo, timeout );
	// pPrepareEnvMap( baseShaderConfig, cinfo, timeout );
	// pPrepareLuminance( baseShaderConfig, cinfo, timeout );
	// pPrepareGIMaterial( baseShaderConfig, cinfo, timeout );
}

void deoglSTPipelinesPropField::pPrepareShadowOrthogonalCascaded( deoglPipelineConfiguration &,
deoglSkinShaderConfig &, const ChannelInfo &, deoglShaderLoadingTimeout & ){
	// not supported
}
