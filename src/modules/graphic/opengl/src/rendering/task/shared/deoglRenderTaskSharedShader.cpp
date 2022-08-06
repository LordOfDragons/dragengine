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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglRenderTaskSharedPool.h"
#include "deoglRenderTaskSharedShader.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderTaskSharedShader
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTaskSharedShader::deoglRenderTaskSharedShader( deoglRenderTaskSharedPool &pool, int index ) :
pPool( pool ),
pIndex( index ),
pShader( NULL ),
pSPBInstanceIndexBase( -1 ),
pDrawIDOffset( -1 ){
}

deoglRenderTaskSharedShader::~deoglRenderTaskSharedShader(){
}



// Management
///////////////

void deoglRenderTaskSharedShader::SetShader( deoglShaderProgram *shader ){
	pShader = shader;
}

void deoglRenderTaskSharedShader::SetSPBInstanceIndexBase( int parameter ){
	pSPBInstanceIndexBase = parameter;
}

void deoglRenderTaskSharedShader::SetDrawIDOffset( int parameter ){
	pDrawIDOffset = parameter;
}



void deoglRenderTaskSharedShader::Clear(){
	pShader = NULL;
	pSPBInstanceIndexBase = -1;
	pDrawIDOffset = -1;
}

void deoglRenderTaskSharedShader::ReturnToPool(){
	pPool.ReturnShader( this );
}
