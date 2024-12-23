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
