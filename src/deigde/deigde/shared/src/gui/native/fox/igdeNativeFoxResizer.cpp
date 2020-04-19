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

FXDEFMAP( igdeNativeFoxResizer ) igdeNativeResizerMap[] = {
	FXMAPFUNC( SEL_PAINT, 0, igdeNativeFoxResizer::onPaint ),
	FXMAPFUNC( SEL_ENTER, 0, igdeNativeFoxResizer::onEnter ),
	FXMAPFUNC( SEL_LEAVE, 0, igdeNativeFoxResizer::onLeave ),
	FXMAPFUNC( SEL_LEFTBUTTONPRESS, 0, igdeNativeFoxResizer::onLeftMouseDown ),
	FXMAPFUNC( SEL_LEFTBUTTONRELEASE, 0, igdeNativeFoxResizer::onLeftMouseUp ),
	FXMAPFUNC( SEL_MOTION, 0, igdeNativeFoxResizer::onMouseMoved ),
};


FXIMPLEMENT( igdeNativeFoxResizer, FXWindow, igdeNativeResizerMap, ARRAYNUMBER( igdeNativeResizerMap ) )



// Class igdeNativeResizer
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeFoxResizer::igdeNativeFoxResizer(){
}

igdeNativeFoxResizer::igdeNativeFoxResizer( FXComposite *parent, FXObject *target, FXSelector selector ) :
FXWindow( parent, LAYOUT_FILL_X ),
pIsDragging( false ),
pCursor( NULL ),
pGripSize( 6 ),
pGripWidth( 50 ),
pActiveColor( FXRGB( 0, 0, 255 ) )
{
	// NOTE FOX has an annoying bug here. CURSOR_UPDOWN is supposed to produce an up-down
	//      moving cursor. In the source code the enumeration is hard coded to an array of
	//      platform specific cursors. The windows #ifdef selects the correct cursor.
	//      the linux #ifdef though has the up-down and left-right cursors reversed.
	//      this requires an #ifdef on our side to apply the wrong cursor to get the
	//      correct cursor on linux
	/*
	#ifdef OS_UNIX
	pCursor = new FXCursor( getApp(), CURSOR_LEFTRIGHT );
	#else
	pCursor = new FXCursor( getApp(), CURSOR_UPDOWN );
	#endif
	setDefaultCursor( pCursor );
	setDragCursor( pCursor );
	*/
	
	// there are default cursors around. they seem to not suffer from these problems
	setDefaultCursor( getApp()->getDefaultCursor( DEF_VSPLIT_CURSOR ) );
	setDragCursor( getDefaultCursor() );
	
	setTarget( target );
	setSelector( selector );
	enable();
	show();
}

igdeNativeFoxResizer::~igdeNativeFoxResizer(){
	if( pCursor ){
		delete pCursor;
	}
}



// Management
///////////////

FXint igdeNativeFoxResizer::getDefaultHeight(){
	return pGripSize;
}

int igdeNativeFoxResizer::SelCommandDraggedDistance( void *data ){
	return ( int )( intptr_t )data;
}



// Events
///////////

long igdeNativeFoxResizer::onPaint( FXObject*, FXSelector, void *data ){
	// nicked from FXToolBarGrip::onPaint
	FXDCWindow dc( this, ( FXEvent* )data );
	const int height = getHeight();
	const int width = getWidth();
	const int gripWidth = decMath::min( pGripWidth, decMath::max( width - 2, 0 ) );
	const int gripHeight = decMath::max( height - 2, 0 );
	const int left = ( width - gripWidth ) / 2;
	const int right = left + gripWidth;
	
	dc.setForeground( getBackColor() );
	dc.fillRectangle( 0, 0, width, height );
	
	dc.setForeground( getApp()->getHiliteColor() );
	dc.fillRectangle( left, 1, 1, gripHeight - 1 );
	dc.fillRectangle( left, 1, gripWidth, 1 );
	
	dc.setForeground( getApp()->getShadowColor() );
	dc.fillRectangle( right - 1, 1, 1, gripHeight );
	dc.fillRectangle( left, gripHeight, gripWidth, 1 );
	
	if( pIsDragging || underCursor() ){
		dc.setForeground( pActiveColor );
		dc.fillRectangle( left + 1, 2, gripWidth - 2, gripHeight - 2 );
	}
	
	return 1;
}

long igdeNativeFoxResizer::onEnter( FXObject*, FXSelector, void* ){
	update();
	return 0;
}

long igdeNativeFoxResizer::onLeave( FXObject*, FXSelector, void* ){
	update();
	return 0;
}

long igdeNativeFoxResizer::onLeftMouseDown( FXObject*, FXSelector, void* ){
	if( pIsDragging || ! isEnabled() ){
		return 0 ;
	}
	
	grab();
	update();
	pIsDragging = true;
	return 0;
}

long igdeNativeFoxResizer::onLeftMouseUp( FXObject*, FXSelector, void* ){
	if( ! pIsDragging ){
		return 0;
	}
	
	ungrab();
	update();
	pIsDragging = false;
	return 0;
}

long igdeNativeFoxResizer::onMouseMoved( FXObject*, FXSelector, void *data ){
	if( ! pIsDragging ){
		return 0;
	}
	
	const FXEvent &event = *( ( FXEvent* )data );
	const int center = getHeight() / 2;
	const int distance = event.win_y - center;
	
	if( getTarget() ){
		return getTarget()->tryHandle( this, FXSEL( SEL_COMMAND, getSelector() ),
			( void* )( intptr_t )distance );
	}
	
	update();
	return 0;
}
