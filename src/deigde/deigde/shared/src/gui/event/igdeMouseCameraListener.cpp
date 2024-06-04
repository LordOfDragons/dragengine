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

#include "igdeMouseCameraListener.h"
#include "../igdeCamera.h"



// Class igdeMouseCameraListener
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMouseCameraListener::igdeMouseCameraListener( igdeCamera *camera ) :
pCamera( camera ),
pEnableRotate( true ),
pEnablePan( true ),
pEnableMove( true ),
pEnableZoom( true ),
pSpeedRotate( 0.2f ),
pSpeedPan( 1.0f / 50.0f ),
pSpeedMove( 1.0f / 50.0f ),
pSpeedZoom( 0.5f / 50.0f ),
pInteraction( eiNone ),
pZoomOrigin( 0.0f ),
pZoomCurrent( 0.0f ){
}

igdeMouseCameraListener::~igdeMouseCameraListener(){
}



// Management
///////////////

void igdeMouseCameraListener::SetCamera( igdeCamera *camera ){
	if( camera == pCamera ){
		return;
	}
	
	Cancel();
	pCamera = camera;
}

void igdeMouseCameraListener::SetEnableRotate( bool enable ){
	pEnableRotate = enable;
}

void igdeMouseCameraListener::SetEnablePan( bool enable ){
	pEnablePan = enable;
}

void igdeMouseCameraListener::SetEnableMove( bool enable ){
	pEnableMove = enable;
}

void igdeMouseCameraListener::SetEnableZoom( bool enable ){
	pEnableZoom = enable;
}

void igdeMouseCameraListener::SetEnabledAll( bool enable ){
	pEnableRotate = enable;
	pEnablePan = enable;
	pEnableMove = enable;
	pEnableZoom = enable;
}



void igdeMouseCameraListener::SetSpeedRotate( float degreesPerPixel ){
	pSpeedRotate = decMath::max( degreesPerPixel, 0.0f );
}

void igdeMouseCameraListener::SetSpeedPan( float metersPerPixel ){
	pSpeedPan = decMath::max( metersPerPixel, 0.0f );
}

void igdeMouseCameraListener::SetSpeedMove( float metersPerPixel ){
	pSpeedMove = decMath::max( metersPerPixel, 0.0f );
}

void igdeMouseCameraListener::SetSpeedZoom( float metersPerPixel ){
	pSpeedZoom = decMath::max( metersPerPixel, 0.0f );
}



void igdeMouseCameraListener::SetInteraction( eInteraction interaction ){
	pInteraction = interaction;
}



void igdeMouseCameraListener::SetMoveOrigin( const decDVector &position ){
	pMoveOrigin = position;
}

void igdeMouseCameraListener::SetMoveCurrent( const decDVector &position ){
	pMoveCurrent = position;
}

void igdeMouseCameraListener::SetRotateOrigin( const decVector &rotation ){
	pRotateOrigin = rotation;
}

void igdeMouseCameraListener::SetRotationCurrent( const decVector &rotation ){
	pRotateCurrent = rotation;
}

void igdeMouseCameraListener::SetZoomOrigin( float zoom ){
	pZoomOrigin = decMath::max( zoom, 0.0f );
}

void igdeMouseCameraListener::SetZoomCurrent( float zoom ){
	pZoomCurrent = decMath::max( zoom, 0.0f );
}

void igdeMouseCameraListener::SetMoveMatrix( const decDMatrix &matrix ){
	pMoveMatrix = matrix;
}



// Interaction events
///////////////////////

igdeMouseCameraListener::eInteraction igdeMouseCameraListener::ChooseInteraction(){
	if( GetDragState() != edsRight ){
		return eiNone;
	}
	
	if( GetShiftOrigin() && GetControlOrigin() ){
		return eiZoom;
		
	}else if( GetShiftOrigin() ){
		return eiPan;
		
	}else if( GetControlOrigin() ){
		return eiMove;
		
	}else{
		return eiRotate;
	}
	
	return eiNone;
}



bool igdeMouseCameraListener::OnRotateBegin(){
	if( ! pCamera ){
		return false;
	}
	
	pRotateOrigin = pCamera->GetOrientation();
	pRotateCurrent = pRotateOrigin;
	return true;
}

