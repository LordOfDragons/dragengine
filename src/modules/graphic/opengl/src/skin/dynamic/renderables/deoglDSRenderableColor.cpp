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

#include "deoglDSRenderableColor.h"
#include "render/deoglRDSRenderableColor.h"
#include "../deoglDynamicSkin.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableColor.h>



// Class deoglDSRenderableColor
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDSRenderableColor::deoglDSRenderableColor( deoglDynamicSkin &dynamicSkin, const deDSRenderableColor &renderable ) :
deoglDSRenderable( dynamicSkin, renderable ),
pRenderableColor( renderable ),
pRRenderableColor( NULL ),
pDirty( true )
{
	try{
		pRRenderableColor = new deoglRDSRenderableColor( *dynamicSkin.GetRDynamicSkin() );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglDSRenderableColor::~deoglDSRenderableColor(){
	pCleanUp();
}



// Management
///////////////

deoglRDSRenderable *deoglDSRenderableColor::GetRRenderable() const{
	return pRRenderableColor;
}

void deoglDSRenderableColor::RenderableChanged(){
	pDirty = true;
}

void deoglDSRenderableColor::SyncToRender(){
	if( pDirty ){
		pRRenderableColor->SetName( pRenderableColor.GetName() );
		pRRenderableColor->SetColor( pRenderableColor.GetColor() );
		pDirty = false;
	}
}



// Private Functions
//////////////////////

void deoglDSRenderableColor::pCleanUp(){
	if( pRRenderableColor ){
		pRRenderableColor->FreeReference();
	}
}
