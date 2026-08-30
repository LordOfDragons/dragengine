/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifdef IGDE_TOOLKIT_BEOS

#include "igdeNativeBeosNVSocket.h"

#include <dragengine/common/exceptions.h>
#include <cmath>


// Class igdeNativeBeosNVSocket
/////////////////////////////////

igdeNativeBeosNVSocket::igdeNativeBeosNVSocket(BRect frame, const char *label, eSocketType type) :
BView(frame, "socket", B_FOLLOW_NONE, B_WILL_DRAW),
pLabel(label ? label : ""),
pType(type),
pConnected(false),
pColor({ 128, 128, 128, 255 })
{
	SetViewColor(B_TRANSPARENT_COLOR);
}

igdeNativeBeosNVSocket::~igdeNativeBeosNVSocket() = default;


// Management
//////////////

void igdeNativeBeosNVSocket::SetLabel(const char *label){
	if(label){
		pLabel = label;
		
	}else{
		pLabel.Empty();
	}
	Invalidate();
}

void igdeNativeBeosNVSocket::SetType(eSocketType type){
	pType = type;
	Invalidate();
}

void igdeNativeBeosNVSocket::SetConnected(bool connected){
	pConnected = connected;
	Invalidate();
}

void igdeNativeBeosNVSocket::SetColor(rgb_color color){
	pColor = color;
	Invalidate();
}

BPoint igdeNativeBeosNVSocket::GetSocketCenter() const {
	BRect bounds = Bounds();
	if(pType == estInput){
		return BPoint(bounds.left + 4, bounds.top + bounds.Height() / 2.0f);
		
	} else {
		return BPoint(bounds.right - 4, bounds.top + bounds.Height() / 2.0f);
	}
}

void igdeNativeBeosNVSocket::Draw(BRect updateRect){
	BRect bounds = Bounds();
	const float socketRadius = 4.0f;
	
	// Get socket center position
	BPoint socketCenter = GetSocketCenter();
	
	// Draw socket circle
	if(pConnected){
		SetHighColor(pColor);
		
	}else{
		SetHighColor({100, 100, 100, 255});
	}
	FillEllipse(socketCenter, socketRadius, socketRadius);
	
	// Draw socket border
	SetHighColor({0, 0, 0, 255});
	StrokeEllipse(socketCenter, socketRadius, socketRadius);
	
	// Draw label text
	if(!pLabel.IsEmpty()){
		SetHighColor({0, 0, 0, 255});
		BFont font;
		GetFont(&font);
		font_height fh;
		font.GetHeight(&fh);
		
		float textX, textY = bounds.top + (bounds.Height() + fh.ascent) / 2.0f;
		if(pType == estInput){
			textX = bounds.left + 12;
		} else {
			textX = bounds.right - 12 - font.StringWidth(pLabel.GetString());
		}
		
		DrawString(pLabel.GetString(), BPoint(textX, textY));
	}
}

void igdeNativeBeosNVSocket::MouseDown(BPoint where){
	// Connection start event could be handled here
	SetMouseEventMask(B_POINTER_EVENTS, B_LOCK_WINDOW_FOCUS);
}

void igdeNativeBeosNVSocket::MouseUp(BPoint where){
	// Connection end event could be handled here
}

#endif