void igdeMouseCameraListener::OnRotateUpdate(){
	if( ! pCamera ){
		return;
	}
	
	const decPoint distance( GetDragDistance() );
	
	pRotateCurrent.y = decMath::normalize(
		pRotateOrigin.y - pSpeedRotate * ( float )distance.x, 360.0f );
	pRotateCurrent.x = decMath::normalize(
		pRotateOrigin.x + pSpeedRotate * ( float )distance.y, 360.0f );
	
	pCamera->SetOrientation( pRotateCurrent );
	
	OnCameraChanged();
}

void igdeMouseCameraListener::OnRotateFinish( bool cancelled ){
}



bool igdeMouseCameraListener::OnPanBegin(){
	if( ! pCamera ){
		return false;
	}
	
	pMoveOrigin = pCamera->GetPosition();
	pMoveCurrent = pMoveOrigin;
	pMoveMatrix = pCamera->GetViewMatrix();
	return true;
}

void igdeMouseCameraListener::OnPanUpdate(){
	if( ! pCamera ){
		return;
	}
	
	const decPoint distance( GetDragDistance() );
	
	pMoveCurrent = pMoveOrigin
		- pMoveMatrix.TransformRight() * ( double )( pSpeedPan * ( float )distance.x )
		+ pMoveMatrix.TransformUp() * ( double )( pSpeedPan * ( float )distance.y );
	pCamera->SetPosition( pMoveCurrent );
	
	OnCameraChanged();
}

void igdeMouseCameraListener::OnPanFinish( bool cancelled ){
}



bool igdeMouseCameraListener::OnMoveBegin(){
	if( ! pCamera ){
		return false;
	}
	
	pMoveOrigin = pCamera->GetPosition();
	pMoveCurrent = pMoveOrigin;
	pMoveMatrix = pCamera->GetViewMatrix();
	return true;
}

void igdeMouseCameraListener::OnMoveUpdate(){
	if( ! pCamera ){
		return;
	}
	
	const decPoint distance( GetDragDistance() );
	
	pMoveCurrent = pMoveOrigin
		- pMoveMatrix.TransformView() * ( double )( pSpeedMove * ( float )distance.y );
	
	pCamera->SetPosition( pMoveCurrent );
	
	OnCameraChanged();
}

void igdeMouseCameraListener::OnMoveFinish( bool cancelled ){
}



bool igdeMouseCameraListener::OnZoomBegin(){
	if( ! pCamera ){
		return false;
	}
	
	pZoomOrigin = pCamera->GetDistance();
	pZoomCurrent = pZoomOrigin;
	return true;
}

void igdeMouseCameraListener::OnZoomUpdate(){
	if( ! pCamera ){
		return;
	}
	
	const decPoint distance( GetDragDistance() );
	
	pZoomCurrent = decMath::max( pZoomOrigin + pSpeedZoom * ( float )distance.y, 0.0f );
	
	pCamera->SetDistance( pZoomCurrent );
	
	OnCameraChanged();
}

void igdeMouseCameraListener::OnZoomFinish( bool cancelled ){
}



void igdeMouseCameraListener::OnCameraChanged(){
}



// Dragging events
////////////////////

bool igdeMouseCameraListener::OnDragBegin(){
	pInteraction = ChooseInteraction();
	
	switch( pInteraction ){
	case eiRotate:
		return OnRotateBegin();
		
	case eiPan:
		return OnPanBegin();
		
	case eiMove:
		return OnMoveBegin();
		
	case eiZoom:
		return OnZoomBegin();
		
	default:
		return false;
	}
}

void igdeMouseCameraListener::OnDragUpdate(){
	switch( pInteraction ){
	case eiRotate:
		OnRotateUpdate();
		break;
		
	case eiPan:
		OnPanUpdate();
		break;
		
	case eiMove:
		OnMoveUpdate();
		break;
		
	case eiZoom:
		OnZoomUpdate();
		break;
		
	default:
		break;
	}
}

void igdeMouseCameraListener::OnDragFinish( bool cancelled ){
	switch( pInteraction ){
	case eiRotate:
		OnRotateFinish( cancelled );
		break;
		
	case eiPan:
		OnPanFinish( cancelled );
		break;
		
	case eiMove:
		OnMoveFinish( cancelled );
		break;
		
	case eiZoom:
		OnZoomFinish( cancelled );
		break;
		
	default:
		break;
	}
	
	pInteraction = eiNone;
}
