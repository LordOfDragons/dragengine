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

#include "igdeNativeFoxToggleButton.h"
#include "foxIcons.h"
#include "../../igdeToggleButton.h"
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

FXDEFMAP( igdeNativeFoxToggleButton ) igdeNativeFoxToggleButtonMap[] = {
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxToggleButton::ID_SELF, igdeNativeFoxToggleButton::onCommand ),
	FXMAPFUNC( SEL_UPDATE, igdeNativeFoxToggleButton::ID_SELF, igdeNativeFoxToggleButton::onUpdate )
};



// Class igdeNativeFoxToggleButton
////////////////////////////////////

FXIMPLEMENT( igdeNativeFoxToggleButton, FXToggleButton, igdeNativeFoxToggleButtonMap, ARRAYNUMBER( igdeNativeFoxToggleButtonMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxToggleButton::igdeNativeFoxToggleButton(){ }

igdeNativeFoxToggleButton::igdeNativeFoxToggleButton( igdeToggleButton &owner, FXComposite *parent,
int layoutFlags, const igdeGuiTheme &guitheme ) :
FXToggleButton( parent, ButtonText( owner ), ButtonText( owner ),
	ButtonIconOn( owner ), ButtonIconOff( owner ), this, ID_SELF,
	layoutFlags | ButtonFlags( owner ), 0, 0, 0, 0,
	ButtonPadLeft( guitheme ), ButtonPadRight( guitheme ),
	ButtonPadTop( guitheme ), ButtonPadBottom( guitheme ) ),
pOwner( &owner ),
pFont( ButtonFont( owner, guitheme ) ),
pDeleted( NULL )
{
	setFont( (FXFont*)pFont->GetNativeFont() );
	setState( owner.GetToggled() );
	
	if( ! owner.GetEnabled() ){
		disable();
	}
	setTipText( owner.GetDescription().GetString() );
	setHelpText( owner.GetDescription().GetString() );
}

igdeNativeFoxToggleButton::~igdeNativeFoxToggleButton(){
	if( pDeleted ){
		*pDeleted = true;
	}
}

igdeNativeFoxToggleButton *igdeNativeFoxToggleButton::CreateNativeWidget( igdeToggleButton &owner ){
	if( ! owner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const parent = ( FXComposite* )owner.GetParent()->GetNativeContainer();
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxToggleButton( owner, parent,
		igdeUIFoxHelper::GetChildLayoutFlags( &owner ), *owner.GetGuiTheme() );
}

void igdeNativeFoxToggleButton::PostCreateNativeWidget(){
	FXComposite &parent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( parent.id() ){
		create();
	}
}

void igdeNativeFoxToggleButton::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

FXbool igdeNativeFoxToggleButton::canFocus() const{
	if( pOwner->GetStyle() == igdeToggleButton::ebsToolBar ){
		return false;
		
	}else{
		return FXToggleButton::canFocus();
	}
}

void igdeNativeFoxToggleButton::UpdateStyle(){
	setText( igdeNativeFoxToggleButton::ButtonText( *pOwner ) );
	setIcon( igdeNativeFoxToggleButton::ButtonIconOn( *pOwner ) );
	setAltText( igdeNativeFoxToggleButton::ButtonText( *pOwner ) );
	setAltIcon( igdeNativeFoxToggleButton::ButtonIconOff( *pOwner ) );
	setToggleStyle( igdeNativeFoxToggleButton::ButtonFlags( *pOwner ) );
}

void igdeNativeFoxToggleButton::UpdateText(){
	setText( igdeNativeFoxToggleButton::ButtonText( *pOwner ) );
}

void igdeNativeFoxToggleButton::UpdateDescription(){
	const char * const description = pOwner->GetDescription();
	setTipText( description );
	setHelpText( description );
	setAltTipText( description );
	setAltHelpText( description );
}

void igdeNativeFoxToggleButton::UpdateIcons(){
	setIcon( igdeNativeFoxToggleButton::ButtonIconOn( *pOwner ) );
	setAltIcon( igdeNativeFoxToggleButton::ButtonIconOff( *pOwner ) );
}

void igdeNativeFoxToggleButton::UpdateEnabled(){
	if( pOwner->GetEnabled() ){
		enable();
		
	}else{
		disable();
	}
}

void igdeNativeFoxToggleButton::UpdateToggled(){
	setState( pOwner->GetToggled() );
}



const char *igdeNativeFoxToggleButton::ButtonText( const igdeToggleButton &owner ){
	if( owner.GetStyle() == igdeToggleButton::ebsToolBar && owner.GetIcon() ){
		return "";
		
	}else{
		return owner.GetText();
	}
}

FXIcon *igdeNativeFoxToggleButton::ButtonIconOn( const igdeToggleButton &owner ){
	return owner.GetIcon() ? ( FXIcon* )owner.GetIcon()->GetNativeIcon() : NULL;
}

FXIcon *igdeNativeFoxToggleButton::ButtonIconOff( const igdeToggleButton &owner ){
	return owner.GetIcon() ? ( FXIcon* )owner.GetIcon()->GetNativeIcon() : NULL;
}

int igdeNativeFoxToggleButton::ButtonFlags( const igdeToggleButton &owner ){
	int flags = TOGGLEBUTTON_KEEPSTATE;
	
	if( owner.GetStyle() == igdeToggleButton::ebsToolBar ){
		flags |= TOGGLEBUTTON_TOOLBAR | FRAME_RAISED;
		
	}else{
		flags |= FRAME_RAISED | JUSTIFY_NORMAL | ICON_BEFORE_TEXT;
		//flags |= TOGGLEBUTTON_NORMAL;
	}
	
	return flags;
}

igdeFont *igdeNativeFoxToggleButton::ButtonFont( const igdeToggleButton &owner, const igdeGuiTheme &guitheme ){
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

int igdeNativeFoxToggleButton::ButtonPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::buttonPaddingLeft, DEFAULT_PAD );
}

int igdeNativeFoxToggleButton::ButtonPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::buttonPaddingRight, DEFAULT_PAD );
}

int igdeNativeFoxToggleButton::ButtonPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::buttonPaddingTop, DEFAULT_PAD );
}

int igdeNativeFoxToggleButton::ButtonPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::buttonPaddingBottom, DEFAULT_PAD );
}



// Events
///////////

long igdeNativeFoxToggleButton::onCommand( FXObject *sender, FXSelector selector, void *data ){
	if( ! pOwner->GetEnabled() ){
		return 0;
	}
	
	bool deleted = false;
	pDeleted = &deleted;
	
	try{
		pOwner->OnAction();
		
	}catch( const deException &e ){
		pDeleted = NULL;
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
		return 0;
	}
	
	pDeleted = NULL;
	if( deleted ){
		return 1;
	}
	
	// it is possible the toggle states divergates due to actions done by OnAction().
	// multiple parties are involved with each having their own state:
	// - FXToggleButton with getState/setState
	// - igdeToggleButton with GetToggled
	// - igdeAction with GetSelected()
	// we have to ensure they are all in sync again
	igdeAction * const action = pOwner->GetAction();
	if( ! action ){
		return 1;
	}
	
	try{
		action->Update();
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
	}
	
	pOwner->OnParameterChanged( action );
	
	return 1;
}

long igdeNativeFoxToggleButton::onUpdate( FXObject *sender, FXSelector selector, void *data ){
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
