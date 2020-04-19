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

#include "deoglRTChoices.h"
#include "deoglRenderThread.h"
#include "../capabilities/deoglCapabilities.h"
#include "../extensions/deoglExtensions.h"
#include "../extensions/deoglExtResult.h"

#include <dragengine/common/exceptions.h>



// Class deoglRTChoices
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTChoices::deoglRTChoices( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ){
	// base-vertex allows to render Shared-VBO instances without needing to pre-offset indices
	// using the first point of a VBO block. using base-vertex reduces the amount of VBO required.
	// 
	// NOTE shared-SPB is not affected by this since it uses instancing.
	pSharedVBOUseBaseVertex = renderThread.GetExtensions().GetHasExtension(
		deoglExtensions::ext_ARB_draw_elements_base_vertex );
	//pSharedVBOUseBaseVertex = false;
	
	// Using SSBOs allows to use a larger number of parameter block per shared SPB than using UBO
	pSharedSPBUseSSBO = renderThread.GetExtensions().GetHasExtension(
			deoglExtensions::ext_ARB_shader_storage_buffer_object );
	
	// use global shared SPB lists for SSBO. for UBO it is not favorable
	pGlobalSharedSPBLists = pSharedSPBUseSSBO;
	
	// render real transparent particles
	pRealTransparentParticles = false;
}

deoglRTChoices::~deoglRTChoices(){
}
