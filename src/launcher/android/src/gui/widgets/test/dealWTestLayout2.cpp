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

#include "dealWTestLayout2.h"
#include "../dealWidgetLayoutFlow.h"
#include "../dealWidgetLabel.h"
#include "../../dealDisplay.h"
#include "../../../common/exceptions.h"
#include "../../../logger/deLogger.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealWTestLayout2
///////////////////////////

// Constructors, destructors
//////////////////////////////

dealWTestLayout2::dealWTestLayout2( dealDisplay &display ) :
dealWidgetLayoutFlow( display, false, 20, eltFill, eltFill ){
	SetPaddingLeft( 50 );
	SetPaddingTop( 100 );
	SetPaddingRight( 50 );
	SetPaddingBottom( 50 );
	
// 	AddLayout2( dealWidgetLabel::eaTop );
// 	AddLayout2( dealWidgetLabel::eaMiddle );
// 	AddLayout2( dealWidgetLabel::eaBottom );
	
	AddLayout2( dealWidgetLabel::eaTop );
	AddLayout3( dealWidgetLabel::eaLeft | dealWidgetLabel::eaTop );
}

dealWTestLayout2::~dealWTestLayout2(){
}



// Management
///////////////

void dealWTestLayout2::AddLayout( dealWidgetLabel::eAlignments verticalAlign ){
	dealWidgetLayoutFlow *layout = new dealWidgetLayoutFlow( GetDisplay(), false, 5, eltFill, eltFill );
	layout->SetBackgroundColor( decColor( 0.75f, 0.75f, 0.75f ) );
	layout->SetPadding( 5, 5 );
	
	dealWidgetLabel *widget = new dealWidgetLabel( GetDisplay(), "Left aligned" );
	widget->SetBackgroundColor( decColor( 1.0f, 0.8f, 0.8f ) );
	widget->SetAlignment( verticalAlign | dealWidgetLabel::eaLeft );
	layout->AddWidget( widget );
	widget->FreeReference();
	
	widget = new dealWidgetLabel( GetDisplay(), "Center aligned" );
	widget->SetBackgroundColor( decColor( 0.8f, 1.0f, 0.8f ) );
	widget->SetAlignment( verticalAlign | dealWidgetLabel::eaCenter );
	layout->AddWidget( widget );
	widget->FreeReference();
	
	widget = new dealWidgetLabel( GetDisplay(), "Right aligned" );
	widget->SetBackgroundColor( decColor( 0.8f, 0.8f, 1.0f ) );
	widget->SetAlignment( verticalAlign | dealWidgetLabel::eaRight );
	layout->AddWidget( widget );
	widget->FreeReference();
	
	AddWidget( layout );
	layout->FreeReference();
}

void dealWTestLayout2::AddLayout2( dealWidgetLabel::eAlignments verticalAlign ){
	dealWidgetLayoutFlow *layout = new dealWidgetLayoutFlow( GetDisplay(), false, 5, eltFill, eltFill );
	layout->SetBackgroundColor( decColor( 0.75f, 0.75f, 0.75f ) );
	layout->SetPadding( 5, 5 );
	
	dealWidgetLabel *widget = new dealWidgetLabel( GetDisplay(), "Left aligned\nMultiline Text with\nthree lines" );
	widget->SetBackgroundColor( decColor( 1.0f, 0.8f, 0.8f ) );
	widget->SetAlignment( verticalAlign | dealWidgetLabel::eaLeft );
	widget->SetFontSize( GetDisplay().GetDefaultFontSize() / 2 );
	layout->AddWidget( widget );
	widget->FreeReference();
	
	widget = new dealWidgetLabel( GetDisplay(), "Center aligned\nMultiline Text with\nthree lines" );
	widget->SetBackgroundColor( decColor( 0.8f, 1.0f, 0.8f ) );
	widget->SetAlignment( verticalAlign | dealWidgetLabel::eaCenter );
	widget->SetFontSize( GetDisplay().GetDefaultFontSize() / 2 );
	layout->AddWidget( widget );
	widget->FreeReference();
	
	widget = new dealWidgetLabel( GetDisplay(), "Right aligned\nMultiline Text with\nthree lines" );
	widget->SetBackgroundColor( decColor( 0.8f, 0.8f, 1.0f ) );
	widget->SetAlignment( verticalAlign | dealWidgetLabel::eaRight );
	widget->SetFontSize( GetDisplay().GetDefaultFontSize() / 2 );
	layout->AddWidget( widget );
	widget->FreeReference();
	
	AddWidget( layout );
	layout->FreeReference();
}

void dealWTestLayout2::AddLayout3( dealWidgetLabel::eAlignments align ){
	dealWidgetLayoutFlow *layout = new dealWidgetLayoutFlow( GetDisplay(), false, 5, eltFill, eltFill );
	layout->SetBackgroundColor( decColor( 0.75f, 0.75f, 0.75f ) );
	layout->SetPadding( 5, 5 );
	
	dealWidgetLabel *widget = new dealWidgetLabel( GetDisplay(), "A short line.\n"
		"This is a long line that is word-wrapped to the next line. "
		"It contains a bunch of text which is not much of interest except filling space.\n"
		"Another short line." );
	widget->SetBackgroundColor( decColor( 1.0f, 0.8f, 0.8f ) );
	widget->SetAlignment( align );
	layout->AddWidget( widget );
	widget->FreeReference();
	
	AddWidget( layout );
	layout->FreeReference();
}
