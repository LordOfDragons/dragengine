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

#include "igdeSpinTextField.h"
#include "igdeContainer.h"
#include "native/toolkit.h"
#include "igdeCommonDialogs.h"
#include "event/igdeSpinTextFieldListener.h"
#include "resources/igdeFont.h"
#include "resources/igdeFontReference.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Native Widget
//////////////////

class cNativeIgdeSpinTextField : public FXSpinner{
	FXDECLARE( cNativeIgdeSpinTextField )
	
protected:
	cNativeIgdeSpinTextField();
	
public:
	enum eFoxIDs{
		ID_SELF = FXSpinner::ID_LAST,
	};
	
private:
	igdeSpinTextField *pOwner;
	igdeFontReference pFont;
	
public:
	cNativeIgdeSpinTextField( igdeSpinTextField &owner, FXComposite *parent,
		int layoutFlags, const igdeGuiTheme &guitheme );
	virtual ~cNativeIgdeSpinTextField();
	
	void UpdateRange();
	virtual void Focus();
	
	long onCommand( FXObject *sender, FXSelector selector, void *data );
	
	static int SpinTextFieldFlags( const igdeSpinTextField &owner );
	static igdeFont *SpinTextFieldFont( const igdeSpinTextField &owner, const igdeGuiTheme &guitheme );
	static int SpinTextFieldPadLeft( const igdeGuiTheme &guitheme );
	static int SpinTextFieldPadRight( const igdeGuiTheme &guitheme );
	static int SpinTextFieldPadTop( const igdeGuiTheme &guitheme );
	static int SpinTextFieldPadBottom( const igdeGuiTheme &guitheme );
};


FXDEFMAP( cNativeIgdeSpinTextField ) cNativeIgdeSpinTextFieldMap[] = {
	FXMAPFUNC( SEL_COMMAND, cNativeIgdeSpinTextField::ID_SELF, cNativeIgdeSpinTextField::onCommand )
};


FXIMPLEMENT( cNativeIgdeSpinTextField, FXSpinner, cNativeIgdeSpinTextFieldMap, ARRAYNUMBER( cNativeIgdeSpinTextFieldMap ) )

cNativeIgdeSpinTextField::cNativeIgdeSpinTextField(){ }

cNativeIgdeSpinTextField::cNativeIgdeSpinTextField( igdeSpinTextField &owner, FXComposite *parent,
int layoutFlags, const igdeGuiTheme &guitheme ) :
FXSpinner( parent, owner.GetColumns(), this, ID_SELF, layoutFlags | SpinTextFieldFlags( owner ),
	0, 0, 0, 0,
	SpinTextFieldPadLeft( guitheme ), SpinTextFieldPadRight( guitheme ),
	SpinTextFieldPadTop( guitheme ), SpinTextFieldPadBottom( guitheme ) ),
pOwner( &owner ),
pFont( SpinTextFieldFont( owner, guitheme ) )
{
	setFont( (FXFont*)pFont->GetNativeFont() );
	
	if( ! owner.GetEnabled() ){
		disable();
	}
	
	setRange( owner.GetLower(), owner.GetUpper() );
	setValue( owner.GetValue() );
	setTipText( owner.GetDescription().GetString() );
	setHelpText( owner.GetDescription().GetString() );
}

cNativeIgdeSpinTextField::~cNativeIgdeSpinTextField(){
}

void cNativeIgdeSpinTextField::UpdateRange(){
	setRange( pOwner->GetLower(), pOwner->GetUpper() );
}

void cNativeIgdeSpinTextField::Focus(){
	setFocus();
}

long cNativeIgdeSpinTextField::onCommand( FXObject *sender, FXSelector selector, void *data ){
	if( ! pOwner->GetEnabled() ){
		return 0;
	}
	
	try{
		pOwner->SetValue( getValue() );
		pOwner->NotifyValueChanged();
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
		return 0;
	}
	
	return 1;
}

int cNativeIgdeSpinTextField::SpinTextFieldFlags( const igdeSpinTextField &owner ){
	return FRAME_SUNKEN;
}

