/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
