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

#include <stdio.h>
#include <stdlib.h>

#include "igdeWindow.h"
#include "igdeCommonDialogs.h"
#include "igdeContainerReference.h"
#include "igdeWidget.h"
#include "igdeWidgetReference.h"
#include "native/toolkit.h"
#include "resources/igdeIcon.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>


// Class igdeWindow
/////////////////////

// Constructor, destructor
////////////////////////////

igdeWindow::igdeWindow( igdeEnvironment &environment, const char *title, igdeIcon *icon, bool canResize ) :
igdeContainer( environment ),
pTitle( title ),
pIcon( icon ),
pCanResize( canResize ),
pPosition( 10, 50 ),
pSize( 400, 300 ),
pEnabled( true )
{
	SetVisible( false );
}

igdeWindow::~igdeWindow(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeWindow::SetTitle( const char *title ){
	if( pTitle == title ){
		return;
	}
	
	pTitle = title;
	OnTitleChanged();
}

void igdeWindow::SetIcon( igdeIcon* icon ){
	if( pIcon == icon ){
		return;
	}
	
	pIcon = icon;
	OnIconChanged();
}

void igdeWindow::SetSize( const decPoint &size ){
	if( size == pSize ){
		return;
	}
	
	pSize = size;
	OnSizeChanged();
}

void igdeWindow::SetEnabled( bool enabled ){
	if( enabled == pEnabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeWindow::SetPosition( const decPoint &position ){
	if( position == pPosition ){
		return;
	}
	
	pPosition = position;
	OnPositionChanged();
}

void igdeWindow::RaiseAndActivate(){
	OnRaiseAndActivate();
}



void igdeWindow::AddChild( igdeWidget *child ){
	if( GetChildCount() > 0 ){
		DETHROW( deeInvalidParam );
	}
	igdeContainer::AddChild( child );
}



bool igdeWindow::CloseWindow(){
	return true;
}

void igdeWindow::Close(){
	if( GetNativeWidget() ){
		DestroyNativeWidget();
	}
}



// IGDE Internal Use Only
///////////////////////////

void igdeWindow::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	if( GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	igdeNativeWindow * const native = igdeNativeWindow::CreateNativeWidget( *this );
	SetNativeWidget( native );
	CreateChildWidgetNativeWidgets();
	native->PostCreateNativeWidget();
}

void igdeWindow::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeWindow* )GetNativeWidget() )->DestroyNativeWidget();
	DropNativeWidget();
}



void igdeWindow::OnTitleChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeWindow* )GetNativeWidget() )->UpdateTitle();
	}
}

void igdeWindow::OnIconChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeWindow* )GetNativeWidget() )->UpdateIcon();
	}
}

void igdeWindow::OnSizeChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeWindow* )GetNativeWidget() )->UpdateSize();
	}
}

void igdeWindow::OnPositionChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeWindow* )GetNativeWidget() )->UpdatePosition();
	}
}

void igdeWindow::OnVisibleChanged(){
	if( GetVisible() ){
		CreateNativeWidget();
	}
	igdeWidget::OnVisibleChanged();
}

void igdeWindow::OnEnabledChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeWindow* )GetNativeWidget() )->UpdateEnabled();
	}
}

void igdeWindow::OnRaiseAndActivate(){
	if( GetNativeWidget() ){
		( ( igdeNativeWindow* )GetNativeWidget() )->RaiseAndActivate();
	}
}
