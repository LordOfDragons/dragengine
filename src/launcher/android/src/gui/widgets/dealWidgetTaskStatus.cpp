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

#include "dealWidgetTaskStatus.h"
#include "dealWidgetLayoutFlow.h"
#include "dealWidgetLabel.h"
#include "dealWidgetProgressBar.h"
#include "../dealDisplay.h"
#include "../dealFont.h"
#include "../../dealLauncher.h"
#include "../../logger/deLogger.h"
#include "../../common/exceptions.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealWidgetTaskStatus
///////////////////////////////

// Constructors, destructors
//////////////////////////////

dealWidgetTaskStatus::dealWidgetTaskStatus( dealDisplay &display ) :
dealWidgetLayoutStack( display, true ),

pLabelName( NULL ),
pLabelStatus( NULL ),
pProgressBar( NULL ),
pBgPending( NULL ),
pBgFinished( NULL ),

pFinished( false )
{
	pBuildContent();
}

dealWidgetTaskStatus::~dealWidgetTaskStatus(){
}



// Management
///////////////

void dealWidgetTaskStatus::SetName( const char *name ){
	if( pName == name ){
		return;
	}
	
	pName = name;
	
	pLabelName->SetText( name );
}

void dealWidgetTaskStatus::SetStatus( const char *text ){
	if( pStatus == text ){
		return;
	}
	
	pStatus = text;
	
	pLabelStatus->SetText( text );
}

void dealWidgetTaskStatus::SetProgressRange( int range ){
	pProgressBar->SetRange( 0, range );
}

float dealWidgetTaskStatus::GetProgress() const{
	return pProgressBar->GetProgress();
}

void dealWidgetTaskStatus::SetProgress( int progress ){
	pProgressBar->SetProgress( progress );
}



void dealWidgetTaskStatus::SetFinished( bool finished ){
	if( pFinished == finished ){
		return;
	}
	
	pFinished = finished;
	
	if( pBgPending ){
		pBgPending->SetVisible( ! pFinished );
	}
	if( pBgFinished ){
		pBgFinished->SetVisible( pFinished );
	}
}
/*@}*/



// Private functions
//////////////////////

void dealWidgetTaskStatus::pBuildContent(){
	dealDisplay &display = GetDisplay();
	const int fontSize = display.GetDefaultFontSize();
	
	// background
	pBgFinished = new dealWidget( display );
	pBgFinished->SetBackgroundColor( decColor( 0.5f, 1.0f, 0.5f ) );
	pBgFinished->SetVisible( false );
	AddWidget( pBgFinished );
	
	pBgPending = new dealWidget( display );
	pBgPending->SetBackgroundColor( decColor( 1.0f, 0.5f, 0.5f ) );
	AddWidget( pBgPending );
	
	// information
	dealWidgetLayoutFlow * const layout = new dealWidgetLayoutFlow( display, false,
		fontSize / 2, dealWidgetLayoutFlow::eltFill, dealWidgetLayoutFlow::eltFill );
	layout->SetPadding( fontSize, fontSize / 2 );
	
	pLabelName = new dealWidgetLabel( display, "" );
	layout->AddWidget( pLabelName );
	
	pLabelStatus = new dealWidgetLabel( display, "" );
	layout->AddWidget( pLabelStatus );
	
	pProgressBar = new dealWidgetProgressBar( display, 0, 100 );
	layout->AddWidget( pProgressBar );
	
	AddWidget( layout );
	layout->FreeReference();
}
