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

#include "igdeNativeFoxProgressBar.h"
#include "../../igdeProgressBar.h"
#include "../../resources/igdeFont.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../theme/propertyNames.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Event map
//////////////

FXDEFMAP( igdeNativeFoxProgressBar ) igdeNativeFoxProgressBarMap[] = { };

FXIMPLEMENT( igdeNativeFoxProgressBar, FXProgressBar,
	igdeNativeFoxProgressBarMap, ARRAYNUMBER( igdeNativeFoxProgressBarMap ) )



// Class igdeNativeFoxProgressBar
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeFoxProgressBar::igdeNativeFoxProgressBar(){ }

igdeNativeFoxProgressBar::igdeNativeFoxProgressBar( igdeProgressBar &owner, FXComposite *parent,
	const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme ) :
FXProgressBar( parent, NULL, 0, layoutFlags.flags | ProgressBarFlags( owner ), 0, 0, 0, 0,
	ProgressBarPadLeft( guitheme ), ProgressBarPadRight( guitheme ),
	ProgressBarPadTop( guitheme ), ProgressBarPadBottom( guitheme ) ),
pOwner( &owner )
{
	setFont( (FXFont*)ProgressBarFont( owner, guitheme )->GetNativeFont() );
	if( ! pOwner->GetVisible() ){
		hide();
	}
	
	UpdateRange();
	UpdateValue();
	UpdateDescription();
}

igdeNativeFoxProgressBar::~igdeNativeFoxProgressBar(){
}



// Management
///////////////

void igdeNativeFoxProgressBar::UpdateRange(){
	setTotal( pOwner->GetUpper() - pOwner->GetLower() );
}

void igdeNativeFoxProgressBar::UpdateValue(){
	setProgress( pOwner->GetValue() - pOwner->GetLower() );
}

void igdeNativeFoxProgressBar::UpdateEnabled(){
	if( pOwner->GetEngine() ){
		enable();
		
	}else{
		disable();
	}
}

void igdeNativeFoxProgressBar::UpdateDescription(){
	setHelpTag( pOwner->GetDescription().GetString() );
}



int igdeNativeFoxProgressBar::ProgressBarFlags( const igdeProgressBar &owner ){
	int flags = PROGRESSBAR_NORMAL;
	
	switch( owner.GetOrientation() ){
	case igdeProgressBar::eoHorizontal:
		flags |= PROGRESSBAR_HORIZONTAL;
		break;
		
	case igdeProgressBar::eoVertical:
		flags |= PROGRESSBAR_VERTICAL;
		break;
		
	default:
		break;
	}
	
	return flags;
}

igdeFont *igdeNativeFoxProgressBar::ProgressBarFont( const igdeProgressBar &owner, const igdeGuiTheme &guitheme ){
	igdeFont::sConfiguration configuration;
	owner.GetEnvironment().GetApplicationFont( configuration );
	
	if( guitheme.HasProperty( igdeGuiThemePropertyNames::progressBarFontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::progressBarFontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::progressBarFontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::progressBarFontSize, 1.0f );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f );
	}
	
	return owner.GetEnvironment().GetSharedFont( configuration );
}

int igdeNativeFoxProgressBar::ProgressBarPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::progressBarPaddingLeft, DEFAULT_PAD );
}

int igdeNativeFoxProgressBar::ProgressBarPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::progressBarPaddingRight, DEFAULT_PAD );
}

int igdeNativeFoxProgressBar::ProgressBarPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::progressBarPaddingTop, DEFAULT_PAD );
}

int igdeNativeFoxProgressBar::ProgressBarPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::progressBarPaddingBottom, DEFAULT_PAD );
}
