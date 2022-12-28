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

#include "deoglSTPipelinesHeightMap2.h"
#include "../../deoglSkinTexture.h"
#include "../../shader/deoglSkinShaderManager.h"
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../renderthread/deoglRTChoices.h"
#include "../../../renderthread/deoglRTShader.h"

#include <dragengine/common/exceptions.h>



// Class deoglSTPipelinesHeightMap2
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSTPipelinesHeightMap2::deoglSTPipelinesHeightMap2( const deoglSkinTexture &texture ) :
	deoglSTPipelinesHeightMap1 ( texture ){
}

deoglSTPipelinesHeightMap2::~deoglSTPipelinesHeightMap2(){
}



// Protected Functions
////////////////////////

void deoglSTPipelinesHeightMap2::pPipelineConfigGeometry( deoglPipelineConfiguration &config ){
	deoglSTPipelinesHeightMap1::pPipelineConfigGeometry( config );
	
	config.EnableBlendBlend();
}
