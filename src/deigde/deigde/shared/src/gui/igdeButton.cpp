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

#include "igdeButton.h"
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

class cNativeWidget : public FXButton{
	FXDECLARE( cNativeWidget )
	
protected:
	cNativeWidget();
	
public:
	enum eFoxIDs{
		ID_SELF = FXButton::ID_LAST,
	};
	
private:
	igdeButton *pOwner;
	igdeFontReference pFont;
	
public:
	cNativeWidget( igdeButton &owner, FXComposite *parent,
		int layoutFlags, const igdeGuiTheme &guitheme );
	virtual ~cNativeWidget();
	
	long onCommand( FXObject *sender, FXSelector selector, void *data );
	long onUpdate( FXObject *sender, FXSelector selector, void *data );
	virtual FXbool canFocus() const;
	
	virtual void Focus();
	
	static const char *ButtonText( const igdeButton &owner );
	static FXIcon *ButtonIcon( const igdeButton &owner );
	static int ButtonFlags( const igdeButton &owner );
	static igdeFont *ButtonFont( const igdeButton &owner, const igdeGuiTheme &guitheme );
	static int ButtonPadLeft( const igdeGuiTheme &guitheme );
	static int ButtonPadRight( const igdeGuiTheme &guitheme );
	static int ButtonPadTop( const igdeGuiTheme &guitheme );
	static int ButtonPadBottom( const igdeGuiTheme &guitheme );
};


FXDEFMAP( cNativeWidget ) cNativeWidgetMap[] = {
	FXMAPFUNC( SEL_COMMAND, cNativeWidget::ID_SELF, cNativeWidget::onCommand ),
	FXMAPFUNC( SEL_UPDATE, cNativeWidget::ID_SELF, cNativeWidget::onUpdate )
};


FXIMPLEMENT( cNativeWidget, FXButton, cNativeWidgetMap, ARRAYNUMBER( cNativeWidgetMap ) )

cNativeWidget::cNativeWidget(){ }

cNativeWidget::cNativeWidget( igdeButton &owner, FXComposite *parent,
int layoutFlags, const igdeGuiTheme &guitheme ) :
FXButton( parent, ButtonText( owner ), ButtonIcon( owner ), this, ID_SELF,
	layoutFlags | ButtonFlags( owner ), 0, 0, 0, 0,
	ButtonPadLeft( guitheme ), ButtonPadRight( guitheme ),
	ButtonPadTop( guitheme ), ButtonPadBottom( guitheme ) ),
pOwner( &owner ),
pFont( ButtonFont( owner, guitheme ) )
{
	setFont( (FXFont*)pFont->GetNativeFont() );
	
	if( ! owner.GetEnabled() ){
		disable();
	}
	setTipText( owner.GetDescription().GetString() );
	setHelpText( owner.GetDescription().GetString() );
}

cNativeWidget::~cNativeWidget(){
}

long cNativeWidget::onCommand( FXObject *sender, FXSelector selector, void *data ){
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
	if( pOwner->GetStyle() == igdeButton::ebsToolBar ){
		return false;
		
	}else{
		return FXButton::canFocus();
	}
}

void cNativeWidget::Focus(){
	setFocus();
}

const char *cNativeWidget::ButtonText( const igdeButton &owner ){
	if( owner.GetStyle() == igdeButton::ebsToolBar ){
		return "";
		
	}else{
		return owner.GetText();
	}
}

FXIcon *cNativeWidget::ButtonIcon( const igdeButton &owner ){
	if( owner.GetIcon() ){
		return ( FXIcon* )owner.GetIcon()->GetNativeIcon();
		
	}else{
		return NULL;
	}
}

int cNativeWidget::ButtonFlags( const igdeButton &owner ){
	int flags = 0;
	
	if( owner.GetStyle() == igdeButton::ebsToolBar ){
		flags = BUTTON_TOOLBAR | FRAME_RAISED;
		
	}else{
		flags = FRAME_RAISED | JUSTIFY_NORMAL | ICON_BEFORE_TEXT;
		//flags = BUTTON_NORMAL;
	}
	
	if( owner.GetDefault() ){
		flags |= BUTTON_DEFAULT | BUTTON_INITIAL;
	}
	
	return flags;
}

