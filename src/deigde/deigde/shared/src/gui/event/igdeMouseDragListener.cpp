/* 
 * Drag[en]gine IGDE
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

#include "igdeMouseDragListener.h"



// Class igdeMouseDragListener
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMouseDragListener::igdeMouseDragListener() :
pDragState( edsNone ),
pShiftOrigin( false ),
pShiftNow( false ),
pControlOrigin( false ),
pControlNow( false ){
}

igdeMouseDragListener::~igdeMouseDragListener(){
}



// Management
///////////////

void igdeMouseDragListener::SetDragOrigin( const decPoint &origin ){
	pDragOrigin = origin;
}

void igdeMouseDragListener::SetDragPosition( const decPoint &position ){
	pDragPosition = position;
}

decPoint igdeMouseDragListener::GetDragDistance() const{
	return pDragPosition - pDragOrigin;
}

void igdeMouseDragListener::SetDragState( eDragState state ){
	pDragState = state;
}

void igdeMouseDragListener::Cancel(){
	if( pDragState == edsNone ){
		return;
	}
	
	OnDragFinish( false );
	pDragState = edsNone;
}

void igdeMouseDragListener::SetShiftOrigin( bool pressed ){
	pShiftOrigin = pressed;
}

void igdeMouseDragListener::SetShiftNow( bool pressed ){
	pShiftNow = pressed;
}

void igdeMouseDragListener::SetControlOrigin( bool pressed ){
	pControlOrigin = pressed;
}

void igdeMouseDragListener::SetControlNow( bool pressed ){
	pControlNow = pressed;
}



// Dragging events
////////////////////

bool igdeMouseDragListener::OnDragBegin(){
	return false;
}

void igdeMouseDragListener::OnDragUpdate(){
}

void igdeMouseDragListener::OnDragFinish( bool ){
}



// Events
///////////

void igdeMouseDragListener::OnButtonPress( igdeWidget *widget,
int button, const decPoint &position, int modifiers ){
	if( pDragState != edsNone ){
		return;
	}
	
	switch( button ){
	case deInputEvent::embcLeft:
		pDragState = edsLeft;
		break;
		
	case deInputEvent::embcRight:
		pDragState = edsRight;
		break;
		
	default:
		return;
	}
	
	pDragOrigin = position;
	pDragPosition = pDragOrigin;
	pShiftOrigin = ( modifiers & deInputEvent::esmShift ) == deInputEvent::esmShift;
	pShiftNow = pShiftOrigin;
	pControlOrigin = ( modifiers & deInputEvent::esmControl ) == deInputEvent::esmControl;
	pControlNow = pControlOrigin;
	
	if( ! OnDragBegin() ){
		pDragState = edsNone;
	}
}

void igdeMouseDragListener::OnButtonRelease( igdeWidget *widget,
int button, const decPoint &position, int modifiers ){
	if( pDragState == edsNone ){
		return;
	}
	
	OnDragFinish( true );
	pDragState = edsNone;
}

void igdeMouseDragListener::OnMouseMoved( igdeWidget *widget,
const decPoint &position, int modifiers ){
	if( pDragState == edsNone ){
		return;
	}
	
	pDragPosition = position;
	pShiftNow = ( modifiers & deInputEvent::esmShift ) == deInputEvent::esmShift;
	pControlNow = ( modifiers & deInputEvent::esmControl ) == deInputEvent::esmControl;
	
	OnDragUpdate();
}
