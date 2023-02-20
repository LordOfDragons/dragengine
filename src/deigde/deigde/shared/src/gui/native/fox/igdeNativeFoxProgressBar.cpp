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

#ifdef IGDE_TOOLKIT_FOX

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "igdeNativeFoxProgressBar.h"
#include "../../igdeContainer.h"
#include "../../igdeProgressBar.h"
#include "../../resources/igdeFont.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../theme/propertyNames.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Event map
//////////////

FXIMPLEMENT( igdeNativeFoxProgressBar, FXProgressBar, nullptr, 0 )



// Class igdeNativeFoxProgressBar
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeFoxProgressBar::igdeNativeFoxProgressBar(){ }

igdeNativeFoxProgressBar::igdeNativeFoxProgressBar( igdeProgressBar &powner, FXComposite *pparent,
	const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme ) :
FXProgressBar( pparent, NULL, 0, layoutFlags.flags | ProgressBarFlags( powner ), 0, 0, 0, 0,
	ProgressBarPadLeft( guitheme ), ProgressBarPadRight( guitheme ),
	ProgressBarPadTop( guitheme ), ProgressBarPadBottom( guitheme ) ),
pOwner( &powner )
{
	setFont( (FXFont*)ProgressBarFont( powner, guitheme )->GetNativeFont() );
	if( ! pOwner->GetVisible() ){
		hide();
	}
	
	UpdateRange();
	UpdateValue();
	UpdateDescription();
}

igdeNativeFoxProgressBar::~igdeNativeFoxProgressBar(){
}

igdeNativeFoxProgressBar *igdeNativeFoxProgressBar::CreateNativeWidget( igdeProgressBar &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const pparent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! pparent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxProgressBar( powner, pparent,
		igdeUIFoxHelper::GetChildLayoutFlagsAll( &powner ), *powner.GetGuiTheme() );
}

void igdeNativeFoxProgressBar::PostCreateNativeWidget(){
	FXComposite &pparent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( pparent.id() ){
		create();
	}
}

void igdeNativeFoxProgressBar::DestroyNativeWidget(){
	delete this;
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



int igdeNativeFoxProgressBar::ProgressBarFlags( const igdeProgressBar &powner ){
	int fflags = PROGRESSBAR_NORMAL;
	
	switch( powner.GetOrientation() ){
	case igdeProgressBar::eoHorizontal:
		fflags |= PROGRESSBAR_HORIZONTAL;
		break;
		
	case igdeProgressBar::eoVertical:
		fflags |= PROGRESSBAR_VERTICAL;
		break;
		
	default:
		break;
	}
	
	return fflags;
}

igdeFont *igdeNativeFoxProgressBar::ProgressBarFont( const igdeProgressBar &powner, const igdeGuiTheme &guitheme ){
	igdeFont::sConfiguration configuration;
	powner.GetEnvironment().GetApplicationFont( configuration );
	
	if( guitheme.HasProperty( igdeGuiThemePropertyNames::progressBarFontSizeAbsolute ) ){
		configuration.size = ( float )guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::progressBarFontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::progressBarFontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::progressBarFontSize, 1.0f );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSizeAbsolute ) ){
		configuration.size = ( float )guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f );
	}
	
	return powner.GetEnvironment().GetSharedFont( configuration );
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

#endif
