/* 
 * Drag[en]gine Android Launcher
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

#include "dealWidgetLayoutStack.h"
#include "../../common/exceptions.h"
#include "../../logger/deLogger.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealWidgetLayoutStack
////////////////////////////////

// Constructors, destructors
//////////////////////////////

dealWidgetLayoutStack::dealWidgetLayoutStack( dealDisplay &display ) :
dealWidgetLayout( display ),
pFullSize( true ){
}

dealWidgetLayoutStack::dealWidgetLayoutStack( dealDisplay &display, bool fullSize ) :
dealWidgetLayout( display ),
pFullSize( fullSize ){
}

dealWidgetLayoutStack::~dealWidgetLayoutStack(){
}



// Management
///////////////

void dealWidgetLayoutStack::SetFullSize( bool fullSize ){
	if( fullSize == pFullSize ){
		return;
	}
	
	pFullSize = fullSize;
	DirtyLayout();
}



decPoint dealWidgetLayoutStack::GetMinimumSize(){
	if( GetHasExplicitMinimumSize() ){
		return GetExplicitMinimumSize();
	}
	return GetRealMinimumSize();
}



void dealWidgetLayoutStack::LayoutWidgets(){
	const int count = GetWidgetCount();
	if( count == 0 ){
		return;
	}
	
	const decPoint contentArea( GetContentArea() );
	int i;
	
	if( pFullSize ){
		const decPoint position( GetPaddingLeft(), GetPaddingTop() );
		
		for( i=0; i<count; i++ ){
			dealWidget &widget = *GetWidgetAt( i );
			if( ! widget.GetVisible() ){
				continue;
			}
			
			widget.SetPosition( position );
			widget.SetSize( contentArea );
		}
		
	}else{
		const decPoint center( decPoint( GetPaddingLeft(), GetPaddingTop() ) + contentArea / 2 );
		
		for( i=0; i<count; i++ ){
			dealWidget &widget = *GetWidgetAt( i );
			if( ! widget.GetVisible() ){
				continue;
			}
			
			decPoint widgetSize( widget.GetMinimumSize() );
			
			widget.SetPosition( center - widgetSize / 2 );
			widget.SetSize( widgetSize );
		}
	}
}

decPoint dealWidgetLayoutStack::GetRealMinimumSize(){
	const int count = GetWidgetCount();
	if( count == 0 ){
		return decPoint( GetPaddingLeft() + GetPaddingRight(), GetPaddingTop() + GetPaddingBottom() );
	}
	
	decPoint size;
	int i;
	
	for( i=0; i<count; i++ ){
		dealWidget &widget = *GetWidgetAt( i );
		if( ! widget.GetVisible() ){
			continue;
		}
		
		size.SetLargest( widget.GetMinimumSize() );
	}
	
	size.x += GetPaddingLeft() + GetPaddingRight();
	size.y += GetPaddingTop() + GetPaddingBottom();
	
	return decPoint().Largest( size );
}
