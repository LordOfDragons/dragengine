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

#include <stdlib.h>

#include "deainpOverlayCirclePad.h"
#include "../deAndroidInput.h"
#include "../deainpDeviceButton.h"
#include "../deainpDeviceKeyboard.h"
#include "../deainpDeviceMouse.h"
#include "../deainpDeviceManager.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasImage.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>



// Class deainpOverlayCirclePad
///////////////////////////////

// Constructor, destructor
////////////////////////////

deainpOverlayCirclePad::deainpOverlayCirclePad( deAndroidInput &androidInput ) :
deainpOverlay( androidInput ),

pPressedLeft( false ),
pPressedUp( false ),
pPressedRight( false ),
pPressedDown( false ),

pPressedUpLeft( false ),
pPressedUpRight( false ),
pPressedDownLeft( false ),
pPressedDownRight( false ),

pPadRadius( 192 ), // 128
pOuterRadius( 0.9f ),
pInnerRadius( 0.05f ),
pCenter( 192, 192 ),

pLeftRightPerSecond( 1000.0f ),
pUpDownPerSecond( 1000.0f ),

pCImage( NULL )
{
	deCanvasManager &canvasManager = *androidInput.GetGameEngine()->GetCanvasManager();
	deImageManager &imageManager = *androidInput.GetGameEngine()->GetImageManager();
	deImage *image = NULL;
	
	const decPoint size( pPadRadius * 2, pPadRadius * 2 );
	
	SetLayoutHorizontal( deainpLayout( decPoint(), size ) );
	SetLayoutVertical( deainpLayout( decPoint(), size ) );
	
	try{
		GetCanvas()->SetSize( size );
		
		pCImage = canvasManager.CreateCanvasImage();
		pCImage->SetSize( size );
		image = imageManager.LoadImage( &GetAndroidInput().GetVFS(), "/share/images/circlepad.png", "/" );
		pCImage->SetImage( image );
		image->FreeReference();
		image = NULL;
		GetCanvas()->AddCanvas( pCImage );
		
		/*
		deCanvasPaint *temp = canvasManager.CreateCanvasPaint();
		temp->SetSize( decPoint( 256, 256 ) );
		temp->SetShapeType( deCanvasPaint::estRectangle );
		temp->SetLineColor( decColor( 1.0f, 0.0f, 0.0f, 0.25f ) );
		temp->SetFillColor( decColor( 1.0f, 1.0f, 1.0f, 0.25f ) );
		GetCanvas()->AddCanvas( temp );
		temp->FreeReference();
		*/
		
	}catch( const deException & ){
		if( image ){
			image->FreeReference();
		}
		pCleanUp();
		throw;
	}
}

deainpOverlayCirclePad::~deainpOverlayCirclePad(){
	pCleanUp();
}



// Management
///////////////

void deainpOverlayCirclePad::SetAxisLeftRight( const deainpInputBinding &binding ){
	pAxisLeftRight = binding;
}

void deainpOverlayCirclePad::SetAxisUpDown( const deainpInputBinding &binding ){
	pAxisUpDown = binding;
}



void deainpOverlayCirclePad::SetKeyCodeLeft( const deainpInputBinding &binding ){
	pKeyCodeLeft = binding;
}

void deainpOverlayCirclePad::SetKeyCodeUp( const deainpInputBinding &binding ){
	pKeyCodeUp = binding;
}

void deainpOverlayCirclePad::SetKeyCodeRight( const deainpInputBinding &binding ){
	pKeyCodeRight = binding;
}

void deainpOverlayCirclePad::SetKeyCodeDown( const deainpInputBinding &binding ){
	pKeyCodeDown = binding;
}



void deainpOverlayCirclePad::SetKeyCodeUpLeft( const deainpInputBinding &binding ){
	pKeyCodeUpLeft = binding;
}

void deainpOverlayCirclePad::SetKeyCodeUpRight( const deainpInputBinding &binding ){
	pKeyCodeUpRight = binding;
}

void deainpOverlayCirclePad::SetKeyCodeDownLeft( const deainpInputBinding &binding ){
	pKeyCodeDownLeft = binding;
}

void deainpOverlayCirclePad::SetKeyCodeDownRight( const deainpInputBinding &binding ){
	pKeyCodeDownRight = binding;
}



void deainpOverlayCirclePad::SetPressedLeft( bool pressed ){
	if( pressed == pPressedLeft ){
		return;
	}
	pPressedLeft = pressed;
	pKeyCodeLeft.ChangeButtonState( GetAndroidInput(), pressed );
}

void deainpOverlayCirclePad::SetPressedUp( bool pressed ){
	if( pressed == pPressedUp ){
		return;
	}
	pPressedUp = pressed;
	pKeyCodeUp.ChangeButtonState( GetAndroidInput(), pressed );
}

void deainpOverlayCirclePad::SetPressedRight( bool pressed ){
	if( pressed == pPressedRight ){
		return;
	}
	pPressedRight = pressed;
	pKeyCodeRight.ChangeButtonState( GetAndroidInput(), pressed );
}

