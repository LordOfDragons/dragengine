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

#include "igdeNativeFoxButton.h"
#include "foxIcons.h"
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



// Events
///////////

FXDEFMAP( igdeNativeFoxButton ) igdeNativeFoxButtonMap[] = {
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxButton::ID_SELF, igdeNativeFoxButton::onCommand ),
	FXMAPFUNC( SEL_UPDATE, igdeNativeFoxButton::ID_SELF, igdeNativeFoxButton::onUpdate )
};



// Class igdeNativeFoxButton
//////////////////////////////

FXIMPLEMENT( igdeNativeFoxButton, FXButton, igdeNativeFoxButtonMap, ARRAYNUMBER( igdeNativeFoxButtonMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxButton::igdeNativeFoxButton(){ }

igdeNativeFoxButton::igdeNativeFoxButton( igdeButton &powner, FXComposite *pparent,
int layoutFlags, const igdeGuiTheme &guitheme ) :
FXButton( pparent, ButtonText( powner ), ButtonIcon( powner ), this, ID_SELF,
	layoutFlags | ButtonFlags( powner ), 0, 0, 0, 0,
	ButtonPadLeft( guitheme ), ButtonPadRight( guitheme ),
	ButtonPadTop( guitheme ), ButtonPadBottom( guitheme ) ),
pOwner( &powner ),
pFont( ButtonFont( powner, guitheme ) )
{
	setFont( (FXFont*)pFont->GetNativeFont() );
	
	if( ! powner.GetEnabled() ){
		disable();
	}
	setTipText( powner.GetDescription().GetString() );
	setHelpText( powner.GetDescription().GetString() );
}

igdeNativeFoxButton::~igdeNativeFoxButton(){
}

igdeNativeFoxButton *igdeNativeFoxButton::CreateNativeWidget( igdeButton &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const pparent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! pparent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxButton( powner, pparent,
		igdeUIFoxHelper::GetChildLayoutFlags( &powner ), *powner.GetGuiTheme() );
}

void igdeNativeFoxButton::PostCreateNativeWidget(){
	FXComposite &pparent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( pparent.id() ){
		create();
	}
}

void igdeNativeFoxButton::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

FXbool igdeNativeFoxButton::canFocus() const{
	if( pOwner->GetStyle() == igdeButton::ebsToolBar ){
		return false;
		
	}else{
		return FXButton::canFocus();
	}
}

void igdeNativeFoxButton::Focus(){
	setFocus();
}

void igdeNativeFoxButton::UpdateStyle(){
	setText( igdeNativeFoxButton::ButtonText( *pOwner ) );
	setIcon( igdeNativeFoxButton::ButtonIcon( *pOwner ) );
	setButtonStyle( igdeNativeFoxButton::ButtonFlags( *pOwner ) );
}

void igdeNativeFoxButton::UpdateText(){
	setText( igdeNativeFoxButton::ButtonText( *pOwner ) );
}

void igdeNativeFoxButton::UpdateDescription(){
	const char * const description = pOwner->GetDescription();
	setTipText( description );
	setHelpText( description );
}

void igdeNativeFoxButton::UpdateIcon(){
	setIcon( igdeNativeFoxButton::ButtonIcon( *pOwner ) );
}

void igdeNativeFoxButton::UpdateEnabled(){
	if( pOwner->GetEnabled() ){
		enable();
		
	}else{
		disable();
	}
}



const char *igdeNativeFoxButton::ButtonText( const igdeButton &powner ){
	if( powner.GetStyle() == igdeButton::ebsToolBar ){
		return "";
		
	}else{
		return powner.GetText();
	}
}

FXIcon *igdeNativeFoxButton::ButtonIcon( const igdeButton &powner ){
	if( powner.GetIcon() ){
		return ( FXIcon* ) powner.GetIcon()->GetNativeIcon();
		
	}else{
		return NULL;
	}
}

int igdeNativeFoxButton::ButtonFlags( const igdeButton &powner ){
	int fflags = 0;
	
	if( powner.GetStyle() == igdeButton::ebsToolBar ){
		fflags = BUTTON_TOOLBAR | FRAME_RAISED;
		
	}else{
		fflags = FRAME_RAISED | JUSTIFY_NORMAL | ICON_BEFORE_TEXT;
		//flags = BUTTON_NORMAL;
	}
	
	if( powner.GetDefault() ){
		fflags |= BUTTON_DEFAULT | BUTTON_INITIAL;
	}
	
	return fflags;
}

igdeFont *igdeNativeFoxButton::ButtonFont( const igdeButton &powner, const igdeGuiTheme &guitheme ){
	igdeFont::sConfiguration configuration;
	powner.GetEnvironment().GetApplicationFont( configuration );
	
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
	
	return powner.GetEnvironment().GetSharedFont( configuration );
}

int igdeNativeFoxButton::ButtonPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::buttonPaddingLeft, DEFAULT_PAD );
}

int igdeNativeFoxButton::ButtonPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::buttonPaddingRight, DEFAULT_PAD );
}

int igdeNativeFoxButton::ButtonPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::buttonPaddingTop, DEFAULT_PAD );
}

int igdeNativeFoxButton::ButtonPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::buttonPaddingBottom, DEFAULT_PAD );
}



// Events
///////////

long igdeNativeFoxButton::onCommand( FXObject*, FXSelector, void* ){
	if( ! pOwner->GetEnabled() ){
		return 0;
	}
	
	try{
		pOwner->OnAction();
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
		return 0;
	}
	
	return 1;
}

long igdeNativeFoxButton::onUpdate( FXObject*, FXSelector, void* ){
	igdeAction * const action = pOwner->GetAction();
	if( ! action ){
		return 0;
	}
	
	try{
		action->Update();
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
	}
	
	return 0;
}

#endif
