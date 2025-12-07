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

#include "igdeMouseDragListener.h"



// Class igdeMouseDragListener
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMouseDragListener::igdeMouseDragListener() :
pDragState(edsNone),
pShiftOrigin(false),
pShiftNow(false),
pControlOrigin(false),
pControlNow(false){
}

igdeMouseDragListener::~igdeMouseDragListener(){
}



// Management
///////////////

void igdeMouseDragListener::SetDragOrigin(const decPoint &origin){
	pDragOrigin = origin;
}

void igdeMouseDragListener::SetDragPosition(const decPoint &position){
	pDragPosition = position;
}

decPoint igdeMouseDragListener::GetDragDistance() const{
	return pDragPosition - pDragOrigin;
}

void igdeMouseDragListener::SetDragState(eDragState state){
	pDragState = state;
}

void igdeMouseDragListener::Cancel(){
	if(pDragState == edsNone){
		return;
	}
	
	OnDragFinish(false);
	pDragState = edsNone;
}

void igdeMouseDragListener::SetShiftOrigin(bool pressed){
	pShiftOrigin = pressed;
}

void igdeMouseDragListener::SetShiftNow(bool pressed){
	pShiftNow = pressed;
}

void igdeMouseDragListener::SetControlOrigin(bool pressed){
	pControlOrigin = pressed;
}

void igdeMouseDragListener::SetControlNow(bool pressed){
	pControlNow = pressed;
}



// Dragging events
////////////////////

bool igdeMouseDragListener::OnDragBegin(){
	return false;
}

void igdeMouseDragListener::OnDragUpdate(){
}

void igdeMouseDragListener::OnDragFinish(bool){
}



// Events
///////////

void igdeMouseDragListener::OnButtonPress(igdeWidget *widget,
int button, const decPoint &position, int modifiers){
	if(pDragState != edsNone){
		return;
	}
	
	switch(button){
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
	pShiftOrigin = (modifiers & deInputEvent::esmShift) == deInputEvent::esmShift;
	pShiftNow = pShiftOrigin;
	pControlOrigin = (modifiers & deInputEvent::esmControl) == deInputEvent::esmControl;
	pControlNow = pControlOrigin;
	
	if(! OnDragBegin()){
		pDragState = edsNone;
	}
}

void igdeMouseDragListener::OnButtonRelease(igdeWidget *widget,
int button, const decPoint &position, int modifiers){
	if(pDragState == edsNone){
		return;
	}
	
	OnDragFinish(true);
	pDragState = edsNone;
}

void igdeMouseDragListener::OnMouseMoved(igdeWidget *widget,
const decPoint &position, int modifiers){
	if(pDragState == edsNone){
		return;
	}
	
	pDragPosition = position;
	pShiftNow = (modifiers & deInputEvent::esmShift) == deInputEvent::esmShift;
	pControlNow = (modifiers & deInputEvent::esmControl) == deInputEvent::esmControl;
	
	OnDragUpdate();
}
