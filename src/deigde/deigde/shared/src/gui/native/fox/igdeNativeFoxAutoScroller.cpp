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

#ifdef IGDE_TOOLKIT_FOX

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "igdeNativeFoxAutoScroller.h"

#include <dragengine/common/exceptions.h>



// Event map
//////////////

FXDEFMAP( igdeNativeFoxAutoScroller ) igdeNativeFoxAutoScrollerMap[] = {
};


FXIMPLEMENT( igdeNativeFoxAutoScroller, FXComposite,
	igdeNativeFoxAutoScrollerMap, ARRAYNUMBER( igdeNativeFoxAutoScrollerMap ) )



// Class igdeNativeFoxAutoScroller
////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeFoxAutoScroller::igdeNativeFoxAutoScroller(){
	flags |= FLAG_SHOWN;
}

igdeNativeFoxAutoScroller::igdeNativeFoxAutoScroller( FXComposite *pparent, int opts ) :
FXComposite( pparent, opts, 0, 0, 0, 0 ),
pScrollPosition( 0 )
{
	flags |= FLAG_SHOWN;
}

igdeNativeFoxAutoScroller::~igdeNativeFoxAutoScroller(){
}



// Management
///////////////

FXWindow *igdeNativeFoxAutoScroller::GetContentWindow() const{
	return numChildren() > 0 ? childAtIndex( 0 ) : NULL;
}

decPoint igdeNativeFoxAutoScroller::GetContentSize() const{
	FXWindow * const window = GetContentWindow();
	if( ! window ){
		return decPoint( 1, 1 );
	}
	
	const int hints = window->getLayoutHints();
	return decPoint( 1, 1 ).Largest( decPoint(
		hints & LAYOUT_FIX_WIDTH ? window->getWidth() : window->getDefaultWidth(),
		hints & LAYOUT_FIX_HEIGHT ? window->getHeight() : window->getDefaultHeight() ) );
}

void igdeNativeFoxAutoScroller::SetScrollPosition( int position ){
	pScrollPosition = position;
	
	FXWindow * const window = GetContentWindow();
	if( window ){
		window->move( -pScrollPosition, window->getY() );
	}
}



FXint igdeNativeFoxAutoScroller::getDefaultWidth(){
	return 1;
}

FXint igdeNativeFoxAutoScroller::getDefaultHeight(){
	return GetContentSize().y;
}

void igdeNativeFoxAutoScroller::layout(){
	FXComposite::layout();
	
	FXWindow * const window = GetContentWindow();
	if( ! window ){
		return;
	}
	
	const int hints = window->getLayoutHints();
	const decPoint viewSize( getWidth(), getHeight() );
	decPoint size( GetContentSize() );
	decPoint position( -pScrollPosition, 0 );
	
	if( size.x < viewSize.x ){
		if( hints & LAYOUT_FILL_X ){
			size.x = viewSize.x;
		}
		
		if( hints & LAYOUT_CENTER_X ){
			position.x = ( viewSize.x - size.x ) / 2;
			
		}else if( hints & LAYOUT_RIGHT ){
			position.x = viewSize.x - size.x;
			
		}else{
			position.x = 0;
		}
	}
	
	if( size.y < viewSize.y ){
		if( hints & LAYOUT_FILL_Y ){
			size.y = viewSize.y;
		}
		
		if( hints & LAYOUT_CENTER_Y ){
			position.y = ( viewSize.y - size.y ) / 2;
			
		}else if( hints & LAYOUT_BOTTOM ){
			position.y = viewSize.y - size.y;
			
		}else{
			position.y = 0;
		}
	}
	
	window->position( position.x, position.y, size.x, size.y );
	recalc();
}

void igdeNativeFoxAutoScroller::MoveContent( int x ){
	if( x == pScrollPosition ){
		return;
	}
	
	FXWindow * const window = GetContentWindow();
	if( ! window ){
		pScrollPosition = x;
		return;
	}
	
	const int hints = window->getLayoutHints();
	const decPoint viewSize( getWidth(), getHeight() );
	decPoint size( GetContentSize() );
	decPoint position( -pScrollPosition, 0 );
	
	if( size.x < viewSize.x ){
		if( hints & LAYOUT_FILL_X ){
			size.x = viewSize.x;
		}
		
		if( hints & LAYOUT_CENTER_X ){
			position.x = ( viewSize.x - size.x ) / 2;
			
		}else if( hints & LAYOUT_RIGHT ){
			position.x = viewSize.x - size.x;
			
		}else{
			position.x = 0;
		}
	}
	
	if( size.y < viewSize.y ){
		if( hints & LAYOUT_FILL_Y ){
			size.y = viewSize.y;
		}
		
		if( hints & LAYOUT_CENTER_Y ){
			position.y = ( viewSize.y - size.y ) / 2;
			
		}else if( hints & LAYOUT_BOTTOM ){
			position.y = viewSize.y - size.y;
			
		}else{
			position.y = 0;
		}
	}
	
	window->move( position.x, position.y );
	
	pScrollPosition = x;
}



// Events
///////////

long igdeNativeFoxAutoScroller::onChildMouseMoved( FXObject *sender, FXSelector, void *pdata ){
	const int viewWidth = getWidth();
	const int childWidth = GetContentSize().x;
	if( viewWidth < 1 || childWidth <= viewWidth ){
		SetScrollPosition( 0 );
		return 0; // return 0 to let original implementation run. 1 would stop
	}
	
	const FXEvent &event = *( ( FXEvent* )pdata );
	FXint x, y;
	translateCoordinatesFrom( x, y, ( FXWindow* )sender, event.win_x, event.win_y );
	
	SetScrollPosition( ( childWidth - viewWidth ) * x / viewWidth );
	return 0; // return 0 to let original implementation run. 1 would stop
}

#endif
