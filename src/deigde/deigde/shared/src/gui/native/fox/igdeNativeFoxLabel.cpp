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

#include "igdeNativeFoxLabel.h"
#include "foxIcons.h"
#include "../../igdeLabel.h"
#include "../../igdeContainer.h"
#include "../../igdeCommonDialogs.h"
#include "../../event/igdeAction.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../theme/propertyNames.h"
#include "../../resources/igdeIcon.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeNativeFoxLabel
//////////////////////////////

FXIMPLEMENT( igdeNativeFoxLabel, FXLabel, nullptr, 0 )

// Constructor, destructor
////////////////////////////

igdeNativeFoxLabel::igdeNativeFoxLabel(){ }

igdeNativeFoxLabel::igdeNativeFoxLabel( igdeLabel &powner, FXComposite *pparent,
int layoutFlags, const igdeGuiTheme &guitheme ) :
FXLabel( pparent, powner.GetText().GetString(), LabelIcon( powner ),
	layoutFlags | LabelFlags( powner ), 0, 0, 0, 0, 0, 0, 0, 0 ),
pOwner( &powner ),
pFont( LabelFont( powner, guitheme ) )
{
	setFont( (FXFont*)pFont->GetNativeFont() );
	
	setTipText( powner.GetDescription().GetString() );
	setHelpText( powner.GetDescription().GetString() );
}

igdeNativeFoxLabel::~igdeNativeFoxLabel(){
}

igdeNativeFoxLabel *igdeNativeFoxLabel::CreateNativeWidget( igdeLabel &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const pparent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! pparent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxLabel( powner, pparent,
		igdeUIFoxHelper::GetChildLayoutFlags( &powner ), *powner.GetGuiTheme() );
}

void igdeNativeFoxLabel::PostCreateNativeWidget(){
	FXComposite &pparent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( pparent.id() ){
		create();
	}
}

void igdeNativeFoxLabel::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxLabel::UpdateText(){
	setText( pOwner->GetText().GetString() );
}

void igdeNativeFoxLabel::UpdateAlignment(){
	setFrameStyle( igdeNativeFoxLabel::LabelFlags( *pOwner ) );
}

void igdeNativeFoxLabel::UpdateDescription(){
	const char * const description = pOwner->GetDescription();
	setTipText( description );
	setHelpText( description );
}

void igdeNativeFoxLabel::UpdateIcon(){
	setIcon( igdeNativeFoxLabel::LabelIcon( *pOwner ) );
}



FXIcon *igdeNativeFoxLabel::LabelIcon( const igdeLabel &powner ){
	if( powner.GetIcon() ){
		return ( FXIcon* ) powner.GetIcon()->GetNativeIcon();
		
	}else{
		return NULL;
	}
}

int igdeNativeFoxLabel::LabelFlags( const igdeLabel &powner ){
	const int alignment = powner.GetAlignment();
	int fflags = ICON_BEFORE_TEXT;
	
	if( ( alignment & igdeLabel::eaLeft ) == igdeLabel::eaLeft ){
		fflags |= JUSTIFY_LEFT;
		
	}else if( ( alignment & igdeLabel::eaRight ) == igdeLabel::eaRight ){
		fflags |= JUSTIFY_RIGHT;
		
	}else{
		fflags |=  JUSTIFY_CENTER_X;
	}
	
	if( ( alignment & igdeLabel::eaTop ) == igdeLabel::eaTop ){
		fflags |= JUSTIFY_TOP;
		
	}else if( ( alignment & igdeLabel::eaBottom ) == igdeLabel::eaBottom ){
		fflags |= JUSTIFY_BOTTOM;
		
	}else{
		fflags |=  JUSTIFY_CENTER_Y;
	}
	
	return fflags;
}

igdeFont *igdeNativeFoxLabel::LabelFont( const igdeLabel &powner, const igdeGuiTheme &guitheme ){
	igdeFont::sConfiguration configuration;
	powner.GetEnvironment().GetApplicationFont( configuration );
	
	if( guitheme.HasProperty( igdeGuiThemePropertyNames::labelFontSizeAbsolute ) ){
		configuration.size = ( float )guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::labelFontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::labelFontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::labelFontSize, 1.0f );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSizeAbsolute ) ){
		configuration.size = ( float )guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f );
	}
	
	return powner.GetEnvironment().GetSharedFont( configuration );
}

#endif
