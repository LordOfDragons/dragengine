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
#include <stdint.h>

#include "igdeNativeFoxStatusBar.h"
#include "../../igdeContainer.h"
#include "../../igdeStatusBar.h"
#include "../../resources/igdeFont.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../theme/propertyNames.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Event map
//////////////

FXDEFMAP( igdeNativeFoxStatusBar ) igdeNativeFoxStatusBarMap[] = {
	FXMAPFUNC( SEL_CONFIGURE, 0, igdeNativeFoxStatusBar::onResize ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxStatusBar::onChildLayoutFlags )
};

FXIMPLEMENT( igdeNativeFoxStatusBar, FXStatusBar,
	igdeNativeFoxStatusBarMap, ARRAYNUMBER( igdeNativeFoxStatusBarMap ) )



// Class igdeNativeFoxStatusBar
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeFoxStatusBar::igdeNativeFoxStatusBar(){ }

igdeNativeFoxStatusBar::igdeNativeFoxStatusBar( igdeStatusBar &owner, FXComposite *parent,
	const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme ) :
FXStatusBar( parent, layoutFlags.flags | StatusBarFlags( owner ) ),
pOwner( &owner )
{
	if( ! pOwner->GetVisible() ){
		hide();
	}
	
	getStatusLine()->setFont( ( FXFont* )StatusBarFont( owner, guitheme )->GetNativeFont() );
	
	UpdateText();
}

igdeNativeFoxStatusBar::~igdeNativeFoxStatusBar(){
}

igdeNativeFoxStatusBar *igdeNativeFoxStatusBar::CreateNativeWidget( igdeStatusBar &owner ){
	if( ! owner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const parent = ( FXComposite* )owner.GetParent()->GetNativeContainer();
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxStatusBar( owner, parent,
		igdeUIFoxHelper::GetChildLayoutFlagsAll( &owner ), *owner.GetGuiTheme() );
}

void igdeNativeFoxStatusBar::PostCreateNativeWidget(){
	FXComposite &parent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( parent.id() ){
		create();
	}
}

void igdeNativeFoxStatusBar::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxStatusBar::UpdateText(){
	getStatusLine()->setNormalText( pOwner->GetText().GetString() );
	getStatusLine()->setText( pOwner->GetText().GetString() );
}



int igdeNativeFoxStatusBar::StatusBarFlags( const igdeStatusBar & ){
	return 0;
}

igdeFont *igdeNativeFoxStatusBar::StatusBarFont( const igdeStatusBar &owner, const igdeGuiTheme &guitheme ){
	igdeFont::sConfiguration configuration;
	owner.GetEnvironment().GetApplicationFont( configuration );
	
	if( guitheme.HasProperty( igdeGuiThemePropertyNames::textFieldFontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::textFieldFontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::textFieldFontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::textFieldFontSize, 1.0f );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f );
	}
	
	return owner.GetEnvironment().GetSharedFont( configuration );
}



// Events
///////////

long igdeNativeFoxStatusBar::onResize( FXObject*, FXSelector, void* ){
	pOwner->OnResize();
	return 1;
}

long igdeNativeFoxStatusBar::onChildLayoutFlags( FXObject*, FXSelector, void *data ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )data );
	clflags.flags = LAYOUT_FILL_X | LAYOUT_FILL_Y;
	return 1;
}
