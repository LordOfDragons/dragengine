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

#include "deComponentTexture.h"
#include "../skin/deSkin.h"
#include "../skin/dynamic/deDynamicSkin.h"
#include "../image/deImage.h"
#include "../../common/exceptions.h"



// Class deComponentTexture
/////////////////////////////

// Constructor, destructor
////////////////////////////

deComponentTexture::deComponentTexture(){
	pSkin = NULL;
	pTexture = 0;
	pDynamicSkin = NULL;
}

deComponentTexture::~deComponentTexture(){
	if( pDynamicSkin ){
		pDynamicSkin->FreeReference();
	}
	if( pSkin ){
		pSkin->FreeReference();
	}
}



// Management
///////////////

void deComponentTexture::SetSkin( deSkin *skin ){
	if( skin != pSkin ){
		if( pSkin ){
			pSkin->FreeReference();
		}
		
		pSkin = skin;
		
		if( skin ){
			skin->AddReference();
		}
	}
}

void deComponentTexture::SetTexture( int texture ){
	pTexture = texture;
}

void deComponentTexture::SetTransform( const decTexMatrix2 &matrix ){
	pTransform = matrix;
}

void deComponentTexture::SetDynamicSkin( deDynamicSkin *dynamicSkin ){
	if( dynamicSkin == pDynamicSkin ){
		return;
	}
	
	if( pDynamicSkin ){
		pDynamicSkin->FreeReference();
	}
	
	pDynamicSkin = dynamicSkin;
	
	if( dynamicSkin ){
		dynamicSkin->AddReference();
	}
}
