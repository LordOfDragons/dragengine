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

#include "igdeCheckBox.h"
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

class cNativeIgdeCheckBox : public FXCheckButton{
	FXDECLARE( cNativeIgdeCheckBox )
	
protected:
	cNativeIgdeCheckBox();
	
public:
	enum eFoxIDs{
		ID_SELF = FXCheckButton::ID_LAST,
	};
	
private:
	igdeCheckBox *pOwner;
	igdeFontReference pFont;
	
public:
	cNativeIgdeCheckBox( igdeCheckBox &owner, FXComposite *parent,
		int layoutFlags, const igdeGuiTheme &guitheme );
	virtual ~cNativeIgdeCheckBox();
	
	long onCommand( FXObject *sender, FXSelector selector, void *data );
	long onUpdate( FXObject *sender, FXSelector selector, void *data );
	virtual FXbool canFocus() const;
	
	static const char *CheckBoxText( const igdeCheckBox &owner );
	static FXIcon *CheckBoxIcon( const igdeCheckBox &owner );
	static int CheckBoxFlags( const igdeCheckBox &owner );
	static igdeFont *CheckBoxFont( const igdeCheckBox &owner, const igdeGuiTheme &guitheme );
	static int CheckBoxPadLeft( const igdeGuiTheme &guitheme );
	static int CheckBoxPadRight( const igdeGuiTheme &guitheme );
	static int CheckBoxPadTop( const igdeGuiTheme &guitheme );
	static int CheckBoxPadBottom( const igdeGuiTheme &guitheme );
};


FXDEFMAP( cNativeIgdeCheckBox ) cNativeIgdeCheckBoxMap[] = {
	FXMAPFUNC( SEL_COMMAND, cNativeIgdeCheckBox::ID_SELF, cNativeIgdeCheckBox::onCommand ),
	FXMAPFUNC( SEL_UPDATE, cNativeIgdeCheckBox::ID_SELF, cNativeIgdeCheckBox::onUpdate )
};


FXIMPLEMENT( cNativeIgdeCheckBox, FXCheckButton, cNativeIgdeCheckBoxMap, ARRAYNUMBER( cNativeIgdeCheckBoxMap ) )

cNativeIgdeCheckBox::cNativeIgdeCheckBox(){ }

cNativeIgdeCheckBox::cNativeIgdeCheckBox( igdeCheckBox &owner, FXComposite *parent,
int layoutFlags, const igdeGuiTheme &guitheme ) :
FXCheckButton( parent, CheckBoxText( owner ), this, ID_SELF, layoutFlags | CheckBoxFlags( owner ),
	0, 0, 0, 0,
	CheckBoxPadLeft( guitheme ), CheckBoxPadRight( guitheme ),
	CheckBoxPadTop( guitheme ), CheckBoxPadBottom( guitheme ) ),
pOwner( &owner ),
pFont( CheckBoxFont( owner, guitheme ) )
{
	setFont( (FXFont*)pFont->GetNativeFont() );
	
	if( ! owner.GetEnabled() ){
		disable();
	}
	setCheck( owner.GetChecked() );
	setIcon( CheckBoxIcon( owner ) );
	setTipText( owner.GetDescription().GetString() );
	setHelpText( owner.GetDescription().GetString() );
}

cNativeIgdeCheckBox::~cNativeIgdeCheckBox(){
}

long cNativeIgdeCheckBox::onCommand( FXObject *sender, FXSelector selector, void *data ){
	if( ! pOwner->GetEnabled() ){
		return 0;
	}
	
	pOwner->SetChecked( getCheck() );
	
	try{
		pOwner->OnAction();
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
		return 0;
	}
	
	return 1;
}

long cNativeIgdeCheckBox::onUpdate( FXObject *sender, FXSelector selector, void *data ){
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

FXbool cNativeIgdeCheckBox::canFocus() const{
	if( pOwner->GetStyle() == igdeCheckBox::ebsToolBar ){
		return false;
		
	}else{
		return FXCheckButton::canFocus();
	}
}

const char *cNativeIgdeCheckBox::CheckBoxText( const igdeCheckBox &owner ){
	return owner.GetText();
}

FXIcon *cNativeIgdeCheckBox::CheckBoxIcon( const igdeCheckBox &owner ){
	if( owner.GetIcon() ){
		return ( FXIcon* )owner.GetIcon()->GetNativeIcon();
		
	}else{
		return NULL;
	}
}

int cNativeIgdeCheckBox::CheckBoxFlags( const igdeCheckBox &owner ){
	if( owner.GetStyle() == igdeCheckBox::ebsToolBar ){
		return JUSTIFY_LEFT | ICON_BEFORE_TEXT | FRAME_RAISED;
		
	}else{
		return JUSTIFY_LEFT | ICON_BEFORE_TEXT;
	}
}

igdeFont *cNativeIgdeCheckBox::CheckBoxFont( const igdeCheckBox &owner, const igdeGuiTheme &guitheme ){
	igdeFont::sConfiguration configuration;
	owner.GetEnvironment().GetApplicationFont( configuration );
	
	if( guitheme.HasProperty( igdeGuiThemePropertyNames::checkBoxFontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::checkBoxFontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::checkBoxFontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::checkBoxFontSize, 1.0f );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f );
	}
	
	return owner.GetEnvironment().GetSharedFont( configuration );
}

