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

#include <stdlib.h>

#include "deoglRenderTaskConfigTexture.h"
#include "../shared/deoglRenderTaskSharedInstance.h"
#include "../shared/deoglRenderTaskSharedTexture.h"
#include "../shared/deoglRenderTaskSharedVAO.h"
#include "../../../pipeline/deoglPipeline.h"


// Class deoglRenderTaskConfigTexture
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTaskConfigTexture::deoglRenderTaskConfigTexture(){
	Clear();
}

deoglRenderTaskConfigTexture::~deoglRenderTaskConfigTexture(){
}



// Management
///////////////

void deoglRenderTaskConfigTexture::SetRenderTaskFilter( int filter ){
	pRenderTaskFilter = filter;
}

void deoglRenderTaskConfigTexture::SetPipeline( const deoglPipeline *pipeline ){
	pPipeline = pipeline;
	pPipelineIndex = pipeline ? pipeline->GetRTSPipelineIndex() : -1;
}

void deoglRenderTaskConfigTexture::SetTexture( const deoglRenderTaskSharedTexture *texture ){
	pTexture = texture;
	pTextureIndex = texture ? texture->GetIndex() : 0;
}

void deoglRenderTaskConfigTexture::SetVAO( const deoglRenderTaskSharedVAO *vao ){
	pVAO = vao;
	pVAOIndex = vao ? vao->GetIndex() : 0;
}

void deoglRenderTaskConfigTexture::SetInstance( const deoglRenderTaskSharedInstance *instance ){
	pInstance = instance;
	pInstanceIndex = instance ? instance->GetIndex() : 0;
}

void deoglRenderTaskConfigTexture::SetGroupIndex( int groupIndex ){
	pGroupIndex = groupIndex;
}



void deoglRenderTaskConfigTexture::Clear(){
	pRenderTaskFilter = 0;
	pPipeline = nullptr;
	pTexture = nullptr;
	pVAO = nullptr;
	pInstance = nullptr;
	pPipelineIndex = -1;
	pTextureIndex = 0;
	pVAOIndex = 0;
	pInstanceIndex = 0;
	pGroupIndex = 0;
}
