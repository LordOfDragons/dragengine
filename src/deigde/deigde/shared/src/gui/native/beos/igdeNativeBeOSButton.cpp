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

#ifdef IGDE_TOOLKIT_BEOS

#include "igdeNativeBeOSButton.h"
#include "../../igdeButton.h"
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



// Class igdeNativeBeOSButton
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeBeOSButton::igdeNativeBeOSButton( igdeButton &owner, const igdeGuiTheme &guitheme ) :
BButton( ButtonText( owner ), new BMessage( eeClicked ) ),
pOwner( owner ),
pFont( ButtonFont( owner, guitheme ) )
{
	SetTarget( this );
	
	// padding
	// ButtonPadLeft( guitheme ), ButtonPadRight( guitheme ),
	// ButtonPadTop( guitheme ), ButtonPadBottom( guitheme ) )
	SetIcon( ButtonIcon( owner ) );
	SetFlat( owner.GetStyle() == igdeButton::ebsToolBar );
	SetFont( ( BFont* )pFont->GetNativeFont() );
	SetEnabled( owner.GetEnabled() );
	SetToolTip( owner.GetDescription() );
	MakeDefault( owner.GetDefault()  );
}

igdeNativeBeOSButton::~igdeNativeBeOSButton(){
}

igdeNativeBeOSButton *igdeNativeBeOSButton::CreateNativeWidget( igdeButton &owner ){
	igdeNativeBeOSButton * const native = new igdeNativeBeOSButton( owner, *owner.GetGuiTheme() );
	igdeUIBeOSHelper::AddView( native, owner.GetParent() );
	return native;
}

void igdeNativeBeOSButton::PostCreateNativeWidget(){
}

void igdeNativeBeOSButton::DestroyNativeWidget(){
	igdeUIBeOSHelper::DestroyView( this );
}



// Management
///////////////

void igdeNativeBeOSButton::Focus(){
	MakeFocus();
}

void igdeNativeBeOSButton::UpdateStyle(){
	SetLabel( igdeNativeBeOSButton::ButtonText( pOwner ) );
	SetIcon( igdeNativeBeOSButton::ButtonIcon( pOwner ) );
	SetFlat( pOwner.GetStyle() == igdeButton::ebsToolBar );
}

void igdeNativeBeOSButton::UpdateText(){
	SetLabel( igdeNativeBeOSButton::ButtonText( pOwner ) );
}

void igdeNativeBeOSButton::UpdateDescription(){
	SetToolTip( pOwner.GetDescription() );
}

void igdeNativeBeOSButton::UpdateIcon(){
	SetIcon( igdeNativeBeOSButton::ButtonIcon( pOwner ) );
}

void igdeNativeBeOSButton::UpdateEnabled(){
	SetEnabled( pOwner.GetEnabled() );
}



void igdeNativeBeOSButton::MessageReceived( BMessage *message ){
	if( message->what == eeClicked ){
		if( ! pOwner.GetEnabled() ){
			return;
		}
		
		try{
			pOwner.OnAction();
			
		}catch( const deException &e ){
			pOwner.GetLogger()->LogException( "IGDE", e );
			igdeCommonDialogs::Exception( &pOwner, e );
		}
		
	}else{
		BButton::MessageReceived( message );
	}
}

void igdeNativeBeOSButton::UpdateAction(){
	igdeAction * const action = pOwner.GetAction();
	if( ! action ){
		return;
	}
	
	try{
		action->Update();
		
	}catch( const deException &e ){
		pOwner.GetLogger()->LogException( "IGDE", e );
	}
}



const char *igdeNativeBeOSButton::ButtonText( const igdeButton &owner ){
	if( owner.GetStyle() == igdeButton::ebsToolBar ){
		return "";
		
	}else{
		return owner.GetText();
	}
}

BBitmap *igdeNativeBeOSButton::ButtonIcon( const igdeButton &owner ){
	if( owner.GetIcon() ){
		return ( BBitmap* )owner.GetIcon()->GetNativeIcon();
		
	}else{
		return NULL;
	}
}

igdeFont *igdeNativeBeOSButton::ButtonFont( const igdeButton &owner, const igdeGuiTheme &guitheme ){
	igdeFont::sConfiguration configuration;
	owner.GetEnvironment().GetApplicationFont( configuration );
	
	if( guitheme.HasProperty( igdeGuiThemePropertyNames::buttonFontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::buttonFontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::buttonFontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::buttonFontSize, 1.0f );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f );
	}
	
	return owner.GetEnvironment().GetSharedFont( configuration );
}

int igdeNativeBeOSButton::ButtonPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::buttonPaddingLeft, 0 );
}

int igdeNativeBeOSButton::ButtonPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::buttonPaddingRight, 0 );
}

int igdeNativeBeOSButton::ButtonPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::buttonPaddingTop, 0 );
}

int igdeNativeBeOSButton::ButtonPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::buttonPaddingBottom, 0 );
}

#endif