int cNativeIgdeCheckBox::CheckBoxPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::checkBoxPaddingLeft, DEFAULT_PAD );
}

int cNativeIgdeCheckBox::CheckBoxPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::checkBoxPaddingRight, DEFAULT_PAD );
}

int cNativeIgdeCheckBox::CheckBoxPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::checkBoxPaddingTop, DEFAULT_PAD );
}

int cNativeIgdeCheckBox::CheckBoxPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::checkBoxPaddingBottom, DEFAULT_PAD );
}



// Class igdeCheckBox
/////////////////////

// Constructor, destructor
////////////////////////////

igdeCheckBox::igdeCheckBox( igdeEnvironment &environment, const char *text,
	igdeIcon *icon, eButtonStyle style ) :
igdeButton( environment, text, icon, style ),
pChecked( false ){
}

igdeCheckBox::igdeCheckBox( igdeEnvironment &environment, const char *text,
	const char *description, igdeIcon *icon, eButtonStyle style ) :
igdeButton( environment, text, description, icon, style ),
pChecked( false ){
}

igdeCheckBox::igdeCheckBox( igdeEnvironment &environment, igdeAction *action, eButtonStyle style ) :
igdeButton( environment, action, style ),
pChecked( false )
{
	SetAction( action );
}

igdeCheckBox::~igdeCheckBox(){
}



// Management
///////////////

void igdeCheckBox::SetChecked( bool checked ){
	if( pChecked == checked ){
		return;
	}
	
	pChecked = checked;
	OnCheckedChanged();
}



void igdeCheckBox::OnParameterChanged( igdeAction *action ){
	igdeButton::OnParameterChanged( action );
	
	SetChecked( action->GetSelected() );
}



void igdeCheckBox::CreateNativeWidget(){
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
	cNativeIgdeCheckBox * const foxWidget = new cNativeIgdeCheckBox(
		*this, foxParent, layoutFlags, *GetGuiTheme() );
	SetNativeWidget( foxWidget );
	if( foxParent->id() ){
		foxWidget->create();
	}
}

void igdeCheckBox::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( cNativeIgdeCheckBox* )GetNativeWidget();
	DropNativeWidget();
}



void igdeCheckBox::OnCheckedChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeCheckBox &checkbox = *( ( cNativeIgdeCheckBox* )GetNativeWidget() );
	if( pChecked != checkbox.getCheck() ){
		checkbox.setCheck( pChecked );
	}
}

void igdeCheckBox::OnStyleChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeCheckBox &checkbox = *( ( cNativeIgdeCheckBox* )GetNativeWidget() );
	checkbox.setText( cNativeIgdeCheckBox::CheckBoxText( *this ) );
	checkbox.setIcon( cNativeIgdeCheckBox::CheckBoxIcon( *this ) );
	checkbox.setCheckButtonStyle( cNativeIgdeCheckBox::CheckBoxFlags( *this ) );
}

void igdeCheckBox::OnTextChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeCheckBox &checkbox = *( ( cNativeIgdeCheckBox* )GetNativeWidget() );
	checkbox.setText( cNativeIgdeCheckBox::CheckBoxText( *this ) );
}

void igdeCheckBox::OnDescriptionChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeCheckBox &checkbox = *( ( cNativeIgdeCheckBox* )GetNativeWidget() );
	checkbox.setTipText( GetDescription().GetString() );
	checkbox.setHelpText( GetDescription().GetString() );
}

void igdeCheckBox::OnIconChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeCheckBox &checkbox = *( ( cNativeIgdeCheckBox* )GetNativeWidget() );
	checkbox.setIcon( cNativeIgdeCheckBox::CheckBoxIcon( *this ) );
}

void igdeCheckBox::OnEnabledChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeCheckBox &checkbox = *( ( cNativeIgdeCheckBox* )GetNativeWidget() );
	
	if( GetEnabled() ){
		checkbox.enable();
		
	}else{
		checkbox.disable();
	}
}
