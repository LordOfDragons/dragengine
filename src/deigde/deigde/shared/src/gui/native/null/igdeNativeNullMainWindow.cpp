/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifdef IGDE_TOOLKIT_NULL

#include "igdeNativeNullApplication.h"
#include "igdeNativeNullMainWindow.h"
#include "../../igdeApplication.h"
#include "../../igdeMainWindow.h"
#include "../../../engine/igdeEngineController.h"


// Class igdeNativeNullMainWindow
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeNullMainWindow::igdeNativeNullMainWindow( igdeMainWindow &owner ) :
pOwner( owner )
{
	owner.GetEngineController().InitEngine();
	owner.StartEngine();
}

igdeNativeNullMainWindow::~igdeNativeNullMainWindow(){
	pOwner.GetEngineController().UnparentMainRenderWindow();
	( ( igdeNativeNullApplication* )igdeApplication::app().GetNativeApplication() )->Quit();
}

igdeNativeNullMainWindow *igdeNativeNullMainWindow::CreateNativeWidget( igdeMainWindow &owner ){
	return new igdeNativeNullMainWindow( owner );
}

void igdeNativeNullMainWindow::PostCreateNativeWidget(){
}

void igdeNativeNullMainWindow::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

decColor igdeNativeNullMainWindow::GetSystemColor( igdeEnvironment::eSystemColors color ) const{
	switch( color ){
	case igdeEnvironment::escWindowForeground:
	case igdeEnvironment::escWidgetForeground:
	case igdeEnvironment::escWidgetSelectedBackground:
		return decColor( 0.0f, 0.0f, 0.0f );
		
	case igdeEnvironment::escWidgetBackground:
		return decColor( 0.65f, 0.65f, 0.65f );
		
	case igdeEnvironment::escWidgetHighlight:
		return decColor( 0.9f, 0.9f, 0.9f );
		
	case igdeEnvironment::escWidgetShadow:
		return decColor( 0.2f, 0.2f, 0.2f );
		
	case igdeEnvironment::escWindowBackground:
	case igdeEnvironment::escWidgetSelectedForeground:
	default:
		return decColor( 1.0f, 1.0f, 1.0f );
	}
}

void igdeNativeNullMainWindow::UpdateWindowState(){
}

void igdeNativeNullMainWindow::UpdateEnabled(){
}

void igdeNativeNullMainWindow::UpdatePosition(){
}

void igdeNativeNullMainWindow::UpdateIcon(){
}

void igdeNativeNullMainWindow::UpdateTitle(){
}

void igdeNativeNullMainWindow::UpdateSize(){
}

void igdeNativeNullMainWindow::SetWindowState(){
}

void igdeNativeNullMainWindow::GetAppFontConfig( igdeFont::sConfiguration &config ){
	config.name = "Courier";
	config.size = 12.0;
	config.bold = false;
	config.italic = false;
	config.underline = false;
	config.strikeThrough = false;
}

#endif
