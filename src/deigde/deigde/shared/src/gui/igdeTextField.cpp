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

#include "igdeTextField.h"
#include "igdeContainer.h"
#include "native/toolkit.h"
#include "igdeCommonDialogs.h"
#include "event/igdeTextFieldListener.h"
#include "resources/igdeFont.h"
#include "resources/igdeFontReference.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Native Widget
//////////////////

class cNativeIgdeTextField : public FXTextField{
	FXDECLARE( cNativeIgdeTextField )
	
protected:
	cNativeIgdeTextField();
	
public:
	enum eFoxIDs{
		ID_SELF = FXTextField::ID_LAST,
	};
	
private:
	igdeTextField *pOwner;
	igdeFontReference pFont;
	FXColor pOrgBackColor;
	FXColor pInvalidBackColor;
	bool pPreventKeyPressHook;
	
public:
	cNativeIgdeTextField( igdeTextField &owner, FXComposite *parent,
		int layoutFlags, const igdeGuiTheme &guitheme );
	virtual ~cNativeIgdeTextField();
	
	long onCommand( FXObject *sender, FXSelector selector, void *data );
	long onChanged( FXObject *sender, FXSelector selector, void *data );
	long onKeyPress( FXObject *sender, FXSelector selector, void *data );
	
	virtual void Focus();
	virtual void OnInvalidValueChanged();
	virtual void UpdateText();
	virtual void UpdateEnabled();
	virtual void UpdateDescription();
	virtual void UpdateEditable();
	
	static int TextFieldFlags( const igdeTextField &owner );
	static igdeFont *TextFieldFont( const igdeTextField &owner, const igdeGuiTheme &guitheme );
	static int TextFieldPadLeft( const igdeGuiTheme &guitheme );
	static int TextFieldPadRight( const igdeGuiTheme &guitheme );
	static int TextFieldPadTop( const igdeGuiTheme &guitheme );
	static int TextFieldPadBottom( const igdeGuiTheme &guitheme );
};


FXDEFMAP( cNativeIgdeTextField ) cNativeIgdeTextFieldMap[] = {
	FXMAPFUNC( SEL_COMMAND, cNativeIgdeTextField::ID_SELF, cNativeIgdeTextField::onCommand ),
	FXMAPFUNC( SEL_CHANGED, cNativeIgdeTextField::ID_SELF, cNativeIgdeTextField::onChanged ),
	FXMAPFUNC( SEL_KEYPRESS, cNativeIgdeTextField::ID_SELF, cNativeIgdeTextField::onKeyPress )
};


FXIMPLEMENT( cNativeIgdeTextField, FXTextField, cNativeIgdeTextFieldMap, ARRAYNUMBER( cNativeIgdeTextFieldMap ) )

cNativeIgdeTextField::cNativeIgdeTextField(){ }

cNativeIgdeTextField::cNativeIgdeTextField( igdeTextField &owner, FXComposite *parent,
int layoutFlags, const igdeGuiTheme &guitheme ) :
FXTextField( parent, owner.GetColumns(), this, ID_SELF, layoutFlags | TextFieldFlags( owner ),
	0, 0, 0, 0,
	TextFieldPadLeft( guitheme ), TextFieldPadRight( guitheme ),
	TextFieldPadTop( guitheme ), TextFieldPadBottom( guitheme ) ),
pOwner( &owner ),
pFont( TextFieldFont( owner, guitheme ) ),
pOrgBackColor( getBackColor() ),
pInvalidBackColor( igdeUIFoxHelper::BlendColor( pOrgBackColor, FXRGB( 255, 0, 0 ), 0.25f ) ),
pPreventKeyPressHook( false )
{
	setFont( (FXFont*)pFont->GetNativeFont() );
	
	UpdateEditable();
	UpdateEnabled();
	UpdateText();
	UpdateDescription();
}

cNativeIgdeTextField::~cNativeIgdeTextField(){
}

void cNativeIgdeTextField::OnInvalidValueChanged(){
	setBackColor( pOwner->GetInvalidValue() ? pInvalidBackColor : pOrgBackColor );
}

void cNativeIgdeTextField::UpdateText(){
	if( pOwner->GetText() != getText().text() ){
		setText( pOwner->GetText().GetString() );
	}
}