igdeFont *cNativeIgdeSpinTextField::SpinTextFieldFont( const igdeSpinTextField &owner, const igdeGuiTheme &guitheme ){
	igdeFont::sConfiguration configuration;
	owner.GetEnvironment().GetApplicationFont( configuration );
	
	if( guitheme.HasProperty( igdeGuiThemePropertyNames::spinTextFieldFontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::spinTextFieldFontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::spinTextFieldFontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::spinTextFieldFontSize, 1.0f );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f );
	}
	
	return owner.GetEnvironment().GetSharedFont( configuration );
}

int cNativeIgdeSpinTextField::SpinTextFieldPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::spinTextFieldPaddingLeft, DEFAULT_PAD );
}

int cNativeIgdeSpinTextField::SpinTextFieldPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::spinTextFieldPaddingRight, DEFAULT_PAD );
}

int cNativeIgdeSpinTextField::SpinTextFieldPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::spinTextFieldPaddingTop, DEFAULT_PAD );
}

int cNativeIgdeSpinTextField::SpinTextFieldPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::spinTextFieldPaddingBottom, DEFAULT_PAD );
}



// Class igdeSpinTextField
///////////////////////

// Constructor, destructor
////////////////////////////

igdeSpinTextField::igdeSpinTextField( igdeEnvironment &environment,
	int lower, int upper, int columns, const char *description ) :
igdeWidget( environment ),
pEnabled( true ),
pColumns( columns ),
pLower( lower ),
pUpper( decMath::max( upper, lower ) ),
pValue( lower ),
pDescription( description )
{
	if( columns < 1 ){
		DETHROW( deeInvalidParam );
	}
}

igdeSpinTextField::~igdeSpinTextField(){
	igdeSpinTextField::DestroyNativeWidget();
}



// Management
///////////////

void igdeSpinTextField::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeSpinTextField::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeSpinTextField::Focus(){
	if( GetNativeWidget() ){
		( ( cNativeIgdeSpinTextField* )GetNativeWidget() )->Focus();
	}
}



void igdeSpinTextField::SetValue( int value ){
	if( value == pValue ){
		return;
	}
	
	pValue = value;
	OnValueChanged();
}

void igdeSpinTextField::SetRange( int lower, int upper ){
	if( lower == pLower && upper == pUpper ){
		return;
	}
	
	pLower = lower;
	pUpper = decMath::max( upper, lower );
	OnRangeChanged();
}



void igdeSpinTextField::AddListener( igdeSpinTextFieldListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeSpinTextField::RemoveListener( igdeSpinTextFieldListener *listener ){
	pListeners.Remove( listener );
}

void igdeSpinTextField::NotifyValueChanged(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeSpinTextFieldListener* )listeners.GetAt( i ) )->OnValueChanged( this );
	}
}



void igdeSpinTextField::CreateNativeWidget(){
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
	cNativeIgdeSpinTextField * const foxWidget = new cNativeIgdeSpinTextField(
		*this, foxParent, layoutFlags, *GetGuiTheme() );
	SetNativeWidget( foxWidget );
	if( foxParent->id() ){
		foxWidget->create();
	}
}

void igdeSpinTextField::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( cNativeIgdeSpinTextField* )GetNativeWidget();
	DropNativeWidget();
}

void igdeSpinTextField::OnRangeChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeSpinTextField &native = *( ( cNativeIgdeSpinTextField* )GetNativeWidget() );
	native.UpdateRange();
}

void igdeSpinTextField::OnValueChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeSpinTextField &native = *( ( cNativeIgdeSpinTextField* )GetNativeWidget() );
	native.setValue( pValue );
}

void igdeSpinTextField::OnEnabledChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeSpinTextField &native = *( ( cNativeIgdeSpinTextField* )GetNativeWidget() );
	if( pEnabled ){
		native.enable();
		
	}else{
		native.disable();
	}
}

void igdeSpinTextField::OnDescriptionChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	cNativeIgdeSpinTextField &native = *( ( cNativeIgdeSpinTextField* )GetNativeWidget() );
	native.setTipText( pDescription.GetString() );
	native.setHelpText( pDescription.GetString() );
}