void deainpOverlayCirclePad::SetPressedDown( bool pressed ){
	if( pressed == pPressedDown ){
		return;
	}
	pPressedDown = pressed;
	pKeyCodeDown.ChangeButtonState( GetAndroidInput(), pressed );
}



void deainpOverlayCirclePad::SetPressedUpLeft( bool pressed ){
	if( pressed == pPressedUpLeft ){
		return;
	}
	pPressedUpLeft = pressed;
	pKeyCodeUpLeft.ChangeButtonState( GetAndroidInput(), pressed );
}

void deainpOverlayCirclePad::SetPressedUpRight( bool pressed ){
	if( pressed == pPressedUpRight ){
		return;
	}
	pPressedUpRight = pressed;
	pKeyCodeUpRight.ChangeButtonState( GetAndroidInput(), pressed );
}

void deainpOverlayCirclePad::SetPressedDownLeft( bool pressed ){
	if( pressed == pPressedDownLeft ){
		return;
	}
	pPressedDownLeft = pressed;
	pKeyCodeDownLeft.ChangeButtonState( GetAndroidInput(), pressed );
}

void deainpOverlayCirclePad::SetPressedDownRight( bool pressed ){
	if( pressed == pPressedDownRight ){
		return;
	}
	pPressedDownRight = pressed;
	pKeyCodeDownRight.ChangeButtonState( GetAndroidInput(), pressed );
}



void deainpOverlayCirclePad::SetPressedAll( bool left, bool up, bool right, bool down,
bool upLeft, bool upRight, bool downLeft, bool downRight ){
	SetPressedLeft( left );
	SetPressedUp( up );
	SetPressedRight( right );
	SetPressedDown( down );
	SetPressedUpLeft( upLeft );
	SetPressedUpRight( upRight );
	SetPressedDownLeft( downLeft );
	SetPressedDownRight( downRight );
}



void deainpOverlayCirclePad::SetPadRadius( int radius ){
	radius = decMath::max( radius, 0 );
	if( radius == pPadRadius ){
		return;
	}
	
	pPadRadius = radius;
	
	//GetCanvas()->SetSize( decPoint( pOuterRadius * 2, pOuterRadius * 2 ) );
	//pCImage->SetSize( decPoint( pOuterRadius * 2, pOuterRadius * 2 ) );
	
	// TODO
}

