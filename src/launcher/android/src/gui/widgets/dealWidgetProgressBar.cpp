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

#include "dealWidgetProgressBar.h"
#include "dealWidgetLayoutFlow.h"
#include "dealWidgetLabel.h"
#include "../dealDisplay.h"
#include "../dealFont.h"
#include "../../dealLauncher.h"
#include "../../common/exceptions.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealWidgetProgressBar
////////////////////////////////

// Constructors, destructors
//////////////////////////////

dealWidgetProgressBar::dealWidgetProgressBar( dealDisplay &display ) :
dealWidgetLayoutStack( display, true ),

pBoxProgress( NULL ),
pLabelProgress( NULL ),

pMinProgress( 0 ),
pMaxProgress( 100 ),
pProgress( 0 )
{
	pBuildContent();
}

dealWidgetProgressBar::dealWidgetProgressBar( dealDisplay &display, int minProgress, int maxProgress ) :
dealWidgetLayoutStack( display, true ),

pBoxProgress( NULL ),
pLabelProgress( NULL ),

pMinProgress( minProgress ),
pMaxProgress( decMath::max( maxProgress, minProgress ) ),
pProgress( minProgress )
{
	pBuildContent();
}

dealWidgetProgressBar::~dealWidgetProgressBar(){
}



// Management
///////////////

void dealWidgetProgressBar::SetRange( int minProgress, int maxProgress ){
	maxProgress = decMath::max( maxProgress, minProgress );
	
	if( pMinProgress == minProgress && pMaxProgress == maxProgress ){
		return;
	}
	
	pMinProgress = minProgress;
	pMaxProgress = maxProgress;
	pProgress = decMath::clamp( pProgress, minProgress, maxProgress );
	
	pUpdateContent();
}

void dealWidgetProgressBar::SetProgress( int progress ){
	progress = decMath::clamp( progress, pMinProgress, pMaxProgress );
	
	if( pProgress == progress ){
		return;
	}
	
	pProgress = progress;
	
	pUpdateContent();
}

float dealWidgetProgressBar::GetProgressPercentage() const{
	return decMath::linearStep( ( float )pProgress, ( float )pMinProgress, ( float )pMaxProgress );
}



void dealWidgetProgressBar::OnSizeChanged(){
	pUpdateContent();
}

void dealWidgetProgressBar::OnPaddingChanged(){
	pUpdateContent();
}



// Private functions
//////////////////////

void dealWidgetProgressBar::pBuildContent(){
	dealDisplay &display = GetDisplay();
	
	dealWidgetLayoutFlow * const layoutBox = new dealWidgetLayoutFlow( display,
		true, 0, dealWidgetLayoutFlow::eltTop, dealWidgetLayoutFlow::eltFill );
	layoutBox->SetBackgroundColor( decColor( 0.2f, 0.2f, 0.2f ) );
	
	pBoxProgress = new dealWidget( display );
	pBoxProgress->SetBackgroundColor( decColor( 0.0f, 0.0f, 1.0f ) );
	layoutBox->AddWidget( pBoxProgress );
	AddWidget( layoutBox );
	layoutBox->FreeReference();
	
	pLabelProgress = new dealWidgetLabel( display, "0 %" );
	AddWidget( pLabelProgress );
}

void dealWidgetProgressBar::pUpdateContent(){
	const float percentage = GetProgressPercentage();
	
	decString text;
	text.Format( "%.0f %%", percentage * 100.0f );
	pLabelProgress->SetText( text );
	
	const float colorBlend = decMath::linearStep( percentage, 0.4f, 0.6f );
	const decColor color1( 1.0f, 1.0f, 1.0f );
	const decColor color2( 1.0f, 1.0f, 0.0f );
	pLabelProgress->SetColor( color1 * ( 1.0f - colorBlend ) + color2 * colorBlend );
	
	const int width = ( int )( percentage * ( float )GetContentArea().x );
	pBoxProgress->SetExplicitMinimumSize( decPoint( width, 0 ) );
}
