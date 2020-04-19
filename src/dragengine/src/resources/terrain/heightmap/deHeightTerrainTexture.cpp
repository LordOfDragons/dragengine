/* 
 * Drag[en]gine Game Engine
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

#include "deHeightTerrainTexture.h"
#include "../../skin/deSkin.h"
#include "../../image/deImage.h"
#include "../../../common/exceptions.h"



// Class deHeightTerrainTexture
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deHeightTerrainTexture::deHeightTerrainTexture(){
	pSkin = NULL;
	
	pProjScaling.Set( 1.0f, 1.0f );
	pProjRotation = 0.0f;
	
	pMaskImage = NULL;
}

deHeightTerrainTexture::~deHeightTerrainTexture(){
	if( pMaskImage ) pMaskImage->FreeReference();
	if( pSkin ) pSkin->FreeReference();
}



// Management
///////////////


void deHeightTerrainTexture::SetSkin( deSkin *skin ){
	if( pSkin ) pSkin->FreeReference();
	
	pSkin = skin;
	
	if( skin ) skin->AddReference();
}



void deHeightTerrainTexture::SetProjectionOffset( const decVector2 &offset ){
	pProjOffset = offset;
}

void deHeightTerrainTexture::SetProjectionScaling( const decVector2 &scaling ){
	pProjScaling = scaling;
}

void deHeightTerrainTexture::SetProjectionRotation( float rotation ){
	pProjRotation = rotation;
}



void deHeightTerrainTexture::SetMaskImage( deImage *image ){
	if( pMaskImage ) pMaskImage->FreeReference();
	
	pMaskImage = image;
	
	if( image ) image->AddReference();
}
