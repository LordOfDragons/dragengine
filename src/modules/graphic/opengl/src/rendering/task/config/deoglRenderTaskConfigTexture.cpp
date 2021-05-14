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
#include "../shared/deoglRenderTaskSharedShader.h"
#include "../shared/deoglRenderTaskSharedTexture.h"
#include "../shared/deoglRenderTaskSharedVAO.h"


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

void deoglRenderTaskConfigTexture::SetShader( const deoglRenderTaskSharedShader *shader ){
	pShader = shader;
	pIndexShader = 0;
	if( shader ){
		pIndexShader = shader->GetIndex();
	}
}

void deoglRenderTaskConfigTexture::SetTexture( const deoglRenderTaskSharedTexture *texture ){
	pTexture = texture;
	pIndexTexture = 0;
	if( texture ){
		pIndexTexture = texture->GetIndex();
	}
}

void deoglRenderTaskConfigTexture::SetVAO( const deoglRenderTaskSharedVAO *vao ){
	pVAO = vao;
	pIndexVAO = 0;
	if( vao ){
		pIndexVAO = vao->GetIndex();
	}
}

void deoglRenderTaskConfigTexture::SetInstance( const deoglRenderTaskSharedInstance *instance ){
	pInstance = instance;
	pIndexInstance = 0;
	if( instance ){
		pIndexInstance = instance->GetIndex();
	}
}

void deoglRenderTaskConfigTexture::SetGroupIndex( int groupIndex ){
	pGroupIndex = groupIndex;
}



void deoglRenderTaskConfigTexture::Clear(){
	pRenderTaskFilter = 0;
	pShader = NULL;
	pTexture = NULL;
	pVAO = NULL;
	pInstance = NULL;
	pIndexShader = 0;
	pIndexTexture = 0;
	pIndexVAO = 0;
	pIndexInstance = 0;
	pGroupIndex = 0;
}
