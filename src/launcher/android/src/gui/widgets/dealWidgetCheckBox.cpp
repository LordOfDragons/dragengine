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
#include <android/native_activity.h>

#include "dealGuiTheme.h"
#include "dealWidgetCheckBox.h"
#include "dealWidgetLabel.h"
#include "dealWidgetImage.h"
#include "dealWidgetLayoutFlow.h"
#include "dealWidgetLayoutStack.h"
#include "../dealDisplay.h"
#include "../dealFont.h"
#include "../dealImage.h"
#include "../../dealLauncher.h"
#include "../../common/exceptions.h"
#include "../../logger/deLoggerFile.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealWidgetCheckBox
///////////////////////////

// Constructors, destructors
//////////////////////////////

dealWidgetCheckBox::dealWidgetCheckBox( dealDisplay &display, dealImage *image, const char *label ) :
dealWidgetLayoutFlow( display, true, 0, eltTop, eltCenterFill ),

pLabel( NULL ),
pImage( NULL ),
pBoxImageUnchecked( NULL ),
pBoxImageChecked( NULL ),
pChecked( false )
{
	SetGap( display.GetDefaultFontSize() / 4 );
	SetPadding( display.GetDefaultFontSize() / 4, display.GetDefaultFontSize() / 4 );
	pBuildContent();
	SetLabel( label );
	SetImage( image );
}

dealWidgetCheckBox::~dealWidgetCheckBox(){
}



// Management
///////////////

void dealWidgetCheckBox::SetFromGuiTheme( const dealGuiTheme &guitheme ){
	dealWidget::SetFromGuiTheme( guitheme );
	
	SetBackgroundColor( guitheme.GetCheckBoxBackgroundColor() );
	pLabel->SetColor( guitheme.GetCheckBoxTextColor() );
	
	pBoxImageUnchecked->SetImage( guitheme.GetCheckBoxImageUnchecked() );
	pBoxImageUnchecked->SetScaling( guitheme.GetCheckBoxImageScale() );
	pBoxImageUnchecked->SetColorize( guitheme.GetCheckBoxImageUncheckedColorize() );
	
	pBoxImageChecked->SetImage( guitheme.GetCheckBoxImageChecked() );
	pBoxImageChecked->SetScaling( guitheme.GetCheckBoxImageScale() );
	pBoxImageChecked->SetColorize( guitheme.GetCheckBoxImageCheckedColorize() );
}



const decColor &dealWidgetCheckBox::GetTextColor() const{
	return pLabel->GetColor();
}

void dealWidgetCheckBox::SetTextColor( const decColor &color ){
	pLabel->SetColor( color );
}



const decString &dealWidgetCheckBox::GetLabel() const{
	return pLabel->GetText();
}

void dealWidgetCheckBox::SetLabel( const char *label ){
	if( pLabel->GetText() == label ){
		return;
	}
	
	pLabel->SetText( label );
	pLabel->SetVisible( ! pLabel->GetText().IsEmpty() );
}

dealImage *dealWidgetCheckBox::GetImage() const{
	return pImage->GetImage();
}

void dealWidgetCheckBox::SetImage( dealImage *image ){
	if( pImage->GetImage() == image ){
		return;
	}
	
	pImage->SetImage( image );
	pImage->SetVisible( image != NULL );
}



void dealWidgetCheckBox::SetChecked( bool checked ){
	if( checked == pChecked ){
		return;
	}
	
	pChecked = checked;
	pBoxImageChecked->SetVisible( checked );
	pBoxImageUnchecked->SetVisible( ! checked );
	
	OnStateChanged();
}



void dealWidgetCheckBox::OnEnabledChanged(){
	dealWidgetLayoutFlow::OnEnabledChanged();
	
	const bool enabled = GetEnabled();
	pImage->SetEnabled( enabled );
	pLabel->SetEnabled( enabled );
	pBoxImageUnchecked->SetEnabled( enabled );
	pBoxImageChecked->SetEnabled( enabled );
}

void dealWidgetCheckBox::OnStateChanged(){
}



void dealWidgetCheckBox::OnKeyPress( int keycode ){
}

void dealWidgetCheckBox::OnKeyRelease( int keycode ){
}

void dealWidgetCheckBox::OnMousePress( int buttons, const decPoint &position ){
	if( ! GetEnabled() ){
		return;
	}
	
	if( buttons != 0 ){
		// buttons checked while touching screen. protects also against multiple touching
		return;
	}
	
	SetChecked( ! pChecked );
}



// Private functions
//////////////////////

void dealWidgetCheckBox::pBuildContent(){
	dealDisplay &display = GetDisplay();
	
	// content
	dealWidgetLayoutStack * const boxes = new dealWidgetLayoutStack( display, false );
	
	pBoxImageUnchecked = new dealWidgetImage( display );
	boxes->AddWidget( pBoxImageUnchecked );
	
	pBoxImageChecked = new dealWidgetImage( display );
	pBoxImageChecked->SetVisible( false );
	boxes->AddWidget( pBoxImageChecked );
	
	AddWidget( boxes );
	
	pImage = new dealWidgetImage( display );
	pImage->SetVisible( false );
	AddWidget( pImage );
	
	pLabel = new dealWidgetLabel( display, "" );
	pLabel->SetVisible( false );
	AddWidget( pLabel );
}
