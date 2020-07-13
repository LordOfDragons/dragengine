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

#include "igdeNativeFoxSpinTextField.h"
#include "foxIcons.h"
#include "../../igdeSpinTextField.h"
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

FXDEFMAP( igdeNativeFoxSpinTextField ) igdeNativeFoxSpinTextFieldMap[] = {
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxSpinTextField::ID_SELF, igdeNativeFoxSpinTextField::onCommand )
};



// Class igdeNativeFoxSpinTextField
//////////////////////////////

FXIMPLEMENT( igdeNativeFoxSpinTextField, FXSpinner, igdeNativeFoxSpinTextFieldMap, ARRAYNUMBER( igdeNativeFoxSpinTextFieldMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxSpinTextField::igdeNativeFoxSpinTextField(){ }

igdeNativeFoxSpinTextField::igdeNativeFoxSpinTextField( igdeSpinTextField &owner, FXComposite *parent,
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

igdeNativeFoxSpinTextField::~igdeNativeFoxSpinTextField(){
}

igdeNativeFoxSpinTextField *igdeNativeFoxSpinTextField::CreateNativeWidget( igdeSpinTextField &owner ){
	if( ! owner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const parent = ( FXComposite* )owner.GetParent()->GetNativeContainer();
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxSpinTextField( owner, parent,
		igdeUIFoxHelper::GetChildLayoutFlags( &owner ), *owner.GetGuiTheme() );
}

void igdeNativeFoxSpinTextField::PostCreateNativeWidget(){
	FXComposite &parent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( parent.id() ){
		create();
	}
}

void igdeNativeFoxSpinTextField::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxSpinTextField::Focus(){
	setFocus();
}

void igdeNativeFoxSpinTextField::UpdateRange(){
	setRange( pOwner->GetLower(), pOwner->GetUpper() );
}

void igdeNativeFoxSpinTextField::UpdateEnabled(){
	if( pOwner->GetEnabled() ){
		enable();
		
	}else{
		disable();
	}
}

void igdeNativeFoxSpinTextField::UpdateValue(){
	setValue( pOwner->GetValue() );
}

void igdeNativeFoxSpinTextField::UpdateDescription(){
	const char * const description = pOwner->GetDescription();
	setTipText( description );
	setHelpText( description );
}




int igdeNativeFoxSpinTextField::SpinTextFieldFlags( const igdeSpinTextField &owner ){
	return FRAME_SUNKEN;
}

igdeFont *igdeNativeFoxSpinTextField::SpinTextFieldFont( const igdeSpinTextField &owner, const igdeGuiTheme &guitheme ){
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

int igdeNativeFoxSpinTextField::SpinTextFieldPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::spinTextFieldPaddingLeft, DEFAULT_PAD );
}

int igdeNativeFoxSpinTextField::SpinTextFieldPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::spinTextFieldPaddingRight, DEFAULT_PAD );
}

int igdeNativeFoxSpinTextField::SpinTextFieldPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::spinTextFieldPaddingTop, DEFAULT_PAD );
}

int igdeNativeFoxSpinTextField::SpinTextFieldPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::spinTextFieldPaddingBottom, DEFAULT_PAD );
}



// Events
///////////

long igdeNativeFoxSpinTextField::onCommand( FXObject *sender, FXSelector selector, void *data ){
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
