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

#include "deoglCanvasRenderWorld.h"
#include "render/deoglRCanvasRenderWorld.h"
#include "../deGraphicOpenGl.h"
#include "../world/deoglCamera.h"
#include "../world/deoglWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvas.h>
#include <dragengine/resources/canvas/deCanvasRenderWorld.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/world/deWorld.h>



// Class deoglCanvasRenderWorld
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCanvasRenderWorld::deoglCanvasRenderWorld( deGraphicOpenGl &ogl, deCanvasRenderWorld &canvas ) :
deoglCanvas( ogl, canvas ),
pCanvasRenderWorld( canvas ),
pRCanvasRenderWorld( NULL ),
pCamera( NULL ){
}

deoglCanvasRenderWorld::~deoglCanvasRenderWorld(){
	if( pCamera ){
		pCamera->GetNotifyCanvas().Remove( this );
	}
}



// Management
///////////////

void deoglCanvasRenderWorld::DropRCanvas(){
	pRCanvasRenderWorld = NULL;
	deoglCanvas::DropRCanvas();
}

void deoglCanvasRenderWorld::SyncContentToRender(){
	if( pCamera ){
		pRCanvasRenderWorld->SetCamera( pCamera->GetRCamera() );
		
	}else{
		pRCanvasRenderWorld->SetCamera( NULL );
	}
	
	if( pCamera ){
		pCamera->SyncToRender();
	}
}

void deoglCanvasRenderWorld::CameraRequiresSync(){
	deoglCanvas::ContentChanged();
}

void deoglCanvasRenderWorld::DropCamera(){
	pCamera = NULL;
}



// Notifications
//////////////////

void deoglCanvasRenderWorld::ContentChanged(){
	if( pCamera ){
		pCamera->GetNotifyCanvas().Remove( this );
	}
	
	if( pCanvasRenderWorld.GetCamera() ){
		pCamera = ( deoglCamera* )pCanvasRenderWorld.GetCamera()->GetPeerGraphic();
		pCamera->GetNotifyCanvas().Add( this );
		
	}else{
		pCamera = NULL;
	}
	
	deoglCanvas::ContentChanged();
}



// Protected Functions
////////////////////////

deoglRCanvas *deoglCanvasRenderWorld::CreateRCanvas(){
	pRCanvasRenderWorld = new deoglRCanvasRenderWorld( GetOgl().GetRenderThread() );
	return pRCanvasRenderWorld;
}