void deainpOverlayCirclePad::SetOuterRadius( float radius ){
	radius = decMath::clamp( radius, 0.0f, 1.0f );
	if( fabsf( pOuterRadius - radius ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pOuterRadius = radius;
	
	//GetCanvas()->SetSize( decPoint( pPadRadius * 2, pPadRadius * 2 ) );
	//pCImage->SetSize( decPoint( pPadRadius * 2, pPadRadius * 2 ) );
	
	// TODO
}

void deainpOverlayCirclePad::SetInnerRadius( float radius ){
	radius = decMath::clamp( radius, 0.0f, 1.0f );
	if( fabsf( pInnerRadius - radius ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pInnerRadius = radius;
}

void deainpOverlayCirclePad::SetCenter( const decPoint &center ){
	if( center == pCenter ){
		return;
	}
	
	pCenter = center;
	
	// TODO
}



void deainpOverlayCirclePad::SetLeftRightPerSecond( float unitsPerSecond ){
	unitsPerSecond = decMath::max( unitsPerSecond, 0.0f );
	if( fabsf( pLeftRightPerSecond - unitsPerSecond ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pLeftRightPerSecond = unitsPerSecond;
}

void deainpOverlayCirclePad::SetUpDownPerSecond( float unitsPerSecond ){
	unitsPerSecond = decMath::max( unitsPerSecond, 0.0f );
	if( fabsf( pUpDownPerSecond - unitsPerSecond ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pUpDownPerSecond = unitsPerSecond;
}



void deainpOverlayCirclePad::GenerateEvents(){
	if( ! HasPointer() ){
		return;
	}
	
	deAndroidInput &androidInput = GetAndroidInput();
	const float elapsedTime = androidInput.GetElapsedTime();
	decPoint padPos( GetPointerPosition() - pCenter );
	float radius = padPos.Length();
	float angle = atan2f( ( float )padPos.x, ( float )-padPos.y );
	radius = decMath::linearStep( radius, ( float )pPadRadius * pInnerRadius,
		( float )pPadRadius * pOuterRadius, 0.0f, 1.0f );
	
	padPos.x = ( int )( sinf( angle ) * radius * ( pLeftRightPerSecond * elapsedTime ) );
	padPos.y = ( int )( cosf( angle ) * radius * ( pUpDownPerSecond * elapsedTime ) );
	//androidInput.LogInfoFormat( "padpos=(%i,%i) radius=%f angle=%f", padPos.x, padPos.y, radius, angle/DEG2RAD );
	
	const timeval eventTime( androidInput.TimeValNow() );
	
	if( pAxisLeftRight.GetAxisIndex() == -1 && pAxisUpDown.GetAxisIndex() == -1 ){
		const float shiftedAngle = angle / DEG2RAD - 22.5f;
		
		if( shiftedAngle < -157.5f ){ // down
			SetPressedAll( false, false, false, true,  false, false, false, false );
			
		}else if( shiftedAngle < -112.5f ){ // down-left
			if( pKeyCodeDownLeft.GetButtonIndex() == -1 ){
				SetPressedAll( true,  false, false, true,  false, false, false,  false );
				
			}else{
				SetPressedAll( false, false, false, false, false, false, true,  false );
			}
			
		}else if( shiftedAngle < -67.5f ){ // left
			SetPressedAll( true,  false, false, false, false, false, false, false );
			
		}else if( shiftedAngle < -22.5f ){ // up-left
			if( pKeyCodeUpLeft.GetButtonIndex() == -1 ){
				SetPressedAll( true,  true,  false, false, false, false, false, false );
				
			}else{
				SetPressedAll( false, false, false, false, true,  false, false, false );
			}
			
		}else if( shiftedAngle < 22.5f ){ // up
			SetPressedAll( false, true,  false, false, false, false, false, false );
			
		}else if( shiftedAngle < 67.5f ){ // up-right
			if( pKeyCodeUpRight.GetButtonIndex() == -1 ){
				SetPressedAll( false, true,  true,  false, false, false, false, false );
				
			}else{
				SetPressedAll( false, false, false, false, false, true,  false, false );
			}
			
		}else if( shiftedAngle < 112.5f ){ // right
			SetPressedAll( false, false, true,  false, false, false, false, false );
			
		}else{ // down-right
			if( pKeyCodeDownRight.GetButtonIndex() == -1 ){
				SetPressedAll( false, false, true,  true,  false, false, false, false );
				
			}else{
				SetPressedAll( false, false, false, false, false, false, false, true  );
			}
		}
		
	}else{
		decPoint distance;
		
		if( pAxisLeftRight.GetAxisIndex() != -1 ){
			if( pAxisLeftRight.GetInvertAxis() ){
				distance.x = -padPos.x;
				
			}else{
				distance.x = padPos.x;
			}
		}
		
		if( pAxisUpDown.GetAxisIndex() != -1 ){
			if( pAxisUpDown.GetInvertAxis() ){
				distance.y = -padPos.y;
				
			}else{
				distance.y = padPos.y;
			}
		}
		
		//androidInput.LogInfoFormat( "addMouseMove %i %i", dx, dy );
		if( pAxisLeftRight.GetDeviceIndex() == pAxisUpDown.GetDeviceIndex() ){
			androidInput.AddMouseMove( pAxisLeftRight.GetDeviceIndex(), 0, distance, eventTime );
			
		}else{
			androidInput.AddMouseMove( pAxisLeftRight.GetDeviceIndex(), 0, decPoint(distance.x, 0), eventTime );
			androidInput.AddMouseMove( pAxisUpDown.GetDeviceIndex(), 0, decPoint(0, distance.y), eventTime );
		}
	}
}



void deainpOverlayCirclePad::UpdateBindingIndices(){
	const deainpDeviceManager &devices = GetAndroidInput().GetDevices();
	
	pAxisLeftRight.UpdateIndices( devices );
	pAxisUpDown.UpdateIndices( devices );
	
	pKeyCodeLeft.UpdateIndices( devices );
	pKeyCodeUp.UpdateIndices( devices );
	pKeyCodeRight.UpdateIndices( devices );
	pKeyCodeDown.UpdateIndices( devices );
	
	pKeyCodeUpLeft.UpdateIndices( devices );
	pKeyCodeUpRight.UpdateIndices( devices );
	pKeyCodeDownLeft.UpdateIndices( devices );
	pKeyCodeDownRight.UpdateIndices( devices );
}

void deainpOverlayCirclePad::UpdateContent(){
	if( ! GetCanvas()->GetParentView() ){
		return;
	}
	GenerateEvents();
}

bool deainpOverlayCirclePad::OnTouch( int pointerId, const decPoint &position ){
	if( ( int )( ( position - pCenter ).Length() ) > pPadRadius ){
		return false;
	}
	
	SetPointer( pointerId, position );
	return true;
}

void deainpOverlayCirclePad::OnMove( const decPoint &position ){
	PointerMove( position );
}

void deainpOverlayCirclePad::OnRelease(){
	deainpOverlay::OnRelease();
	
	if( pAxisLeftRight.GetAxisIndex() == -1 && pAxisUpDown.GetAxisIndex() == -1 ){
		SetPressedAll( false, false, false, false,  false, false, false, false );
	}
}



// Private Functions
//////////////////////

void deainpOverlayCirclePad::pCleanUp(){
	if( pCImage ){
		pCImage->FreeReference();
	}
}
