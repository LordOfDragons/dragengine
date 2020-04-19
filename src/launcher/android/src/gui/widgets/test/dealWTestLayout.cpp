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

#include "dealWTestLayout.h"
#include "../dealWidgetLayoutFlow.h"
#include "../../dealDisplay.h"
#include "../../../common/exceptions.h"
#include "../../../logger/deLogger.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealWTestLayout
//////////////////////////

// Constructors, destructors
//////////////////////////////

dealWTestLayout::dealWTestLayout( dealDisplay &display ) :
dealWidgetLayout( display ){
	const int gap = 50;
	const int height = 55 * 7;
	const int offset = height + gap;
	
	if( true ){
		AddLayoutLinesX( 0, dealWidgetLayoutFlow::eltFill );
		AddLayoutLinesX( offset, dealWidgetLayoutFlow::eltTop );
		AddLayoutLinesX( offset * 2, dealWidgetLayoutFlow::eltCenter );
		AddLayoutLinesX( offset * 3, dealWidgetLayoutFlow::eltBottom );
		
	}else{
		AddLayoutLinesY( 0, 0, dealWidgetLayoutFlow::eltFill );
		AddLayoutLinesY( offset, 0, dealWidgetLayoutFlow::eltTop );
		AddLayoutLinesY( 0, ( display.GetHeight() - 80 ) / 2, dealWidgetLayoutFlow::eltCenter );
		AddLayoutLinesY( offset, ( display.GetHeight() - 80 ) / 2, dealWidgetLayoutFlow::eltBottom );
	}
}

dealWTestLayout::~dealWTestLayout(){
}



// Management
///////////////

void dealWTestLayout::AddLayoutLinesX( int top, dealWidgetLayoutFlow::eLayoutTypes layoutY ){
	const int screenWidth = GetDisplay().GetWidth() - 40;
	int i;
	
	const dealWidgetLayoutFlow::eLayoutTypes layoutTypes[ 7 ] = {
		dealWidgetLayoutFlow::eltFill,
		dealWidgetLayoutFlow::eltTop,
		dealWidgetLayoutFlow::eltTopFill,
		dealWidgetLayoutFlow::eltCenter,
		dealWidgetLayoutFlow::eltCenterFill,
		dealWidgetLayoutFlow::eltBottom,
		dealWidgetLayoutFlow::eltBottomFill
	};
	
	for( i=0; i<7; i++ ){
		dealWidgetLayoutFlow * const layout = CreateLayoutLineX();
		layout->SetPosition( decPoint( 25, top + 55 * i ) );
		layout->SetSize( decPoint( screenWidth - 50, 50 ) );
		layout->SetHorizontal( true );
		layout->SetGap( 5 );
		layout->SetLayoutX( layoutTypes[ i ] );
		layout->SetLayoutY( layoutY );
		AddWidget( layout );
		layout->FreeReference();
	}
}

void dealWTestLayout::AddLayoutLinesY( int left, int top, dealWidgetLayoutFlow::eLayoutTypes layoutX ){
	const int screenHeight = ( GetDisplay().GetHeight() - 80 ) / 2 - 50;
	int i;
	
	const dealWidgetLayoutFlow::eLayoutTypes layoutTypes[ 7 ] = {
		dealWidgetLayoutFlow::eltFill,
		dealWidgetLayoutFlow::eltTop,
		dealWidgetLayoutFlow::eltTopFill,
		dealWidgetLayoutFlow::eltCenter,
		dealWidgetLayoutFlow::eltCenterFill,
		dealWidgetLayoutFlow::eltBottom,
		dealWidgetLayoutFlow::eltBottomFill
	};
	
	for( i=0; i<7; i++ ){
		dealWidgetLayoutFlow * const layout = CreateLayoutLineY();
		layout->SetPosition( decPoint( left + 55 * i, top + 25 ) );
		layout->SetSize( decPoint( 50, screenHeight - 50 ) );
		layout->SetHorizontal( false );
		layout->SetGap( 5 );
		layout->SetLayoutY( layoutTypes[ i ] );
		layout->SetLayoutX( layoutX );
		AddWidget( layout );
		layout->FreeReference();
	}
}

dealWidgetLayoutFlow *dealWTestLayout::CreateLayoutLineX(){
	dealWidgetLayoutFlow *layout = new dealWidgetLayoutFlow( GetDisplay() );
	layout->SetBackgroundColor( decColor( 0.75f, 0.75f, 0.75f ) );
	layout->SetPadding( 5, 5 );
	
	int i;
	for( i=0; i<5; i++ ){
		dealWidget * const widget = new dealWidget( GetDisplay() );
		widget->SetExplicitMinimumSize( decPoint( 100, 25 ) );
		widget->SetBackgroundColor( decColor( ( float )i / 4.0f, 1.0f - ( float )i / 4.0f, 0.0f ) );
		layout->AddWidget( widget );
		widget->FreeReference();
	}
	
	return layout;
}

dealWidgetLayoutFlow *dealWTestLayout::CreateLayoutLineY(){
	dealWidgetLayoutFlow *layout = new dealWidgetLayoutFlow( GetDisplay() );
	layout->SetBackgroundColor( decColor( 0.75f, 0.75f, 0.75f ) );
	layout->SetPadding( 5, 5 );
	
	int i;
	for( i=0; i<5; i++ ){
		dealWidget * const widget = new dealWidget( GetDisplay() );
		widget->SetExplicitMinimumSize( decPoint( 25, 100 ) );
		widget->SetBackgroundColor( decColor( ( float )i / 4.0f, 1.0f - ( float )i / 4.0f, 0.0f ) );
		layout->AddWidget( widget );
		widget->FreeReference();
	}
	
	return layout;
}
