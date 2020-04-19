/* 
 * Drag[en]gine Android Input Module
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
