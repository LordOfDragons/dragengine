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

#include "deoglSky.h"
#include "deoglRSky.h"
#include "../deGraphicOpenGl.h"
#include "../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyLayer.h>



// Class deoglSky
///////////////////

// Constructor, destructor
////////////////////////////

deoglSky::deoglSky( deGraphicOpenGl &ogl, const deSky &sky ) :
pOgl( ogl ),
pSky( sky ),
pRSky( NULL ),
pDirtyParameters( true ),
pUpdateTracker( 0 )
{
	pRSky = new deoglRSky( pOgl.GetRenderThread() );
}

deoglSky::~deoglSky(){
	if( pRSky ){
		pRSky->FreeReference();
		pRSky = NULL;
	}
}



// Management
///////////////

void deoglSky::SyncToRender(){
	if( pDirtyParameters ){
		pDirtyParameters = false;
		
		pRSky->SetBgColor( pSky.GetBgColor() );
		pRSky->RebuildLinks( pSky );
		pRSky->RebuildLayers( pSky );
	}
}



// Notifications
//////////////////

void deoglSky::ParametersChanged(){
	if( pDirtyParameters ){
		return;
	}
	
	pDirtyParameters = true;
	pUpdateTracker++;
}
