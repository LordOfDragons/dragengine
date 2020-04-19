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

#include "deoglDSRenderableValue.h"
#include "render/deoglRDSRenderableValue.h"
#include "../deoglDynamicSkin.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableValue.h>



// Class deoglDSRenderableValue
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDSRenderableValue::deoglDSRenderableValue( deoglDynamicSkin &dynamicSkin, const deDSRenderableValue &renderable ) :
deoglDSRenderable( dynamicSkin, renderable ),
pRenderableValue( renderable ),
pRRenderableValue( NULL ),
pDirty( true )
{
	try{
		pRRenderableValue = new deoglRDSRenderableValue( *dynamicSkin.GetRDynamicSkin() );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglDSRenderableValue::~deoglDSRenderableValue(){
	pCleanUp();
}



// Management
///////////////

deoglRDSRenderable *deoglDSRenderableValue::GetRRenderable() const{
	return pRRenderableValue;
}

void deoglDSRenderableValue::RenderableChanged(){
	pDirty = true;
}

void deoglDSRenderableValue::SyncToRender(){
	if( pDirty ){
		pRRenderableValue->SetName( pRenderableValue.GetName() );
		pRRenderableValue->SetValue( pRenderableValue.GetValue() );
		pDirty = false;
	}
}



// Private Functions
//////////////////////

void deoglDSRenderableValue::pCleanUp(){
	if( pRRenderableValue ){
		pRRenderableValue->FreeReference();
	}
}
