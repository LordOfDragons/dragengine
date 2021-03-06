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

deoglRTChoices::deoglRTChoices( deoglRenderThread &renderThread ){
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
	
	// GI move using probes using ray cache instead of all rays
	pGIMoveUsingCache = true;
	
	// transform component vertices on the GPU
	#ifdef OS_ANDROID
		// NOTE android OpenGL ES 3.0 does not support texture buffer objects (TBO). as a replacement
		//      another input VBO can be used (uniform buffers are too small). this requires only
		//      changing the generation of TBO to be a generation of VBO data instead and changing
		//      the shader to use VBO input instead of TBO sampling. the VBO requires indexing to
		//      get the right weights. we use already an index in the model VBO to sample the right
		//      TBO texel. this indexing though is relative to the vertex but the model VBO is indexed
		//      relative to face points. this is a problem since it would require storing weight
		//      matrices per point not per vertex. if this is done it requires a copy of weight data
		//      to the VBO which would allow to reduce the resolution to 16-bit. all in all tricky
		//      
		//      the TBO uses GL_RGBA32F to allow copy the weight matrices right into the texture
		//      without additional conversion. the same is possible for VBOs.
		pGPUTransformVertices = egputvNone;
		
	#else
		pGPUTransformVertices = egputvApproximate;
	#endif
}

deoglRTChoices::~deoglRTChoices(){
}
