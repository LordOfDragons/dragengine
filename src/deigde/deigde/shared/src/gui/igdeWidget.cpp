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
#include <string.h>

#include "igdeWidget.h"
#include "igdeContainer.h"
#include "native/toolkit.h"
#include "../engine/igdeEngineController.h"
#include "../environment/igdeEnvironment.h"
#include "../gameproject/igdeGameProject.h"

#include <dragengine/common/exceptions.h>



// Class igdeWidget
/////////////////////

// Constructor, destructor
////////////////////////////

igdeWidget::igdeWidget( igdeEnvironment &environment ) :
pEnvironment( environment ),
pNativeWidget( NULL ),
pParent( NULL ),
pVisible( true ){
}

igdeWidget::~igdeWidget(){
}



// Management
///////////////

void igdeWidget::SetVisible( bool visible ){
	if( pVisible == visible ){
		return;
	}
	
	pVisible = visible;
	OnVisibleChanged();
}

igdeEngineController &igdeWidget::GetEngineController() const{
	return *pEnvironment.GetEngineController();
}

igdeGameProject *igdeWidget::GetGameProject() const{
	return pEnvironment.GetGameProject();
}

igdeGameDefinition *igdeWidget::GetGameDefinition() const{
	return GetGameProject()->GetGameDefinition();
}

deEngine *igdeWidget::GetEngine() const{
	return GetEngineController().GetEngine();
}

deLogger *igdeWidget::GetLogger() const{
	return pEnvironment.GetLogger();
}



void igdeWidget::SetWidgetGuiTheme( igdeGuiTheme *guitheme ){
	if( pGuiTheme == guitheme ){
		return;
	}
	
	pGuiTheme = guitheme;
	OnGuiThemeChanged();
}

void igdeWidget::SetWidgetGuiThemeName( const char *guitheme ){
	if( pGuiThemeName == guitheme ){
		return;
	}
	
	pGuiThemeName = guitheme;
	OnGuiThemeChanged();
}

igdeGuiTheme *igdeWidget::GetGuiTheme() const{
	if( pGuiTheme ){
		return pGuiTheme;
		
	}else if( ! pGuiThemeName.IsEmpty() ){
		return pEnvironment.GetGuiThemeNamed( pGuiThemeName );
		
	}else if( pParent ){
		return pParent->GetGuiTheme();
		
	}else{
		return pEnvironment.GetDefaultGuiTheme();
	}
}



void igdeWidget::SetParent( igdeContainer *parent ){
	pParent = parent;
}

void igdeWidget::OnResize(){
}



decPoint igdeWidget::WidgetToScreen( const decPoint &position ) const{
	return igdeNativeWidget::WidgetToScreen( *this, position );
}



void igdeWidget::OnVisibleChanged(){
	igdeNativeWidget::UpdateVisible( *this );
}

void igdeWidget::OnGuiThemeChanged(){
}

void igdeWidget::SetNativeWidget( void *nativeWidget ){
	pNativeWidget = nativeWidget;
}

void igdeWidget::DropNativeWidget(){
	pNativeWidget = NULL;
}
