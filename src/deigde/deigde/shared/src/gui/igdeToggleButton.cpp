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

#include "igdeToggleButton.h"
#include "igdeContainer.h"
#include "native/toolkit.h"
#include "igdeCommonDialogs.h"
#include "event/igdeAction.h"
#include "resources/igdeIcon.h"
#include "resources/igdeFont.h"
#include "resources/igdeFontReference.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Native Widget
//////////////////

namespace{

class cNativeWidget : public FXToggleButton{
	FXDECLARE( cNativeWidget )
	
protected:
	cNativeWidget();
	
public:
	enum eFoxIDs{
		ID_SELF = FXToggleButton::ID_LAST,
	};
	
private:
	igdeToggleButton *pOwner;
	igdeFontReference pFont;
	bool *pDeleted;
	
public:
	cNativeWidget( igdeToggleButton &owner, FXComposite *parent,
		int layoutFlags, const igdeGuiTheme &guitheme );
	virtual ~cNativeWidget();
	
	long onCommand( FXObject *sender, FXSelector selector, void *data );
	long onUpdate( FXObject *sender, FXSelector selector, void *data );
	virtual FXbool canFocus() const;
	
	static const char *ButtonText( const igdeToggleButton &owner );
	static FXIcon *ButtonIconOn( const igdeToggleButton &owner );
	static FXIcon *ButtonIconOff( const igdeToggleButton &owner );
	static int ButtonFlags( const igdeToggleButton &owner );
	static igdeFont *ButtonFont( const igdeToggleButton &owner, const igdeGuiTheme &guitheme );
	static int ButtonPadLeft( const igdeGuiTheme &guitheme );
	static int ButtonPadRight( const igdeGuiTheme &guitheme );
	static int ButtonPadTop( const igdeGuiTheme &guitheme );
	static int ButtonPadBottom( const igdeGuiTheme &guitheme );
};


FXDEFMAP( cNativeWidget ) cNativeWidgetMap[] = {
	FXMAPFUNC( SEL_COMMAND, cNativeWidget::ID_SELF, cNativeWidget::onCommand ),
	FXMAPFUNC( SEL_UPDATE, cNativeWidget::ID_SELF, cNativeWidget::onUpdate )
};


FXIMPLEMENT( cNativeWidget, FXToggleButton, cNativeWidgetMap, ARRAYNUMBER( cNativeWidgetMap ) )

cNativeWidget::cNativeWidget(){ }

cNativeWidget::cNativeWidget( igdeToggleButton &owner, FXComposite *parent,
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

cNativeWidget::~cNativeWidget(){
	if( pDeleted ){
		*pDeleted = true;
	}
}

long cNativeWidget::onCommand( FXObject *sender, FXSelector selector, void *data ){
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

long cNativeWidget::onUpdate( FXObject *sender, FXSelector selector, void *data ){
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

FXbool cNativeWidget::canFocus() const{
	if( pOwner->GetStyle() == igdeToggleButton::ebsToolBar ){
		return false;
		
	}else{
		return FXToggleButton::canFocus();
	}
}

const char *cNativeWidget::ButtonText( const igdeToggleButton &owner ){
	if( owner.GetStyle() == igdeToggleButton::ebsToolBar && owner.GetIcon() ){
		return "";
		
	}else{
		return owner.GetText();
	}
}

FXIcon *cNativeWidget::ButtonIconOn( const igdeToggleButton &owner ){
	return owner.GetIcon() ? ( FXIcon* )owner.GetIcon()->GetNativeIcon() : NULL;
}

FXIcon *cNativeWidget::ButtonIconOff( const igdeToggleButton &owner ){
	return owner.GetIcon() ? ( FXIcon* )owner.GetIcon()->GetNativeIcon() : NULL;
}

int cNativeWidget::ButtonFlags( const igdeToggleButton &owner ){
	int flags = TOGGLEBUTTON_KEEPSTATE;
	
	if( owner.GetStyle() == igdeToggleButton::ebsToolBar ){
		flags |= TOGGLEBUTTON_TOOLBAR | FRAME_RAISED;
		
	}else{
		flags |= FRAME_RAISED | JUSTIFY_NORMAL | ICON_BEFORE_TEXT;
		//flags |= TOGGLEBUTTON_NORMAL;
	}
	
	return flags;
}

igdeFont *cNativeWidget::ButtonFont( const igdeToggleButton &owner, const igdeGuiTheme &guitheme ){
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

int cNativeWidget::ButtonPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::buttonPaddingLeft, DEFAULT_PAD );
}

int cNativeWidget::ButtonPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::buttonPaddingRight, DEFAULT_PAD );
}

int cNativeWidget::ButtonPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::buttonPaddingTop, DEFAULT_PAD );
}

