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

#include "dealDialog.h"
#include "dealDisplay.h"
#include "widgets/dealWidgetLayoutFlow.h"
#include "../dealLauncher.h"
#include "../common/exceptions.h"
#include "../logger/deLogger.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealDialog
/////////////////////

// Constructors, destructors
//////////////////////////////

dealDialog::dealDialog( dealDisplay &display, const char *title ) :
pDisplay( display ),

pTitle( title ),
pContent( NULL ),
pPopupDialog( NULL )
{
	pContent = new dealWidgetLayoutFlow( display, false, 0,
		dealWidgetLayoutFlow::eltFill, dealWidgetLayoutFlow::eltFill );
	pContent->SetSize( decPoint( display.GetWidth(), display.GetHeight() ) );
	pContent->SetBackgroundColor( decColor( 0.9f, 0.9f, 0.9f ) );
}

dealDialog::~dealDialog(){
	if( pPopupDialog ){
		pPopupDialog->FreeReference();
	}
	if( pContent ){
		pContent->FreeReference();
	}
}



// Management
///////////////

void dealDialog::SetTitle( const char *title ){
	if( pTitle == title ){
		return;
	}
	
	pTitle = title;
}

void dealDialog::SetPopupDialog( dealDialog *dialog ){
	if( pPopupDialog == dialog ){
		return;
	}
	
	if( pPopupDialog ){
		pPopupDialog->OnDeactivate();
		pPopupDialog->FreeReference();
	}
	
	pPopupDialog = dialog;
	
	if( dialog ){
		dialog->AddReference();
		
		const decPoint screenSize( pDisplay.GetWidth(), pDisplay.GetHeight() );
		const decPoint popupSize( dialog->pContent->GetMinimumSize() );
		
		dialog->pContent->SetSize( popupSize );
		dialog->pContent->SetPosition( ( screenSize - popupSize ) / 2 );
		
		dialog->OnActivate();
	}
}



void dealDialog::OnActivate(){
}

void dealDialog::OnDeactivate(){
}

void dealDialog::OnFrameUpdate(){
}

dealDialog *dealDialog::BackKeyDialog(){
	return NULL;
}