void cNativeIgdeTextField::UpdateEnabled(){
	if( pOwner->GetEnabled() ){
		enable();
		
	}else{
		disable();
	}
}

void cNativeIgdeTextField::UpdateDescription(){
	setTipText( pOwner->GetDescription().GetString() );
	setHelpText( pOwner->GetDescription().GetString() );
}

void cNativeIgdeTextField::UpdateEditable(){
	setEditable( pOwner->GetEditable() );
}

long cNativeIgdeTextField::onCommand( FXObject*, FXSelector, void* ){
	if( ! pOwner->GetEnabled() ){
		return 0;
	}
	
	try{
		pOwner->SetText( getText().text(), false, true );
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
		return 0;
	}
	
	return 1;
}

long cNativeIgdeTextField::onChanged( FXObject*, FXSelector, void* ){
	if( ! pOwner->GetEnabled() ){
		return 0;
	}
	
	try{
		pOwner->SetText( getText().text(), true );
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
		return 0;
	}
	
	return 1;
}

long cNativeIgdeTextField::onKeyPress( FXObject *sender, FXSelector selector, void *data ){
	// returning 1 from this function stops FXTextField from processing
	if( ! pOwner->GetEnabled() || pPreventKeyPressHook ){
		return 0;
	}
	
	const FXEvent &event = *( ( FXEvent* )data );
	if( event.code != KEY_Return && event.code != KEY_KP_Enter ){
		return 0;
	}
	
	// we do it this way instead of a silent hook-method because we want the super-class
	// implementation to call SEL_COMMAND before we send our NotifyEnterKey. do avoid a
	// dead-loop in this case pPreventKeyPressHook is used.
	pPreventKeyPressHook = true;
	const int result = onKeyPress( sender, selector, data );
	pPreventKeyPressHook = false;
	
	try{
		pOwner->NotifyEnterKey();
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
	}
	
	return result;
}

void cNativeIgdeTextField::Focus(){
	setFocus();
}

int cNativeIgdeTextField::TextFieldFlags( const igdeTextField & ){
	return FRAME_SUNKEN;
}

igdeFont *cNativeIgdeTextField::TextFieldFont( const igdeTextField &owner, const igdeGuiTheme &guitheme ){
	igdeFont::sConfiguration configuration;
	owner.GetEnvironment().GetApplicationFont( configuration );
	
	if( guitheme.HasProperty( igdeGuiThemePropertyNames::textFieldFontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::textFieldFontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::textFieldFontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::textFieldFontSize, 1.0f );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f );
	}
	
	return owner.GetEnvironment().GetSharedFont( configuration );
}

int cNativeIgdeTextField::TextFieldPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::textFieldPaddingLeft, DEFAULT_PAD );
}

int cNativeIgdeTextField::TextFieldPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::textFieldPaddingRight, DEFAULT_PAD );
}

int cNativeIgdeTextField::TextFieldPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::textFieldPaddingTop, DEFAULT_PAD );
}

int cNativeIgdeTextField::TextFieldPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::textFieldPaddingBottom, DEFAULT_PAD );
}



// Class igdeTextField
///////////////////////

// Constructor, destructor
////////////////////////////

igdeTextField::igdeTextField( igdeEnvironment &environment, int columns, const char *description ) :
igdeWidget( environment ),
pEnabled( true ),
pColumns( columns ),
pEditable( true ),
pDescription( description ),
pPrecision( 3 ),
pInvalidValue( false )
{
	if( columns < 1 ){
		DETHROW( deeInvalidParam );
	}
}

igdeTextField::igdeTextField( igdeEnvironment &environment, int columns, bool editable,
	const char *description ) :
igdeWidget( environment ),
pEnabled( true ),
pColumns( columns ),
pEditable( editable ),
pDescription( description ),
pPrecision( 3 ),
pInvalidValue( false )
{
	if( columns < 1 ){
		DETHROW( deeInvalidParam );
	}
}

igdeTextField::~igdeTextField(){
	igdeTextField::DestroyNativeWidget();
}



// Management
///////////////

void igdeTextField::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeTextField::SetEditable( bool editable ){
	if( pEditable == editable ){
		return;
	}
	
	pEditable = editable;
	OnEditableChanged();
}

void igdeTextField::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeTextField::SetPrecision( int precision ){
	if( precision < 0 ){
		DETHROW( deeInvalidParam );
	}
	pPrecision = precision;
}

