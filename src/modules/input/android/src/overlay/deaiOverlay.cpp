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

#include "deaiOverlay.h"
#include "../deAndroidInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasManager.h>



// Class deaiOverlay
//////////////////////

// Constructor, destructor
////////////////////////////

deaiOverlay::deaiOverlay( deAndroidInput &androidInput ) :
pAndroidInput( androidInput ),

pCanvas( NULL ),
pPointer( -1 ),

pLayoutHorizontal( decPoint(), decPoint( 256, 256 ) ),
pLayoutVertical( decPoint(), decPoint( 256, 256 ) )
{
	deCanvasManager &canvasManager = *androidInput.GetGameEngine()->GetCanvasManager();
	pCanvas = canvasManager.CreateCanvasView();
	pCanvas->SetSize( decPoint( 256, 256 ) );
	pCanvas->SetTransparency( 0.35f ); // 35% coverage
}

deaiOverlay::~deaiOverlay(){
	if( pCanvas ){
		pCanvas->FreeReference();
	}
}



// Management
///////////////

bool deaiOverlay::HasPointer() const{
	return pPointer != -1;
}

void deaiOverlay::ClearPointer(){
	pPointer = -1;
}

void deaiOverlay::SetPointer( int pointer, const decPoint &position ){
	pPointer = pointer;
	pPointerPosition = position;
}

decPoint deaiOverlay::PointerMove( const decPoint &position ){
	const decPoint move( position - pPointerPosition );
	pPointerPosition = position;
	return move;
}



void deaiOverlay::SetLayoutHorizontal( const deaiLayout &layout ){
	if( layout == pLayoutHorizontal ){
		return;
	}
	pLayoutHorizontal = layout;
}

void deaiOverlay::SetLayoutVertical( const deaiLayout &layout ){
	if( layout == pLayoutVertical ){
		return;
	}
	pLayoutVertical = layout;
}



void deaiOverlay::UpdateFromHorizontalLayout(){
	UpdateFromLayout( pLayoutHorizontal );
}

void deaiOverlay::UpdateFromVerticalLayout(){
	UpdateFromLayout( pLayoutVertical );
}

void deaiOverlay::UpdateFromLayout( const deaiLayout &layout ){
	pCanvas->SetPosition( layout.GetPosition() );
	pCanvas->SetSize( layout.GetSize() );
}



void deaiOverlay::UpdateBindingIndices(){
}

void deaiOverlay::UpdateContent(){
}

bool deaiOverlay::OnTouch( int pointerId, const decPoint &position ){
	return false;
}

void deaiOverlay::OnMove( const decPoint &position ){
}

void deaiOverlay::OnRelease(){
	ClearPointer();
}
