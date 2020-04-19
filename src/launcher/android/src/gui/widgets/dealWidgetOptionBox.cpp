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
#include "dealWidgetOptionBox.h"
#include "dealWidgetOptionGroup.h"
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



// Class dealWidgetOptionBox
//////////////////////////////

// Constructors, destructors
//////////////////////////////

dealWidgetOptionBox::dealWidgetOptionBox( dealDisplay &display, dealImage *image, const char *label ) :
dealWidgetLayoutFlow( display, true, 0, eltTop, eltCenterFill ),

pLabel( NULL ),
pImage( NULL ),
pBoxImageDeselected( NULL ),
pBoxImageSelected( NULL ),

pGroup( NULL ),
pSelected( false )
{
	SetGap( display.GetDefaultFontSize() / 4 );
	SetPadding( display.GetDefaultFontSize() / 4, display.GetDefaultFontSize() / 4 );
	pBuildContent();
	SetLabel( label );
	SetImage( image );
}

dealWidgetOptionBox::~dealWidgetOptionBox(){
	SetGroup( NULL );
}



// Management
///////////////

void dealWidgetOptionBox::SetFromGuiTheme( const dealGuiTheme &guitheme ){
	dealWidget::SetFromGuiTheme( guitheme );
	
	SetBackgroundColor( guitheme.GetOptionBoxBackgroundColor() );
	pLabel->SetColor( guitheme.GetOptionBoxTextColor() );
	
	pBoxImageDeselected->SetImage( guitheme.GetOptionBoxImageDeselected() );
	pBoxImageDeselected->SetScaling( guitheme.GetOptionBoxImageScale() );
	pBoxImageDeselected->SetColorize( guitheme.GetOptionBoxImageDeselectedColorize() );
	
	pBoxImageSelected->SetImage( guitheme.GetOptionBoxImageSelected() );
	pBoxImageSelected->SetScaling( guitheme.GetOptionBoxImageScale() );
	pBoxImageSelected->SetColorize( guitheme.GetOptionBoxImageSelectedColorize() );
}



const decColor &dealWidgetOptionBox::GetTextColor() const{
	return pLabel->GetColor();
}

void dealWidgetOptionBox::SetTextColor( const decColor &color ){
	pLabel->SetColor( color );
}



const decString &dealWidgetOptionBox::GetLabel() const{
	return pLabel->GetText();
}

void dealWidgetOptionBox::SetLabel( const char *label ){
	if( pLabel->GetText() == label ){
		return;
	}
	
	pLabel->SetText( label );
	pLabel->SetVisible( ! pLabel->GetText().IsEmpty() );
}

dealImage *dealWidgetOptionBox::GetImage() const{
	return pImage->GetImage();
}

void dealWidgetOptionBox::SetImage( dealImage *image ){
	if( pImage->GetImage() == image ){
		return;
	}
	
	pImage->SetImage( image );
	pImage->SetVisible( image != NULL );
}



void dealWidgetOptionBox::SetGroup( dealWidgetOptionGroup *group ){
	if( group == pGroup ){
		return;
	}
	
	if( pGroup ){
		pGroup->Remove( this );
		pGroup->FreeReference();
	}
	
	if( group ){
		group->Add( this );
		group->AddReference();
	}
	
	pGroup = group;
}

void dealWidgetOptionBox::SetSelected( bool selected ){
	if( selected == pSelected ){
		return;
	}
	
	pSelected = selected;
	pBoxImageSelected->SetVisible( selected );
	pBoxImageDeselected->SetVisible( ! selected );
	
	if( selected && pGroup ){
		pGroup->Select( this );
	}
	
	OnStateChanged();
}



void dealWidgetOptionBox::OnEnabledChanged(){
	dealWidgetLayoutFlow::OnEnabledChanged();
	
	const bool enabled = GetEnabled();
	pImage->SetEnabled( enabled );
	pLabel->SetEnabled( enabled );
	pBoxImageDeselected->SetEnabled( enabled );
	pBoxImageSelected->SetEnabled( enabled );
}

void dealWidgetOptionBox::OnStateChanged(){
}



void dealWidgetOptionBox::OnKeyPress( int keycode ){
}

void dealWidgetOptionBox::OnKeyRelease( int keycode ){
}

void dealWidgetOptionBox::OnMousePress( int buttons, const decPoint &position ){
	if( ! GetEnabled() ){
		return;
	}
	
	if( buttons != 0 ){
		// buttons selected while touching screen. protects also against multiple touching
		return;
	}
	
	SetSelected( true );
}



// Private functions
//////////////////////

void dealWidgetOptionBox::pBuildContent(){
	dealDisplay &display = GetDisplay();
	
	// content
	dealWidgetLayoutStack * const boxes = new dealWidgetLayoutStack( display, false );
	
	pBoxImageDeselected = new dealWidgetImage( display );
	boxes->AddWidget( pBoxImageDeselected );
	
	pBoxImageSelected = new dealWidgetImage( display );
	pBoxImageSelected->SetVisible( false );
	boxes->AddWidget( pBoxImageSelected );
	
	AddWidget( boxes );
	
	pImage = new dealWidgetImage( display );
	pImage->SetVisible( false );
	AddWidget( pImage );
	
	pLabel = new dealWidgetLabel( display, "" );
	pLabel->SetVisible( false );
	AddWidget( pLabel );
}