void igdeTextField::SetInvalidValue( bool invalidValue ){
	if( pInvalidValue == invalidValue ){
		return;
	}
	
	pInvalidValue = invalidValue;
	OnInvalidValueChanged();
}



void igdeTextField::SetText( const char *text, bool changing, bool forceNotify ){
	if( pText == text && ! forceNotify ){
		return;
	}
	
	pText = text;
	OnTextChanged();
	
	if( changing ){
		NotifyTextChanging();
		
	}else{
		NotifyTextChanged();
	}
}

void igdeTextField::ClearText(){
	if( pText.IsEmpty() ){
		return;
	}
	
	pText.Empty();
	OnTextChanged();
}

int igdeTextField::GetInteger() const{
	return pText.ToInt();
}

void igdeTextField::SetInteger( int value, bool changing ){
	decString text;
	text.Format( "%d", value );
	SetText( text, changing );
}

float igdeTextField::GetFloat() const{
	return pText.ToFloat();
}

void igdeTextField::SetFloat( float value, bool changing ){
	decString format, text;
	format.Format( "%%.%df", pPrecision );
	text.Format( format, value );
	SetText( text, changing );
}

double igdeTextField::GetDouble() const{
	return pText.ToDouble();
}

void igdeTextField::SetDouble( double value, bool changing ){
	decString format, text;
	format.Format( "%%.%df", pPrecision );
	text.Format( format, value );
	SetText( text, changing );
}



int igdeTextField::GetCursorPosition() const{
	if( ! GetNativeWidget() ){
		return 0;
	}
	
	cNativeIgdeTextField &native = *( ( cNativeIgdeTextField* )GetNativeWidget() );
	return native.getCursorPos();
}

void igdeTextField::SetCursorPosition( int position ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	if( position < 0 || position >= pText.GetLength() ){
		DETHROW( deeInvalidParam );
	}
	
	cNativeIgdeTextField &native = *( ( cNativeIgdeTextField* )GetNativeWidget() );
	native.setCursorPos( position );
}

void igdeTextField::Focus(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeTextField &native = *( ( cNativeIgdeTextField* )GetNativeWidget() );
	return native.Focus();
}



void igdeTextField::AddListener( igdeTextFieldListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeTextField::RemoveListener( igdeTextFieldListener *listener ){
	pListeners.Remove( listener );
}

void igdeTextField::NotifyTextChanged(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeTextFieldListener* )listeners.GetAt( i ) )->OnTextChanged( this );
	}
}

void igdeTextField::NotifyTextChanging(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeTextFieldListener* )listeners.GetAt( i ) )->OnTextChanging( this );
	}
}

void igdeTextField::NotifyEnterKey(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeTextFieldListener* )listeners.GetAt( i ) )->OnEnterKey( this );
	}
}



void igdeTextField::CreateNativeWidget(){
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
	cNativeIgdeTextField * const foxWidget = new cNativeIgdeTextField(
		*this, foxParent, layoutFlags, *GetGuiTheme() );
	SetNativeWidget( foxWidget );
	if( foxParent->id() ){
		foxWidget->create();
	}
}

void igdeTextField::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( cNativeIgdeTextField* )GetNativeWidget();
	DropNativeWidget();
}

void igdeTextField::OnTextChanged(){
	if( GetNativeWidget() ){
		( ( cNativeIgdeTextField* )GetNativeWidget() )->UpdateText();
	}
}

void igdeTextField::OnEnabledChanged(){
	if( GetNativeWidget() ){
		( ( cNativeIgdeTextField* )GetNativeWidget() )->UpdateEnabled();
	}
}

void igdeTextField::OnEditableChanged(){
	if( GetNativeWidget() ){
		( ( cNativeIgdeTextField* )GetNativeWidget() )->UpdateEditable();
	}
}

void igdeTextField::OnDescriptionChanged(){
	if( GetNativeWidget() ){
		( ( cNativeIgdeTextField* )GetNativeWidget() )->UpdateDescription();
	}
}

void igdeTextField::OnInvalidValueChanged(){
	if( GetNativeWidget() ){
		( ( cNativeIgdeTextField* )GetNativeWidget() )->OnInvalidValueChanged();
	}
}