igdeFont *cNativeWidget::ButtonFont( const igdeButton &owner, const igdeGuiTheme &guitheme ){
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



// Class igdeButton
/////////////////////

// Constructor, destructor
////////////////////////////

igdeButton::igdeButton( igdeEnvironment &environment, const char *text,
	igdeIcon *icon, eButtonStyle style ) :
igdeWidget( environment ),
pStyle( style ),
pText( text ),
pIcon( icon ),
pEnabled( true ),
pDefault( false ){
}

igdeButton::igdeButton( igdeEnvironment &environment, const char *text,
	const char *description, igdeIcon *icon, eButtonStyle style ) :
igdeWidget( environment ),
pStyle( style ),
pText( text ),
pDescription( description ),
pIcon( icon ),
pEnabled( true ),
pDefault( false ){
}

igdeButton::igdeButton( igdeEnvironment &environment, igdeAction *action, eButtonStyle style ) :
igdeWidget( environment ),
pStyle( style ),
pEnabled( true ),
pDefault( false )
{
	SetAction( action );
}

igdeButton::~igdeButton(){
	DestroyNativeWidget();
	SetAction( NULL );
}



// Management
///////////////

void igdeButton::SetStyle( eButtonStyle style ){
	if( pStyle == style ){
		return;
	}
	
	pStyle = style;
	OnStyleChanged();
}

void igdeButton::SetText( const char *text ){
	if( pText == text ){
		return;
	}
	
	pText = text;
	OnTextChanged();
}

void igdeButton::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeButton::SetIcon( igdeIcon *icon ){
	if( pIcon == icon ){
		return;
	}
	
	pIcon = icon;
	OnIconChanged();
}

void igdeButton::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeButton::SetDefault( bool isdefault ){
	if( pDefault == isdefault ){
		return;
	}
	
	pDefault = isdefault;
	OnDefaultChanged();
}

void igdeButton::SetAction( igdeAction *action ){
	if( pAction == action ){
		return;
	}
	
	if( pAction ){
		pAction->RemoveListener( this );
	}
	
	pAction = action;
	
	if( action ){
		action->AddListener( this );
		OnParameterChanged( action );
	}
}

void igdeButton::Focus(){
	if( GetNativeWidget() ){
		( ( cNativeWidget* )GetNativeWidget() )->Focus();
	}
}


void igdeButton::OnAction(){
	if( pAction ){
		pAction->OnAction();
	}
}

void igdeButton::OnParameterChanged( igdeAction *action ){
	SetText( action->GetText() );
	SetDescription( action->GetDescription() );
	SetIcon( action->GetIcon() );
	SetEnabled( action->GetEnabled() );
	SetDefault( action->GetDefault() );
}

void igdeButton::OnDestroyed( igdeAction *action ){
	GetLogger()->LogWarnFormat( "IGDE", "igdeButton::OnDestroyed: "
		"Action(%s) destroyed while still listening on it", action->GetText().GetString() );
	pAction = NULL;
}



void igdeButton::CreateNativeWidget(){
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
	cNativeWidget * const foxWidget = new cNativeWidget(
		*this, foxParent, layoutFlags, *GetGuiTheme() );
	SetNativeWidget( foxWidget );
	if( foxParent->id() ){
		foxWidget->create();
	}
}

void igdeButton::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( cNativeWidget* )GetNativeWidget();
	DropNativeWidget();
}



void igdeButton::OnStyleChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	FXButton &button = *( ( FXButton* )GetNativeWidget() );
	button.setText( cNativeWidget::ButtonText( *this ) );
	button.setIcon( cNativeWidget::ButtonIcon( *this ) );
	button.setButtonStyle( cNativeWidget::ButtonFlags( *this ) );
}

void igdeButton::OnTextChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( FXButton* )GetNativeWidget() )->setText( cNativeWidget::ButtonText( *this ) );
}

void igdeButton::OnDescriptionChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	FXButton &button = *( ( FXButton* )GetNativeWidget() );
	button.setTipText( pDescription.GetString() );
	button.setHelpText( pDescription.GetString() );
}

void igdeButton::OnIconChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( FXButton* )GetNativeWidget() )->setIcon( cNativeWidget::ButtonIcon( *this ) );
}

void igdeButton::OnEnabledChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	if( pEnabled ){
		( ( FXButton* )GetNativeWidget() )->enable();
		
	}else{
		( ( FXButton* )GetNativeWidget() )->disable();
	}
}

void igdeButton::OnDefaultChanged(){
	// only used during widget construction
}
