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



// Events
///////////

FXDEFMAP( igdeNativeFoxLabel ) igdeNativeFoxLabelMap[] = {
};



// Class igdeNativeFoxLabel
//////////////////////////////

FXIMPLEMENT( igdeNativeFoxLabel, FXLabel, igdeNativeFoxLabelMap, ARRAYNUMBER( igdeNativeFoxLabelMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxLabel::igdeNativeFoxLabel(){ }

igdeNativeFoxLabel::igdeNativeFoxLabel( igdeLabel &owner, FXComposite *parent,
int layoutFlags, const igdeGuiTheme &guitheme ) :
FXLabel( parent, owner.GetText().GetString(), LabelIcon( owner ),
	layoutFlags | LabelFlags( owner ), 0, 0, 0, 0, 0, 0, 0, 0 ),
pOwner( &owner ),
pFont( LabelFont( owner, guitheme ) )
{
	setFont( (FXFont*)pFont->GetNativeFont() );
	
	setTipText( owner.GetDescription().GetString() );
	setHelpText( owner.GetDescription().GetString() );
}

igdeNativeFoxLabel::~igdeNativeFoxLabel(){
}

igdeNativeFoxLabel *igdeNativeFoxLabel::CreateNativeWidget( igdeLabel &owner ){
	if( ! owner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const parent = ( FXComposite* )owner.GetParent()->GetNativeContainer();
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxLabel( owner, parent,
		igdeUIFoxHelper::GetChildLayoutFlags( &owner ), *owner.GetGuiTheme() );
}

void igdeNativeFoxLabel::PostCreateNativeWidget(){
	FXComposite &parent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( parent.id() ){
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



FXIcon *igdeNativeFoxLabel::LabelIcon( const igdeLabel &owner ){
	if( owner.GetIcon() ){
		return ( FXIcon* )owner.GetIcon()->GetNativeIcon();
		
	}else{
		return NULL;
	}
}

int igdeNativeFoxLabel::LabelFlags( const igdeLabel &owner ){
	const int alignment = owner.GetAlignment();
	int flags = ICON_BEFORE_TEXT;
	
	if( ( alignment & igdeLabel::eaLeft ) == igdeLabel::eaLeft ){
		flags |= JUSTIFY_LEFT;
		
	}else if( ( alignment & igdeLabel::eaRight ) == igdeLabel::eaRight ){
		flags |= JUSTIFY_RIGHT;
		
	}else{
		flags |=  JUSTIFY_CENTER_X;
	}
	
	if( ( alignment & igdeLabel::eaTop ) == igdeLabel::eaTop ){
		flags |= JUSTIFY_TOP;
		
	}else if( ( alignment & igdeLabel::eaBottom ) == igdeLabel::eaBottom ){
		flags |= JUSTIFY_BOTTOM;
		
	}else{
		flags |=  JUSTIFY_CENTER_Y;
	}
	
	return flags;
}

igdeFont *igdeNativeFoxLabel::LabelFont( const igdeLabel &owner, const igdeGuiTheme &guitheme ){
	igdeFont::sConfiguration configuration;
	owner.GetEnvironment().GetApplicationFont( configuration );
	
	if( guitheme.HasProperty( igdeGuiThemePropertyNames::labelFontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::labelFontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::labelFontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::labelFontSize, 1.0f );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f );
	}
	
	return owner.GetEnvironment().GetSharedFont( configuration );
}

#endif