int cNativeWidget::ButtonPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::buttonPaddingBottom, DEFAULT_PAD );
}

}



// Class igdeToggleButton
/////////////////////

// Constructor, destructor
////////////////////////////

igdeToggleButton::igdeToggleButton( igdeEnvironment &environment, const char *text,
	igdeIcon *icon, eButtonStyle style ) :
igdeButton( environment, text, icon, style ),
pToggled( false ){
}

igdeToggleButton::igdeToggleButton( igdeEnvironment &environment, const char *text,
	const char *description, igdeIcon *icon, eButtonStyle style ) :
igdeButton( environment, text, description, icon, style ),
pToggled( false ){
}

igdeToggleButton::igdeToggleButton( igdeEnvironment &environment, igdeAction *action,
	eButtonStyle style ) :
igdeButton( environment, action, style ),
pToggled( false ){
}

igdeToggleButton::~igdeToggleButton(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeToggleButton::SetToggled( bool toggled ){
	if( pToggled == toggled ){
		return;
	}
	
	pToggled = toggled;
	OnToggledChanged();
}


void igdeToggleButton::OnAction(){
	SetToggled( ! pToggled );
	igdeButton::OnAction();
}

void igdeToggleButton::OnParameterChanged( igdeAction *action ){
	igdeButton::OnParameterChanged( action );
	SetToggled( action->GetSelected() );
}



void igdeToggleButton::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	if( ! GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const foxParent = ( FXComposite* )GetParent()->GetNativeContainer();
	if( ! foxParent ){
		DETHROW( deeInvalidParam );
	}
	
	int layoutFlags = igdeUIFoxHelper::GetChildLayoutFlags( this );
	cNativeWidget * const native = new cNativeWidget( *this, foxParent, layoutFlags, *GetGuiTheme() );
	SetNativeWidget( native );
	if( foxParent->id() ){
		native->create();
	}
}

void igdeToggleButton::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( cNativeWidget* )GetNativeWidget();
	DropNativeWidget();
}



void igdeToggleButton::OnStyleChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeWidget &native = *( ( cNativeWidget* )GetNativeWidget() );
	native.setText( cNativeWidget::ButtonText( *this ) );
	native.setIcon( cNativeWidget::ButtonIconOn( *this ) );
	native.setAltText( cNativeWidget::ButtonText( *this ) );
	native.setAltIcon( cNativeWidget::ButtonIconOff( *this ) );
	native.setToggleStyle( cNativeWidget::ButtonFlags( *this ) );
}

void igdeToggleButton::OnTextChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( cNativeWidget* )GetNativeWidget() )->setText( cNativeWidget::ButtonText( *this ) );
}

void igdeToggleButton::OnDescriptionChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeWidget &native = *( ( cNativeWidget* )GetNativeWidget() );
	native.setTipText( GetDescription().GetString() );
	native.setHelpText( GetDescription().GetString() );
	native.setAltTipText( GetDescription().GetString() );
	native.setAltHelpText( GetDescription().GetString() );
}

void igdeToggleButton::OnIconChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeWidget &native = *( ( cNativeWidget* )GetNativeWidget() );
	native.setIcon( cNativeWidget::ButtonIconOn( *this ) );
	native.setAltIcon( cNativeWidget::ButtonIconOff( *this ) );
}

void igdeToggleButton::OnEnabledChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeWidget &native = *( ( cNativeWidget* )GetNativeWidget() );
	if( GetEnabled() ){
		native.enable();
		
	}else{
		native.disable();
	}
}

void igdeToggleButton::OnDefaultChanged(){
	// only used during widget construction
}

void igdeToggleButton::OnToggledChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeWidget &native = *( ( cNativeWidget* )GetNativeWidget() );
	native.setState( pToggled );
}
