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

#ifdef IGDE_TOOLKIT_FOX

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "igdeNativeFoxResizer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/logger/deLogger.h>



// Event map
//////////////

FXDEFMAP(igdeNativeFoxResizer) igdeNativeResizerMap[] = {
	FXMAPFUNC(SEL_PAINT, 0, igdeNativeFoxResizer::onPaint),
	FXMAPFUNC(SEL_ENTER, 0, igdeNativeFoxResizer::onEnter),
	FXMAPFUNC(SEL_LEAVE, 0, igdeNativeFoxResizer::onLeave),
	FXMAPFUNC(SEL_LEFTBUTTONPRESS, 0, igdeNativeFoxResizer::onLeftMouseDown),
	FXMAPFUNC(SEL_LEFTBUTTONRELEASE, 0, igdeNativeFoxResizer::onLeftMouseUp),
	FXMAPFUNC(SEL_MOTION, 0, igdeNativeFoxResizer::onMouseMoved),
};


FXIMPLEMENT(igdeNativeFoxResizer, FXWindow, igdeNativeResizerMap, ARRAYNUMBER(igdeNativeResizerMap))



// Class igdeNativeResizer
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeFoxResizer::igdeNativeFoxResizer(){
}

igdeNativeFoxResizer::igdeNativeFoxResizer(FXComposite *pparent, FXObject *ttarget, FXSelector selector) :
FXWindow(pparent, LAYOUT_FILL_X),
pIsDragging(false),
pCursor(nullptr),
pGripSize(6),
pGripWidth(50),
pActiveColor(FXRGB(0, 0, 255))
{
	// NOTE FOX has an annoying bug here. CURSOR_UPDOWN is supposed to produce an up-down
	//      moving cursor. In the source code the enumeration is hard coded to an array of
	//      platform specific cursors. The windows #ifdef selects the correct cursor.
	//      the linux #ifdef though has the up-down and left-right cursors reversed.
	//      this requires an #ifdef on our side to apply the wrong cursor to get the
	//      correct cursor on linux
	/*
	#ifdef OS_UNIX
	pCursor = new FXCursor(getApp(), CURSOR_LEFTRIGHT);
	#else
	pCursor = new FXCursor(getApp(), CURSOR_UPDOWN);
	#endif
	setDefaultCursor(pCursor);
	setDragCursor(pCursor);
	*/
	
	// there are default cursors around. they seem to not suffer from these problems
	setDefaultCursor(getApp()->getDefaultCursor(DEF_VSPLIT_CURSOR));
	setDragCursor(getDefaultCursor());
	
	setTarget(ttarget);
	setSelector(selector);
	enable();
	show();
}

igdeNativeFoxResizer::~igdeNativeFoxResizer(){
	if(pCursor){
		delete pCursor;
	}
}



// Management
///////////////

FXint igdeNativeFoxResizer::getDefaultHeight(){
	return pGripSize;
}

int igdeNativeFoxResizer::SelCommandDraggedDistance(void *pdata){
	return (int)(intptr_t)pdata;
}



// Events
///////////

long igdeNativeFoxResizer::onPaint(FXObject*, FXSelector, void *pdata){
	// nicked from FXToolBarGrip::onPaint
	FXDCWindow dc(this, (FXEvent*)pdata);
	const int hheight = getHeight();
	const int wwidth = getWidth();
	const int gripWidth = decMath::min(pGripWidth, decMath::max(wwidth - 2, 0));
	const int gripHeight = decMath::max(hheight - 2, 0);
	const int left = (wwidth - gripWidth) / 2;
	const int right = left + gripWidth;
	
	dc.setForeground(getBackColor());
	dc.fillRectangle(0, 0, wwidth, hheight);
	
	dc.setForeground(getApp()->getHiliteColor());
	dc.fillRectangle(left, 1, 1, gripHeight - 1);
	dc.fillRectangle(left, 1, gripWidth, 1);
	
	dc.setForeground(getApp()->getShadowColor());
	dc.fillRectangle(right - 1, 1, 1, gripHeight);
	dc.fillRectangle(left, gripHeight, gripWidth, 1);
	
	if(pIsDragging || underCursor()){
		dc.setForeground(pActiveColor);
		dc.fillRectangle(left + 1, 2, gripWidth - 2, gripHeight - 2);
	}
	
	return 1;
}

long igdeNativeFoxResizer::onEnter(FXObject*, FXSelector, void*){
	update();
	return 0;
}

long igdeNativeFoxResizer::onLeave(FXObject*, FXSelector, void*){
	update();
	return 0;
}

long igdeNativeFoxResizer::onLeftMouseDown(FXObject*, FXSelector, void*){
	if(pIsDragging || !isEnabled()){
		return 0 ;
	}
	
	grab();
	update();
	pIsDragging = true;
	return 0;
}

long igdeNativeFoxResizer::onLeftMouseUp(FXObject*, FXSelector, void*){
	if(!pIsDragging){
		return 0;
	}
	
	ungrab();
	update();
	pIsDragging = false;
	return 0;
}

long igdeNativeFoxResizer::onMouseMoved(FXObject*, FXSelector, void *pdata){
	if(!pIsDragging){
		return 0;
	}
	
	const FXEvent &event = *((FXEvent*)pdata);
	const int center = getHeight() / 2;
	const int distance = event.win_y - center;
	
	if(getTarget()){
		return getTarget()->tryHandle(this, FXSEL(SEL_COMMAND, getSelector()),
			(void*)(intptr_t)distance);
	}
	
	update();
	return 0;
}

#endif
